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

#ifndef __DIS_MC6809_H__
#define __DIS_MC6809_H__

#include "config_mc6809.h"
#include "dis_base.h"
#include "insn_mc6809.h"
#include "reg_mc6809.h"
#include "table_mc6809.h"

namespace libasm {
namespace mc6809 {

class DisMc6809
    : public Disassembler,
      public Config {
public:
    DisOperand &getFormatter() override { return _formatter; }

    const char *listCpu() const override { return TableMc6809.listCpu(); }
    bool setCpu(const char *cpu) override { return TableMc6809.setCpu(cpu); }
    const char *getCpu() const override { return TableMc6809.getCpu(); }

protected:
    RegBase &getRegister() override { return _regs; }

private:
    DisMotoOperand _formatter;
    RegMc6809 _regs;

    void outRegister(RegName regName);

    // MC6809
    Error decodeDirectPage(DisMemory &memory, InsnMc6809 &insn);
    Error decodeIndexed(DisMemory &memory, InsnMc6809 &insn);
    Error decodeExtended(DisMemory &memory, InsnMc6809 &insn);
    Error decodeRelative(DisMemory &memory, InsnMc6809 &insn);
    Error decodeImmediate(DisMemory &memory, InsnMc6809 &insn);
    Error decodeStackOp(DisMemory &memory, InsnMc6809 &insn);
    Error decodeRegisters(DisMemory &memory, InsnMc6809 &insn);
    // HD6309
    Error decodeImmediatePlus(DisMemory &memory, InsnMc6809 &insn);
    Error decodeBitOperation(DisMemory &memory, InsnMc6809 &insn);
    Error decodeTransferMemory(DisMemory &memory, InsnMc6809 &insn);

    Error decode(DisMemory &memory, Insn &insn) override;
};

} // namespace mc6809
} // namespace libasm

#endif // __DIS_MC6809_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
