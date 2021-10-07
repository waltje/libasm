/*
 * Copyright 2021 Tadashi G. Takaoka
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

#include "dis_tms32010.h"

#include "table_tms32010.h"

namespace libasm {
namespace tms32010 {

static const char COMMA[] PROGMEM = ", ";

Error DisTms32010::decodeDirect(StrBuffer &out, Config::opcode_t opc) {
    // Store Status Register can access Data Page 1 only.
    static constexpr uint8_t SST = 0x7C;
    uint8_t dma = static_cast<uint8_t>(opc) & 0x7F;
    if ((opc >> 8) == SST) {
        dma |= (1 << 7);
        if (dma > TableTms32010.dataMemoryLimit())
            return setError(OVERFLOW_RANGE);
    }
    outAbsAddr(out, dma, 8);
    return OK;
}

Error DisTms32010::decodeIndirect(StrBuffer &out, uint8_t mam) {
    switch ((mam >> 4) & 3) {
    case 0:
        out.letter('*');
        return OK;
    case 1:
        out.letter('*').letter('-');
        return OK;
    case 2:
        out.letter('*').letter('+');
        return OK;
    default:
        return setError(UNKNOWN_INSTRUCTION);
    }
}

Error DisTms32010::decodeNextArp(StrBuffer &out, uint8_t mam) {
    if ((mam & (1 << 7)) == 0)
        return OK;       // Direct memory address;
    if (mam & (1 << 3))  // No next auxilialy register pointer
        return (mam & 7) == 0 ? OK : setError(UNKNOWN_INSTRUCTION);
    const RegName arp = (mam & 1) == 0 ? REG_AR0 : REG_AR1;
    _regs.outRegName(out.comma(), arp);
    return OK;
}

Error DisTms32010::decodeShiftCount(StrBuffer &out, uint8_t count, uint8_t mam, AddrMode mode) {
    if (mode == M_LS4 || (mode == M_LS3 && (count == 0 || count == 1 || count == 4)) ||
            (mode == M_LS0 && count == 0)) {
        const bool indir = mam & (1 << 7);
        const bool nar = (mam & (1 << 3)) == 0;
        if (count || (indir && nar)) {
            out.comma();
            outDec(out, count, 4);
        }
        return OK;
    }
    return setError(UNKNOWN_INSTRUCTION);
}

Error DisTms32010::decodeOperand(
        DisMemory &memory, InsnTms32010 &insn, StrBuffer &out, AddrMode mode) {
    const Config::opcode_t opc = insn.opCode();
    switch (mode) {
    case M_MAM:
        if (opc & (1 << 7))
            return decodeIndirect(out, opc);
        return decodeDirect(out, opc);
    case M_NARP:
        return decodeNextArp(out, opc);
    case M_LS4:
        return decodeShiftCount(out, (opc >> 8) & 0xF, opc, mode);
    case M_LS3:
    case M_LS0:
        return decodeShiftCount(out, (opc >> 8) & 7, opc, mode);
    case M_AR:
        _regs.outRegName(out, (opc & (1 << 8)) == 0 ? REG_AR0 : REG_AR1);
        break;
    case M_PA:
        _regs.outRegName(out, RegName(((opc >> 8) & 7) + int8_t(REG_PA0)));
        break;
    case M_ARK:
        _regs.outRegName(out, (opc & 1) == 0 ? REG_AR0 : REG_AR1);
        break;
    case M_DPK:
        out.letter((opc & 1) == 0 ? '0' : '1');
        break;
    case M_IM8:
        outDec(out, static_cast<uint8_t>(opc), 8);
        break;
    case M_IM13: {
        // Sign extends 13-bit number as 0x1000 is a sign bit.
        const int16_t im13 = (opc & 0xFFF) - (opc & 0x1000);
        outDec(out, im13, -13);
        break;
    }
    case M_PMA: {
        uint16_t pma = insn.readUint16(memory);
        if (pma & 0xF000)
            return setError(OVERFLOW_RANGE);
        outAbsAddr(out, pma);
        break;
    }
    default:
        break;
    }
    return OK;
}

Error DisTms32010::decode(DisMemory &memory, Insn &_insn, StrBuffer &out) {
    InsnTms32010 insn(_insn);
    Config::opcode_t opCode = insn.readUint16(memory);

    insn.setOpCode(opCode);
    if (TableTms32010.searchOpCode(insn))
        return setError(TableTms32010.getError());

    const AddrMode op1 = insn.op1();
    if (op1 == M_NO)
        return OK;
    if (decodeOperand(memory, insn, out, op1))
        return getError();
    const AddrMode op2 = insn.op2();
    if (op2 == M_NO)
        return OK;
    if (!(op2 == M_LS4 || op2 == M_LS3 || op2 == M_LS0 || op2 == M_NARP))
        out.comma();
    if (decodeOperand(memory, insn, out, op2))
        return getError();
    const AddrMode op3 = insn.op3();
    if (op3 == M_NO)
        return OK;
    return decodeOperand(memory, insn, out, op3);
}

}  // namespace tms32010
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
