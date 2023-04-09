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

class DisNs32000 : public Disassembler, public Config {
public:
    DisNs32000();

    const ConfigBase &config() const override { return *this; }
    void reset() override;

private:
    NationalValueFormatter _formatter;
    bool _stringOptionBracket;
    bool _pcRelativeParen;
    bool _externalParen;
    bool _floatPrefix;
    const struct OptFloatPrefix : public BoolOption {
        OptFloatPrefix(bool &var);
    } _opt_floatPrefix{_floatPrefix};
    const struct OptStroptBracket : public BoolOption {
        OptStroptBracket(bool &var, const OptionBase &next);
    } _opt_stroptBracket{_stringOptionBracket, _opt_floatPrefix};
    const struct OptExteranlParen : public BoolOption {
        OptExteranlParen(bool &var, const OptionBase &next);
    } _opt_externalParen{_externalParen, _opt_stroptBracket};
    const struct OptPcrelParen : public BoolOption {
        OptPcrelParen(bool &var, const OptionBase &next);
    } _opt_pcrelParen{_pcRelativeParen, _opt_externalParen};

    struct Displacement {
        int32_t val32;
        uint8_t bits;
    };

    StrBuffer &outDisplacement(StrBuffer &out, const Displacement &disp);

    Error readIndexByte(DisMemory &memory, InsnNs32000 &insn, AddrMode mode, OprPos pos);
    Error readDisplacement(DisMemory &memory, InsnNs32000 &insn, Displacement &disp);

    Error decodeLength(DisMemory &memory, InsnNs32000 &insn, StrBuffer &out, AddrMode mode);
    Error decodeBitField(DisMemory &memory, InsnNs32000 &insn, StrBuffer &out, AddrMode mode);
    Error decodeImmediate(DisMemory &memory, InsnNs32000 &insn, StrBuffer &out, AddrMode mode);
    Error decodeDisplacement(DisMemory &memory, InsnNs32000 &insn, StrBuffer &out, AddrMode mode);
    Error decodeRelative(DisMemory &memory, InsnNs32000 &insn, StrBuffer &out);
    Error decodeConfig(const InsnNs32000 &insn, StrBuffer &out, OprPos pos);
    Error decodeStrOpt(const InsnNs32000 &insn, StrBuffer &out, OprPos pos);
    Error decodeRegisterList(DisMemory &memory, InsnNs32000 &insn, StrBuffer &out);
    Error decodeGeneric(
            DisMemory &memory, InsnNs32000 &insn, StrBuffer &out, AddrMode mode, OprPos pos);
    Error decodeOperand(DisMemory &memory, InsnNs32000 &insn, StrBuffer &out, AddrMode mode,
            OprPos pos, OprSize size);

    Error decodeImpl(DisMemory &memory, Insn &insn, StrBuffer &out) override;
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
