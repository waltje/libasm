/*
 * Copyright 2023 Tadashi G. Takaoka
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

#ifndef __INSN_F3850_H__
#define __INSN_F3850_H__

#include "config_f3850.h"
#include "entry_f3850.h"
#include "insn_base.h"

namespace libasm {
namespace f3850 {

struct EntryInsn : EntryInsnBase<Config, Entry> {
    AddrMode mode1() const { return flags().mode1(); }
    AddrMode mode2() const { return flags().mode2(); }
    void setAddrMode(AddrMode mode1, AddrMode mode2) {
        setFlags(Entry::Flags::create(mode1, mode2));
    }
};

struct AsmInsn final : AsmInsnImpl<Config>, EntryInsn {
    AsmInsn(Insn &insn) : AsmInsnImpl(insn) {}

    void emitInsn() { emitByte(opCode(), 0); }
    void emitOperand8(uint8_t val) { emitByte(val, 1); }
    void emitOperand16(uint16_t val) { emitUint16(val, 1); }
};

struct DisInsn final : DisInsnImpl<Config>, EntryInsn {
    DisInsn(Insn &insn, DisMemory &memory) : DisInsnImpl(insn, memory) {}
};

}  // namespace f3850
}  // namespace libasm

#endif  // __INSN_F3850_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
