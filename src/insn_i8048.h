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

#ifndef __INSN_I8048_H__
#define __INSN_I8048_H__

#include "config_i8048.h"
#include "entry_i8048.h"
#include "insn_base.h"

namespace libasm {
namespace i8048 {

class InsnI8048 : public InsnImpl<Config, Entry> {
public:
    InsnI8048(Insn &insn) : InsnImpl(insn) {}

    AddrMode dstMode() const { return flags().dstMode(); }
    AddrMode srcMode() const { return flags().srcMode(); }
    void setAddrMode(AddrMode dst, AddrMode src) { setFlags(Entry::Flags::create(dst, src)); }

    void emitInsn() { emitByte(opCode(), 0); }
    void emitOperand8(uint8_t val) { emitByte(val, 1); }
};

}  // namespace i8048
}  // namespace libasm

#endif  // __INSN_I8048_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
