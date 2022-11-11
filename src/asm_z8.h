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

#ifndef __ASM_Z8_H__
#define __ASM_Z8_H__

#include "asm_base.h"
#include "config_z8.h"
#include "insn_z8.h"
#include "reg_z8.h"
#include "table_z8.h"

namespace libasm {
namespace z8 {

class AsmZ8 : public Assembler, public Config {
public:
    AsmZ8() : Assembler(_parser, TableZ8::TABLE), _parser() { reset(); }

    const ConfigBase &config() const override { return *this; }
    void reset() override { setRegPointer(-1); }
    const Options &options() const override { return _options; }

private:
    IntelValueParser _parser;
    int16_t _regPointer0;
    int16_t _regPointer1;
    struct OptSetrp : public IntOptionBase {
        OptSetrp(const /*PROGMEM*/ char *name_P, const /*PROGMEM*/ char *desc_P, AsmZ8 *assembler,
                bool (AsmZ8::*set)(int16_t), const OptionBase &next)
            : IntOptionBase(name_P, desc_P, next), _assembler(assembler), _set(set) {}
        OptSetrp(const /*PROGMEM*/ char *name_P, const /*PROGMEM*/ char *desc_P, AsmZ8 *assembler,
                bool (AsmZ8::*set)(int16_t))
            : IntOptionBase(name_P, desc_P), _assembler(assembler), _set(set) {}
        Error check(int32_t value) const override {
            return (_assembler->*_set)(value) ? OK : OPERAND_NOT_ALLOWED;
        }
        void set(int32_t value) const override { (void)value; }
        AsmZ8 *_assembler;
        bool (AsmZ8::*_set)(int16_t);
    };
    const OptSetrp _opt_setrp1{OPT_INT_SETRP1, OPT_DESC_SETRP1, this, &AsmZ8::setRegPointer1};
    const OptSetrp _opt_setrp0{
            OPT_INT_SETRP0, OPT_DESC_SETRP0, this, &AsmZ8::setRegPointer0, _opt_setrp1};
    const OptSetrp _opt_setrp{
            OPT_INT_SETRP, OPT_DESC_SETRP, this, &AsmZ8::setRegPointer, _opt_setrp0};
    const Options _options{_opt_setrp};

    bool setRegPointer(int16_t rp);
    bool setRegPointer0(int16_t rp);
    bool setRegPointer1(int16_t rp);
    bool isWorkReg(uint8_t regAddr) const;

    struct Operand : public OperandBase {
        AddrMode mode;
        RegName reg;
        CcName cc;
        uint16_t val16;
        Operand() : mode(M_NONE), reg(REG_UNDEF), cc(CC_UNDEF), val16(0) {}
    };

    Error parseOperand(StrScanner &scan, Operand &op) const;
    Error setRp(StrScanner &scan, bool (AsmZ8::*)(int16_t));
    Error processPseudo(StrScanner &scan, const char *name);

    void encodeOperand(InsnZ8 &insn, const AddrMode mode, const Operand &op);
    void encodeAbsolute(InsnZ8 &insn, const Operand &dstOp, const Operand &srcOp);
    void encodeRelative(InsnZ8 &insn, const Operand &op);
    void encodeIndexed(InsnZ8 &insn, const Operand &dstOp, const Operand &srcOp);
    void encodeIndirectRegPair(InsnZ8 &insn, const Operand &dstOp, const Operand &srcOp);
    void encodeInOpCode(InsnZ8 &insn, const Operand &dstOp, const Operand &srcOp);
    void encodeMultiOperands(
            InsnZ8 &insn, const Operand &dstOp, const Operand &srcOp, const Operand &extOp);
    void encodePostByte(
            InsnZ8 &insn, const Operand &dstOp, const Operand &srcOp, const Operand &extOp);
    Error encodeImpl(StrScanner &scan, Insn &insn) override;

    static const char OPT_INT_SETRP[] PROGMEM;
    static const char OPT_DESC_SETRP[] PROGMEM;
    static const char OPT_INT_SETRP0[] PROGMEM;
    static const char OPT_DESC_SETRP0[] PROGMEM;
    static const char OPT_INT_SETRP1[] PROGMEM;
    static const char OPT_DESC_SETRP1[] PROGMEM;
};

}  // namespace z8
}  // namespace libasm

#endif  // __ASM_Z8_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
