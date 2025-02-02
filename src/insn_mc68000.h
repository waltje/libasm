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

namespace libasm {
namespace mc68000 {

struct EntryInsn : EntryInsnBase<Config, Entry> {
    AddrMode src() const { return flags().src(); }
    AddrMode dst() const { return flags().dst(); }
    OprPos srcPos() const { return flags().srcPos(); }
    OprPos dstPos() const { return flags().dstPos(); }
    bool alias() const { return flags().alias(); }
    OprSize oprSize() const { return flags().oprSize(); }
    InsnSize insnSize() const { return flags().insnSize(); }
    bool hasSize() const { return flags().hasSize(); }
    void setAddrMode(AddrMode src, AddrMode dst) { flags().setAddrMode(src, dst); }

    void setInsnSize(OprSize osize) { flags().setInsnSize(InsnSize(osize)); }
};

struct AsmInsn final : AsmInsnImpl<Config>, EntryInsn {
    AsmInsn(Insn &insn) : AsmInsnImpl(insn) {}

    void emitInsn() { emitUint16(opCode(), 0); }
    void emitOperand16(uint16_t val16) { emitUint16(val16, operandPos()); }
    void emitOperand32(uint32_t val32) { emitUint32(val32, operandPos()); }

    OprSize parseInsnSize();

private:
    uint8_t operandPos() const {
        uint8_t pos = length();
        if (pos == 0)
            pos = sizeof(Config::opcode_t);
        return pos;
    }
};

struct DisInsn final : DisInsnImpl<Config>, EntryInsn {
    DisInsn(Insn &insn, DisMemory &memory) : DisInsnImpl(insn, memory) {}
};

}  // namespace mc68000
}  // namespace libasm

#endif  // __INSN_MC68000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
