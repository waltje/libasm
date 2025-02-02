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

#ifndef __INSN_I8086_H__
#define __INSN_I8086_H__

#include "config_i8086.h"
#include "entry_i8086.h"
#include "insn_base.h"

namespace libasm {
namespace i8086 {

struct EntryInsn : EntryInsnBase<Config, Entry> {
    EntryInsn() : _segment(0) {}

    AddrMode dst() const { return flags().dst(); }
    AddrMode src() const { return flags().src(); }
    AddrMode ext() const { return flags().ext(); }
    OprPos dstPos() const { return flags().dstPos(); }
    OprPos srcPos() const { return flags().srcPos(); }
    OprPos extPos() const { return flags().extPos(); }
    OprSize size() const { return flags().size(); }
    bool stringInst() const { return flags().stringInst(); }
    void setAddrMode(AddrMode dst, AddrMode src, AddrMode ext) {
        setFlags(Entry::Flags::create(dst, src, ext, P_NONE, P_NONE, P_NONE, SZ_NONE));
    }

    void setSegment(Config::opcode_t segment) { _segment = segment; }
    Config::opcode_t segment() const { return _segment; }

protected:
    Config::opcode_t _segment;
};

struct AsmInsn final : AsmInsnImpl<Config>, EntryInsn {
    AsmInsn(Insn &insn) : AsmInsnImpl(insn), _modReg(0), _hasModReg(false) {}

    void embedModReg(Config::opcode_t data) {
        _modReg |= data;
        _hasModReg = true;
    }
    Config::opcode_t modReg() const { return _modReg; }

    void prepairModReg() {
        if (hasPrefix())
            return;
        const OprPos dst = dstPos();
        const OprPos src = srcPos();
        if (dst == P_MOD || dst == P_REG || dst == P_MREG || src == P_MOD || src == P_REG)
            embedModReg(0);
    }

    void emitInsn() {
        uint8_t pos = 0;
        if (_segment)
            emitByte(_segment, pos++);
        if (hasPrefix())
            emitByte(prefix(), pos++);
        emitByte(opCode(), pos++);
        if (_hasModReg)
            emitByte(_modReg, pos);
    }
    void emitOperand8(uint8_t val8) { emitByte(val8, operandPos()); }
    void emitOperand16(uint16_t val16) { emitUint16(val16, operandPos()); }

private:
    Config::opcode_t _modReg;
    bool _hasModReg;

    uint8_t operandPos() const {
        uint8_t pos = length();
        if (pos == 0) {
            if (_segment)
                pos++;
            if (hasPrefix())
                pos++;
            pos++;
            if (_hasModReg)
                pos++;
        }
        return pos;
    }
};

struct DisInsn final : DisInsnImpl<Config>, EntryInsn {
    DisInsn(Insn &insn, DisMemory &memory) : DisInsnImpl(insn, memory) {}
    DisInsn(Insn &insn, DisInsn &o) : DisInsnImpl(insn, o) {}

    void readModReg() {
        const OprPos dst = dstPos();
        const OprPos src = srcPos();
        if (dst == P_MOD || dst == P_REG || src == P_MOD || src == P_REG)
            _modReg = readByte();
        else if (dst == P_OMOD || src == P_OMOD)
            _modReg = opCode();
    }
    Config::opcode_t modReg() const { return _modReg; }

private:
    Config::opcode_t _modReg;
};

}  // namespace i8086
}  // namespace libasm

#endif  // __INSN_I8086_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
