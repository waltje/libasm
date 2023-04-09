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

#ifndef __TABLE_INS8060_H__
#define __TABLE_INS8060_H__

#include "config_ins8060.h"
#include "entry_table.h"
#include "insn_ins8060.h"

namespace libasm {
namespace ins8060 {

struct TableIns8060 : entry::Table {
public:
    TableIns8060();

    static TableIns8060 TABLE;

    Error searchName(InsnIns8060 &insn) const;
    Error searchOpCode(InsnIns8060 &insn, StrBuffer &out) const;

    const /* PROGMEM */ char *cpu_P() const override { return _cpu->name_P(); }
    bool setCpu(const char *cpu) override;

    typedef entry::TableBase<Entry> EntryPage;
    typedef entry::CpuBase<CpuType, EntryPage> Cpu;

private:
    const Cpu *const _cpu;
};

}  // namespace ins8060
}  // namespace libasm

#endif  // __TABLE_INS8060_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
