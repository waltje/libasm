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

#include "asm_tms9900.h"
#include "table_tms9900.h"

namespace libasm {
namespace tms9900 {

Error AsmTms9900::checkComma() {
    const char *p = skipSpaces(_scan);
    if (*p != ',') return setErrorIf(MISSING_COMMA);
    _scan = skipSpaces(p + 1);
    return OK;
}

Error AsmTms9900::encodeImm(InsnTms9900 &insn, bool emitInsn) {
    uint16_t val;
    if (getOperand(val)) return getError();
    if (emitInsn) insn.emitInsn();
    insn.emitOperand16(val);
    return OK;
}

Error AsmTms9900::encodeImmMod(InsnTms9900 &insn) {
    uint16_t mode;
    if (endOfLine(_scan)) {
        mode = 0;
    } else {
        if (getOperand(mode)) return getError();
        if (mode == 3 || mode >= 5) return setError(OPERAND_NOT_ALLOWED);
    }
    insn.embed(mode);
    insn.emitInsn();
    return OK;
}

Error AsmTms9900::encodeReg(InsnTms9900 &insn, bool emitInsn) {
    RegName regName = RegTms9900::parseRegName(_scan);
    if (regName == REG_UNDEF) return setError(UNKNOWN_OPERAND);
    _scan += RegTms9900::regNameLen(regName);
    const uint16_t regNo = RegTms9900::encodeRegNumber(regName);
    switch (insn.addrMode()) {
    case REG:
    case REG_IMM:
    case CNT_REG:
        insn.embed(regNo);
        break;
    case REG_SRC:
        insn.embed(regNo << 6);
        break;
    default:
        return setError(INTERNAL_ERROR);
    }
    if (emitInsn) insn.emitInsn();
    return setOK();
}

Error AsmTms9900::encodeCnt(InsnTms9900 &insn, bool acceptR0, bool accept16) {
    uint16_t count;
    const RegName reg = RegTms9900::parseRegName(_scan);
    if (reg != REG_UNDEF) {
        if (reg != REG_R0 || !acceptR0) return setError(REGISTER_NOT_ALLOWED);
        _scan += 2;
        count = 0;
    } else {
        uint8_t val8;
        if (getOperand(val8)) return getError();
        if (val8 > 16 || (!accept16 && val8 == 16))
            return setError(OVERFLOW_RANGE);
        count = val8 & 0xf;
    }
    switch (insn.addrMode()) {
    case CNT_REG:
        insn.embed(count << 4);
        break;
    case CNT_SRC:
    case XOP_SRC:
        insn.embed(count << 6);
        break;
    default:
        return setError(INTERNAL_ERROR);
    }
    insn.emitInsn();
    return getError();
}

Error AsmTms9900::encodeOpr(
    InsnTms9900 &insn, bool emitInsn, bool destination) {
    Config::opcode_t opCode;
    uint16_t operand;
    if (encodeOpr(opCode, operand)) return getError();
    insn.embed(destination ? (opCode << 6) : opCode);
    if (emitInsn)
        insn.emitInsn();
    if (needsOperandWord(opCode))
        insn.emitOperand16(operand);
    return getError();
}

Error AsmTms9900::encodeOpr(Config::opcode_t &oprMode, uint16_t &operand) {
    const char *p = _scan;
    RegName regName;
    uint8_t mode = 0;
    if ((regName = RegTms9900::parseRegName(p)) != REG_UNDEF) {
        p += RegTms9900::regNameLen(regName);
        mode = 0;
        setOK();
    } else if (*p == '*') {
        p = skipSpaces(p + 1);
        mode = 1;
        if ((regName = RegTms9900::parseRegName(p)) == REG_UNDEF)
            return setError(UNKNOWN_OPERAND);
        p += RegTms9900::regNameLen(regName);
        if (*p == '+') {
            p++;
            mode = 3;
        }
        setOK();
    } else if (*p == '@') {
        mode = 2;
        _scan = skipSpaces(p + 1);
        if (getOperand(operand)) return getError();
        p = skipSpaces(_scan);
        if (*p == '(') {
            p = skipSpaces(p + 1);
            regName = RegTms9900::parseRegName(p);
            if (regName == REG_UNDEF) return setError(UNKNOWN_OPERAND);
            if (regName == REG_R0) return setError(REGISTER_NOT_ALLOWED);
            p = skipSpaces(p + RegTms9900::regNameLen(regName));
            if (*p != ')') return setError(MISSING_CLOSING_PAREN);
            p++;
        } else {
            regName = REG_R0;
        }
    }
    _scan = p;
    oprMode = (mode << 4) | RegTms9900::encodeRegNumber(regName);
    return OK;
}

bool AsmTms9900::needsOperandWord(Config::opcode_t oprMode) const {
    return (oprMode & 0x30) == 0x20;
}

Error AsmTms9900::encodeRel(InsnTms9900 &insn) {
    const Config::uintptr_t base = insn.address() + 2;
    Config::uintptr_t target;
    if (getOperand(target)) return getError();
    if (getError()) target = base;
    if (target % 2) return setError(OPERAND_NOT_ALIGNED);
    const Config::ptrdiff_t delta = (target - base) >> 1;
    if (delta >= 128 || delta < -128) return setError(OPERAND_TOO_FAR);
    insn.embed(static_cast<uint8_t>(delta));
    insn.emitInsn();
    return getError();
}

Error AsmTms9900::encodeCruOff(InsnTms9900 &insn) {
    uint8_t val8;
    if (getOperand(val8)) return getError();
    insn.embed(val8);
    insn.emitInsn();
    return getError();
}

Error AsmTms9900::encodeDoubleWords(InsnTms9900 &insn) {
    Config::opcode_t srcMode, dstMode = 0;
    uint16_t srcOpr, dstOpr = 0;
    if (encodeOpr(srcMode, srcOpr)) return getError();
    if (*_scan != ',') setError(MISSING_COMMA);
    _scan = skipSpaces(_scan + 1);

    if (insn.addrMode() == DW_BIT_SRC) {
        if ((srcMode & 0x30) == 0x30)
            return setError(OPERAND_NOT_ALLOWED);
        if (getOperand(dstOpr)) return getError();
        if (dstOpr >= 16) return setError(ILLEGAL_BIT_NUMBER);
        dstMode = dstOpr;
    } else if (insn.addrMode() == DW_CNT_SRC) {
        const RegName reg = RegTms9900::parseRegName(_scan);
        if (reg != REG_UNDEF) {
            if (reg != REG_R0) return setError(REGISTER_NOT_ALLOWED);
            _scan += RegTms9900::regNameLen(reg);
            dstOpr = 0;
        } else if (getOperand(dstOpr)) {
            return getError();
        } else if (dstOpr == 0) {
            return setError(OPERAND_NOT_ALLOWED);
        } else if (dstOpr >= 16) {
            return setError(OVERFLOW_RANGE);
        }
        dstMode = dstOpr;
        srcMode |= 0x4000;
    } else if (insn.addrMode() == DW_DST_SRC) {
        if (encodeOpr(dstMode, dstOpr)) return getError();
        srcMode |= 0x4000;
    } else setError(INTERNAL_ERROR);

    insn.emitInsn();
    insn.emitOperand16(srcMode | (dstMode << 6));
    if (needsOperandWord(srcMode)) insn.emitOperand16(srcOpr);
    if (needsOperandWord(dstMode)) insn.emitOperand16(dstOpr);
    return getError();
}

Error AsmTms9900::encode(Insn &_insn) {
    InsnTms9900 insn(_insn);
    const char *endName = _parser.scanSymbol(_scan);
    insn.setName(_scan, endName);

    if (TableTms9900.searchName(insn))
        return setError(TableTms9900.getError());
    _scan = skipSpaces(endName);

    Error error = OK;
    switch (insn.addrMode()) {
    case INH:
        insn.emitInsn();
        if (endOfLine(_scan)) setOK();
        else setError(UNKNOWN_OPERAND);
        break;
    case IMM:
        encodeImm(insn, true);
        break;
    case IMM_MOD:
        encodeImmMod(insn);
        break;
    case REG:
        encodeReg(insn, true);
        break;
    case REG_IMM:
        error = encodeReg(insn, false);
        if (checkComma()) return setError(UNKNOWN_OPERAND);
        encodeImm(insn, true);
        break;
    case CNT_REG:
        error = encodeReg(insn, false);
        if (checkComma()) return setError(UNKNOWN_OPERAND);
        encodeCnt(insn, /* R0 */true, /* 16 */false);
        break;
    case SRC:
        encodeOpr(insn, true);
        break;
    case CNT_SRC:
        error = encodeOpr(insn, false);
        if (checkComma()) return setError(UNKNOWN_OPERAND);
        encodeCnt(insn, /* R0 */false, /* 16 */true);
        break;
    case XOP_SRC:
        error = encodeOpr(insn, false);
        if (checkComma()) return setError(UNKNOWN_OPERAND);
        encodeCnt(insn, /* R0 */false, /* 16 */false);
        break;
    case REG_SRC:
        error = encodeOpr(insn, false);
        if (checkComma()) return setError(UNKNOWN_OPERAND);
        encodeReg(insn, true);
        break;
    case DST_SRC:
        error = encodeOpr(insn, false);
        if (checkComma()) return setError(UNKNOWN_OPERAND);
        encodeOpr(insn, true, true);
        break;
    case REL:
        encodeRel(insn);
        break;
    case CRU_OFF:
        encodeCruOff(insn);
        break;
    case DW_CNT_SRC:
    case DW_DST_SRC:
    case DW_BIT_SRC:
        encodeDoubleWords(insn);
        break;
    default:
        return setError(INTERNAL_ERROR);
    }
    setErrorIf(error);
    if (getError() != OK && getError() != UNDEFINED_SYMBOL)
        _insn.resetAddress(insn.address());
    return checkLineEnd();
}

} // namespace tms9900
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
