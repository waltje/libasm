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

#include "dis_i8080.h"

#include "reg_i8080.h"
#include "table_i8080.h"

namespace libasm {
namespace i8080 {

using namespace reg;

DisI8080::DisI8080() : Disassembler(_hexFormatter, '$'), Config(TABLE) {
    reset();
}

Error DisI8080::decodeOperand(DisInsn &insn, StrBuffer &out, AddrMode mode) {
    switch (mode) {
    case M_IM8:
    case M_IOA:
        outHex(out, insn.readByte(), 8);
        break;
    case M_IM16:
    case M_ABS:
        outHex(out, insn.readUint16(), 16);
        break;
    case M_PTR:
        outRegName(out, decodePointerReg(insn.opCode() >> 4));
        return OK;
    case M_STK:
        outRegName(out, decodeStackReg(insn.opCode() >> 4));
        return OK;
    case M_IDX:
        outRegName(out, decodeIndexReg(insn.opCode() >> 4));
        return OK;
    case M_REG:
        outRegName(out, decodeDataReg(insn.opCode()));
        return OK;
    case M_DST:
        outRegName(out, decodeDataReg(insn.opCode() >> 3));
        return OK;
    case M_VEC:
        outHex(out, (insn.opCode() >> 3) & 7, 3);
        return OK;
    default:
        return OK;
    }
    return setError(insn);
}

Error DisI8080::decodeImpl(DisMemory &memory, Insn &_insn, StrBuffer &out) {
    DisInsn insn(_insn, memory);
    auto opCode = insn.readByte();
    insn.setOpCode(opCode);
    if (TABLE.isPrefix(cpuType(), opCode)) {
        const auto prefix = opCode;
        opCode = insn.readByte();
        insn.setOpCode(opCode, prefix);
    }
    if (setError(insn))
        return getError();

    if (TABLE.searchOpCode(cpuType(), insn, out))
        return setError(insn);

    const auto dst = insn.dst();
    if (dst == M_NONE)
        return OK;
    if (decodeOperand(insn, out, dst))
        return getError();
    const auto src = insn.src();
    if (src == M_NONE)
        return OK;
    out.comma();
    return decodeOperand(insn, out, src);
}

}  // namespace i8080
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
