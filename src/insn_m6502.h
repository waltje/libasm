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

#ifndef __INSN_M6502_H__
#define __INSN_M6502_H__

#include "config_m6502.h"
#include "entry_m6502.h"
#include "insn_base.h"

namespace libasm {
namespace m6502 {

class InsnM6502 : public InsnBase<Config> {
public:
    InsnM6502(Insn &insn) : InsnBase(insn) {}

    AddrMode addrMode() const { return Entry::_addrMode(_flags); }
    bool supported(CpuType cpuType) const {
        const CpuType insnType = Entry::_cpuType(_flags);
        if (host::uint_t(cpuType) < host::uint_t(insnType))
            return false;
        if (cpuType == W65C816 && insnType == R65C02BIT)
            return false;
        return true;
    }

    void setFlags(host::uint_t flags) {
        _flags = flags;
    }

    void setAddrMode(AddrMode addrMode) {
        _flags = Entry::_flags(Entry::_cpuType(_flags), addrMode);
    }

    Config::insn_t insnCode() const { return _insnCode; }
    void setInsnCode(Config::insn_t insnCode) {
        _insnCode = insnCode;
    }
    void emitInsn() {
        emitByte(_insnCode);
    }

private:
    Config::insn_t _insnCode;
    host::uint_t _flags;
};

} // namespace m6502
} // namespace libasm

#endif // __INSN_M6502_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
