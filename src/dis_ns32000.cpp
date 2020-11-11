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

#include "dis_ns32000.h"
#include "table_ns32000.h"

namespace libasm {
namespace ns32000 {

static bool isGenMode(AddrMode mode) {
    return mode == M_GENR || mode == M_GENW || mode == M_GENC
#ifdef NS32000_ENABLE_FLOAT
        || mode == M_FENR || mode == M_FENW
#endif
        ;
}

static uint8_t getOprField(const InsnNs32000 &insn, OprPos pos) {
    if (pos == P_GEN1)
        return (insn.hasPost() ? insn.post() : insn.opCode()) >> 3;
    if (pos == P_GEN2 && insn.hasPost()) {
        const uint8_t post = (insn.post() << 2) & 0x1f;
        return post | (insn.opCode() >> 6);
    }
    if (pos == P_REG)
        return (insn.opCode() >> 3) & 0x7;
    if (pos == P_SHORT && insn.hasPost()) {
        uint8_t val8 = (insn.post() << 1) & 0x0f;
        if (insn.opCode() & 0x80) val8 |= 1;
        if (val8 & 0x8) val8 |= 0xF0;
        return val8;
    }
    return 0;
}

static bool isScaledIndex(uint8_t gen) {
    return gen >= 0x1c;
}

Error DisNs32000::readIndexByte(
        DisMemory &memory, InsnNs32000 &insn, AddrMode mode, OprPos pos) {
    if (isGenMode(mode) && isScaledIndex(getOprField(insn, pos)))
        insn.setIndexByte(insn.readByte(memory), pos);
    return setError(insn);
}

char *DisNs32000::outDisplacement(char *out, const Displacement &disp) {
    switch (disp.bits) {
    case 7:
        out = outDec(out, static_cast<int8_t>(disp.val32), -8);
        break;
    case 14:
        out = outHex(out, static_cast<int16_t>(disp.val32), -16);
        break;
    default:
        out = outHex(out, disp.val32, -30);
        break;
    }
    return out;
}

Error DisNs32000::readDisplacement(
        DisMemory &memory, InsnNs32000 &insn, Displacement &disp) {
    const uint8_t head = insn.readByte(memory);
    // 0xxx|xxxx
    if ((head & 0x80) == 0) {
        const int8_t val8 = (head & 0x40) ? (head | 0x80) : head;
        disp.val32 = val8;
        disp.bits = 7;
    } else {
        const int8_t val8 = (head & 0x20) ? (head | 0xC0) : (head & ~0xC0);
        const int16_t val16 = (static_cast<int16_t>(val8) << 8)
            | insn.readByte(memory);
        // 10xx|xxxx
        if ((head & 0x40) == 0) {
            disp.val32 = static_cast<int32_t>(val16);
            disp.bits = 14;
        } else {
            // 1110|0000 is reserved
            if (head == 0xE0) return setError(ILLEGAL_CONSTANT);
            const int32_t val32 = (static_cast<int32_t>(val16) << 16)
                | insn.readUint16(memory);
            if (val32 < -0x1F000000L || val32 >= 0x20000000L)
                return setError(OVERFLOW_RANGE);
            disp.val32 = val32;
            disp.bits = 30;
        }
    }
    return setError(insn);
}

Error DisNs32000::decodeLength(
    DisMemory &memory, InsnNs32000 &insn, char *out, AddrMode mode) {
    Displacement disp;
    if (readDisplacement(memory, insn, disp)) return getError();
    if (disp.val32 < 0) return setError(ILLEGAL_CONSTANT);
    if (disp.val32 >= 16) return setError(OVERFLOW_RANGE);
    uint8_t len = disp.val32;
    if (mode == M_LEN8) {
        if (len % 2) return setError(ILLEGAL_CONSTANT);
        len /= 2;
    }
    if (mode == M_LEN4) {
        if (len % 4) return setError(ILLEGAL_CONSTANT);
        len /= 4;
    }
    len++;
    outDec(out, len, 5);
    return OK;
}

static char *outComma(char *out) {
    out += strlen(out);
    *out++ = ',';
    *out++ = ' ';
    return out;
}

Error DisNs32000::decodeBitField(
    DisMemory &memory, InsnNs32000 &insn, char *out, AddrMode mode) {
    if (mode == M_BFLEN) return OK;
    const uint8_t data = insn.readByte(memory);
    const uint8_t len = (data & 0x1F) + 1;
    const uint8_t off = (data >> 5);
    out = outHex(out, off, 3);  // M_BFOFF
    out = outComma(out);
    outDec(out, len, 6);  // M_BFLEN
    return setError(insn);
}

Error DisNs32000::decodeImmediate(
    DisMemory &memory, InsnNs32000 &insn, char *out, AddrMode mode) {
    const OprSize size = (mode == M_GENC) ? SZ_BYTE : insn.oprSize();
    switch (size) {
    case SZ_BYTE:
        if (mode == M_GENC) {
            outDec(out, static_cast<int8_t>(insn.readByte(memory)), -8);
        } else {
            outHex(out, insn.readByte(memory), 8);
        }
        break;
    case SZ_WORD:
        outHex(out, insn.readUint16(memory), 16);
        break;
    case SZ_LONG:
        outHex(out, insn.readUint32(memory), 32);
        break;
#ifdef NS32000_ENABLE_FLOAT
    case SZ_FLOAT:
        outHex(out, insn.readUint32(memory), 32);
        break;
    case SZ_DOUBLE:
        // TODO: double
        out = outHex(out, insn.readUint32(memory), 32);
        *out++ = ':';
        outHex(out, insn.readUint32(memory), 32);
        break;
#endif
    default:
        break;
    }
    return setError(insn);
}

Error DisNs32000::decodeDisplacement(
    DisMemory &memory, InsnNs32000 &insn, char *out, AddrMode mode) {
    Displacement disp;
    if (readDisplacement(memory, insn, disp)) return getError();
    if (mode == M_LEN32) {
        if (disp.val32 <= 0) return setError(ILLEGAL_CONSTANT);
        if (disp.val32 > 32) return setError(OVERFLOW_RANGE);
        outDec(out, disp.val32, 6);
    }
    if (mode == M_DISP)
        outDisplacement(out, disp);
    return OK;
}

Error DisNs32000::decodeRelative(
    DisMemory &memory, InsnNs32000 &insn, char *out) {
    Displacement disp;
    if (readDisplacement(memory, insn, disp)) return getError();
    const Config::uintptr_t target = insn.address() + disp.val32;
    outRelAddr(out, target, insn.address(), disp.bits);
    return OK;
}

Error DisNs32000::decodeConfig(
    const InsnNs32000 &insn, char *out, OprPos pos) {
    const uint8_t configs = getOprField(insn, pos);
    _regs.outConfigNames(out, configs);
    return OK;
}

Error DisNs32000::decodeStrOpt(
    const InsnNs32000 &insn, char *out, OprPos pos) {
    const uint8_t strOpts = getOprField(insn, pos);
    _regs.outStrOptNames(out, strOpts);
    return OK;
}

Error DisNs32000::decodeRegisterList(
    DisMemory &memory, InsnNs32000 &insn, char *out, AddrMode mode) {
    uint8_t list = insn.readByte(memory);
    if (setError(insn)) return getError();
    if (list == 0) return setError(OPCODE_HAS_NO_EFFECT);
    const uint8_t mask = (mode == M_POP) ? 0x80 : 0x01;
    *out++ = '[';
    char sep = 0;
    for (uint8_t reg = 0; list; reg++) {
        if (list & mask) {
            if (sep) *out++ = sep;
            out = _regs.outRegName(out, RegNs32000::decodeRegName(reg));
            sep = ',';
        }
        if (mode == M_POP) list <<= 1;
        else list >>= 1;
    }
    *out++ = ']';
    *out = 0;
    return OK;
}

Error DisNs32000::decodeGeneric(
    DisMemory &memory, InsnNs32000 &insn, char *out,
    AddrMode mode, OprPos pos) {
    uint8_t gen = getOprField(insn, pos);
    RegName index = REG_UNDEF;
    OprSize indexSize = SZ_NONE;
    if (isScaledIndex(gen)) {
        indexSize = OprSize(gen & 0x3);
        const uint8_t indexByte = insn.indexByte(pos);
        index = RegNs32000::decodeRegName(indexByte);
        gen = indexByte >> 3;
    }
    Displacement disp, disp2;
    RegName reg;
    switch (gen) {
    case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7:
#ifdef NS32000_ENABLE_FLOAT
        reg = RegNs32000::decodeRegName(gen, (mode == M_FENR || mode == M_FENW));
#else
        reg = RegNs32000::decodeRegName(gen);
#endif
        out = _regs.outRegName(out, reg);
        break;
    case 8: case 9: case 10: case 11: case 12: case 13: case 14: case 15:
        reg = RegNs32000::decodeRegName(gen);
        if (readDisplacement(memory, insn, disp)) return getError();
        out = outDisplacement(out, disp);
        *out++ = '(';
        out = _regs.outRegName(out, reg);
        *out++ = ')';
        break;
    case 0x10: reg = REG_FP; goto m_mrel;
    case 0x11: reg = REG_SP; goto m_mrel;
    case 0x12: reg = REG_SB; goto m_mrel;
    m_mrel:
        if (readDisplacement(memory, insn, disp)) return getError();
        if (readDisplacement(memory, insn, disp2)) return getError();
        out = outDisplacement(out, disp2);
        *out++ = '(';
        out = outDisplacement(out, disp);
        *out++ = '(';
        out = _regs.outRegName(out, reg);
        *out++ = ')';
        *out++ = ')';
        break;
    case 0x13:
        return setError(ILLEGAL_OPERAND_MODE);
    case 0x14:
#ifdef NS32000_ENABLE_FLOAT
        if (mode == M_GENW || mode == M_FENW)
            return setError(OPERAND_NOT_ALLOWED);
#else
        if (mode == M_GENW)
            return setError(OPERAND_NOT_ALLOWED);
#endif
        return decodeImmediate(memory, insn, out, mode);
    case 0x15:
        if (readDisplacement(memory, insn, disp)) return getError();
        *out++ = '@';
        out = outDisplacement(out, disp);
        break;
    case 0x16:
        if (readDisplacement(memory, insn, disp)) return getError();
        if (readDisplacement(memory, insn, disp2)) return getError();
        out = _regs.outRegName(out, REG_EXT);
        *out++ = '(';
        out = outDisplacement(out, disp);
        *out++ = ')';
        *out++ = '+';
        if (disp2.val32 < 0) *out++ = '(';
        out = outDisplacement(out, disp2);
        if (disp2.val32 < 0) *out++ = ')';
        break;
    case 0x17:
        out = _regs.outRegName(out, REG_TOS);
        break;
    case 0x18: reg = REG_FP; goto m_mem;
    case 0x19: reg = REG_SP; goto m_mem;
    case 0x1A: reg = REG_SB; goto m_mem;
    case 0x1B: reg = REG_PC; goto m_mem;
    m_mem:
        if (readDisplacement(memory, insn, disp)) return getError();
        if (reg == REG_PC) {
            const Config::uintptr_t target = insn.address() + disp.val32;
            out = outRelAddr(out, target, insn.address(), disp.bits);
        } else {
            out = outDisplacement(out, disp);
        }
        *out++ = '(';
        out = _regs.outRegName(out, reg);
        *out++ = ')';
        break;
    default:
        return setError(ILLEGAL_OPERAND_MODE);
    }
    if (index != REG_UNDEF) {
        *out++ = '[';
        out = _regs.outRegName(out, index);
        *out++ = ':';
        *out++ = _regs.indexSizeChar(indexSize);
        *out++ = ']';
    }
    *out = 0;
    return OK;
}

Error DisNs32000::decodeOperand(
    DisMemory &memory, InsnNs32000 &insn, char*out,
    AddrMode mode, OprPos pos) {
    const uint8_t field = getOprField(insn, pos);
    switch (mode) {
    case M_GREG:
        out = _regs.outRegName(out, RegNs32000::decodeRegName(field));
        break;
    case M_PREG: {
        const PregName preg = RegNs32000::decodePregName(field);
        if (preg == PREG_UNDEF) return setError(UNKNOWN_REGISTER);
        out = _regs.outPregName(out, preg);
        break;
    }
#ifdef NS32000_ENABLE_MMU
    case M_MREG:
        out = _regs.outMregName(out, RegNs32000::decodeMregName(field));
        break;
#endif
    case M_CONF:
        return decodeConfig(insn, out, pos);
    case M_SOPT:
        return decodeStrOpt(insn, out, pos);
    case M_PUSH:
    case M_POP:
        return decodeRegisterList(memory, insn, out, mode);
    case M_GENR:
    case M_GENC:
    case M_GENW:
#ifdef NS32000_ENABLE_FLOAT
    case M_FENW:
    case M_FENR:
#endif
        return decodeGeneric(memory, insn, out, mode, pos);
    case M_INT4:
        outDec(out, static_cast<int8_t>(getOprField(insn, pos)), -4);
        break;
    case M_BFOFF:
    case M_BFLEN:
        return decodeBitField(memory, insn, out, mode);
    case M_REL:
        return decodeRelative(memory, insn, out);
    case M_DISP:
    case M_LEN32:
        return decodeDisplacement(memory, insn, out, mode);
    case M_LEN16:
    case M_LEN8:
    case M_LEN4:
        return decodeLength(memory, insn, out, mode);
    default:
        return setError(INTERNAL_ERROR);
    }
    return OK;
}

Error DisNs32000::decode(DisMemory &memory, Insn &_insn, char *out) {
    InsnNs32000 insn(_insn);
    Config::opcode_t opCode = insn.readByte(memory);
    insn.setOpCode(opCode);
    if (TableNs32000.isPrefixCode(opCode)) {
        const Config::opcode_t prefix = opCode;
        opCode = insn.readByte(memory);
        insn.setOpCode(opCode, prefix);
    }
    if (setError(insn)) return getError();

    if (TableNs32000.searchOpCode(insn, memory))
        return setError(TableNs32000.getError());
    if (readIndexByte(memory, insn, insn.srcMode(), insn.srcPos()))
        return getError();
    if (readIndexByte(memory, insn, insn.dstMode(), insn.dstPos()))
        return getError();
    if (readIndexByte(memory, insn, insn.ex1Mode(), insn.ex1Pos()))
        return getError();

    const AddrMode src = insn.srcMode();
    if (src == M_NONE) return setOK();
    if (decodeOperand(memory, insn, out, src, insn.srcPos()))
        return getError();
    const AddrMode dst = insn.dstMode();
    if (dst == M_NONE) return setOK();
    out = outComma(out);
    if (decodeOperand(memory, insn, out, dst, insn.dstPos()))
        return getError();
    const AddrMode ex1 = insn.ex1Mode();
    if (ex1 == M_NONE) return setOK();
    out = outComma(out);
    if (decodeOperand(memory, insn, out, ex1, insn.ex1Pos()))
        return getError();
    const AddrMode ex2 = insn.ex2Mode();
    if (ex2 == M_NONE || ex2 == M_BFLEN) return setOK();
    out = outComma(out);
    if (decodeOperand(memory, insn, out, ex2, insn.ex2Pos()))
        return getError();
    return setOK();
}

} // namespace ns32000
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
