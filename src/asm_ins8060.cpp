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

#include "asm_ins8060.h"

#include "table_ins8060.h"

namespace libasm {
namespace ins8060 {

Error AsmIns8060::encodeRel8(InsnIns8060 &insn, const Operand &op) {
    Config::ptrdiff_t delta;
    if (op.mode == DISP) {
        delta = static_cast<Config::ptrdiff_t>(op.val16);
        if (overflowRel8(delta))
            return setError(OVERFLOW_RANGE);
        insn.embed(RegIns8060::encodePointerReg(op.reg));
    } else {
        // PC points the last byte of instruction.
        const auto base = insn.address() + 1;
        // PC will be incremented before fetching next instruction.
        const uint8_t fetch = (insn.addrMode() == REL8) ? 1 : 0;
        const auto target = op.getError() ? base : op.val16;
        // Program space is paged by 4kB.
        if (page(target) != page(base))
            return setError(OVERWRAP_PAGE);
        const int16_t diff = offset(target - fetch) - offset(base);
        // Sign extends 12-bit number.
        delta = (diff & 0x7FF) - (diff & 0x800);
        // delta -128 is for E reg.
        if (delta == -128 || overflowRel8(delta))
            return setError(OPERAND_TOO_FAR);
        if (op.getError())
            delta = 0;
        insn.embed(RegIns8060::encodePointerReg(REG_PC));
    }
    insn.emitInsn();
    insn.emitByte(delta);
    return getError();
}

Error AsmIns8060::encodeIndx(InsnIns8060 &insn, const Operand &op) {
    if (op.mode == REL8)
        return encodeRel8(insn, op);
    insn.embed(RegIns8060::encodePointerReg(op.reg));
    if (op.mode == INDX) {  // auto displacement mode
        if (insn.addrMode() != INDX)
            return setError(OPERAND_NOT_ALLOWED);
        insn.embed(4);
    }
    if (overflowRel8(static_cast<int16_t>(op.val16)))
        return setError(OVERFLOW_RANGE);
    insn.emitInsn();
    insn.emitByte(op.val16);
    return getError();
}

Error AsmIns8060::parseOperand(const char *scan, Operand &op) {
    const char *p = skipSpaces(scan);
    _scan = p;
    if (endOfLine(p)) {
        op.mode = INHR;
        return OK;
    }

    const bool autoDisp = (*p == '@');
    if (autoDisp)
        p++;

    const RegName reg = RegIns8060::parseRegName(p);
    if (reg == REG_E) {
        p += RegIns8060::regNameLen(reg);
        op.val16 = -128;
    } else if (reg != REG_UNDEF) {
        _scan = p + RegIns8060::regNameLen(reg);
        op.mode = PNTR;
        op.reg = reg;
        return OK;
    } else {
        op.val16 = parseExpr16(p);
        if (parserError())
            return getError();
        op.setError(getError());
        p = _scan;
    }
    p = skipSpaces(p);
    if (*p == '(') {
        p++;
        const RegName base = RegIns8060::parseRegName(p);
        if (!RegIns8060::isPointerReg(base))
            return setError(UNKNOWN_OPERAND);
        p += RegIns8060::regNameLen(base);
        if (*p != ')')
            return setError(MISSING_CLOSING_PAREN);
        _scan = p + 1;
        op.reg = base;
        op.mode = autoDisp ? INDX : DISP;
        return OK;
    }

    if (autoDisp || reg == REG_E)
        return setError(UNKNOWN_OPERAND);
    _scan = p;
    op.mode = REL8;  // May be IMM8 too
    return OK;
}

Error AsmIns8060::encode(Insn &_insn) {
    InsnIns8060 insn(_insn);
    const char *endName = _parser.scanSymbol(_scan);
    insn.setName(_scan, endName);

    Operand op;
    if (parseOperand(endName, op))
        return getError();
    const char *p = skipSpaces(_scan);
    if (!endOfLine(p))
        return setError(GARBAGE_AT_END);
    setErrorIf(op.getError());

    insn.setAddrMode(op.mode);
    if (TableIns8060.searchName(insn))
        return setError(TableIns8060.getError());

    switch (insn.addrMode()) {
    case INHR:
        insn.emitInsn();
        break;
    case REL8:
        encodeRel8(insn, op);
        break;
    case PNTR:
        insn.embed(RegIns8060::encodePointerReg(op.reg));
        insn.emitInsn();
        break;
    case DISP:
    case INDX:
        encodeIndx(insn, op);
        break;
    case IMM8:
        if (overflowUint8(op.val16))
            return setError(OVERFLOW_RANGE);
        insn.emitInsn();
        insn.emitByte(op.val16);
        break;
    default:
        return setError(INTERNAL_ERROR);
    }
    if (page(insn.address()) != page(insn.address() + insn.length() - 1)) {
        setErrorIf(OVERWRAP_PAGE);
        insn.reset();
    }
    return getError();
}

}  // namespace ins8060
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
