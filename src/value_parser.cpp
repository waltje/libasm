/*
 * Copyright 2020 Tadashi G. Takaoka
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "value_parser.h"

#include <ctype.h>

namespace libasm {

NumberParser ValueParser::NUMBER_PARSER;
IntelNumberParser IntelValueParser::NUMBER_PARSER;
MotorolaNumberParser MotorolaValueParser::NUMBER_PARSER;
NationalNumberParser NationalValueParser::NUMBER_PARSER;
FairchildNumberParser FairchildValueParser::NUMBER_PARSER;

bool Value::overflowRel8(int16_t s16) {
    return s16 < -128 || s16 >= 128;
}

bool Value::overflowRel8(int32_t s32) {
    return s32 < -128 || s32 >= 128;
}

bool Value::overflowRel16(int32_t s32) {
    return s32 < -32768L || s32 >= 32768L;
}

bool Value::overflowUint8(uint16_t u16) {
    const int16_t s16 = static_cast<int16_t>(u16);
    return s16 < -128 || (s16 >= 0 && u16 >= 256);
}

bool Value::overflowUint8(uint32_t u32) {
    const int32_t s32 = static_cast<int32_t>(u32);
    return s32 < -128 || (s32 >= 0 && u32 >= 0x100);
}

bool Value::overflowUint16(uint32_t u32) {
    const int32_t s32 = static_cast<int32_t>(u32);
    return s32 < -32768L || (s32 >= 0 && u32 >= 0x10000L);
}

bool Value::overflowUint(uint32_t u32, uint8_t bitw) {
    const int32_t s32 = static_cast<int32_t>(u32);
    if (bitw >= 32)
        return false;
    const uint32_t max = 1UL << bitw;
    const int32_t sign = max >> 1;
    return s32 < -sign || (s32 >= 0 && u32 >= max);
}

static bool isoctal(char c) {
    return c >= '0' && c < '8';
}

char ValueParser::readChar(StrScanner &scan) {
    setOK();
    StrScanner p(scan);
    char c = *p++;
    if (c != '\\') {
        scan = p;
        return c;
    }
    Radix radix = RADIX_10;
    if (p.iexpect('x')) {
        radix = RADIX_16;
    } else if (isoctal(*p)) {
        radix = RADIX_8;
    } else {
        switch (c = *p++) {
        case '\'':
        case '"':
        case '?':
        case '\\':
            break;
        case 'b':
            c = 0x08;
            break;
        case 't':
            c = 0x09;
            break;
        case 'n':
            c = 0x0a;
            break;
        case 'r':
            c = 0x0d;
            break;
        default:
            setError(scan, UNKNOWN_ESCAPE_SEQUENCE);
            return 0;
        }
        scan = p;
        return c;
    }
    Value value;
    NUMBER_PARSER.parseNumber(p, value, radix);
    if (getError())
        return 0;
    if (value.overflowUint8()) {
        setError(scan, OVERFLOW_RANGE);
        return 0;
    }
    scan = p;
    return value.getUnsigned();
}

static bool isValidDigit(const char c, const Radix radix) {
    if (radix == RADIX_16)
        return isxdigit(c);
    return c >= '0' && c < '0' + uint8_t(radix);
}

static uint8_t toNumber(const char c, const Radix radix) {
    if (radix == RADIX_16 && c >= 'A')
        return (c & ~0x20) - 'A' + 10;
    return c - '0';
}

Error NumberParser::parseNumber(StrScanner &scan, Value &val, const Radix radix) {
    StrScanner p(scan);
    if (!isValidDigit(*p, radix))
        return setError(scan, ILLEGAL_CONSTANT);
    const uint32_t limit = UINT32_MAX / uint8_t(radix);
    const uint8_t limit_digit = UINT32_MAX % uint8_t(radix);
    uint32_t v = 0;
    while (isValidDigit(*p, radix)) {
        const uint8_t n = toNumber(*p, radix);
        if (v > limit || (v == limit && n > limit_digit))
            return setError(scan, OVERFLOW_RANGE);
        v *= uint8_t(radix);
        v += n;
        ++p;
    }
    scan = p;
    val.setValue(v);
    return setOK();
}

Value ValueParser::eval(StrScanner &expr, const SymbolTable *symtab) {
    setOK();
    Stack<OprAndLval> stack;
    return parseExpr(expr, stack, symtab);
}

StrScanner ValueParser::scanExpr(const StrScanner &scan, char delim) {
    StrScanner p(scan);
    while (!endOfLine(*p)) {
        if (_numberParser.numberPrefix(p)) {
            Value val;
            if (setError(_numberParser.readNumber(p, val)) == OK)
                continue;
        }
        char prefix;
        if (charPrefix(p, prefix)) {
            readChar(p);
            if (isOK() && charSuffix(p, prefix))
                continue;
        }
        if (*p == delim)
            return StrScanner(scan.str(), p.str());
        if (*p == '(' || *p == '[') {
            const char close = (*p++ == '(') ? ')' : ']';
            const StrScanner atom = scanExpr(p, close);
            if (atom.size() == 0)
                break;
            p += atom.size() + 1;
        } else {
            ++p;
        }
    }
    return StrScanner(p.str(), p.str());
}
Value ValueParser::parseExpr(
        StrScanner &scan, Stack<OprAndLval> &stack, const SymbolTable *symtab) {
    if (stack.full()) {
        setError(scan, TOO_COMPLEX_EXPRESSION);
        return Value();
    }
    stack.push(OprAndLval());
    Value value(readAtom(scan, stack, symtab));
    if (getError())
        return Value();
    while (!stack.empty()) {
        Operator opr(readOperator(scan));
        while (opr._precedence <= stack.top().precedence()) {
            if (stack.top().isEnd()) {
                stack.pop();
                return value;
            }
            value = evalExpr(stack.top()._opr._op, stack.top()._value, value);
            stack.pop();
        }
        if (stack.full()) {
            setError(scan, TOO_COMPLEX_EXPRESSION);
            return Value();
        }
        stack.push(OprAndLval(opr, value));
        value = readAtom(scan, stack, symtab);
        if (getError())
            return Value();
    }
    return Value();
}

Value ValueParser::readAtom(StrScanner &scan, Stack<OprAndLval> &stack, const SymbolTable *symtab) {
    StrScanner p(scan);
    if (endOfLine(*p.skipSpaces())) {
        setError(scan, ILLEGAL_CONSTANT);
        return Value();
    }
    if (p.expect('(')) {
        Value value(parseExpr(p, stack, symtab));
        if (isOK()) {
            if (!p.skipSpaces().expect(')')) {
                setError(p, MISSING_CLOSING_PAREN);
                return Value();
            }
            scan = p;
            return value;
        }
        return value;
    }
    char prefix;
    if (charPrefix(p, prefix)) {
        Value value(readCharacterConstant(p));
        if (isOK() && !charSuffix(p, prefix))
            setError(p, MISSING_CLOSING_QUOTE);
        scan = p;
        return value;
    }
    if (p.expect('~')) {
        Value value(readAtom(p, stack, symtab));
        scan = p;
        if (!value.isUndefined())
            return value.setValue(~value.getUnsigned());
        return value;
    }
    if (*p == '-' || *p == '+') {
        const char c = *p;
        if (*++p == '-' || *p == '+') {
            setError(scan, UNKNOWN_EXPR_OPERATOR);
            return Value();
        }
        Value value(readAtom(p, stack, symtab));
        if (c == '-' && !value.isUndefined()) {
            if (value.isUnsigned() && value.getUnsigned() > 0x80000000)
                setError(scan, OVERFLOW_RANGE);
            scan = p;
            return Value::makeSigned(-value.getSigned());
        }
        scan = p;
        return value;
    }

    if (locationSymbol(p)) {
        scan = p;
        return Value::makeUnsigned(_origin);
    }

    Value val;
    if (_numberParser.numberPrefix(p))
        goto read_number;

    if (symbolLetter(*p, true)) {
        if (_funcParser)
            _funcParser->setAt(p);
        const StrScanner symbol = readSymbol(p);
        if (*p.skipSpaces() == '(' && _funcParser) {
            StrScanner params(p);
            params.expect('(');
            if (_funcParser->parseFunc(*this, symbol, params, val, symtab) == OK) {
                if (!params.expect(')')) {
                    setError(p, MISSING_CLOSING_PAREN);
                    return Value();
                }
                scan = params;
                return val;
            } else if (_funcParser->getError() != UNKNOWN_FUNCTION) {
                setError(*_funcParser);
                return Value();
            }
        }
        scan = p;
        if (symtab) {
            if (symtab->hasSymbol(symbol)) {
                const uint32_t v = symtab->lookupSymbol(symbol);
                return Value().setValue(v);
            }
            setAt(symbol);
            return Value();
        }
        return Value();
    }

read_number:
    if (_numberParser.readNumber(p, val)) {
        setError(_numberParser);
    } else if (symbolLetter(*p)) {
        setError(ILLEGAL_CONSTANT);
    } else {
        scan = p;
    }
    return val;
}

ValueParser::FuncParser *ValueParser::setFuncParser(FuncParser *parser) {
    auto prev = _funcParser;
    _funcParser = parser;
    return prev;
}

Value ValueParser::readCharacterConstant(StrScanner &scan) {
    const auto c = readChar(scan);
    if (getError())
        return Value();
    return Value::makeSigned(c);
}

// Operator precedence (larger value means higher precedence).
// The same order of C/C++ language.
ValueParser::Operator ValueParser::readOperator(StrScanner &scan) {
    if (endOfLine(*scan.skipSpaces()))
        return Operator(OP_NONE, 0);
    const char c = *scan++;
    switch (c) {
    case '*':
        return Operator(OP_MUL, 13);
    case '/':
        return Operator(OP_DIV, 13);
    case '%':
        return Operator(OP_MOD, 13);
    case '+':
        return Operator(OP_ADD, 12);
    case '-':
        return Operator(OP_SUB, 12);
    case '<':
        if (scan.expect('<'))
            return Operator(OP_BIT_SHL, 11);
        setError(scan, UNKNOWN_EXPR_OPERATOR);
        break;
    case '>':
        if (scan.expect('>'))
            return Operator(OP_BIT_SHR, 11);
        setError(scan, UNKNOWN_EXPR_OPERATOR);
        break;
    case '&':
        return Operator(OP_BIT_AND, 8);
    case '^':
        return Operator(OP_BIT_XOR, 7);
    case '|':
        return Operator(OP_BIT_OR, 6);
    default:
        break;
    }
    --scan;
    return Operator(OP_NONE, 0);
}

bool ValueParser::symbolLetter(char c, bool head) const {
    if (isalpha(c) || c == '_')
        return true;
    if (head && c == '.')
        return true;
    return !head && isdigit(c);
}

StrScanner ValueParser::readSymbol(StrScanner &scan) const {
    StrScanner p(scan);
    p.trimStart([this](char c) { return this->symbolLetter(c); });
    StrScanner symbol = StrScanner(scan.str(), p.str());
    scan = p;
    return symbol;
}

static uint32_t shift_left(uint32_t value, uint8_t count) {
    for (unsigned i = 0; i <= 32 && i < count; i++)
        value <<= 1;
    return value;
}

static uint32_t shift_right(uint32_t value, uint8_t count) {
    for (unsigned i = 0; i <= 32 && i < count; i++)
        value >>= 1;
    return value;
}

static int32_t shift_right_negative(int32_t value, uint8_t count) {
    for (unsigned i = 0; i <= 32 && i < count; i++) {
        value >>= 1;
        value |= 0x80000000;
    }
    return value;
}

Value ValueParser::evalExpr(const Op op, const Value lhs, const Value rhs) {
    if (lhs.isUndefined() || rhs.isUndefined())
        return Value();

    const bool bsigned = lhs.isSigned() && rhs.isSigned();

    switch (op) {
    case OP_ADD:
        return Value::makeUnsigned(lhs.getUnsigned() + rhs.getUnsigned()).setSign(bsigned);
    case OP_SUB:
        return Value::makeUnsigned(lhs.getUnsigned() - rhs.getUnsigned()).setSign(bsigned);
    case OP_MUL:
        return bsigned ? Value::makeSigned(lhs.getSigned() * rhs.getSigned())
                       : Value::makeUnsigned(lhs.getUnsigned() * rhs.getUnsigned());
    case OP_DIV:
        if (rhs.getUnsigned() == 0) {
            setError(DIVIDE_BY_ZERO);
            return Value();
        }
        return bsigned ? Value::makeSigned(lhs.getSigned() / rhs.getSigned())
                       : Value::makeUnsigned(lhs.getUnsigned() / rhs.getUnsigned());
    case OP_MOD:
        if (rhs.getUnsigned() == 0) {
            setError(DIVIDE_BY_ZERO);
            return Value();
        }
        return bsigned ? Value::makeSigned(lhs.getSigned() % rhs.getSigned())
                       : Value::makeUnsigned(lhs.getUnsigned() % rhs.getUnsigned());
    case OP_BIT_AND:
        return Value::makeUnsigned(lhs.getUnsigned() & rhs.getUnsigned());
    case OP_BIT_XOR:
        return Value::makeUnsigned(lhs.getUnsigned() ^ rhs.getUnsigned());
    case OP_BIT_OR:
        return Value::makeUnsigned(lhs.getUnsigned() | rhs.getUnsigned());
    case OP_BIT_SHL:
        return Value::makeUnsigned(shift_left(lhs.getUnsigned(), rhs.getUnsigned()));
    case OP_BIT_SHR:
        if (lhs.isSigned() && lhs.getSigned() < 0)
            return Value::makeSigned(shift_right_negative(lhs.getSigned(), rhs.getUnsigned()));
        return Value::makeUnsigned(shift_right(lhs.getUnsigned(), rhs.getUnsigned()));
    default:
        return Value();
    }
}

Error NumberParser::scanNumberEnd(const StrScanner &scan, const Radix radix, char suffix) const {
    StrScanner p(scan);
    if (suffix == 'B') {
        // Check whether intel binary or C-style binary
        if (*p == '0' && toupper(*++p) == 'B' && (*++p == '0' || *p == '1'))
            return ILLEGAL_CONSTANT;  // expect intel but found C-style
        p = scan;
    }
    if (isValidDigit(*p, radix)) {
        p.trimStart([radix](char c) { return isValidDigit(c, radix); });
        if (suffix == 0)
            return OK;
        if (toupper(*p) == suffix)
            return OK;
    }
    return ILLEGAL_CONSTANT;
}

bool ValueParser::locationSymbol(StrScanner &scan) const {
    return (scan.expect(_locSym) || scan.expect('.')) && !symbolLetter(*scan);
}

bool ValueParser::charPrefix(StrScanner &scan, char &prefix) const {
    return (prefix = scan.expect('\''));
}

bool ValueParser::charSuffix(StrScanner &scan, char prefix) const {
    return scan.expect(prefix);
}

bool NumberParser::numberPrefix(const StrScanner &scan) const {
    StrScanner p(scan);
    return p.expect('0') && (p.iexpect('X') || p.iexpect('B'));
}

Error NumberParser::expectNumberSuffix(StrScanner &scan, char suffix) const {
    if (getError())
        return getError();
    StrScanner p(scan);
    if (isalpha(suffix)) {
        // Alphabet suffix is mandatory
        if (toupper(*p++) != suffix)
            return ILLEGAL_CONSTANT;
    } else if (suffix) {
        // Non-alphabet suffix is optional, such as H'xx'
        p.expect(suffix);
    }
    scan = p;
    return OK;
}

Error NumberParser::readNumber(StrScanner &scan, Value &val) {
    StrScanner p(scan);
    Radix radix = RADIX_10;
    if (p.expect('0')) {
        if (isoctal(*p)) {
            if (scanNumberEnd(p, RADIX_8) == OK)
                radix = RADIX_8;
        } else if (p.iexpect('X')) {
            if (scanNumberEnd(p, RADIX_16) == OK)
                radix = RADIX_16;
        } else if (p.iexpect('B')) {
            if (scanNumberEnd(p, RADIX_2) == OK)
                radix = RADIX_2;
        } else {
            p = scan;
        }
    }
    if (radix == RADIX_10 && (!isdigit(*p) || scanNumberEnd(p, RADIX_10) != OK))
        setError(scan, ILLEGAL_CONSTANT);
    parseNumber(p, val, radix);
    return setError(expectNumberSuffix(scan = p));
}

bool MotorolaValueParser::charSuffix(StrScanner &scan, char prefix) const {
    return scan.expect('\'') || !_closingQuote;
}

bool MotorolaNumberParser::numberPrefix(const StrScanner &scan) const {
    const char c = *scan;
    const char n = scan[1];
    if (c == '$')
        return isxdigit(n);
    if (c == '@')
        return n >= '0' || n < '8';
    if (c == '%')
        return n == '0' || n == '1';
    return NumberParser::numberPrefix(scan);
}

Error MotorolaNumberParser::readNumber(StrScanner &scan, Value &val) {
    StrScanner p(scan);
    Radix radix = RADIX_10;
    if (p.expect('$')) {
        if (scanNumberEnd(p, RADIX_16) == OK)
            radix = RADIX_16;
    } else if (p.expect('@')) {
        if (scanNumberEnd(p, RADIX_8) == OK)
            radix = RADIX_8;
    } else if (p.expect('%')) {
        if (scanNumberEnd(p, RADIX_2) == OK)
            radix = RADIX_2;
    }
    if (radix != RADIX_10) {
        parseNumber(p, val, radix);
        return setError(expectNumberSuffix(scan = p));
    }
    return NumberParser::readNumber(scan, val);
}

bool IntelNumberParser::numberPrefix(const StrScanner &scan) const {
    return isdigit(*scan) || NumberParser::numberPrefix(scan);
}

Error IntelNumberParser::readNumber(StrScanner &scan, Value &val) {
    Radix radix = RADIX_10;
    char suffix = 0;
    if (scanNumberEnd(scan, RADIX_16, suffix = 'H') == OK) {
        radix = RADIX_16;
    } else if (scanNumberEnd(scan, RADIX_8, suffix = 'O') == OK) {
        radix = RADIX_8;
    } else if (scanNumberEnd(scan, RADIX_8, suffix = 'Q') == OK) {
        radix = RADIX_8;
    } else if (scanNumberEnd(scan, RADIX_2, suffix = 'B') == OK) {
        radix = RADIX_2;
    }
    if (radix != RADIX_10) {
        parseNumber(scan, val, radix);
        return setError(expectNumberSuffix(scan, suffix));
    }
    return NumberParser::readNumber(scan, val);
}

bool NationalValueParser::symbolLetter(char c, bool head) const {
    if (head && c == '$')
        return true;
    return ValueParser::symbolLetter(c, head);
}

bool NationalNumberParser::numberPrefix(const StrScanner &scan) const {
    const char c = toupper(*scan);
    const char q = scan[1];
    if (q == '\'')
        return c == 'H' || c == 'X' || c == 'D' || c == 'O' || c == 'Q' || c == 'B';
    return NumberParser::numberPrefix(scan);
}

Error NationalNumberParser::readNumber(StrScanner &scan, Value &val) {
    StrScanner p(scan);
    Radix radix = Radix(0);
    constexpr char quote = '\'';
    if ((p.iexpect('X') || p.iexpect('H')) && p.expect(quote)) {
        if (scanNumberEnd(p, RADIX_16) == OK)
            radix = RADIX_16;
    } else if (p.iexpect('D') && p.expect(quote)) {
        if (scanNumberEnd(p, RADIX_10) == OK)
            radix = RADIX_10;
    } else if ((p.iexpect('O') || p.iexpect('Q')) && p.expect(quote)) {
        if (scanNumberEnd(p, RADIX_8) == OK)
            radix = RADIX_8;
    } else if (p.iexpect('B') && p.expect(quote)) {
        if (scanNumberEnd(p, RADIX_2) == OK)
            radix = RADIX_2;
    }

    if (uint8_t(radix) != 0) {
        parseNumber(p, val, radix);
        return setError(expectNumberSuffix(scan = p, quote));
    }
    // TODO: Support Decimal(0[fFlL]) and Hexadecimal([fFlL]'/0[yYzZ]) floating ponit constant
    return NumberParser::readNumber(scan, val);
}

bool FairchildValueParser::locationSymbol(StrScanner &scan) const {
    return scan.expect('*') || (!NUMBER_PARSER.numberPrefix(scan) && scan.expect('$'));
}

bool FairchildValueParser::charPrefix(StrScanner &scan, char &prefix) const {
    StrScanner p(scan);
    if (p.iexpect('c') && p.expect('\'')) {
        // C'x'
        prefix = 'C';
        scan = p;
        return true;
    }
    if (p.expect('#')) {
        // #c
        prefix = '#';
        scan = p;
        return true;
    }
    // 'c' or 'c
    return ValueParser::charPrefix(scan, prefix);
}

bool FairchildValueParser::charSuffix(StrScanner &scan, char prefix) const {
    if (prefix == 'C')
        return scan.expect('\'');  // closing quote is necessary for C'x'.
    if (prefix == '#')
        return true;    //  no closing quite for #c
    scan.expect('\'');  // closing quote is optional for 'x'
    return true;
}

bool FairchildValueParser::symbolLetter(char c, bool head) const {
    return ValueParser::symbolLetter(c, head);
}

bool FairchildNumberParser::numberPrefix(const StrScanner &scan) const {
    if (_motorola.numberPrefix(scan))
        return true;
    const char c = toupper(*scan);
    const char q = scan[1];
    if (q == '\'')
        return c == 'H' || c == 'D' || c == 'O' || c == 'B';
    return NumberParser::numberPrefix(scan);
}

Error FairchildNumberParser::readNumber(StrScanner &scan, Value &val) {
    if (_motorola.numberPrefix(scan)) {
        _motorola.readNumber(scan, val);
        return setError(_motorola);
    } else {
        _national.readNumber(scan, val);
        return setError(_national);
    }
}

}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
