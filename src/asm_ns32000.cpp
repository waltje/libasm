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

#include <stdio.h>

#include "asm_ns32000.h"
#include "table_ns32000.h"
#include "reg_ns32000.h"

namespace libasm {
namespace ns32000 {

bool AsmNs32000::endOfLine(const char *scan) const {
    return (*scan == '#') || Assembler::endOfLine(scan);
}

Error AsmNs32000::parseConfigNames(const char *p, Operand &op, bool bracket) {
    uint8_t configs = 0;
    while (true) {
        const ConfigName name = _regs.parseConfigName(p);
        if (name == CONFIG_UNDEF) return UNKNOWN_OPERAND;
        p += _regs.configNameLen(name);
        configs |= uint8_t(name);
        p = skipSpaces(p);
        if (bracket) {
            if (*p == ',') {
                ;
            } else if (*p == ']') {
                p++;
                break;
            } else {
                return UNKNOWN_OPERAND;
            }
        } else {
            if (*p == '/') {
                ;
            } else if (endOfLine(p)) {
                break;
            } else {
                return UNKNOWN_OPERAND;
            }
        }
        p = skipSpaces(p + 1);
    }
    _scan = p;
    op.val32 = configs;
    op.mode = M_CONF;
    return setOK();
}

Error AsmNs32000::parseStrOptNames(const char *p, Operand &op, bool bracket) {
    uint8_t strOpt = 0;
    while (true) {
        const StrOptName name = _regs.parseStrOptName(p);
        if (name == STROPT_UNDEF) return UNKNOWN_OPERAND;
        p += _regs.strOptNameLen(name);
        if (strOpt & uint8_t(name)) return setError(ILLEGAL_OPERAND);
        strOpt |= uint8_t(name);
        p = skipSpaces(p);
        if (bracket) {
            if (*p == ',') {
                ;
            } else if (*p == ']') {
                p++;
                break;
            } else {
                return UNKNOWN_OPERAND;
            }
        } else {
            if (*p == '/') {
                ;
            } else if (endOfLine(p)) {
                break;
            } else {
                return UNKNOWN_OPERAND;
            }
        }
        p = skipSpaces(p + 1);
    }
    _scan = p;
    op.val32 = strOpt;
    op.mode = M_SOPT;
    return setOK();
}

Error AsmNs32000::parseRegisterList(const char *p, Operand &op, bool bracket) {
    uint8_t list = 0;
    uint8_t n = 0;
    while (true) {
        const RegName name = _regs.parseRegName(p);
        if (!_regs.isGeneric(name)) return UNKNOWN_OPERAND;
        list |= (1 << _regs.encodeRegName(name));
        n++;
        p = skipSpaces(p + _regs.regNameLen(name));
        if (bracket) {
            if (*p == ',') {
                ;
            } else if (*p == ']') {
                p++;
                break;
            } else {
                return UNKNOWN_OPERAND;
            }
        } else {
            if (*p == '/') {
                ;
            } else if (endOfLine(p) || *p == ',') {
                break;
            } else {
                return UNKNOWN_OPERAND;
            }
        }
        p = skipSpaces(p + 1);
    }
    if (!bracket && n == 1)     // a single register
        return UNKNOWN_OPERAND;
    _scan = p;
    op.val32 = list;
    op.mode = M_PUSH;
    return setOK();
}

Error AsmNs32000::parseBaseOperand(Operand &op) {
    const char *p = _scan;
    if (endOfLine(p)) return OK;
    if (*p == '@') {
        _scan = p + 1;
        if (getOperand(op.val32)) return getError();
        op.setError(getError());
        op.mode = M_ABS;
        return OK;
    }
    const bool bracket = (*p == '[');
    if (bracket) p = skipSpaces(p + 1);
    if (parseConfigNames(p, op, bracket) != UNKNOWN_OPERAND)
        return getError();
    if (parseStrOptNames(p, op, bracket) != UNKNOWN_OPERAND)
        return getError();
    if (parseRegisterList(p, op, bracket) != UNKNOWN_OPERAND)
        return getError();
    if (bracket) return setError(MISSING_CLOSING_PAREN);

    const PregName preg = _regs.parsePregName(p);
    if (preg != PREG_UNDEF) {
        _scan = p + _regs.pregNameLen(preg);
        op.val32 = _regs.encodePregName(preg);
        op.mode = M_PREG;
        return OK;
    }
    const MregName mreg = _regs.parseMregName(p);
    if (mreg != MREG_UNDEF) {
        _scan = p + _regs.mregNameLen(mreg);
        op.val32 = _regs.encodeMregName(mreg);
        op.mode = M_MREG;
        return OK;
    }
    RegName reg = _regs.parseRegName(p);
    if (reg != REG_UNDEF) {
        p += _regs.regNameLen(reg);
        if (_regs.isGeneric(reg)) {
            _scan = p;
            op.reg = reg;
            op.mode = M_GREG;
            return OK;
        }
        if (_regs.isFloat(reg)) {
            _scan = p;
            op.reg = reg;
            op.mode = M_FREG;
            return OK;
        }
        if (reg == REG_TOS) {
            _scan = p;
            op.mode = M_TOS;
            return OK;
        }
        if (reg == REG_EXT) {
            if (*p != '(')
                return setError(UNKNOWN_OPERAND);
            _scan = p + 1;
            if (getOperand(op.val32)) return getError();
            op.setError(getError());
            p = skipSpaces(_scan);
            if (*p++ != ')')
                return setError(MISSING_CLOSING_PAREN);
            if (*(p = skipSpaces(p)) != '+')
                return setError(UNKNOWN_OPERAND);
            _scan = p + 1;
            if (getOperand(op.disp2)) return getError();
            op.setErrorIf(getError());
            op.mode = M_EXT;
            return OK;
        }
        return setError(UNKNOWN_REGISTER);
    }

    if (getOperand(op.val32)) return getError();
    op.setError(getError());
    p = skipSpaces(_scan);
    if (endOfLine(p) || *p == ',' || *p == '[') {
        _scan = p;
        op.mode = M_IMM; // M_REL
        return OK;
    }
    if (*p++ != '(') return setError(UNKNOWN_OPERAND);
    reg = _regs.parseRegName(p);
    if (reg != REG_UNDEF) {
        p += _regs.regNameLen(reg);
        if (*p++ != ')')
            return setError(MISSING_CLOSING_PAREN);
        if (_regs.isGeneric(reg)) {
            _scan = p;
            op.reg = reg;
            op.mode = M_RREL;
            return OK;
        }
        if (reg == REG_FP || reg == REG_SP || reg == REG_SB || reg == REG_PC) {
            _scan = p;
            op.reg = reg;
            op.mode = M_MEM;
            return OK;
        }
        return setError(UNKNOWN_OPERAND);
    }

    _scan = p;
    if (getOperand(op.disp2)) return getError();
    op.setErrorIf(getError());
    p = skipSpaces(_scan);
    if (*p++ != '(')
        return setError(UNKNOWN_OPERAND);
    reg = _regs.parseRegName(p);
    if (reg != REG_UNDEF) {
        p += _regs.regNameLen(reg);
        if (*p++ != ')')
            return setError(MISSING_CLOSING_PAREN);
        if (*(p = skipSpaces(p)) != ')')
            return setError(MISSING_CLOSING_PAREN);
        if (reg == REG_FP || reg == REG_SP || reg == REG_SB) {
            _scan = p + 1;
            op.reg = reg;
            op.mode = M_MREL;
            return OK;
        }
        return setError(UNKNOWN_OPERAND);
    }

    return setError(UNKNOWN_OPERAND);
}

Error AsmNs32000::parseOperand(Operand &op) {
    if (parseBaseOperand(op)) return getError();
    if (op.mode == M_GREG || op.mode == M_RREL || op.mode == M_MREL
        || op.mode == M_ABS || op.mode == M_EXT || op.mode == M_TOS
        || op.mode == M_MEM) {
        const char *p = skipSpaces(_scan);
        if (*p++ != '[') return OK;
        const RegName index = _regs.parseRegName(p);
        if (!_regs.isGeneric(index)) return setError(UNKNOWN_OPERAND);
        p += _regs.regNameLen(index);
        if (*p++ != ':') return setError(UNKNOWN_OPERAND);
        const OprSize indexSize = _regs.parseIndexSize(p);
        if (indexSize == SZ_NONE) return setError(UNKNOWN_OPERAND);
        p += _regs.indexSizeLen(indexSize);
        if (*p++ != ']') return setError(MISSING_CLOSING_PAREN);
        _scan = p;
        op.index = index;
        op.indexSize = indexSize;
    }
    return OK;
}

static uint16_t reverseBits(uint8_t bits) {
    uint8_t reverse = 0;
    for (uint8_t i = 0; ; i++) {
        if (bits & 1) reverse |= 1;
        if (i == 7) break;
        bits >>= 1;
        reverse <<= 1;
    }
    return reverse;
}

static uint8_t encodeScaledIndex(OprSize indexSize) {
    switch (indexSize) {
    case SZ_BYTE: return 0x1C;
    case SZ_WORD: return 0x1D;
    case SZ_LONG: return 0x1E;
    case SZ_QUAD: return 0x1F;
    default: return 0;
    }
}

static void embedOprField(InsnNs32000 &insn, OprPos pos, uint8_t opr) {
    if (pos == P_GEN1) {
        opr &= 0x1F;
        opr <<= 3;
        if (insn.hasPost()) insn.embedPost(opr);
        else insn.embed(opr);
    }
    if (pos == P_GEN2) {
        opr &= 0x1F;
        insn.embedPost(opr >> 2);
        insn.embed(opr << 6);
    }
    if (pos == P_REG) {
        opr &= 0x7;
        insn.embed(opr << 3);
    }
    if (pos == P_SHORT) {
        opr &= 0xF;
        insn.embedPost(opr >> 1);
        insn.embed(opr << 7);
    }
}

Error AsmNs32000::emitDisplacement(InsnNs32000 &insn, uint32_t val32) {
    const int32_t val = static_cast<int32_t>(val32);
    if (val >= -0x40 && val < 0x40) {
        const uint8_t disp = static_cast<uint8_t>(val) & 0x7F;
        insn.emitOperand8(disp);
        return OK;
    }
    if (val >= -0x2000 && val < 0x2000) {
        const uint16_t disp = (static_cast<uint16_t>(val) & 0x3FFF) | 0x8000;
        insn.emitOperand16(disp);
        return OK;
    }
    if (val >= -0x1F000000L && val < 0x20000000L) {
        const uint32_t disp =
            (static_cast<uint32_t>(val) & 0x3FFFFFFF) | 0xC0000000;
        insn.emitOperand32(disp);
        return OK;
    }
    return setError(OVERFLOW_RANGE);
}

Error AsmNs32000::emitLength(
    InsnNs32000 &insn, AddrMode mode, const Operand &op) {
    uint8_t len = op.getError() ? 0 : op.val32;
    if (op.getError() == OK) {
        const int32_t val = static_cast<int32_t>(op.val32);
        if (val <= 0) return setError(ILLEGAL_CONSTANT);
        if (val > 16) return setError(OVERFLOW_RANGE);
        if (mode == M_LEN4) {
            if (len > 4) return setError(OVERFLOW_RANGE);
            len -= 1;
            len *= 4;
        }
        if (mode == M_LEN8) {
            if (len > 8) return setError(OVERFLOW_RANGE);
            len -= 1;
            len *= 2;
        }
        if (mode == M_LEN16) {
            if (len > 16) return setError(OVERFLOW_RANGE);
            len -= 1;
        }
    }
    return emitDisplacement(insn, len);
}

Error AsmNs32000::emitBitField(
    InsnNs32000 &insn, AddrMode mode, const Operand *offOp, const Operand &lenOp) {
    if (mode == M_BFOFF) return OK;
    if (offOp->val32 >= 8) return setError(ILLEGAL_BIT_NUMBER);
    if (lenOp.getError() == OK && lenOp.val32 == 0)
        return setError(ILLEGAL_CONSTANT);
    if (lenOp.val32 > 32) return setError(OVERFLOW_RANGE);
    const uint8_t len = lenOp.getError() ? 0 : lenOp.val32 - 1;
    const uint8_t data = (static_cast<uint8_t>(offOp->val32) << 5) | len;
    insn.emitOperand8(data);
    return OK;
}

Error AsmNs32000::emitImmediate(
    InsnNs32000 &insn, uint32_t val32, OprSize size) {
    switch (size) {
    case SZ_BYTE:
        insn.emitOperand8(static_cast<uint8_t>(val32));
        break;
    case SZ_WORD:
        insn.emitOperand16(static_cast<uint16_t>(val32));
        break;
    case SZ_LONG:
        insn.emitOperand32(val32);
        break;
    case SZ_FLOAT:
        // TODO:
        insn.emitOperand32(0);
        break;
    case SZ_DOUBLE:
        // TODO;
        insn.emitOperand32(0);
        insn.emitOperand32(0);
        break;
    default: return setError(INTERNAL_ERROR);
    }
    return OK;
}

uint8_t AsmNs32000::encodeGenericField(AddrMode mode, RegName reg) const {
    switch (mode) {
    case M_GREG:
    case M_FREG: return _regs.encodeRegName(reg);
    case M_RREL: return _regs.encodeRegName(reg) | 0x08;
    case M_MREL:
        if (reg == REG_FP) return 0x10;
        if (reg == REG_SP) return 0x11;
        if (reg == REG_SB) return 0x12;
        break;
    case M_IMM: return 0x14;
    case M_ABS: return 0x15;
    case M_EXT: return 0x16;
    case M_TOS: return 0x17;
    case M_MEM:
        if (reg == REG_FP) return 0x18;
        if (reg == REG_SP) return 0x19;
        if (reg == REG_SB) return 0x1A;
        if (reg == REG_PC) return 0x1B;
    default:
        break;
    }
    return 0;
}

Error AsmNs32000::emitIndexByte(
    InsnNs32000 &insn, AddrMode mode, const Operand &op, OprPos pos) const {
    if (op.index == REG_UNDEF) return OK;
    const uint8_t indexByte = (encodeGenericField(op.mode, op.reg) << 3)
        | _regs.encodeRegName(op.index);
    insn.emitOperand8(indexByte);
    return OK;
}

Error AsmNs32000::emitGeneric(
    InsnNs32000 &insn, AddrMode mode, const Operand &op, OprPos pos) {
    const uint8_t field = (op.index == REG_UNDEF)
        ? encodeGenericField(op.mode, op.reg)
        : encodeScaledIndex(op.indexSize);
    embedOprField(insn, pos, field);
    switch (op.mode) {
    case M_RREL:
    case M_ABS:
    case M_MEM:
        if (op.mode == M_MEM && op.reg == REG_PC)
            return emitRelative(insn, op);
        return emitDisplacement(insn, op.val32);
    case M_MREL:
        if (emitDisplacement(insn, op.disp2)) return getError();
        return emitDisplacement(insn, op.val32);
    case M_EXT:
        if (emitDisplacement(insn, op.val32)) return getError();
        return emitDisplacement(insn, op.disp2);
    case M_IMM:
        return emitImmediate(
                insn, op.val32,
                mode == M_GENC ? SZ_BYTE : insn.oprSize());
    default:
        return OK;
    }
}

Error AsmNs32000::emitRelative(InsnNs32000 &insn, const Operand &op) {
    const Config::uintptr_t target = op.getError() ? insn.address() : op.val32;
    const Config::ptrdiff_t disp = target - insn.address();
    if (emitDisplacement(insn, disp)) return setError(OPERAND_TOO_FAR);
    return OK;
}

Error AsmNs32000::emitOperand(
    InsnNs32000 &insn, AddrMode mode, const Operand &op, OprPos pos,
    const Operand *prevOp) {
    switch (mode) {
    case M_GREG:
        embedOprField(insn, pos, _regs.encodeRegName(op.reg));
        break;
    case M_PREG:
    case M_MREG:
    case M_CONF:
    case M_SOPT:
        embedOprField(insn, pos, op.val32);
        break;
    case M_PUSH:
        insn.emitOperand8(op.val32);
        break;
    case M_POP:
        insn.emitOperand8(reverseBits(op.val32));
        break;
    case M_GENR:
    case M_GENC:
    case M_GENW:
    case M_FENR:
    case M_FENW:
        return emitGeneric(insn, mode, op, pos);
    case M_INT4: {
        const int32_t val = static_cast<int32_t>(op.val32);
        if (val < -8 || val >= 8) return setError(OVERFLOW_RANGE);
        embedOprField(insn, pos, op.val32);
        break;
    }
    case M_BFOFF:
    case M_BFLEN:
        return emitBitField(insn, mode, prevOp, op);
    case M_REL:
        return emitRelative(insn, op);
    case M_LEN32: {
        const int32_t val = static_cast<int32_t>(op.val32);
        if (val < 0) return setError(ILLEGAL_CONSTANT);
        if (op.getError() == OK && val == 0) setError(ILLEGAL_CONSTANT);
        if (val > 32) return setError(OVERFLOW_RANGE);
        return emitDisplacement(insn, val);
    }
    case M_DISP:
        return emitDisplacement(insn, op.val32);
    case M_LEN4:
    case M_LEN8:
    case M_LEN16:
        return emitLength(insn, mode, op);
    default:
        return setError(INTERNAL_ERROR);
    }
    return OK;
}

Error AsmNs32000::encode(Insn &_insn) {
    InsnNs32000 insn(_insn);
    const char *endName = _parser.scanSymbol(_scan);
    insn.setName(_scan, endName);

    Operand srcOp, dstOp, ex1Op, ex2Op;
    _scan = skipSpaces(endName);
    if (parseOperand(srcOp)) return getError();
    const char *p = skipSpaces(_scan);
    if (*p == ',') {
        _scan = skipSpaces(p + 1);
        if (parseOperand(dstOp)) return getError();
        p = skipSpaces(_scan);
    }
    if (*p == ',') {
        _scan = skipSpaces(p + 1);
        if (parseOperand(ex1Op)) return getError();
        p = skipSpaces(_scan);
    }
    if (*p == ',') {
        _scan = skipSpaces(p + 1);
        if (parseOperand(ex2Op)) return getError();
        p = skipSpaces(_scan);
    }
    if (!endOfLine(p)) return setError(GARBAGE_AT_END);
    setErrorIf(srcOp.getError());
    setErrorIf(dstOp.getError());
    setErrorIf(ex1Op.getError());
    setErrorIf(ex2Op.getError());

    insn.setAddrMode(srcOp.mode, dstOp.mode, ex1Op.mode, ex2Op.mode);
#if 0
    printf("@@ search: name=%-6s src=%d dst=%d ex1=%d ex2=%d\n", insn.name(), insn.srcMode(), insn.dstMode(), insn.ex1Mode(), insn.ex2Mode());
#endif
    if (TableNs32000.searchName(insn))
        return setError(TableNs32000.getError());
#if 0
    printf("@@  found: name=%-6s src=%d dst=%d ex1=%d ex2=%d sz=%d opc=", insn.name(), insn.srcMode(), insn.dstMode(), insn.ex1Mode(), insn.ex2Mode(), insn.oprSize());
    if (insn.hasPost()) printf("%02X:", insn.post());
    printf("%02X", insn.opCode());
    if (insn.hasPrefix()) printf(":%02X", insn.prefix());
    printf("\n");
#endif
    const AddrMode src = insn.srcMode();
    const AddrMode dst = insn.dstMode();
    const AddrMode ex1 = insn.ex1Mode();
    if (emitIndexByte(insn, src, srcOp, insn.srcPos()))
        return getError();
    if (emitIndexByte(insn, dst, dstOp, insn.dstPos()))
        return getError();
    if (emitIndexByte(insn, ex1, ex1Op, insn.ex1Pos()))
        return getError();
    if (src != M_NONE) {
        if (emitOperand(insn, src, srcOp, insn.srcPos()))
            return getError();
    }
    if (dst != M_NONE) {
        if (emitOperand(insn, dst, dstOp, insn.dstPos()))
            return getError();
    }
    if (ex1 != M_NONE) {
        if (emitOperand(insn, ex1, ex1Op, insn.ex1Pos()))
            return getError();
    }
    const AddrMode ex2 = insn.ex2Mode();
    if (ex2 != M_NONE) {
        const Operand *prevOp = (ex2 == M_BFLEN) ? &ex1Op : nullptr;
        if (emitOperand(insn, ex2, ex2Op, insn.ex2Pos(), prevOp))
            return getError();
    }
    insn.emitInsn();
    return getError();
}

} // namespace ns32000
} // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
