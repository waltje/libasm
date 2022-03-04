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

#ifndef __INSN_INS8070_H__
#define __INSN_INS8070_H__

#include "config_ins8070.h"
#include "entry_ins8070.h"
#include "insn_base.h"

namespace libasm {
namespace ins8070 {

class InsnIns8070 : public InsnImpl<Config, Entry> {
public:
    InsnIns8070(Insn &insn) : InsnImpl(insn) {}

    AddrMode addrMode() const { return flags().mode(); }
    OprFormat dstOpr() const { return flags().dstOpr(); }
    OprFormat srcOpr() const { return flags().srcOpr(); }
    OprSize oprSize() const { return flags().size(); }
    void setOprFormats(OprFormat dst, OprFormat src) {
        setFlags(Entry::Flags::create(M_UNDEF, dst, src, SZ_NONE));
    }

    void emitInsn() { emitByte(opCode(), 0); }
    void emitOperand8(uint8_t val8) { emitByte(val8, operandPos()); }
    void emitOperand16(uint16_t val16) { emitUint16(val16, operandPos()); }

private:
    uint8_t operandPos() const {
        uint8_t pos = length();
        if (pos == 0)
            pos = 1;
        return pos;
    }
};

}  // namespace ins8070
}  // namespace libasm

#endif  // __INSN_INS8070_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
