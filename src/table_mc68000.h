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

#ifndef __TABLE_MC68000_H__
#define __TABLE_MC68000_H__

#include "config_mc68000.h"
#include "insn_mc68000.h"

namespace libasm {
namespace mc68000 {

class TableMc68000 {
public:
    Error searchName(Insn &insn) const;
    Error searchNameAndAddrMode(Insn &insn) const;
    Error searchInsnCode(Insn &insn) const;

    bool setCpu(const char *cpu);
    static const char *listCpu();
};

extern TableMc68000 TableMc68000;

} // namespace mc68000
} // namespace libasm

#endif // __TABLE_MC68000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
