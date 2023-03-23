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

#ifndef __ASM_I8048_H__
#define __ASM_I8048_H__

#include "asm_base.h"
#include "config_i8048.h"
#include "insn_i8048.h"
#include "reg_i8048.h"
#include "table_i8048.h"

namespace libasm {
namespace i8048 {

class AsmI8048 : public Assembler, public Config {
public:
    AsmI8048()
        : Assembler(_parser, TableI8048::TABLE, _pseudos),
          _parser(_number, _comment, _symbol, _letter, _location),
          _pseudos() {}

    const ConfigBase &config() const override { return *this; }

private:
    ValueParser _parser;
    const IntelNumberParser _number;
    const SemicolonCommentParser _comment;
    const DefaultSymbolParser _symbol;
    const DefaultLetterParser _letter;
    const DollarLocationParser _location;
    PseudoBase _pseudos;

    struct Operand : public OperandBase {
        AddrMode mode;
        RegName reg;
        uint16_t val16;
        Operand() : mode(M_NONE), reg(REG_UNDEF), val16(0) {}
    };

    Error parseOperand(StrScanner &scan, Operand &op) const;

    void encodeAddress(InsnI8048 &insn, const AddrMode mode, const Operand &op);
    void encodeOperand(InsnI8048 &insn, const AddrMode mode, const Operand &op);
    Error encodeImpl(StrScanner &scan, Insn &insn) override;
};

}  // namespace i8048
}  // namespace libasm

#endif  // __ASM_I8048_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
