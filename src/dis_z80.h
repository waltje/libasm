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

#ifndef __DIS_Z80_H__
#define __DIS_Z80_H__

#include "config_z80.h"
#include "dis_base.h"
#include "insn_z80.h"
#include "reg_z80.h"

namespace libasm {
namespace z80 {

class DisZ80 final : public Disassembler, public Config {
public:
    DisZ80();

private:
    const SuffixHexFormatter _hexFormatter{'h'};

    StrBuffer &outIndirectAddr(StrBuffer &out, uint16_t addr, uint8_t bits);
    StrBuffer &outIndirectReg(StrBuffer &out, RegName reg);
    StrBuffer &outIndexOffset(StrBuffer &out, RegName reg, int8_t offset);
    StrBuffer &outDataReg(StrBuffer &out, RegName reg);

    Error decodeIndexedBitOp(DisInsn &insn, StrBuffer &out);
    Error decodeRelative(DisInsn &insn, StrBuffer &out);
    Error decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode);

    Error decodeImpl(DisMemory &memory, Insn &insn, StrBuffer &out) override;
    const ConfigBase &config() const override { return *this; }
    ConfigSetter &configSetter() override { return *this; }
};

}  // namespace z80
}  // namespace libasm

#endif  // __DIS_Z80_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
