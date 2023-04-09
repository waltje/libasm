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

#ifndef __ASM_I8080_H__
#define __ASM_I8080_H__

#include "asm_base.h"
#include "config_i8080.h"
#include "insn_i8080.h"

namespace libasm {
namespace i8080 {

class AsmI8080 final : public Assembler, public Config {
public:
    AsmI8080();

    const ConfigBase &config() const override { return *this; }

private:
    const IntelNumberParser _number;
    const SemicolonCommentParser _comment;
    const SimpleSymbolParser _symbol{SymbolParser::ATMARK_QUESTION, SymbolParser::NONE};
    const DefaultLetterParser _letter;
    const DollarLocationParser _location;
    PseudoBase _pseudos;

    struct Operand;
    Error parseOperand(StrScanner &scan, Operand &op) const;

    void encodeOperand(InsnI8080 &insn, const Operand &op, AddrMode mode);
    Error encodeImpl(StrScanner &scan, Insn &insn) override;
};

}  // namespace i8080
}  // namespace libasm

#endif  // __ASM_I8080_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
