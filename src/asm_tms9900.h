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

#ifndef __ASM_TMS9900_H__
#define __ASM_TMS9900_H__

#include "asm_base.h"
#include "config_tms9900.h"
#include "insn_tms9900.h"
#include "reg_tms9900.h"
#include "table_tms9900.h"

namespace libasm {
namespace tms9900 {

class AsmTms9900
    : public Assembler,
      public Config {
public:
    ValueParser *getParser() override { return &_parser; }

private:
    IntelValueParser _parser;

    TableBase &getTable() const override { return TableTms9900; }

    Error checkComma();

    Error encodeImm(InsnTms9900 &insn, bool emitInsn);
    Error encodeImmMod(InsnTms9900 &insn);
    Error encodeReg(InsnTms9900 &insn, bool emitInsn);
    Error encodeCnt(InsnTms9900 &insn, bool acceptR0, bool accept16);
    Error encodeOpr(
        InsnTms9900 &insn, bool emitInsn, bool destination = false);
    Error encodeOpr(Config::opcode_t &opCode, uint16_t &operand);
    bool needsOperandWord(Config::opcode_t opCode) const;
    Error encodeRel(InsnTms9900 &insn);
    Error encodeCruOff(InsnTms9900 &insn);
    Error encodeIoaddr(InsnTms9900 &insn);
    Error encodeDoubleWords(InsnTms9900 &insn);

    Error encode(Insn &insn) override;
};

} // namespace tms9900
} // namespace libasm

#endif // __ASM_TMS9900_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
