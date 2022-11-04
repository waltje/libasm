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

#include "asm_mos6502.h"

namespace libasm {
namespace mos6502 {

const char AsmMos6502::OPT_BOOL_LONGA[] PROGMEM = "longa";
const char AsmMos6502::OPT_DESC_LONGA[] PROGMEM = "enable 16-bit accumulator";
const char AsmMos6502::OPT_BOOL_LONGI[] PROGMEM = "longi";
const char AsmMos6502::OPT_DESC_LONGI[] PROGMEM = "enable 16-bit index registers";

Error AsmMos6502::encodeRelative(InsnMos6502 &insn, AddrMode mode, const Operand &op) {
    const Config::uintptr_t bank = insn.address() & ~0xFFFF;
    const auto len = insn.length();
    const uint16_t base = insn.address() + (len ? len : 1) + (mode == M_REL ? 1 : 2);
    const uint16_t target = op.getError() ? base : op.val32;
    const int16_t delta = target - base;
    if (addressWidth() == ADDRESS_24BIT && op.isOK() && (op.val32 & ~0xFFFF) != bank)
        return setError(op, OPERAND_TOO_FAR);
    if (mode == M_REL) {
        if (overflowRel8(delta))
            return setError(op, OPERAND_TOO_FAR);
        insn.emitOperand8(delta);
    } else {
        insn.emitOperand16(delta);
    }
    return OK;
}

Error AsmMos6502::encodeOperand(InsnMos6502 &insn, AddrMode modeAndFlags, const Operand &op) {
    const auto mode = InsnMos6502::baseMode(modeAndFlags);
    switch (mode) {
    case M_IMA:
        if (TableMos6502::TABLE.longAccumulator()) {
            insn.emitOperand16(op.val32);
        } else {
            insn.emitOperand8(op.val32);
        }
        break;
    case M_IMX:
        if (TableMos6502::TABLE.longIndex()) {
            insn.emitOperand16(op.val32);
        } else {
            insn.emitOperand8(op.val32);
        }
        break;
    case M_ABS:
        insn.emitOperand16(op.val32);
        break;
    case M_ABSL:
        insn.emitOperand16(op.val32);
        insn.emitOperand8(op.val32 >> 16);
        break;
    case M_DPG:
    case M_IM8:
        insn.emitOperand8(op.val32);
        break;
    case M_REL:
    case M_RELL:
        return encodeRelative(insn, mode, op);
    default:
        break;
    }
    return OK;
}

Error AsmMos6502::selectMode(
        char size, Operand &op, AddrMode zp, AddrMode abs, AddrMode labs) const {
    if (size == '}') {
        if (labs == M_NONE)
            return op.setError(UNKNOWN_OPERAND);
        op.mode = labs;
    } else if (size == '>') {
        op.mode = abs;
    } else if (size == '<') {
        op.mode = zp;
    } else if (op.val32 >= 0x10000L) {
        op.mode = labs;
    } else {
        op.mode = (op.val32 < 0x100) ? zp : abs;
    }
    return OK;
}

static AddrMode regName2AddrMode(RegName reg) {
    switch (reg) {
    case REG_X:
        return M_REGX;
    case REG_Y:
        return M_REGY;
    case REG_S:
        return M_REGS;
    default:
        return M_REGA;
    }
}

Error AsmMos6502::parseOpenIndirect(StrScanner &scan, Operand &op, char &indirect) const {
    if (scan.expect('(')) {
        if (indirect)
            return op.setError(UNKNOWN_OPERAND);
        indirect = '(';
        op.mode = I_FLAG;
    } else if (scan.expect('[')) {
        if (indirect)
            return op.setError(UNKNOWN_OPERAND);
        indirect = '[';
        op.mode = L_FLAG;
    } else if (indirect) {
        op.mode = indirect == '(' ? I_FLAG : L_FLAG;
    }
    return OK;
}

Error AsmMos6502::parseCloseIndirect(StrScanner &scan, Operand &op, char &indirect) const {
    if (scan.skipSpaces().expect(')')) {
        if (indirect != '(')
            return op.setError(UNKNOWN_OPERAND);
        indirect = 0;
    } else if (scan.expect(']')) {
        if (indirect != '[')
            return op.setError(UNKNOWN_OPERAND);
        indirect = 0;
    }
    return OK;
}

static char parseSizeOverride(StrScanner &p) {
    if (p.expect('<'))
        return '<';
    if (p.expect('>')) {
        if (p.expect('>'))
            return '}';  // >>
        return '>';
    }
    return 0;
}

Error AsmMos6502::parseOperand(StrScanner &scan, Operand &op, char &indirect) const {
    StrScanner p(scan.skipSpaces());
    op.setAt(p);
    if (endOfLine(*p))
        return OK;

    if (p.expect('#')) {  // #nn
        op.val32 = parseExpr32(p, op);
        if (parserError())
            return getError();
        op.mode = M_IMA;
        scan = p;
        return OK;
    }

    if (parseOpenIndirect(p, op, indirect))
        return op.getError();
    const AddrMode indirectFlags = op.mode;

    const RegName reg = RegMos6502::parseRegName(p.skipSpaces());
    if (reg != REG_UNDEF) {
        op.mode = regName2AddrMode(reg);
    } else {
        const char size = parseSizeOverride(p);
        op.val32 = parseExpr32(p, op);
        if (parserError())
            return op.getError();
        if (selectMode(size, op, M_DPG, M_ABS, M_ABSL))
            return op.getError();
    }
    op.embed(indirectFlags);
    if (indirect && parseCloseIndirect(p, op, indirect))
        return op.getError();
    scan = p;
    return OK;
}

Error AsmMos6502::parseTableOnOff(StrScanner &scan, bool (TableMos6502::*set)(bool val)) {
    StrScanner p(scan.skipSpaces());
    const StrScanner name = _parser.readSymbol(p);
    if (name.iequals_P(PSTR("on"))) {
        if (!(TableMos6502::TABLE.*set)(true))
            setError(scan, OPERAND_NOT_ALLOWED);
    } else if (name.iequals_P(PSTR("off"))) {
        if (!(TableMos6502::TABLE.*set)(false))
            setError(scan, OPERAND_NOT_ALLOWED);
    } else {
        setError(scan, UNKNOWN_OPERAND);
        return OK;
    }
    scan = p;
    return OK;
}

Error AsmMos6502::processPseudo(StrScanner &scan, const char *name) {
    if (strcasecmp_P(name, PSTR("longa")) == 0)
        return parseTableOnOff(scan, &TableMos6502::setLongAccumulator);
    if (strcasecmp_P(name, PSTR("longi")) == 0)
        return parseTableOnOff(scan, &TableMos6502::setLongIndex);
    return UNKNOWN_INSTRUCTION;
}

static bool hasRegister(AddrMode mode) {
    const auto base = uint8_t(InsnMos6502::baseMode(mode));
    return base >= uint8_t(M_REGA) && base <= uint8_t(M_REGS);
}

static bool maybeStackRelativeIndirect(AddrMode mode3) {
    return TableMos6502::TABLE.cpuType() == W65C816 && hasRegister(mode3);
}

Error AsmMos6502::encodeImpl(StrScanner &scan, Insn &_insn) {
    InsnMos6502 insn(_insn);
    insn.nameBuffer().text(_parser.readSymbol(scan));

    if (processPseudo(scan, insn.name()) == OK)
        return getError();

    char indirect = 0;
    Operand op1, op2, op3;
    if (parseOperand(scan, op1, indirect) && op1.hasError())
        return setError(op1);
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, op2, indirect) && op2.hasError())
            return setError(op2);
        scan.skipSpaces();
    }
    if (indirect)
        return setError(scan, indirect == '(' ? MISSING_CLOSING_PAREN : MISSING_CLOSING_BRACKET);
    if (scan.skipSpaces().expect(',')) {
        if (parseOperand(scan, op3, indirect) && op3.hasError())
            return setError(op3);
        scan.skipSpaces();
    }
    if (!endOfLine(*scan))
        return setError(scan, GARBAGE_AT_END);
    setErrorIf(op1);
    setErrorIf(op2);
    setErrorIf(op3);

    insn.setAddrMode(op1.mode, op2.mode, op3.mode);
    const auto error = TableMos6502::TABLE.searchName(insn);
    if (error == OPERAND_NOT_ALLOWED) {
        if (hasRegister(op1.mode))
            return setError(op1, REGISTER_NOT_ALLOWED);
        if (hasRegister(op2.mode) && !maybeStackRelativeIndirect(op3.mode))
            return setError(op2, REGISTER_NOT_ALLOWED);
        if (hasRegister(op3.mode))
            return setError(op3, REGISTER_NOT_ALLOWED);
    }
    if (error)
        return setError(op1, error);

    // W65C816 block move instructions
    if (insn.mode1() == M_BANK && insn.mode2() == M_BANK) {
        insn.emitOperand8(op2.val32 >> 16);  // destination
        insn.emitOperand8(op1.val32 >> 16);  // source
        insn.emitInsn();
        return getError();
    }

    if (encodeOperand(insn, insn.mode1(), op1))
        goto error;
    if (encodeOperand(insn, insn.mode2(), op2)) {
    error:
        insn.reset();
        return getError();
    }
    insn.emitInsn();
    return getError();
}

}  // namespace mos6502
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
