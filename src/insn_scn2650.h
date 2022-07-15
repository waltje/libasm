/*
 * Copyright 2022 Tadashi G. Takaoka
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

#ifndef __INSN_SCN2650_H__
#define __INSN_SCN2650_H__

#include "config_scn2650.h"
#include "entry_scn2650.h"
#include "insn_base.h"

namespace libasm {
namespace scn2650 {

class InsnScn2650 : public InsnImpl<Config, Entry> {
public:
    InsnScn2650(Insn &insn) : InsnImpl(insn) {}

    AddrMode mode1() const { return flags().mode1(); }
    AddrMode mode2() const { return flags().mode2(); }
    void setAddrMode(AddrMode mode1, AddrMode mode2) {
        setFlags(Entry::Flags::create(mode1, mode2));
    }
    bool undefined() const { return mode1() == UNDF; }

    void emitInsn() { emitByte(opCode(), 0); }
    void emitOperand8(uint8_t val) { emitByte(val, 1); }
    void emitOperand16(uint16_t val) { emitUint16(val, 1); }
};

}  // namespace scn2650
}  // namespace libasm

#endif  // __INSN_SCN2650_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
