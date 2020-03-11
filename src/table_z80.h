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

#ifndef __TABLE_Z80_H__
#define __TABLE_Z80_H__

#include "config_z80.h"
#include "insn_z80.h"

class TableZ80 {
public:
    Error searchName(Insn &insn) const;
    Error searchNameAndOprFormats(
        Insn &insn, OprFormat leftOpr, OprFormat rightOpr) const;
    Error searchInsnCode(Insn &insn) const;

    bool setCpu(const char *cpu);
    static const char *listCpu();
    bool isZ80() const { return _cpuType == Z80; }

    static bool isPrefixCode(target::opcode_t opCode);

    static constexpr target::opcode_t PREFIX_IX = 0xDD;
    static constexpr target::opcode_t PREFIX_IY = 0xFD;

private:
    CpuType _cpuType;
};

extern TableZ80 TableZ80;

#endif // __TABLE_Z80_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
