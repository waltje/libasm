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

#ifndef __INSN_MC68000_H__
#define __INSN_MC68000_H__

#include "config_mc68000.h"
#include "entry_mc68000.h"
#include "insn_base.h"
#include "reg_mc68000.h"

namespace libasm {
namespace mc68000 {

class InsnMc68000 : public InsnBase<Config> {
public:
    InsnMc68000(Insn &insn) : InsnBase(insn) {}

    InsnFormat insnFormat() const { return Entry::_insnFormat(_fmt); }

    void setFmt(uint8_t fmt) {
        _fmt = fmt;
    }

    Config::opcode_t opCode() const { return _opCode; }
    void setOpCode(Config::opcode_t opCode) {
        _opCode = opCode;
    }
    void embed(Config::opcode_t data, uint8_t gp = 0) {
        _opCode |= (data << gp);
    }

    void setSize(EaSize size) { _size = size; }
    EaSize size() const { return _size; }
    void appendSize(EaSize size, RegMc68000 regs) {
        _size = size;
        char suffix[4];
        regs.outEaSize(suffix, size);
        _insn.appendName(suffix);
    }

    void emitInsn() {
        emitUint16(_opCode, 0);
    }

    void emitOperand16(uint16_t val16) {
        uint8_t pos = _insn.length();
        if (pos == 0) pos = 2;
        emitUint16(val16, pos);
    }

    void emitOperand32(uint32_t val32) {
        emitOperand16(static_cast<uint16_t>(val32 >> 16));
        emitOperand16(static_cast<uint16_t>(val32));
    }

private:
    Config::opcode_t _opCode;
    uint8_t _fmt;
    EaSize _size;

    void emitUint16(uint16_t val, uint8_t pos) {
        _insn.emitByte(static_cast<uint8_t>(val >> 8), pos + 0);
        _insn.emitByte(static_cast<uint8_t>(val >> 0), pos + 1);
    }
};

} // namespace mc68000
} // namespace libasm

#endif // __INSN_MC68000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
