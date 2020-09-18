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

#ifndef __INSN_I8080_H__
#define __INSN_I8080_H__

#include "config_i8080.h"
#include "entry_i8080.h"
#include "insn_base.h"

namespace libasm {
namespace i8080 {

class InsnI8080 : public InsnBase<Config> {
public:
    InsnI8080(Insn &insn) : InsnBase(insn) {}

    AddrMode addrMode() const { return Entry::_addrMode(_flags); }
    InsnFormat insnFormat() const { return Entry::_insnFormat(_flags); }

    void setFlags(uint8_t flags) { _flags = flags; }

    void setOpCode(Config::opcode_t opCode) {
        _opCode = opCode;
    }

    void embed(Config::opcode_t data) {
        _opCode |= data;
    }

    Config::opcode_t opCode() const { return _opCode; }

private:
    uint8_t _flags;
    Config::opcode_t _opCode;
};

} // namespace i8080
} // namespace libasm

#endif // __INSN_I8080_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
