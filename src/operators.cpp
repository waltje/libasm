/*
 * Copyright 2023 Tadashi G. Takaoka
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

#include "operators.h"

#include <ctype.h>

#include "str_scanner.h"

namespace libasm {

bool Operator::isHigher(const Operator &o) const {
    return _prec < o._prec || (_prec == o._prec && o._assoc == LEFT);
}

bool Operator::isNoneAssoc(const Operator &o) const {
    return _prec == o._prec && _assoc == NONE;
}

Error Operator::eval(ValueStack &stack, uint8_t argc) const {
    if (argc == 0)
        argc = stack.size();
    if (_fn) {
        const auto nargs = _fn->nargs();
        if (nargs < 0 || argc == nargs)
            return _fn->eval(stack, argc);
        return argc > nargs ? TOO_MANY_FUNC_ARGUMENT : TOO_FEW_FUNC_ARGUMENT;
    }
    return argc >= _nargs ? (*_op)(stack) : MISSING_OPERAND;
}

static Error bitwise_not(ValueStack &stack) {
    const auto v = stack.pop().getUnsigned();
    stack.pushUnsigned(~v);
    return OK;
}

static Error logical_not(ValueStack &stack) {
    const auto v = stack.pop().getUnsigned();
    stack.pushUnsigned(!v);
    return OK;
}

static Error unary_minus(ValueStack &stack) {
    const auto v = stack.pop();
    const auto error = v.isUnsigned() && v.getUnsigned() > 0x80000000 ? OVERFLOW_RANGE : OK;
    stack.pushSigned(-v.getSigned());
    return error;
}

static Error unary_plus(ValueStack &stack) {
    const auto v = stack.pop().getUnsigned();
    stack.pushUnsigned(v);
    return OK;
}

static Error multiply(ValueStack &stack) {
    const auto rhs = stack.pop();
    const auto lhs = stack.pop();
    if (lhs.isSigned() || rhs.isSigned()) {
        stack.pushSigned(lhs.getSigned() * rhs.getSigned());
    } else {
        stack.pushUnsigned(lhs.getUnsigned() * rhs.getUnsigned());
    }
    return OK;
}

static Error divide(ValueStack &stack) {
    const auto rhs = stack.pop();
    const auto lhs = stack.pop();
    auto error = OK;
    if (rhs.getUnsigned() == 0) {
        stack.push(Value());
        error = DIVIDE_BY_ZERO;
    } else if (lhs.isSigned() || rhs.isSigned()) {
        stack.pushSigned(lhs.getSigned() / rhs.getSigned());
    } else {
        stack.pushUnsigned(lhs.getUnsigned() / rhs.getUnsigned());
    }
    return error;
}

static Error modulo(ValueStack &stack) {
    const auto rhs = stack.pop();
    const auto lhs = stack.pop();
    auto error = OK;
    if (rhs.getUnsigned() == 0) {
        stack.push(Value());
        error = DIVIDE_BY_ZERO;
    } else if (lhs.isSigned() || rhs.isSigned()) {
        stack.pushSigned(lhs.getSigned() % rhs.getSigned());
    } else {
        stack.pushUnsigned(lhs.getUnsigned() % rhs.getUnsigned());
    }
    return error;
}

static Error add(ValueStack &stack) {
    const auto rhs = stack.pop();
    const auto lhs = stack.pop();
    if (lhs.isSigned() || rhs.isSigned()) {
        stack.pushSigned(lhs.getSigned() + rhs.getSigned());
    } else {
        stack.pushUnsigned(lhs.getUnsigned() + rhs.getUnsigned());
    }
    return OK;
}

static Error subtract(ValueStack &stack) {
    const auto rhs = stack.pop();
    const auto lhs = stack.pop();
    if (lhs.isSigned() || rhs.isSigned()) {
        stack.pushSigned(lhs.getSigned() - rhs.getSigned());
    } else if (lhs.getUnsigned() < rhs.getUnsigned()) {
        stack.pushSigned(lhs.getUnsigned() - rhs.getUnsigned());
    } else {
        stack.pushUnsigned(lhs.getUnsigned() - rhs.getUnsigned());
    }
    return OK;
}

static uint32_t shift_left(uint32_t value, uint8_t count) {
    for (uint8_t i = 0; i <= 32 && i < count; i++)
        value <<= 1;
    return value;
}

static uint32_t shift_right(uint32_t value, uint8_t count) {
    for (uint8_t i = 0; i <= 32 && i < count; i++)
        value >>= 1;
    return value;
}

static int32_t shift_right_signed(int32_t value, uint8_t count) {
    const auto sign = (value < 0) ? 0x80000000 : 0;
    for (uint8_t i = 0; i <= 32 && i < count; i++) {
        value >>= 1;
        value |= sign;
    }
    return value;
}

static Error logical_shift_left(ValueStack &stack) {
    const auto rhs = stack.pop().getUnsigned();
    const auto lhs = stack.pop().getUnsigned();
    stack.pushUnsigned(shift_left(lhs, rhs));
    return OK;
}

static Error arithmetic_shift_right(ValueStack &stack) {
    const auto rhs = stack.pop().getUnsigned();
    const auto lhs = stack.pop();
    if (lhs.isSigned()) {
        stack.pushSigned(shift_right_signed(lhs.getSigned(), rhs));
    } else {
        stack.pushUnsigned(shift_right(lhs.getUnsigned(), rhs));
    }
    return OK;
}

static Error less_than(ValueStack &stack) {
    const auto rhs = stack.pop();
    const auto lhs = stack.pop();
    if (lhs.isSigned() || rhs.isSigned()) {
        stack.pushUnsigned(lhs.getSigned() < rhs.getSigned());
    } else {
        stack.pushUnsigned(lhs.getUnsigned() < rhs.getUnsigned());
    }
    return OK;
}

static Error less_than_or_equal(ValueStack &stack) {
    const auto rhs = stack.pop();
    const auto lhs = stack.pop();
    if (lhs.isSigned() || rhs.isSigned()) {
        stack.pushUnsigned(lhs.getSigned() <= rhs.getSigned());
    } else {
        stack.pushUnsigned(lhs.getUnsigned() <= rhs.getUnsigned());
    }
    return OK;
}

static Error greater_than_or_equal(ValueStack &stack) {
    const auto rhs = stack.pop();
    const auto lhs = stack.pop();
    if (lhs.isSigned() || rhs.isSigned()) {
        stack.pushUnsigned(lhs.getSigned() >= rhs.getSigned());
    } else {
        stack.pushUnsigned(lhs.getUnsigned() >= rhs.getUnsigned());
    }
    return OK;
}

static Error greater_than(ValueStack &stack) {
    const auto rhs = stack.pop();
    const auto lhs = stack.pop();
    if (lhs.isSigned() || rhs.isSigned()) {
        stack.pushUnsigned(lhs.getSigned() > rhs.getSigned());
    } else {
        stack.pushUnsigned(lhs.getUnsigned() > rhs.getUnsigned());
    }
    return OK;
}

static Error logical_equal(ValueStack &stack) {
    const auto rhs = stack.pop().getUnsigned();
    const auto lhs = stack.pop().getUnsigned();
    stack.pushUnsigned(lhs == rhs);
    return OK;
}

static Error logical_not_equal(ValueStack &stack) {
    const auto rhs = stack.pop().getUnsigned();
    const auto lhs = stack.pop().getUnsigned();
    stack.pushUnsigned(lhs != rhs);
    return OK;
}

static Error bitwise_and(ValueStack &stack) {
    const auto rhs = stack.pop().getUnsigned();
    const auto lhs = stack.pop().getUnsigned();
    stack.pushUnsigned(lhs & rhs);
    return OK;
}

static Error bitwise_xor(ValueStack &stack) {
    const auto rhs = stack.pop().getUnsigned();
    const auto lhs = stack.pop().getUnsigned();
    stack.pushUnsigned(lhs ^ rhs);
    return OK;
}

static Error bitwise_or(ValueStack &stack) {
    const auto rhs = stack.pop().getUnsigned();
    const auto lhs = stack.pop().getUnsigned();
    stack.pushUnsigned(lhs | rhs);
    return OK;
}

static Error logical_and(ValueStack &stack) {
    const auto rhs = stack.pop().getUnsigned();
    const auto lhs = stack.pop().getUnsigned();
    stack.pushUnsigned(lhs && rhs);

    return OK;
}

static Error logical_or(ValueStack &stack) {
    const auto rhs = stack.pop().getUnsigned();
    const auto lhs = stack.pop().getUnsigned();
    stack.pushUnsigned(lhs || rhs);
    return OK;
}

constexpr Operator::Assoc R = Operator::RIGHT;
constexpr Operator::Assoc L = Operator::LEFT;
constexpr Operator::Assoc N = Operator::NONE;

// clang-format off
static const Operator OP_BITWISE_NOT( 3, R, 1, bitwise_not);
static const Operator OP_LOGICAL_NOT( 3, R, 1, logical_not);
static const Operator OP_UNARY_MINUS( 3, R, 1, unary_minus);
static const Operator OP_UNARY_PLUS(  3, R, 1, unary_plus);
static const Operator OP_MUL(         5, L, 2, multiply);
static const Operator OP_DIV(         5, L, 2, divide);
static const Operator OP_MOD(         5, L, 2, modulo);
static const Operator OP_ADD(         6, L, 2, add);
static const Operator OP_SUB(         6, L, 2, subtract);
static const Operator OP_SHIFT_LEFT(  7, L, 2, logical_shift_left);
static const Operator OP_SHIFT_RIGHT( 7, L, 2, arithmetic_shift_right);
static const Operator OP_LOGICAL_LT(  9, N, 2, less_than);
static const Operator OP_LOGICAL_LE(  9, N, 2, less_than_or_equal);
static const Operator OP_LOGICAL_GE(  9, N, 2, greater_than_or_equal);
static const Operator OP_LOGICAL_GT(  9, N, 2, greater_than);
static const Operator OP_LOGICAL_EQ( 10, N, 2, logical_equal);
static const Operator OP_LOGICAL_NE( 10, N, 2, logical_not_equal);
static const Operator OP_BITWISE_AND(11, L, 2, bitwise_and);
static const Operator OP_BITWISE_XOR(12, L, 2, bitwise_xor);
static const Operator OP_BITWISE_OR( 13, L, 2, bitwise_or);
static const Operator OP_LOGICAL_AND(14, L, 2, logical_and);
static const Operator OP_LOGICAL_OR( 15, L, 2, logical_or);
// clang-format on

const Operator *CStyleOperatorParser::readOperator(
        StrScanner &scan, ErrorAt &error, Operator::Type type) const {
    auto p = scan;
    const Operator *opr = nullptr;
    if (type == Operator::PREFIX) {
        if (p.expect('~')) {
            opr = &OP_BITWISE_NOT;
        } else if (p.expect('!') && *p != '=') {
            opr = &OP_LOGICAL_NOT;
        } else if (p.expect('-')) {
            opr = &OP_UNARY_MINUS;
        } else if (p.expect('+')) {
            opr = &OP_UNARY_PLUS;
        }
    } else if (type == Operator::INFIX) {
        if (p.expect('*')) {
            opr = &OP_MUL;
        } else if (p.expect('/')) {
            opr = &OP_DIV;
        } else if (p.expect('%')) {
            opr = &OP_MOD;
        } else if (p.expect('+')) {
            opr = &OP_ADD;
        } else if (p.expect('-')) {
            opr = &OP_SUB;
        } else if (p.expect('<')) {
            if (p.expect('<')) {
                opr = &OP_SHIFT_LEFT;
            } else if (p.expect('=')) {
                opr = &OP_LOGICAL_LE;
            } else {
                opr = &OP_LOGICAL_LT;
            }
        } else if (p.expect('>')) {
            if (p.expect('>')) {
                opr = &OP_SHIFT_RIGHT;
            } else if (p.expect('=')) {
                opr = &OP_LOGICAL_GE;
            } else {
                opr = &OP_LOGICAL_GT;
            }
        } else if (p.expect('&')) {
            opr = p.expect('&') ? &OP_LOGICAL_AND : &OP_BITWISE_AND;
        } else if (p.expect('^')) {
            opr = &OP_BITWISE_XOR;
        } else if (p.expect('|')) {
            opr = p.expect('|') ? &OP_LOGICAL_OR : &OP_BITWISE_OR;
        } else if (p.expect('!')) {
            if (p.expect('='))
                opr = &OP_LOGICAL_NE;
        } else if (p.expect('=')) {
            if (p.expect('='))
                opr = &OP_LOGICAL_EQ;
        }
    }
    if (opr)
        scan = p;
    return opr;
}

template <typename BASE>
static BASE power(BASE base, uint32_t exp) {
    BASE prod = 1;
    while (true) {
        if (exp & 1)
            prod *= base;
        if ((exp >>= 1) == 0)
            return prod;
        base *= base;
    }
}

static Error exponential(ValueStack &stack) {
    const auto rhs = stack.pop().getUnsigned();
    const auto lhs = stack.pop();
    if (lhs.isSigned() && lhs.getSigned() < 0) {
        stack.pushSigned(power<int32_t>(lhs.getSigned(), rhs));
    } else {
        stack.pushUnsigned(power<uint32_t>(lhs.getUnsigned(), rhs));
    }
    return OK;
}

static Error rotate_left_16bit(ValueStack &stack) {
    const auto rhs = stack.pop().getUnsigned();
    const auto lhs = stack.pop().getUnsigned() & 0xFFFF;
    const auto v = shift_left(lhs, rhs) | shift_right(lhs, 16 - rhs);
    stack.pushUnsigned(v & 0xFFFF);
    return OK;
}

static Error rotate_right_16bit(ValueStack &stack) {
    const auto rhs = stack.pop().getUnsigned();
    const auto lhs = stack.pop().getUnsigned() & 0xFFFF;
    const auto v = shift_right(lhs, rhs) | shift_left(lhs, 16 - rhs);
    stack.pushUnsigned(v & 0xFFFF);
    return OK;
}

static Error logical_shift_left_16bit(ValueStack &stack) {
    const auto rhs = stack.pop().getUnsigned();
    const auto lhs = stack.pop().getUnsigned() & 0xFFFF;
    stack.pushUnsigned(shift_left(lhs, rhs) & 0xFFFF);
    return OK;
}

static Error logical_shift_right_16bit(ValueStack &stack) {
    const auto rhs = stack.pop().getUnsigned();
    const auto lhs = stack.pop().getUnsigned() & 0xFFFF;
    stack.pushUnsigned(shift_right(lhs, rhs) & 0xFFFF);
    return OK;
}

// clang-format off
static const Operator MC68_EXPONENTIAL( 5, R, 2, exponential);
static const Operator MC68_ROTATE_LEFT( 5, L, 2, rotate_left_16bit);
static const Operator MC68_ROTATE_RIGHT(5, L, 2, rotate_right_16bit);
static const Operator MC68_SHIFT_LEFT(  5, L, 2, logical_shift_left_16bit);
static const Operator MC68_SHIFT_RIGHT( 5, L, 2, logical_shift_right_16bit);
static const Operator MC68_BITWISE_AND( 5, L, 2, bitwise_and);
static const Operator MC68_BITWISE_XOR( 5, L, 2, bitwise_xor);
static const Operator MC68_BITWISE_OR(  5, L, 2, bitwise_or);
// clang-format on

const Operator *Mc68xxOperatorParser::readOperator(
        StrScanner &scan, ErrorAt &error, Operator::Type type) const {
    if (type == Operator::INFIX) {
        auto p = scan;
        if (p.expect('!')) {
            const Operator *opr = nullptr;
            if (p.expect('.')) {
                opr = &MC68_BITWISE_AND;
            } else if (p.expect('+')) {
                opr = &MC68_BITWISE_OR;
            } else if (p.expect('X') && !isalnum(*p)) {
                opr = &MC68_BITWISE_XOR;
            } else if (p.expect('^')) {
                opr = &MC68_EXPONENTIAL;
            } else if (p.expect('<')) {
                opr = &MC68_SHIFT_LEFT;
            } else if (p.expect('>')) {
                opr = &MC68_SHIFT_RIGHT;
            } else if (p.expect('L') && !isalnum(*p)) {
                opr = &MC68_ROTATE_LEFT;
            } else if (p.expect('R') && !isalnum(*p)) {
                opr = &MC68_ROTATE_RIGHT;
            }
            if (opr) {
                scan = p;
                return opr;
            }
        }
    }
    return CStyleOperatorParser::singleton().readOperator(scan, error, type);
}

static Error most_siginificant_byte(ValueStack &stack) {
    const auto v = stack.pop().getUnsigned();
    stack.pushUnsigned((v >> 8) & 0xFF);
    return OK;
}

static Error least_significant_byte(ValueStack &stack) {
    const auto v = stack.pop().getUnsigned();
    stack.pushUnsigned(v & 0xFF);
    return OK;
}

static Error most_siginificant_word(ValueStack &stack) {
    const auto v = stack.pop().getUnsigned();
    stack.pushUnsigned((v >> 16) & 0xFFFF);
    return OK;
}

static Error least_significant_word(ValueStack &stack) {
    const auto v = stack.pop().getUnsigned();
    stack.pushUnsigned(v & 0xFFFF);
    return OK;
}

// clang-format off
static const Operator INTEL_HIGH(        3, R, 1, most_siginificant_byte);
static const Operator INTEL_LOW(         3, R, 1, least_significant_byte);
static const Operator INTEL_MSW(         3, R, 1, most_siginificant_word);
static const Operator INTEL_LSW(         3, R, 1, least_significant_word);
static const Operator INTEL_SHIFT_RIGHT( 5, L, 1, logical_shift_right_16bit);
static const Operator INTEL_SHIFT_LEFT(  5, L, 1, logical_shift_left_16bit);
static const Operator INTEL_UNARY_MINUS( 6, R, 1, unary_minus);
static const Operator INTEL_UNARY_PLUS(  6, R, 1, unary_plus);
static const Operator INTEL_LOGICAL_EQ(  9, N, 2, logical_equal);
static const Operator INTEL_LOGICAL_NE(  9, N, 2, logical_not_equal);
static const Operator INTEL_BITWISE_NOT(10, R, 1, bitwise_not);
static const Operator INTEL_BITWISE_AND(11, L, 2, bitwise_and);
static const Operator INTEL_BITWISE_XOR(12, L, 2, bitwise_xor);
static const Operator INTEL_BITWISE_OR( 12, L, 2, bitwise_or);
// clang-format on

const Operator *IntelOperatorParser::readOperator(
        StrScanner &scan, ErrorAt &error, Operator::Type type) const {
    auto p = scan;
    // TODO: Use SymbolParser
    p.trimStart(isalnum);
    const StrScanner name(scan.str(), p.str());
    const Operator *opr = nullptr;
    if (type == Operator::PREFIX) {
        if (scan.expect('-')) {
            return &INTEL_UNARY_MINUS;
        } else if (scan.expect('+')) {
            return &INTEL_UNARY_PLUS;
        }
        if (name.iequals_P(PSTR("NOT"))) {
            opr = &INTEL_BITWISE_NOT;
        } else if (name.iequals_P(PSTR("HIGH"))) {
            opr = &INTEL_HIGH;
        } else if (name.iequals_P(PSTR("LOW"))) {
            opr = &INTEL_LOW;
        } else if (name.iequals_P(PSTR("MSW"))) {
            opr = &INTEL_MSW;
        } else if (name.iequals_P(PSTR("LSW"))) {
            opr = &INTEL_LSW;
        }
        if (opr) {
            scan += name.size();
            return opr;
        }
    } else if (type == Operator::INFIX) {
        if (name.iequals_P(PSTR("MOD"))) {
            opr = &OP_MOD;
        } else if (name.iequals_P(PSTR("AND"))) {
            opr = &INTEL_BITWISE_AND;
        } else if (name.iequals_P(PSTR("OR"))) {
            opr = &INTEL_BITWISE_OR;
        } else if (name.iequals_P(PSTR("XOR"))) {
            opr = &INTEL_BITWISE_XOR;
        } else if (name.iequals_P(PSTR("SHR"))) {
            opr = &INTEL_SHIFT_RIGHT;
        } else if (name.iequals_P(PSTR("SHL"))) {
            opr = &INTEL_SHIFT_LEFT;
        } else if (name.iequals_P(PSTR("EQ"))) {
            opr = &INTEL_LOGICAL_EQ;
        } else if (name.iequals_P(PSTR("NE"))) {
            opr = &INTEL_LOGICAL_NE;
        } else if (name.iequals_P(PSTR("LT"))) {
            opr = &OP_LOGICAL_LT;
        } else if (name.iequals_P(PSTR("LE"))) {
            opr = &OP_LOGICAL_LE;
        } else if (name.iequals_P(PSTR("GE"))) {
            opr = &OP_LOGICAL_GE;
        } else if (name.iequals_P(PSTR("GT"))) {
            opr = &OP_LOGICAL_GT;
        }
        if (opr) {
            scan += name.size();
            return opr;
        }
    }
    return CStyleOperatorParser::singleton().readOperator(scan, error, type);
}

// clang-format off
static const Operator ZILOG_HIGH16(     3, R, 1, most_siginificant_word);
static const Operator ZILOG_LOW16(      3, R, 1, least_significant_word);
static const Operator ZILOG_SHIFT_RIGHT(5, L, 1, logical_shift_right_16bit);
static const Operator ZILOG_SHIFT_LEFT( 5, L, 1, logical_shift_left_16bit);
static const Operator ZILOG_BITWISE_AND(5, L, 2, bitwise_and);
static const Operator ZILOG_BITWISE_XOR(6, L, 2, bitwise_xor);
static const Operator ZILOG_BITWISE_OR( 6, L, 2, bitwise_or);
// clang-format on

const Operator *ZilogOperatorParser::readOperator(
        StrScanner &scan, ErrorAt &error, Operator::Type type) const {
    auto p = scan;
    // TODO: Use SymbolParser
    p.trimStart(isalnum);
    const StrScanner name(scan.str(), p.str());
    const Operator *opr = nullptr;
    if (type == Operator::PREFIX) {
        if (name.iequals_P(PSTR("LNOT"))) {
            opr = &OP_BITWISE_NOT;
        } else if (name.iequals_P(PSTR("HIGH"))) {
            opr = &INTEL_HIGH;
        } else if (name.iequals_P(PSTR("LOW"))) {
            opr = &INTEL_LOW;
        } else if (name.iequals_P(PSTR("HIGH16"))) {
            opr = &ZILOG_HIGH16;
        } else if (name.iequals_P(PSTR("LOW16"))) {
            opr = &ZILOG_LOW16;
        }
        if (opr) {
            scan += name.size();
            return opr;
        }
    } else if (type == Operator::INFIX) {
        if (name.iequals_P(PSTR("MOD"))) {
            opr = &OP_MOD;
        } else if (name.iequals_P(PSTR("SHR"))) {
            opr = &ZILOG_SHIFT_RIGHT;
        } else if (name.iequals_P(PSTR("SHL"))) {
            opr = &ZILOG_SHIFT_LEFT;
        } else if (name.iequals_P(PSTR("LAND"))) {
            opr = &ZILOG_BITWISE_AND;
        } else if (name.iequals_P(PSTR("LOR"))) {
            opr = &ZILOG_BITWISE_OR;
        } else if (name.iequals_P(PSTR("LXOR"))) {
            opr = &ZILOG_BITWISE_XOR;
        }
        if (opr) {
            scan += name.size();
            return opr;
        }
    }
    return CStyleOperatorParser::singleton().readOperator(scan, error, type);
}

}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
