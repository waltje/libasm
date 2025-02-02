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

#ifndef __DIS_NS32000_H__
#define __DIS_NS32000_H__

#include "config_ns32000.h"
#include "dis_base.h"
#include "insn_ns32000.h"

namespace libasm {
namespace ns32000 {

class DisNs32000 final : public Disassembler, public Config {
public:
    DisNs32000();

    void reset() override;

    Error setPcRelativeParen(bool enable);
    Error setExternalParen(bool enable);
    Error setStringOptionBracket(bool enable);
    Error setFloatPrefix(bool enable);

private:
    const PrefixHexFormatter _hexFormatter{HexFormatter::X_DASH};
    const BoolOption<DisNs32000> _opt_pcrelParen;
    const BoolOption<DisNs32000> _opt_externalParen;
    const BoolOption<DisNs32000> _opt_stroptBracket;
    const BoolOption<DisNs32000> _opt_floatPrefix;

    bool _stringOptionBracket;
    bool _pcRelativeParen;
    bool _externalParen;
    bool _floatPrefix;

    struct Displacement {
        int32_t val32;
        uint8_t bits;
    };

    StrBuffer &outDisplacement(StrBuffer &out, const Displacement &disp);

    Error readIndexByte(DisInsn &insn, AddrMode mode, OprPos pos);
    Error readDisplacement(DisInsn &insn, Displacement &disp);

    Error decodeLength(DisInsn &insn, StrBuffer &out, AddrMode mode);
    Error decodeBitField(DisInsn &insn, StrBuffer &out, AddrMode mode);
    Error decodeImmediate(DisInsn &insn, StrBuffer &out, AddrMode mode);
    Error decodeDisplacement(DisInsn &insn, StrBuffer &out, AddrMode mode);
    Error decodeRelative(DisInsn &insn, StrBuffer &out);
    Error decodeConfig(const DisInsn &insn, StrBuffer &out, OprPos pos);
    Error decodeStrOpt(const DisInsn &insn, StrBuffer &out, OprPos pos);
    Error decodeRegisterList(DisInsn &insn, StrBuffer &out);
    Error decodeGeneric(DisInsn &insn, StrBuffer &out, AddrMode mode, OprPos pos);
    Error decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode, OprPos pos, OprSize size);

    Error decodeImpl(DisMemory &memory, Insn &insn, StrBuffer &out) override;
    const ConfigBase &config() const override { return *this; }
    ConfigSetter &configSetter() override { return *this; }
};

}  // namespace ns32000
}  // namespace libasm

#endif  // __DIS_NS32000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
