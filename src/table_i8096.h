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

#ifndef __TABLE_I8096_H__
#define __TABLE_I8096_H__

#include "config_i8096.h"
#include "insn_i8096.h"
#include "table_base.h"

namespace libasm {
namespace i8096 {

class TableI8096 : public TableBase {
public:
    static TableI8096 TABLE;

    Error searchName(InsnI8096 &insn);
    Error searchOpCode(InsnI8096 &insn);
    bool isPrefix(Config::opcode_t opCode) const;

    const /* PROGMEM */ char *listCpu_P() const override;
    const /* PROGMEM */ char *cpu_P() const override;
    bool setCpu(const char *cpu) override;

    typedef PrefixedEntryPage<Entry> EntryPage;

private:
    Error searchName(InsnI8096 &insn, const EntryPage *pages, const EntryPage *end) const;
    Error searchOpCode(InsnI8096 &insn, const EntryPage *pages, const EntryPage *end) const;
};

}  // namespace i8096
}  // namespace libasm

#endif  // __TABLE_I8096_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
