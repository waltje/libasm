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

#include "asm_i8051.h"

#include <ctype.h>

namespace libasm {
namespace i8051 {

Error AsmI8051::parseOperand(StrScanner &scan, Operand &op) const {
    StrScanner p(scan.skipSpaces());
    op.setAt(p);
    if (endOfLine(*p))
        return OK;

    if (p.expect('#')) {
        op.val16 = parseExpr16(p, op);
        if (parserError())
            return op.getError();
        op.mode = M_IMM16;
        scan = p;
        return OK;
    }

    const bool indir = p.expect('@');
    if (indir && isspace(*p))
        return op.setError(UNKNOWN_OPERAND);

    const StrScanner regp = p;
    op.reg = RegI8051::parseRegName(p);
    if (op.reg != REG_UNDEF) {
        if (indir && op.reg == REG_A && p.expect('+')) {
            const RegName base = RegI8051::parseRegName(p);
            if (base == REG_DPTR || base == REG_PC) {
                op.mode = (base == REG_DPTR) ? M_INDXD : M_INDXP;
                scan = p;
                return OK;
            }
            return op.setError(UNKNOWN_OPERAND);
        }
        scan = p;
        if (indir) {
            if (op.reg == REG_R0 || op.reg == REG_R1) {
                op.mode = M_IDIRR;
                return OK;
            }
            if (op.reg == REG_DPTR) {
                op.mode = M_IDIRD;
                return OK;
            }
            return op.setError(UNKNOWN_OPERAND);
        }
        if (RegI8051::isRReg(op.reg)) {
            op.mode = M_RREG;
            return OK;
        }
        switch (op.reg) {
        case REG_A:
            op.mode = M_AREG;
            return OK;
        case REG_C:
            op.mode = M_CREG;
            return OK;
        case REG_DPTR:
            op.mode = M_DREG;
            return OK;
        case REG_AB:
            op.mode = M_ABREG;
            return OK;
        default:
            return op.setError(regp, UNKNOWN_OPERAND);
        }
    }
    if (indir)
        return op.setError(UNKNOWN_OPERAND);

    const StrScanner addrp = p;
    const bool bitNot = p.expect('/');
    op.val16 = parseExpr16(p.skipSpaces(), op);
    if (parserError())
        return getError();
    if (p.expect('.')) {
        if (op.getError())
            op.val16 = 0x20;
        const StrScanner bitp = p;
        uint16_t bitNo = parseExpr16(p, op);
        if (parserError())
            return op.getError();
        if (bitNo >= 8)
            return op.setError(bitp, ILLEGAL_BIT_NUMBER);
        uint16_t val16 = op.val16;
        if ((val16 & ~0x0F) == 0x20 || (val16 & ~0x78) == 0x80) {
            op.mode = bitNot ? M_NOTAD : M_BITAD;
            if ((val16 & 0x80) == 0)
                op.val16 = (val16 & 0xF) << 3;
            op.val16 |= bitNo;
            scan = p;
            return OK;
        }
        return op.setError(addrp, NOT_BIT_ADDRESSABLE);
    }
    op.mode = bitNot ? M_NOTAD : M_ADR16;
    scan = p;
    return OK;
}

Error AsmI8051::encodeOperand(InsnI8051 &insn, const AddrMode mode, const Operand &op) {
    switch (mode) {
    case M_REL: {
        uint8_t len = insn.length();
        if (len == 0)
            len = 1;
        const Config::uintptr_t base = insn.address() + len + 1;
        const Config::uintptr_t target = op.getError() ? base : op.val16;
        const Config::ptrdiff_t delta = target - base;
        if (overflowRel8(delta))
            return setError(op, OPERAND_TOO_FAR);
        insn.emitOperand8(delta);
        return OK;
    }
    case M_RREG:
    case M_IDIRR:
        insn.embed(RegI8051::encodeRReg(op.reg));
        return OK;
    case M_ADR8:
        if (op.val16 >= 0x100)
            return setError(op, OVERFLOW_RANGE);
        insn.emitOperand8(op.val16);
        return OK;
    case M_IMM8: {
        if (overflowUint8(op.val16))
            return setError(op, OVERFLOW_RANGE);
        insn.emitOperand8(op.val16);
        return OK;
    }
    case M_ADR11: {
        const Config::uintptr_t base = insn.address() + 2;
        const Config::uintptr_t target = op.getError() ? (base & ~0x7FF) : op.val16;
        if ((base & ~0x7FF) != (target & ~0x7FF))
            return setError(op, OPERAND_TOO_FAR);
        insn.embed((target & 0x700) >> 3);
        insn.emitOperand8(target);
        return OK;
    }
    case M_ADR16:
    case M_IMM16:
        insn.emitOperand16(op.val16);
        return OK;
    case M_BITAD:
    case M_NOTAD:
        if (op.val16 >= 0x100)
            return setError(op, NOT_BIT_ADDRESSABLE);
        insn.emitOperand8(op.val16);
        return OK;
    default:
        return OK;
    }
}

Error AsmI8051::encodeImpl(StrScanner &scan, Insn &_insn) {
    InsnI8051 insn(_insn);
    insn.nameBuffer().text(_parser.readSymbol(scan));

    Operand dstOp, srcOp, extOp;
    if (parseOperand(scan, dstOp) && dstOp.hasError())
        return setError(dstOp);
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, srcOp) && srcOp.hasError())
            return setError(srcOp);
        scan.skipSpaces();
    }
    if (scan.expect(',')) {
        if (parseOperand(scan, extOp) && extOp.hasError())
            return setError(extOp);
        scan.skipSpaces();
    }
    if (!endOfLine(*scan))
        return setError(GARBAGE_AT_END);
    setErrorIf(dstOp);
    setErrorIf(srcOp);
    setErrorIf(extOp);

    insn.setAddrMode(dstOp.mode, srcOp.mode, extOp.mode);
    const auto error = TableI8051::TABLE.searchName(insn);
    if (error)
        return setError(dstOp, error);

    const AddrMode dst = insn.dstMode();
    const AddrMode src = insn.srcMode();
    const AddrMode ext = insn.extMode();
    if (dst == M_ADR8 && src == M_ADR8) {
        if (encodeOperand(insn, src, srcOp))
            return getError();
        if (encodeOperand(insn, dst, dstOp))
            return getError();
    } else {
        if (dst && encodeOperand(insn, dst, dstOp))
            return getError();
        if (src && encodeOperand(insn, src, srcOp)) {
            insn.reset();
            return getError();
        }
        if (ext && encodeOperand(insn, ext, extOp)) {
            insn.reset();
            return getError();
        }
    }
    insn.emitInsn();
    return getError();
}

}  // namespace i8051
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
