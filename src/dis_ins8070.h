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

#ifndef __DIS_INS8070_H__
#define __DIS_INS8070_H__

#include "config_ins8070.h"
#include "dis_base.h"
#include "insn_ins8070.h"

namespace libasm {
namespace ins8070 {

class DisIns8070 : public Disassembler, public Config {
public:
    DisIns8070();

    const ConfigBase &config() const override { return *this; }
    void reset() override;

    Error setUseSharpImmediate(bool enable);

private:
    NationalValueFormatter _formatter;
    const BoolOption<DisIns8070> _opt_useSharp;

    bool _useSharp;

    Error decodeImmediate(DisMemory &memory, InsnIns8070 &insn, StrBuffer &out);
    Error decodeAbsolute(DisMemory &memory, InsnIns8070 &insn, StrBuffer &out);
    Error decodeDirect(DisMemory &memory, InsnIns8070 &insn, StrBuffer &out);
    Error decodeRelative(DisMemory &memory, InsnIns8070 &insn, StrBuffer &out, AddrMode mode);
    Error decodeGeneric(DisMemory &memory, InsnIns8070 &insn, StrBuffer &out);
    Error decodeOperand(DisMemory &memory, InsnIns8070 &insn, StrBuffer &out, AddrMode mode);
    Error decodeImpl(DisMemory &memory, Insn &insn, StrBuffer &out) override;
};

}  // namespace ins8070
}  // namespace libasm

#endif  // __DIS_INS8070_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
