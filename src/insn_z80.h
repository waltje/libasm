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

#ifndef __INSN_Z80_H__
#define __INSN_Z80_H__

#include "config_z80.h"
#include "entry_z80.h"
#include "insn_base.h"

namespace libasm {
namespace z80 {

class InsnZ80 : public InsnBase<Config> {
public:
    InsnZ80(Insn &insn) : InsnBase(insn) {}

    AddrMode addrMode() const { return Entry::_addrMode(_flags); }
    InsnFormat insnFormat() const { return Entry::_insnFormat(_flags); }
    OprFormat dstFormat() const { return Entry::_dstFormat(_flags); }
    OprFormat srcFormat() const { return Entry::_srcFormat(_flags); }

    void setFlags(uint16_t flags) {
        _flags = flags;
    }

    void setFlags(const InsnZ80 &other) {
        _flags = other._flags;
    }

    void setOprFormats(OprFormat dst, OprFormat src) {
        _flags = Entry::_flags(insnFormat(), addrMode(), dst, src);
    }

    void setOpCode(Config::opcode_t opCode, Config::opcode_t prefix = 0) {
        _opCode = opCode;
        _prefix = prefix;
    }

    void embed(Config::opcode_t data) {
        _opCode |= data;
    }

    bool hasPrefix() const { return prefix() != 0; }

    Config::opcode_t prefix() const {
        return _prefix;
    }

    Config::opcode_t opCode() const {
        return _opCode;
    }

    void emitInsn() {
        if (hasPrefix())
            emitByte(prefix());
        emitByte(opCode());
    }

private:
    uint16_t _flags;
    Config::opcode_t _opCode;
    Config::opcode_t _prefix;
};

} // namespace z80
} // namespace libasm

#endif // __INSN_Z80_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
