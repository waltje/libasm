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

#ifndef __TABLE_INS8070_H__
#define __TABLE_INS8070_H__

#include "config_ins8070.h"
#include "insn_ins8070.h"

namespace libasm {
namespace ins8070 {

struct TableIns8070 final : InsnTable<CpuType> {
    const /*PROGMEM*/ char *listCpu_P() const override;
    const /*PROGMEM*/ char *cpuName_P(CpuType cpuType) const override;
    Error searchCpuName(StrScanner &name, CpuType &cpuType) const override;

    Error searchName(CpuType, AsmInsn &insn) const;
    Error searchOpCode(CpuType, DisInsn &insn, StrBuffer &out) const;
};

extern const TableIns8070 TABLE;

}  // namespace ins8070
}  // namespace libasm

#endif  // __TABLE_INS8070_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
