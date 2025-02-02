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

#include "reg_mos6502.h"
#include "table_mos6502.h"
#include "text_mos6502.h"

namespace libasm {
namespace mos6502 {

using namespace pseudo;
using namespace reg;
using namespace text::common;
using namespace text::mos6502;

namespace {

const char TEXT_aequal[] PROGMEM = "*=";

constexpr Pseudo PSEUDOS[] PROGMEM = {
        Pseudo{TEXT_aequal, &Assembler::defineOrigin},
        Pseudo{TEXT_dBYTE, &Assembler::defineDataConstant, Assembler::DATA_BYTE},
        Pseudo{TEXT_dWORD, &Assembler::defineDataConstant, Assembler::DATA_WORD},
        Pseudo{TEXT_ALIGN, &Assembler::alignOrigin},
        Pseudo{TEXT_ORG, &Assembler::defineOrigin},
};

}  // namespace

struct AsmMos6502::Operand final : ErrorAt {
    AddrMode mode;
    uint32_t val32;
    Operand() : mode(M_NONE), val32(0) {}
    void embed(AddrMode indirectFlags) {
        mode = AddrMode(
                uint8_t(AsmInsn::indirectFlags(indirectFlags)) | uint8_t(AsmInsn::baseMode(mode)));
    }
};

const ValueParser::Plugins &AsmMos6502::defaultPlugins() {
    static const struct final : ValueParser::Plugins {
        const NumberParser &number() const override { return MotorolaNumberParser::singleton(); }
        const LetterParser &letter() const override { return MotorolaLetterParser::singleton(); }
        const LocationParser &location() const override {
            return AsteriskLocationParser::singleton();
        }
    } PLUGINS{};
    return PLUGINS;
}

AsmMos6502::AsmMos6502(const ValueParser::Plugins &plugins)
    : Assembler(plugins, ARRAY_RANGE(PSEUDOS), &_opt_longa),
      Config(TABLE),
      _opt_longa(this, &AsmMos6502::setLongAccumulator, OPT_BOOL_LONGA, OPT_DESC_LONGA, _opt_longi),
      _opt_longi(this, &AsmMos6502::setLongIndex, OPT_BOOL_LONGI, OPT_DESC_LONGI) {
    reset();
}

void AsmMos6502::reset() {
    Assembler::reset();
    setLongAccumulator(false);
    setLongIndex(false);
}

Error AsmMos6502::setLongAccumulator(bool enable) {
    _longAccumulator = enable;
    if (cpuType() == W65C816)
        return OK;
    _longAccumulator = false;
    return enable ? OPERAND_NOT_ALLOWED : OK;
}

Error AsmMos6502::setLongIndex(bool enable) {
    _longIndex = enable;
    if (cpuType() == W65C816)
        return OK;
    _longIndex = false;
    return enable ? OPERAND_NOT_ALLOWED : OK;
}

void AsmMos6502::encodeRelative(AsmInsn &insn, AddrMode mode, const Operand &op) {
    const Config::uintptr_t bank = insn.address() & ~0xFFFF;
    const auto len = insn.length();
    const auto base = insn.address() + (len ? len : 1) + (mode == M_REL ? 1 : 2);
    const auto target = op.getError() ? base : op.val32;
    const auto delta = branchDelta(base, target, op);
    if (op.isOK() && (op.val32 & ~0xFFFF) != bank)
        setErrorIf(op, OPERAND_TOO_FAR);
    if (mode == M_REL) {
        if (overflowInt8(delta))
            setErrorIf(op, OPERAND_TOO_FAR);
        insn.emitOperand8(delta);
    } else {
        insn.emitOperand16(delta);
    }
}

void AsmMos6502::emitImmediate(AsmInsn &insn, const Operand &op, bool imm16) {
    if (imm16) {
        if (overflowUint16(op.val32))
            setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand16(op.val32);
    } else {
        if (overflowUint8(op.val32))
            setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand8(op.val32);
    }
}

void AsmMos6502::encodeOperand(AsmInsn &insn, AddrMode modeAndFlags, const Operand &op) {
    const auto mode = AsmInsn::baseMode(modeAndFlags);
    switch (mode) {
    case M_IMA:
        emitImmediate(insn, op, _longAccumulator);
        break;
    case M_IMX:
        emitImmediate(insn, op, _longIndex);
        break;
    case M_ABS:
        if (checkAddr(op.val32, ADDRESS_16BIT))
            setErrorIf(op, OVERFLOW_RANGE);
        emitImmediate(insn, op, true);
        break;
    case M_ABSL:
        if (checkAddr(op.val32))
            setErrorIf(op, OVERFLOW_RANGE);
        insn.emitOperand16(op.val32);
        insn.emitOperand8(op.val32 >> 16);
        break;
    case M_DPG:
        if (checkAddr(op.val32, 8))
            setErrorIf(op, OVERFLOW_RANGE);
        /* Fall-through */
    case M_IM8:
        emitImmediate(insn, op, false);
        break;
    case M_REL:
    case M_RELL:
        encodeRelative(insn, mode, op);
        break;
    default:
        break;
    }
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

namespace {

AddrMode regName2AddrMode(RegName reg) {
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

}  // namespace

Error AsmMos6502::parseOpenIndirect(StrScanner &scan, Operand &op, char &indirect) const {
    if (scan.expect('(')) {
        if (indirect)
            return op.setError(UNKNOWN_OPERAND);
        indirect = ')';
        op.mode = I_FLAG;
    } else if (scan.expect('[')) {
        if (indirect)
            return op.setError(UNKNOWN_OPERAND);
        indirect = ']';
        op.mode = L_FLAG;
    } else if (indirect) {
        op.mode = indirect == ')' ? I_FLAG : L_FLAG;
    }
    return OK;
}

Error AsmMos6502::parseCloseIndirect(StrScanner &scan, Operand &op, char &indirect) const {
    if (scan.skipSpaces().expect(')')) {
        if (indirect != ')')
            return op.setError(UNKNOWN_OPERAND);
        indirect = 0;
    } else if (scan.expect(']')) {
        if (indirect != ']')
            return op.setError(UNKNOWN_OPERAND);
        indirect = 0;
    }
    return OK;
}

namespace {

char parseSizeOverride(StrScanner &p) {
    if (p.expect('<'))
        return '<';
    if (p.expect('>')) {
        if (p.expect('>'))
            return '}';  // >>
        return '>';
    }
    return 0;
}

}  // namespace

Error AsmMos6502::parseOperand(StrScanner &scan, Operand &op, char &indirect) const {
    auto p = scan.skipSpaces();
    op.setAt(p);
    if (endOfLine(p))
        return OK;

    if (p.expect('#')) {  // #nn
        const auto bop = p.expect([](char c) { return c == '<' || c == '>'; });
        op.val32 = parseExpr32(p, op);
        if (op.hasError())
            return getError();
        if (bop == '<')
            op.val32 &= 0xFF;
        if (bop == '>')
            op.val32 = (op.val32 >> 8) & 0xFF;
        op.mode = M_IMA;
        scan = p;
        return OK;
    }

    if (parseOpenIndirect(p, op, indirect))
        return op.getError();
    const auto indirectFlags = op.mode;

    const auto reg = parseRegName(p.skipSpaces());
    if (reg != REG_UNDEF) {
        op.mode = regName2AddrMode(reg);
    } else {
        const auto size = parseSizeOverride(p);
        op.val32 = parseExpr32(p, op, indirect);
        if (op.hasError())
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

Error AsmMos6502::parseTableOnOff(StrScanner &scan, BoolOption<AsmMos6502>::Setter setter) {
    auto p = scan.skipSpaces();
    auto name = parser().readSymbol(p);
    auto value = false;
    auto error = OptionBase::parseBoolOption(name, value);
    if (error == OK)
        error = (this->*setter)(value);
    if (error)
        return setError(scan, error);
    scan = p;
    return OK;
}

Error AsmMos6502::processPseudo(StrScanner &scan, Insn &insn) {
    if (strcasecmp_P(insn.name(), OPT_BOOL_LONGA) == 0)
        return parseTableOnOff(scan, &AsmMos6502::setLongAccumulator);
    if (strcasecmp_P(insn.name(), OPT_BOOL_LONGI) == 0)
        return parseTableOnOff(scan, &AsmMos6502::setLongIndex);
    return Assembler::processPseudo(scan, insn);
}

namespace {

bool hasRegister(AddrMode mode) {
    const auto base = uint8_t(AsmInsn::baseMode(mode));
    return base >= uint8_t(M_REGA) && base <= uint8_t(M_REGS);
}

bool maybeStackRelativeIndirect(CpuType cpuType, AddrMode mode3) {
    return cpuType == W65C816 && hasRegister(mode3);
}

}  // namespace

Error AsmMos6502::encodeImpl(StrScanner &scan, Insn &_insn) {
    AsmInsn insn(_insn);
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
    if (!endOfLine(scan))
        return setError(scan, GARBAGE_AT_END);
    setErrorIf(op1);
    setErrorIf(op2);
    setErrorIf(op3);

    insn.setAddrMode(op1.mode, op2.mode, op3.mode);
    const auto error = TABLE.searchName(cpuType(), insn);
    if (error == OPERAND_NOT_ALLOWED) {
        if (hasRegister(op1.mode))
            return setError(op1, REGISTER_NOT_ALLOWED);
        if (hasRegister(op2.mode) && !maybeStackRelativeIndirect(cpuType(), op3.mode))
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
    } else {
        encodeOperand(insn, insn.mode1(), op1);
        encodeOperand(insn, insn.mode2(), op2);
    }
    insn.emitInsn();
    return setErrorIf(insn);
}

}  // namespace mos6502
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
