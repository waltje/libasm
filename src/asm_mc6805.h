/*
 * Copyright 2021 Tadashi G. Takaoka
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

#ifndef __ASM_MC6805_H__
#define __ASM_MC6805_H__

#include "asm_base.h"
#include "config_mc6805.h"
#include "insn_mc6805.h"
#include "reg_mc6805.h"
#include "table_mc6805.h"

namespace libasm {
namespace mc6805 {

class AsmMc6805 : public Assembler, public Config {
public:
    AsmMc6805() : Assembler(_parser, TableMc6805::TABLE, _pseudos), _parser(), _pseudos() {
        reset();
    }

    const ConfigBase &config() const override { return *this; }
    AddressWidth addressWidth() const override;
    void reset() override { _pc_bits = 0; }

    const Options &options() const override { return _options; }

private:
    MotorolaValueParser _parser;
    class PseudoMc6805 : public PseudoBase {
        bool endOfLine(const StrScanner &scan, bool headOfLine) const override;
    } _pseudos;

    uint8_t _pc_bits;
    const struct OptPcBits : public IntOption<uint8_t> {
        OptPcBits(uint8_t &var);
        Error check(int32_t value) const override;
    } _opt_pc_bits{_pc_bits};
    const Options _options{_opt_pc_bits};

    struct Operand : public OperandBase {
        AddrMode mode;
        int8_t size;
        uint16_t val16;
        Operand() : mode(M_NONE), size(0), val16(0) {}
    };

    Error parseOperand(StrScanner &scan, Operand &op) const;
    void emitRelative(InsnMc6805 &insn, const Operand &op);
    void emitBitNumber(InsnMc6805 &insn, const Operand &op);
    void emitOperand(InsnMc6805 &insn, AddrMode mode, const Operand &op);
    Error encodeImpl(StrScanner &scan, Insn &insn) override;
};

}  // namespace mc6805
}  // namespace libasm

#endif  // __ASM_MC6805_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
