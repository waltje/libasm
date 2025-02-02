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

#ifndef __DIS_MC68000_H__
#define __DIS_MC68000_H__

#include "config_mc68000.h"
#include "dis_base.h"
#include "insn_mc68000.h"
#include "reg_mc68000.h"

namespace libasm {
namespace mc68000 {

class DisMc68000 final : public Disassembler, public Config {
public:
    DisMc68000();

private:
    const PrefixHexFormatter _hexFormatter{HexFormatter::DOLLAR};

    StrBuffer &outOprSize(StrBuffer &out, OprSize size);
    StrBuffer &outMoveMltRegs(StrBuffer &out, RegName start, RegName last, char suffix);
    StrBuffer &outMoveMltRegList(StrBuffer &out, uint16_t list, bool pop,
            StrBuffer &(DisMc68000::*outRegs)(StrBuffer &, RegName, RegName, char));

    Error decodeImmediateData(DisInsn &insn, StrBuffer &out, OprSize eaSize);
    Error decodeEffectiveAddr(DisInsn &insn, StrBuffer &out, const EaMc68000 &ea);
    Error decodeRelative(DisInsn &insn, StrBuffer &out, uint8_t rel8);
    Error decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode, uint8_t modePos,
            uint8_t regPos, OprSize size, uint16_t opr16);
    Error checkOperand(AddrMode mode, uint8_t modePos, uint8_t regPos, OprSize size);

    Error decodeImpl(DisMemory &memory, Insn &insn, StrBuffer &out) override;
    const ConfigBase &config() const override { return *this; }
    ConfigSetter &configSetter() override { return *this; }
};

}  // namespace mc68000
}  // namespace libasm

#endif  // __DIS_MC68000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
