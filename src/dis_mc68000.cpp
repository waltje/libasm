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

#include "dis_mc68000.h"
#include "table_mc68000.h"

namespace libasm {
namespace mc68000 {

void DisMc68000::outRegName(RegName regName) {
    _operands = _regs.outRegName(_operands, regName);
}

void DisMc68000::outEaSize(EaSize size) {
    _operands = _regs.outEaSize(_operands, size);
}

Error DisMc68000::decodeImmediateData(
    DisMemory &memory, InsnMc68000 &insn, EaSize size) {
    if (size == SZ_BYTE) {
        uint16_t val16;
        if (insn.readUint16(memory, val16)) return setError(NO_MEMORY);
        outConstant(static_cast<uint8_t>(val16));
        return OK;
    }
    if (size == SZ_WORD) {
        uint16_t val16;
        if (insn.readUint16(memory, val16)) return setError(NO_MEMORY);
        outConstant(val16);
        return OK;
    }
    if (size == SZ_LONG) {
        uint32_t val32;
        if (insn.readUint32(memory, val32)) return setError(NO_MEMORY);
        outConstant(val32);
        return OK;
    }
    return setError(ILLEGAL_SIZE);
}

Error DisMc68000::decodeEffectiveAddr(
    DisMemory &memory, InsnMc68000 &insn,
    const EaMc68000 &ea) {
    const EaMode mode = ea.mode;
    if (mode == M_ILLEGAL)
        return setError(ILLEGAL_OPERAND);
    if (mode == M_DREG || mode == M_AREG) {
        outRegName(ea.reg);
        return setOK();
    }
    if (mode == M_IMM_DATA) {
        *_operands++ = '#';
        return decodeImmediateData(memory, insn, ea.size);
    }

    if (mode == M_PDEC) *_operands++ = '-';
    *_operands++ = '(';
    if (mode == M_DISP || mode == M_PC_DISP) {
        const RegName base = (mode == M_DISP) ? ea.reg : REG_PC;
        uint16_t val16;
        if (insn.readUint16(memory, val16)) return setError(NO_MEMORY);
        if (mode == M_PC_DISP) {
            const Config::uintptr_t target =
                insn.address() + 2 + static_cast<int16_t>(val16);
            if (ea.size == SZ_WORD && (target % 2) != 0)
                return setError(OPERAND_NOT_ALIGNED);
            if (ea.size == SZ_LONG && (target % 4) != 0)
                return setError(OPERAND_NOT_ALIGNED);
            outRelativeAddr(target, insn.address(), 16);
        } else {
            if (val16 & 0x8000) {
                const uint16_t disp16 = 0x10000 - val16;
                *_operands++ = '-';
                outConstant(disp16);
            } else {
                outConstant(val16);
            }
        }
        *_operands++ = ',';
        outRegName(base);
    }
    if (mode == M_AIND || mode == M_PINC || mode == M_PDEC) {
        outRegName(ea.reg);
    }
    if (mode == M_ABS_SHORT || mode == M_ABS_LONG) {
        Config::uintptr_t target;
        if (mode == M_ABS_SHORT) {
            uint16_t val16;
            if (insn.readUint16(memory, val16)) return setError(NO_MEMORY);
            const int16_t signed16 = static_cast<int16_t>(val16);
            target = static_cast<int32_t>(signed16);
        } else {
            uint32_t val32;
            if (insn.readUint32(memory, val32)) return setError(NO_MEMORY);
            target = val32;
        }
        if (ea.size == SZ_WORD && (target % 2) != 0)
            return setError(OPERAND_NOT_ALIGNED);
        if (ea.size == SZ_LONG && (target % 4) != 0)
            return setError(OPERAND_NOT_ALIGNED);
        outConstant(target, 16, false, true, addressBits());
    }
    if (mode == M_INDX || mode == M_PC_INDX) {
        const RegName base = (mode == M_INDX) ? ea.reg : REG_PC;
        BriefExt ext;
        if (insn.readUint16(memory, ext.word)) return setError(NO_MEMORY);
        const uint8_t val8 = ext.disp();
        if (mode == M_PC_INDX) {
            const Config::uintptr_t target =
                insn.address() + 2 + static_cast<int8_t>(val8);
            outRelativeAddr(target, insn.address(), 8);
        } else {
            if (val8 & 0x80) {
                const uint8_t disp8 = 0x100 - val8;
                *_operands++ = '-';
                outConstant(disp8);
            } else {
                outConstant(val8);
            }
        }
        *_operands++ = ',';
        outRegName(base);
        *_operands++ = ',';
        outRegName(ext.index());
        outEaSize(ext.indexSize());
    }
    *_operands++ = ')';
    if (mode == M_ABS_SHORT) outEaSize(SZ_WORD);
    if (mode == M_ABS_LONG)  outEaSize(SZ_LONG);
    if (mode == M_PINC) *_operands++ = '+';
    *_operands = 0;
    return setOK();
}

Error DisMc68000::decodeImplied(
    DisMemory &memory, InsnMc68000 &insn) {
    if (insn.opCode() == 047162) { // STOP
        *_operands++ = '#';
        return decodeImmediateData(memory, insn, SZ_WORD);
    }
    return setOK();
}

// ORI, ANDI, SUBI, ADDI, EORI, CMPI
// NEGX, CLR, NEG, NOT, TST
Error DisMc68000::decodeDestSiz(
    DisMemory &memory, InsnMc68000 &insn) {
    const Config::opcode_t opCode = insn.opCode();
    const EaMc68000 ea(opCode);
    const uint8_t opc = (opCode >> 9) & 7;
    if (ea.size == SZ_INVALID) return setError(ILLEGAL_SIZE);

    if ((opCode >> 12) == 0) { // ORI/ANDI/SUBI/ADDI/EORI/CMPI
        constexpr uint8_t ORI  = 00;
        constexpr uint8_t ANDI = 01;
        constexpr uint8_t EORI = 05;
        constexpr uint8_t CMPI = 06;
        RegName reg = REG_UNDEF;
        if (opc == ORI || opc == ANDI || opc == EORI) {
            if (ea.mode == M_IMM_DATA) {
                if (ea.size == SZ_BYTE) reg = REG_CCR;
                if (ea.size == SZ_WORD) reg = REG_SR;
            }
        }
        if (reg == REG_UNDEF) {
            const EaCat categories = (opc == CMPI) ? EaCat::DATA
                : EaCat::DATA | EaCat::ALTERABLE;
            if (!ea.satisfy(categories))
                return setError(ILLEGAL_OPERAND_MODE);
            if (opc == CMPI && ea.mode == M_IMM_DATA)
                return setError(ILLEGAL_OPERAND_MODE);
            if (opc == CMPI
                && (ea.mode == M_PC_INDX || ea.mode == M_PC_DISP))
                return setError(ILLEGAL_OPERAND_MODE);
        }
        *_operands++ = '#';
        if (decodeImmediateData(memory, insn, ea.size))
            return getError();
        *_operands++ = ',';
        if (reg != REG_UNDEF) {
            outRegName(reg);
            return setOK();
        }
    } else {                    // NEGX/CLR/NEG/NOT/TST
        constexpr uint8_t TST = 5;
        const EaCat categories = (opc == TST) ? EaCat::DATA
            : EaCat::DATA | EaCat::ALTERABLE;
        if (ea.mode == M_IMM_DATA || !ea.satisfy(categories))
            return setError(ILLEGAL_OPERAND_MODE);
    }

    insn.appendSize(ea.size, _regs);
    return decodeEffectiveAddr(memory, insn, ea);
}

// LINK, UNLK
Error DisMc68000::decodeAddrReg(
    DisMemory &memory, InsnMc68000 &insn) {
    const RegName dest = RegMc68000::decodeAddrReg(insn.opCode());
    if (insn.opCode() & 010) { // UNLK
        outRegName(dest);
        return setOK();
    }
    // LINK
    outRegName(dest);
    *_operands++ = ',';
    uint16_t val16;
    if (insn.readUint16(memory, val16)) return setError(NO_MEMORY);
    const int16_t disp = static_cast<int16_t>(val16);
    *_operands++ = '#';
    outConstant(disp);
    return setOK();
}

// DBcc, SWAP
Error DisMc68000::decodeDataReg(
    DisMemory &memory, InsnMc68000 &insn) {
    const RegName dest = RegMc68000::decodeDataReg(insn.opCode());
    if ((insn.opCode() >> 12) == 4) { // SWAP
        outRegName(dest);
        return setOK();
    }

    // DBcc
    outRegName(dest);
    *_operands++ = ',';
    uint16_t val16;
    if (insn.readUint16(memory, val16)) return setError(NO_MEMORY);
    const int16_t disp = static_cast<int16_t>(val16);
    const Config::uintptr_t target = insn.address() + 2 + disp;
    if (target % 2) return setError(OPERAND_NOT_ALIGNED);
    outRelativeAddr(target, insn.address(), 16);
    return setOK();
}

// MOVE USP
Error DisMc68000::decodeMoveUsp(InsnMc68000 &insn) {
    const RegName areg = RegMc68000::decodeAddrReg(insn.opCode());
    if (insn.opCode() & 010) { // USP->An
        outRegName(REG_USP);
        *_operands++ = ',';
        outRegName(areg);
    } else {                    // An ->USP
        outRegName(areg);
        *_operands++ = ',';
        outRegName(REG_USP);
    }
    return setOK();
}

// TRAP
Error DisMc68000::decodeTrapVec(InsnMc68000 &insn) {
    *_operands++ = '#';
    outConstant(static_cast<uint8_t>(insn.opCode() & 017), 10);
    return setOK();
}

// NBCD, PEA, TAS
Error DisMc68000::decodeDataDst(
    DisMemory &memory, InsnMc68000 &insn) {
    const Config::opcode_t opCode = insn.opCode();
    const uint8_t opc = (opCode >> 6) & 077;
    constexpr uint8_t PEA = 041;
    EaSize size = (opc == PEA) ? SZ_LONG : SZ_BYTE;
    const EaMc68000 ea(size,  opCode >> 3, opCode);
    if (opc == PEA) {
        if (!ea.satisfy(EaCat::CONTROL))
            return setError(ILLEGAL_OPERAND_MODE);
    } else {                    // NBCD, TAS
        if (!ea.satisfy(EaCat::DATA | EaCat::ALTERABLE))
            return setError(ILLEGAL_OPERAND_MODE);
    }
    return decodeEffectiveAddr(memory, insn, ea);
}

// BTST, BCHG, BCLR, BSET
// JSR, JMP, Scc,
// ASR, ASL, LSR, LSL, ROR, ROL, ROXR, ROXL
Error DisMc68000::decodeDestOpr(
    DisMemory &memory, InsnMc68000 &insn) {
    EaMc68000 ea(insn.opCode());
    EaSize size = ea.size;

    if ((insn.opCode() >> 12) == 0) { // BTST/BCHG/BCLR/BSET
        const uint8_t opc = (insn.opCode() >> 6) & 3;
        constexpr uint8_t BTST = 0;
        const EaCat categories = (opc == BTST) ? EaCat::DATA
            : EaCat::DATA | EaCat::ALTERABLE;
        if (!ea.satisfy(categories) || ea.mode == M_IMM_DATA)
            return setError(ILLEGAL_OPERAND_MODE);
        uint16_t val16;
        if (insn.readUint16(memory, val16)) return setError(NO_MEMORY);
        if (ea.mode == M_DREG) {
            ea.size = size = SZ_LONG;
            if (val16 >= 32) return setError(ILLEGAL_BIT_NUMBER);
        } else {
            ea.size = size = SZ_BYTE;
            if (val16 >= 8) return setError(ILLEGAL_BIT_NUMBER);
        }
        *_operands++ = '#';
        outConstant(val16, 10);
        *_operands++ = ',';
    } else {                    // JSR/JMP/Scc/ASx/LSx/ROx/ROXx
        const uint8_t opc = (insn.opCode() >> 12) & 077;
        constexpr uint8_t JSR_JMP = 004;
        constexpr uint8_t Scc = 005;
        if (opc == JSR_JMP) {
            if (!ea.satisfy(EaCat::CONTROL))
                return setError(ILLEGAL_OPERAND_MODE);
            size = SZ_NONE;
            ea.size = SZ_WORD;
        } else if (opc == Scc) {
            if (!ea.satisfy(EaCat::DATA | EaCat::ALTERABLE))
                return setError(ILLEGAL_OPERAND_MODE);
            size = SZ_NONE;
            ea.size = SZ_BYTE;
        } else {                // ASx/LSx/ROx/ROXx
            if (!ea.satisfy(EaCat::MEMORY | EaCat::ALTERABLE))
                return setError(ILLEGAL_OPERAND_MODE);
            ea.size = SZ_BYTE;
        }
    }

    insn.appendSize(size, _regs);
    return decodeEffectiveAddr(memory, insn, ea);
}

// EXT
Error DisMc68000::decodeSignExt(InsnMc68000 &insn) {
    const Config::opcode_t opCode = insn.opCode();
    insn.appendSize((opCode & 0100) ? SZ_LONG : SZ_WORD, _regs);
    outRegName(RegMc68000::decodeDataReg(opCode));
    return setOK();
}

// EXT_BRA: BRA, BSR, Bcc
Error DisMc68000::decodeRelative(
    DisMemory &memory, InsnMc68000 &insn) {
    const uint8_t val8 = static_cast<uint8_t>(insn.opCode());
    Config::uintptr_t target = insn.address() + 2;
    if (val8) {
        target += static_cast<int8_t>(val8);
    } else {
        uint16_t val16;
        if (insn.readUint16(memory, val16)) return setError(NO_MEMORY);
        target += static_cast<int16_t>(val16);
    }
    if (target % 2) return setError(OPERAND_NOT_ALIGNED);
    outRelativeAddr(target, insn.address(), val8 ? 8 : 16);
    return setOK();
}

static RegName decodeMoveMltReg(int8_t regno) {
    return (regno < 8)
        ? RegMc68000::decodeDataReg(regno)
        : RegMc68000::decodeAddrReg(regno - 8);
}

void DisMc68000::decodeMoveMltRegList(
    uint16_t list, bool push,
    void (DisMc68000::*outRegs)(RegName, RegName, char)) {
    int8_t start = -1;
    int8_t last = 0;
    uint16_t mask = push ? 0x8000 : 0x0001;
    for (int8_t i = 0; i < 16; i++) {
        if (list & mask) {
            if (start < 0) {
                start = last = i;
            } else if (i == last + 1) {
                last = i;
            } else {
                (this->*outRegs)(
                    decodeMoveMltReg(start), decodeMoveMltReg(last), '/');
                start = last = i;
            }
        }
        if (push) mask >>= 1;
        else mask <<= 1;
    }
    if (start >= 0)
        (this->*outRegs)(
            decodeMoveMltReg(start), decodeMoveMltReg(last), 0);
}

void DisMc68000::outMoveMltRegs(RegName start, RegName last, char suffix) {
    outRegName(start);
    if (start != last) {
        *_operands++ = '-';
        outRegName(last);
    }
    if (suffix) *_operands++ = suffix;
    *_operands = 0;
}

// MOVEM
Error DisMc68000::decodeMoveMlt(
    DisMemory &memory, InsnMc68000 &insn) {
    const Config::opcode_t opCode = insn.opCode();
    const EaMc68000 ea(
        (opCode & 0100) ? SZ_LONG : SZ_WORD, opCode >> 3, opCode);
    if (opCode & 02000) {       // Memory-to-Registers
        if (ea.mode == M_PINC || ea.satisfy(EaCat::CONTROL)) {
            uint16_t list;
            if (insn.readUint16(memory, list)) return setError(NO_MEMORY);
            if (list == 0) return setError(OPCODE_HAS_NO_EFFECT);
            decodeEffectiveAddr(memory, insn, ea);
            *_operands++ = ',';
            decodeMoveMltRegList(list, false, &DisMc68000::outMoveMltRegs);
            *_operands++ = 0;
        } else return setError(ILLEGAL_OPERAND_MODE);
    } else {                    // Registers-to-Memory
        if (ea.mode == M_PDEC || ea.satisfy(EaCat::CONTROL|EaCat::ALTERABLE)) {
            uint16_t list;
            if (insn.readUint16(memory, list)) return setError(NO_MEMORY);
            if (list == 0) return setError(OPCODE_HAS_NO_EFFECT);
            decodeMoveMltRegList(
                list, ea.mode == M_PDEC, &DisMc68000::outMoveMltRegs);
            *_operands++ = ',';
            decodeEffectiveAddr(memory, insn, ea);
        } else return setError(ILLEGAL_OPERAND_MODE);
    }
    insn.appendSize(ea.size, _regs);
    return getError();
}

// MOVE from SR, to SR, to CCR
Error DisMc68000::decodeMoveSr(
    DisMemory &memory, InsnMc68000 &insn) {
    const Config::opcode_t opCode = insn.opCode();
    const uint8_t opc = (opCode >> 8) & 017;
    constexpr uint8_t toCCR = 4;
    constexpr uint8_t fromSR = 0;
    const EaMc68000 ea(SZ_WORD, opCode >> 3, opCode);
    if (opc == fromSR) {
        if (!ea.satisfy(EaCat::DATA | EaCat::ALTERABLE))
            return setError(ILLEGAL_OPERAND_MODE);
        outRegName(REG_SR);
        *_operands++ = ',';
        decodeEffectiveAddr(memory, insn, ea);            
    } else {
        if (!ea.satisfy(EaCat::DATA))
            return setError(ILLEGAL_OPERAND_MODE);
        decodeEffectiveAddr(memory, insn, ea);
        *_operands++ = ',';
        outRegName(opc == toCCR ? REG_CCR : REG_SR);
    }
    return getError();
}

// MOVEQ
Error DisMc68000::decodeMoveQic(InsnMc68000 &insn) {
    const Config::opcode_t opCode = insn.opCode();
    const uint8_t val8 = static_cast<uint8_t>(opCode);
    *_operands++ = '#';
    outConstant(static_cast<int8_t>(val8));
    *_operands++ = ',';
    outRegName(RegMc68000::decodeDataReg(opCode >> 9));
    return setOK();
}

// MOVEP
Error DisMc68000::decodeMovePer(
    DisMemory &memory, InsnMc68000 &insn) {
    const Config::opcode_t opCode = insn.opCode();
    const EaMc68000 ea(
        (opCode & 0100) ? SZ_LONG : SZ_WORD, M_DISP, opCode);
    const RegName dest = RegMc68000::decodeDataReg(opCode >> 9);
    if (opCode & 0200) {      // Dn->EA
        outRegName(dest);
        *_operands++ = ',';
        decodeEffectiveAddr(memory, insn, ea);
    } else {                    // EA->Dn
        decodeEffectiveAddr(memory, insn, ea);
        *_operands++ = ',';
        outRegName(dest);
    }
    insn.appendSize(ea.size, _regs);
    return getError();
}

// AREG_LNG: LEA
// AREG_SIZ: SUBA, CMPA, ADDA
Error DisMc68000::decodeAregSiz(
    DisMemory &memory, InsnMc68000 &insn) {
    const Config::opcode_t opCode = insn.opCode();
    EaSize size = (opCode & 0400) ? SZ_LONG : SZ_WORD;
    if (insn.insnFormat() == AREG_LNG) size = SZ_NONE;
    const EaMc68000 ea(size, opCode >> 3, opCode);
    if (insn.insnFormat() == AREG_LNG) { // LEA
        if (size == SZ_WORD)
            return setError(ILLEGAL_SIZE);
        if (!ea.satisfy(EaCat::CONTROL))
            return setError(ILLEGAL_OPERAND_MODE);
        size = SZ_NONE;
    }

    decodeEffectiveAddr(memory, insn, ea);
    *_operands++ = ',';
    outRegName(RegMc68000::decodeAddrReg(opCode >> 9));
    insn.appendSize(size, _regs);
    return getError();
}

// NO_EXT: BTST, BCHG, BCLR, BSET, CHK, DIVU, DIVS, MULU, MULS
Error DisMc68000::decodeDregDst(
    DisMemory &memory, InsnMc68000 &insn) {
    const Config::opcode_t opCode = insn.opCode();
    const RegName dest = RegMc68000::decodeDataReg(insn.opCode() >> 9);
    EaSize size;

    const uint8_t family = (opCode >> 12) & 017;
    constexpr uint8_t Bxxx = 0;
    if (family == Bxxx) {
        const uint16_t opc = (opCode >> 6) & 7;
        constexpr uint8_t BTST = 4;
        const uint8_t mode = (opCode >> 3) & 7;
        size = (mode == /* M_DREG */0) ? SZ_LONG : SZ_BYTE;
        const EaMc68000 ea(size, mode, opCode);
        const EaCat categories = (opc == BTST) ? EaCat::DATA
            : EaCat::DATA | EaCat::ALTERABLE;
        if (!ea.satisfy(categories) || ea.mode == M_IMM_DATA)
            return setError(ILLEGAL_OPERAND_MODE);
        outRegName(dest);
        *_operands++ = ',';
        decodeEffectiveAddr(memory, insn, ea);
    } else {
        size = SZ_WORD;
        const EaMc68000 ea(size, opCode >> 3, opCode);
        if (!ea.satisfy(EaCat::DATA))
            return setError(ILLEGAL_OPERAND_MODE);
        decodeEffectiveAddr(memory, insn, ea);
        *_operands++ = ',';
        outRegName(dest);
    }
    insn.appendSize(size, _regs);
    return getError();
}

static EaSize moveSize(uint8_t moveSize) {
    switch (moveSize & 3) {
    case 1: return SZ_BYTE;
    case 2: return SZ_LONG;
    case 3: return SZ_WORD;
    default: return SZ_INVALID;
    }
}

// ADDQ, SUBQ
Error DisMc68000::decodeDataQic(
    DisMemory &memory, InsnMc68000 &insn) {
    const Config::opcode_t opCode = insn.opCode();
    const EaMc68000 ea(opCode);
    uint8_t val = (opCode >> 9) & 7;
    if (val == 0) val = 8;
    if (!ea.satisfy(EaCat::ALTERABLE))
        return setError(ILLEGAL_OPERAND_MODE);
    if (ea.mode == M_AREG && ea.size == SZ_BYTE)
        return setError(ILLEGAL_SIZE);
    insn.appendSize(ea.size, _regs);
    *_operands++ = '#';
    outConstant(val);
    *_operands++ = ',';
    return decodeEffectiveAddr(memory, insn, ea);
}

// DMEM_SIZ: OR, SUB, AND, ADD
// DREG_SIZ: CMP, EOR
Error DisMc68000::decodeDmemSiz(
    DisMemory &memory, InsnMc68000 &insn) {
    const Config::opcode_t opCode = insn.opCode();
    const EaMc68000 ea(opCode);
    const RegName dreg = RegMc68000::decodeDataReg(opCode >> 9);
    if (ea.mode == M_AREG && ea.size == SZ_BYTE)
        return setError(ILLEGAL_SIZE);
    insn.appendSize(ea.size, _regs);
    if (opCode & 0400) {      // Dn,<ea>
        if (!ea.satisfy(EaCat::DATA | EaCat::ALTERABLE))
            return setError(ILLEGAL_OPERAND_MODE);
        if (ea.mode == M_DREG && insn.insnFormat() == DMEM_SIZ)
            return setError(ILLEGAL_OPERAND_MODE);
        outRegName(dreg);
        *_operands++ = ',';
        decodeEffectiveAddr(memory, insn, ea);
    } else {                    // <ea>,Dn
        decodeEffectiveAddr(memory, insn, ea);
        *_operands++ = ',';
        outRegName(dreg);
    }
    return getError();
}

// ASR, ASL, LSR, LSL, ROXR, ROXL, ROR, ROL
Error DisMc68000::decodeDregRot(InsnMc68000 &insn) {
    const Config::opcode_t opCode = insn.opCode();
    const RegName dst = RegMc68000::decodeDataReg(opCode);
    const EaSize size = EaSize((opCode >> 6) & 3);
    if (opCode & 040) {       // Dx
        outRegName(RegMc68000::decodeDataReg(opCode >> 9));
    } else {                    // #n
        uint8_t val = (opCode >> 9) & 7;
        if (val == 0) val = 8;
        *_operands++ = '#';
        outConstant(val, 10);
    }
    *_operands++ = ',';
    outRegName(dst);
    insn.appendSize(size, _regs);
    return setOK();
}

// DMEM_DST: SBCD, ABCD
// DMEM_OPR: SUBX, ADDX
// CMPM_SIZ: CMPM
Error DisMc68000::decodeDmemOpr(
    DisMemory &memory, InsnMc68000 &insn) {
    const Config::opcode_t opCode = insn.opCode();
    const uint8_t opc = (opCode >> 12);
    constexpr uint8_t SBCD = 010;
    constexpr uint8_t ABCD = 014;
    constexpr uint8_t CMPM = 013;
    const EaSize size = (opc == SBCD || opc == ABCD) ? SZ_NONE
        : EaSize((opCode >> 6) & 3);
    insn.appendSize(size, _regs);
    if (opCode & 010) {       // -(Ay),-(Ax) or (Ay)+,(Ax)+
        const EaMode mode = (opc == CMPM) ? M_PINC : M_PDEC;
        const EaMc68000 src(SZ_BYTE, static_cast<uint8_t>(mode), opCode);
        const EaMc68000 dst(SZ_BYTE, static_cast<uint8_t>(mode), opCode >> 9);
        decodeEffectiveAddr(memory, insn, src);
        *_operands++ = ',';
        decodeEffectiveAddr(memory, insn, dst);
    } else {                    // Dy,Dx
        outRegName(RegMc68000::decodeDataReg(opCode));
        *_operands++ = ',';
        outRegName(RegMc68000::decodeDataReg(opCode >> 9));
    }
    return setOK();
}

// EXG
Error DisMc68000::decodeRegsExg(InsnMc68000 &insn) {
    const Config::opcode_t opCode = insn.opCode();
    const uint8_t mode = (opCode >> 3) & 031;
    if (mode == 010) {          // Dx,Dy
        outRegName(RegMc68000::decodeDataReg(opCode >> 9));
        *_operands++ = ',';
        outRegName(RegMc68000::decodeDataReg(opCode));
    } else if (mode == 011) {   // Ax,Ay
        outRegName(RegMc68000::decodeAddrReg(opCode >> 9));
        *_operands++ = ',';
        outRegName(RegMc68000::decodeAddrReg(opCode));
    } else {                    // Dx,Ay
        outRegName(RegMc68000::decodeDataReg(opCode >> 9));
        *_operands++ = ',';
        outRegName(RegMc68000::decodeAddrReg(opCode));
    }
    return setOK();
}

// NO_EXT: MOVE, MOVEA
Error DisMc68000::decodeMoveOpr(
    DisMemory &memory, InsnMc68000 &insn) {
    const Config::opcode_t opCode = insn.opCode();
    const EaSize size = moveSize(opCode >> 12);
    const EaMc68000 src(size, opCode >> 3, opCode);
    const EaMc68000 dst(size, opCode >> 6, opCode >> 9);
    if (src.mode == M_AREG && size == SZ_BYTE)
        return setError(ILLEGAL_SIZE);
    if (dst.mode == M_AREG) { // MOVEA
        if (size == SZ_BYTE) return setError(ILLEGAL_SIZE);
    } else {
        if (!dst.satisfy(EaCat::DATA | EaCat::ALTERABLE))
            return setError(ILLEGAL_OPERAND_MODE);
    }
    if (decodeEffectiveAddr(memory, insn, src)) return getError();
    *_operands++ = ',';
    if (decodeEffectiveAddr(memory, insn, dst)) return getError();
    insn.appendSize(size, _regs);
    return setOK();
}

Error DisMc68000::decode(
    DisMemory &memory, Insn &_insn) {
    InsnMc68000 insn(_insn);
    Config::opcode_t opCode;
    if (insn.readUint16(memory, opCode)) return setError(NO_MEMORY);
    insn.setOpCode(opCode);
    if (TableMc68000.searchOpCode(insn))
        return setError(TableMc68000.getError());

    switch (insn.insnFormat()) {
    case IMPLIED:  return decodeImplied(memory, insn);
    case DEST_SIZ: return decodeDestSiz(memory, insn);
    case ADDR_REG: return decodeAddrReg(memory, insn);
    case DATA_REG: return decodeDataReg(memory, insn);
    case MOVE_USP: return decodeMoveUsp(insn);
    case TRAP_VEC: return decodeTrapVec(insn);
    case DATA_DST: return decodeDataDst(memory, insn);
    case DEST_OPR: return decodeDestOpr(memory, insn);
    case SIGN_EXT: return decodeSignExt(insn);
    case RELATIVE: return decodeRelative(memory, insn);
    case MOVE_MLT: return decodeMoveMlt(memory, insn);
    case MOVE_SR:  return decodeMoveSr(memory, insn);
    case AREG_LNG:
    case AREG_SIZ: return decodeAregSiz(memory, insn);
    case DREG_DST: return decodeDregDst(memory, insn);
    case MOVE_QIC: return decodeMoveQic(insn);
    case MOVE_PER: return decodeMovePer(memory, insn);
    case DATA_QIC: return decodeDataQic(memory, insn);
    case DREG_SIZ:
    case DMEM_SIZ: return decodeDmemSiz(memory, insn);
    case DREG_ROT: return decodeDregRot(insn);
    case CMPM_SIZ:
    case DMEM_DST:
    case DMEM_OPR: return decodeDmemOpr(memory, insn);
    case REGS_EXG: return decodeRegsExg(insn);
    case MOVA_OPR:
    case MOVE_OPR: return decodeMoveOpr(memory, insn);
    }
    return setError(UNKNOWN_INSTRUCTION);
}

} // namespace mc68000
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
