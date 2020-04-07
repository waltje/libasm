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

#ifndef __INSN_MC6800_H__
#define __INSN_MC6800_H__

#include "insn_base.h"
#include "entry_mc6800.h"

namespace libasm {
namespace mc6800 {

class InsnMc6800 : public InsnBase<Config::endian, Config::uintptr_t> {
public:
    InsnMc6800(Insn<Config::uintptr_t> &insn) : InsnBase(insn) {}

    AddrMode addrMode() const { return Entry::_addrMode(_flags); }
    InsnAdjust insnAdjust() const { return Entry::_insnAdjust(_flags); }
    OprSize oprSize() const { return Entry::_oprSize(_flags); }

    void setFlags(host::uint_t flags) {
        _flags = flags;
    }

    void setAddrMode(AddrMode addrMode) {
        _flags = Entry::_flags(
            addrMode, Entry::_insnAdjust(_flags), Entry::_oprSize(_flags));
    }

    Config::insn_t insnCode() const { return _insnCode; }
    void setInsnCode(Config::insn_t insnCode) {
        _insnCode = insnCode;
    }
    void embed(Config::opcode_t data) {
        _insnCode |= data;
    }
    void emitInsn() {
        emitByte(_insnCode);
    }

private:
    Config::insn_t _insnCode;
    host::uint_t _flags;
};

} // namespace mc6800
} // namespace libasm

#endif // __INSN_MC6800_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
