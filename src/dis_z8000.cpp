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

#include "dis_z8000.h"

#include "table_z8000.h"

namespace libasm {
namespace z8000 {

const char DisZ8000::OPT_BOOL_IOADDR_PREFIX[] PROGMEM = "ioaddr-prefix";
const char DisZ8000::OPT_DESC_IOADDR_PREFIX[] PROGMEM = "I/O address prefix # (default none)";
const char DisZ8000::OPT_BOOL_SHORT_DIRECT[] PROGMEM = "short-direct";
const char DisZ8000::OPT_DESC_SHORT_DIRECT[] PROGMEM = "short direct addressing as ||";

StrBuffer &DisZ8000::outRegister(StrBuffer &out, RegName name) {
    return _regs.outRegName(out, name);
}

StrBuffer &DisZ8000::outConditionCode(StrBuffer &out, uint8_t num) {
    const CcName cc = _regs.decodeCcNum(num);
    return _regs.outCcName(out, cc);
}

StrBuffer &DisZ8000::outImmediate(StrBuffer &out, uint8_t data, AddrMode mode) {
    uint8_t val = data;
    if (mode == M_CNT)
        val = data + 1;
    if (mode == M_QCNT)
        val = (data & 2) ? 2 : 1;
    return outHex(out.letter('#'), val, 8);
}

Error DisZ8000::decodeImmediate(
        DisMemory &memory, InsnZ8000 &insn, StrBuffer &out, AddrMode mode, OprSize size) {
    out.letter('#');
    if (mode == M_SCNT || mode == M_NCNT) {
        uint16_t data = insn.readUint16(memory);
        if (size == SZ_BYTE && (data & 0xFF00) != 0)
            return setError(ILLEGAL_OPERAND);
        const int16_t count =
                (size == SZ_BYTE) ? static_cast<int8_t>(data) : static_cast<int16_t>(data);
        if (count < 0) {
            if (TableZ8000::TABLE.searchOpCodeAlias(insn, memory))
                return setError(insn);
            data = -count;
        }
        if (size == SZ_BYTE && data > 8)
            return setError(ILLEGAL_OPERAND);
        if (size == SZ_WORD && data > 16)
            return setError(ILLEGAL_OPERAND);
        if (size == SZ_LONG && data > 32)
            return setError(ILLEGAL_OPERAND);
        outDec(out, data, 6);
    } else if (size == SZ_BYTE) {
        outHex(out, insn.readUint16(memory), 8);
    } else if (size == SZ_WORD) {
        outHex(out, insn.readUint16(memory), 16);
    } else if (size == SZ_LONG) {
        outHex(out, insn.readUint32(memory), 32);
    }
    return setError(insn);
}

Error DisZ8000::decodeFlags(StrBuffer &out, uint8_t flags) {
    flags &= 0xF;
    if (flags == 0)
        return setError(OPCODE_HAS_NO_EFFECT);
    _regs.outFlagNames(out, flags);
    return OK;
}

Error DisZ8000::decodeGeneralRegister(StrBuffer &out, uint8_t num, OprSize size, bool indirect) {
    const RegName reg = _regs.decodeRegNum(num, size);
    if (reg == REG_ILLEGAL)
        return setError(ILLEGAL_REGISTER);
    if (indirect)
        out.letter('@');
    outRegister(out, reg);
    return OK;
}

Error DisZ8000::decodeDoubleSizedRegister(StrBuffer &out, uint8_t num, OprSize size) {
    if (size == SZ_BYTE)
        return decodeGeneralRegister(out, num, SZ_WORD);
    if (size == SZ_WORD)
        return decodeGeneralRegister(out, num, SZ_LONG);
    if (size == SZ_LONG)
        return decodeGeneralRegister(out, num, SZ_QUAD);
    return setError(INTERNAL_ERROR);
}

Error DisZ8000::decodeControlRegister(StrBuffer &out, uint8_t ctlNum, OprSize size) {
    const RegName reg = _regs.decodeCtlReg(ctlNum);
    if (reg == REG_ILLEGAL)
        return setError(ILLEGAL_REGISTER);
    if (size == SZ_BYTE && reg != REG_FLAGS)
        return setError(ILLEGAL_SIZE);
    if (size == SZ_WORD && reg == REG_FLAGS)
        return setError(ILLEGAL_SIZE);
    outRegister(out, reg);
    return OK;
}

Error DisZ8000::decodeBaseAddressing(
        DisMemory &memory, InsnZ8000 &insn, StrBuffer &out, AddrMode mode, uint8_t num) {
    num &= 0xf;
    if (num == 0)
        return setError(REGISTER_NOT_ALLOWED);
    if (decodeGeneralRegister(out, num, SZ_ADDR))
        return getError();
    out.letter('(');
    if (mode == M_BA) {
        outHex(out.letter('#'), insn.readUint16(memory), -16);
    } else {  // M_BX
        if (decodeGeneralRegister(out, insn.post() >> 8, SZ_WORD))
            return getError();
    }
    out.letter(')');
    return setError(insn);
}

Error DisZ8000::decodeGenericAddressing(
        DisMemory &memory, InsnZ8000 &insn, StrBuffer &out, AddrMode mode, uint8_t num) {
    num &= 0xF;
    const uint8_t addressing = insn.opCode() >> 14;
    if (addressing == 0 && num == 0 && mode == M_GENI) {  // M_IM
        return decodeImmediate(memory, insn, out, M_IM, insn.size());
    }
    if (addressing == 2 && num && (mode == M_GENI || mode == M_GEND)) {  // M_R
        return decodeGeneralRegister(out, num, insn.size());
    }
    if (addressing == 0 && num) {  // M_IR
        return decodeGeneralRegister(out, num, SZ_ADDR, true);
    }
    if (addressing == 1) {  // M_DA/M_X
        if (decodeDirectAddress(memory, insn, out))
            return getError();
        if (num) {
            out.letter('(');
            if (decodeGeneralRegister(out, num, SZ_WORD))
                return getError();
            out.letter(')');
        }
        return OK;
    }
    return setError(INTERNAL_ERROR);
}

Error DisZ8000::decodeDirectAddress(DisMemory &memory, InsnZ8000 &insn, StrBuffer &out) {
    const uint16_t addr = insn.readUint16(memory);
    if (TableZ8000::TABLE.segmentedModel()) {
        const uint32_t seg = static_cast<uint32_t>(addr & 0x7F00) << 8;
        uint16_t off = static_cast<uint8_t>(addr);
        bool shortDirect = _shortDirect;
        if (addr & 0x8000) {
            if (addr & 0x00FF)
                setError(ILLEGAL_OPERAND);
            off = insn.readUint16(memory);
            shortDirect = false;
        }
        const uint32_t linear = seg | off;
        if (shortDirect)
            out.letter('|');
        outAbsAddr(out, linear);
        if (shortDirect)
            out.letter('|');
    } else {
        outAbsAddr(out, addr);
    }
    return setError(insn);
}

Error DisZ8000::decodeRelativeAddressing(
        DisMemory &memory, InsnZ8000 &insn, StrBuffer &out, AddrMode mode) {
    int16_t delta = 0;
    if (mode == M_RA) {
        delta = static_cast<int16_t>(insn.readUint16(memory));
    }
    if (mode == M_RA12) {
        // Sign extends 12-bit number as 0x800 is a sign bit.
        const int16_t ra12 = (insn.opCode() & 0x7FF) - (insn.opCode() & 0x800);
        delta = -ra12 * 2;
    }
    if (mode == M_RA8) {
        // Sign extends 8-bit number as 0x80 is a sign bit
        const int16_t ra8 = (insn.opCode() & 0x7F) - (insn.opCode() & 0x80);
        delta = ra8 * 2;
    }
    if (mode == M_RA7) {
        // Unsigned 7-bit as always negative offset.
        const uint16_t ra7 = insn.opCode() & 0x7F;
        delta = -static_cast<int16_t>(ra7) * 2;
    }
    const Config::uintptr_t base = insn.address() + insn.length();
    const Config::uintptr_t target = base + delta;
    if (mode == M_RA12 && (target % 2) != 0)
        return setError(OPERAND_NOT_ALIGNED);
    outRelAddr(out, target, insn.address(), mode == M_RA ? 16 : 13);
    return setError(insn);
}

static uint8_t modeField(const InsnZ8000 &insn, ModeField field) {
    switch (field) {
    case MF_C0:
        return (insn.opCode() & 0xF);
    case MF_C4:
        return (insn.opCode() >> 4) & 0xF;
    case MF_C8:
        return (insn.opCode() >> 8) & 0xF;
    case MF_P0:
        return (insn.post() & 0xF);
    case MF_P4:
        return (insn.post() >> 4) & 0xF;
    case MF_P8:
        return (insn.post() >> 8) & 0xF;
    default:
        return 0;
    }
}

Error DisZ8000::decodeOperand(
        DisMemory &memory, InsnZ8000 &insn, StrBuffer &out, AddrMode mode, ModeField field) {
    uint8_t num = modeField(insn, field);
    switch (mode) {
    case M_NONE:
        return OK;
    case M_BA:
    case M_BX:
        return decodeBaseAddressing(memory, insn, out, mode, num);
    case M_CC:
        outConditionCode(out, num);
        return OK;
    case M_CTL:
        return decodeControlRegister(out, num, insn.size());
    case M_FLAG:
        return decodeFlags(out, num);
    case M_IM8:
        outImmediate(out, insn.opCode(), mode);
        return OK;
    case M_IO:
        if (_ioAddressPrefix)
            out.letter('#');
        outAbsAddr(out, insn.readUint16(memory), 16);
        return OK;
    case M_IRIO:
        if (num == 0)
            return setError(REGISTER_NOT_ALLOWED);
        return decodeGeneralRegister(out, num, SZ_WORD, true);
    case M_IR:
        if (num == 0)
            return setError(REGISTER_NOT_ALLOWED);
        return decodeGeneralRegister(out, num, SZ_ADDR, true);
    case M_INTR:
        num &= 3;
        if (num == 3)
            return setError(OPCODE_HAS_NO_EFFECT);
        _regs.outIntrNames(out, num);
        return OK;
    case M_RA:
    case M_RA12:
    case M_RA8:
    case M_RA7:
        return decodeRelativeAddressing(memory, insn, out, mode);
    case M_DR:
        return decodeDoubleSizedRegister(out, num, insn.size());
    case M_R:
        return decodeGeneralRegister(out, num, insn.size());
    case M_WR07:
        if (num >= 8)
            return setError(REGISTER_NOT_ALLOWED);
        /* Fall-through */
    case M_WR:
        return decodeGeneralRegister(out, num, SZ_WORD);
    case M_DA:
        return decodeDirectAddress(memory, insn, out);
    case M_GENA:
    case M_GEND:
    case M_GENI:
        return decodeGenericAddressing(memory, insn, out, mode, num);
    case M_BIT:
        if (insn.size() == SZ_BYTE && num >= 8)
            return setError(ILLEGAL_BIT_NUMBER);
        /* Fall-through */
    case M_CNT:
    case M_QCNT:
        outImmediate(out, num, mode);
        return OK;
    case M_IM:
    case M_SCNT:
    case M_NCNT:
        return decodeImmediate(memory, insn, out, mode, insn.size());
    case M_X:
        return decodeGenericAddressing(memory, insn, out, mode, num);
    default:
        return setError(UNKNOWN_INSTRUCTION);
    }
}

Error DisZ8000::checkPostWord(const InsnZ8000 &insn) {
    if (insn.hasPost()) {
        uint16_t mask;
        switch (insn.postFormat()) {
        case PF_0X0X:
            mask = 0x0F0F;
            break;
        case PF_0X00:
            mask = 0x0F00;
            break;
        case PF_0XXX:
            mask = 0x0FFF;
            break;
        default:
            mask = 0x0FF0;
            break;
        }
        if ((insn.post() & ~mask) == insn.postVal())
            return OK;
        return setError(UNKNOWN_POSTBYTE);
    }
    return OK;
}

static OprSize registerSize(const InsnZ8000 &insn, AddrMode mode) {
    if (mode == M_IR)
        return SZ_ADDR;
    if (mode == M_GEND && (insn.opCode() >> 14) == 2)  // M_R
        return insn.size();
    return SZ_WORD;
}

Error DisZ8000::checkRegisterOverlap(const InsnZ8000 &insn) {
    const AddrMode dmode = insn.dst();
    const AddrMode smode = insn.src();
    const uint8_t dnum = modeField(insn, insn.dstField());
    const uint8_t snum = modeField(insn, insn.srcField());
    const OprSize dsize = registerSize(insn, dmode);
    const OprSize ssize = registerSize(insn, smode);
    const RegName dst = RegZ8000::decodeRegNum(dnum, dsize);
    const RegName src = RegZ8000::decodeRegNum(snum, ssize);
    if (insn.isPushPopInsn()) {
        if (RegZ8000::checkOverlap(dst, src))
            return setError(REGISTERS_OVERLAPPED);
        return OK;
    }
    if (dmode == M_IR && dnum == 0)
        return setError(REGISTER_NOT_ALLOWED);
    if (snum == 0)
        return setError(REGISTER_NOT_ALLOWED);
    const uint8_t cnum = modeField(insn, MF_P8);
    const RegName cnt = RegZ8000::decodeRegNum(modeField(insn, MF_P8), SZ_WORD);
    if (insn.isTranslateInsn()) {
        // @R1 isn't allowed as dst/src.
        if (!TableZ8000::TABLE.segmentedModel() && (dnum == 1 || snum == 1))
            return setError(REGISTER_NOT_ALLOWED);
        // R1 isn't allowed as cnt.
        if (cnum == 1)
            return setError(REGISTER_NOT_ALLOWED);
    }

    if (dst == REG_ILLEGAL || src == REG_ILLEGAL)
        return OK;
    if (RegZ8000::checkOverlap(dst, src, cnt))
        return setError(REGISTERS_OVERLAPPED);
    return OK;
}

StrBuffer &DisZ8000::outComma(
        StrBuffer &out, const InsnZ8000 &insn, AddrMode mode, ModeField field) {
    if (mode == M_CC && _regs.decodeCcNum(modeField(insn, field)) == CC_T)
        return out;
    return out.comma();
}

Error DisZ8000::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) {
    InsnZ8000 insn(_insn);
    const Config::opcode_t opCode = insn.readUint16(memory);
    if (setError(insn))
        return getError();
    insn.setOpCode(opCode);

    if (TableZ8000::TABLE.searchOpCode(insn, memory))
        return setError(insn);
    if (checkPostWord(insn))
        return getError();
    if ((insn.isPushPopInsn() || insn.isThreeRegsInsn()) && checkRegisterOverlap(insn))
        return getError();
    if (insn.isLoadMultiInsn()) {
        const uint8_t reg = modeField(insn, MF_P8);
        const uint8_t cnt = modeField(insn, MF_P0);
        if (reg + cnt >= 16)
            return setError(OVERFLOW_RANGE);
    }

    const AddrMode dst = insn.dst();
    if (dst == M_NONE)
        return setOK();
    if (decodeOperand(memory, insn, out, dst, insn.dstField()))
        return getError();
    const AddrMode src = insn.src();
    if (src == M_NONE)
        return setOK();
    outComma(out, insn, dst, insn.dstField());
    if (decodeOperand(memory, insn, out, src, insn.srcField()))
        return getError();
    const AddrMode ex1 = insn.ex1();
    if (ex1 == M_NONE)
        return setOK();
    const ModeField ex1Field = (ex1 == M_CNT ? MF_P0 : MF_P8);
    outComma(out, insn, ex1, ex1Field);
    if (decodeOperand(memory, insn, out, ex1, ex1Field))
        return getError();
    const AddrMode ex2 = insn.ex2();
    if (ex2 == M_NONE)
        return setOK();
    outComma(out, insn, ex2, MF_P0);
    return decodeOperand(memory, insn, out, ex2, MF_P0);
}

}  // namespace z8000
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
