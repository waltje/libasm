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

#ifndef __ASM_W65C816_H__
#define __ASM_W65C816_H__

#include "asm_base.h"
#include "config_w65c816.h"
#include "insn_w65c816.h"
#include "reg_mos6502.h"
#include "table_w65c816.h"

namespace libasm {
namespace w65c816 {

class AsmW65C816
    : public Assembler,
      public Config {
public:
    AsmOperand &getParser() override { return _parser; }

    // Config
    const char *listCpu() const override { return TableW65C816.listCpu(); }
    bool setCpu(const char *cpu) override { return TableW65C816.setCpu(cpu); }

private:
    AsmMotoOperand _parser;
    mos6502::RegMos6502 _regs;

    struct Operand : public ErrorReporter {
        AddrMode mode;
        uint32_t val32;
    };

    Error selectMode(
        char modifier, Operand &op, AddrMode labs, AddrMode abs, AddrMode zp);
    Error parseOperand(Operand &op);

    Error encodeLongRelative(InsnW65C816 &insn);
    Error encodeRelative(InsnW65C816 &insn, bool emitInsn);
    Error encodeZeroPageRelative(InsnW65C816 &insn);
    Error encodeBlockMove(InsnW65C816 &insn);

    Error encode(Insn &insn) override;
};

} // namespace w65c816
} // namespace libasm

#endif // __ASM_W65C816_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
