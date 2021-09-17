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

#include "config_tms32010.h"

#include "entry_tms32010.h"
#include "table_tms32010.h"
#include "text_tms32010.h"

#include <ctype.h>
#include <string.h>

namespace libasm {
namespace tms32010 {

#define E3(_opc, _name, _op1, _op2, _op3) \
    { _opc, Entry::Flags::create(_op1, _op2, _op3), _name }
#define E2(_opc, _name, _op1, _op2) E3(_opc, _name, _op1, _op2, M_NO)
#define E1(_opc, _name, _op1) E2(_opc, _name, _op1, M_NO)
#define E0(_opc, _name) E1(_opc, _name, M_NO)

// clang-format off
static constexpr Entry TABLE_TMS32010[] PROGMEM = {
    E3(0x0000, TEXT_ADD,  M_MAM,  M_LS4,  M_NARP),
    E3(0x1000, TEXT_SUB,  M_MAM,  M_LS4,  M_NARP),
    E3(0x2000, TEXT_LAC,  M_MAM,  M_LS4,  M_NARP),
    E3(0x3000, TEXT_SAR,  M_AR,   M_MAM,  M_NARP),
    E3(0x3800, TEXT_LAR,  M_AR,   M_MAM,  M_NARP),
    E3(0x4000, TEXT_IN,   M_MAM,  M_PA,   M_NARP),
    E3(0x4800, TEXT_OUT,  M_MAM,  M_PA,   M_NARP),
    E3(0x5000, TEXT_SACL, M_MAM,  M_LS0,  M_NARP),
    E3(0x5800, TEXT_SACH, M_MAM,  M_LS3,  M_NARP),
    E2(0x6000, TEXT_ADDH, M_MAM,  M_NARP),
    E2(0x6100, TEXT_ADDS, M_MAM,  M_NARP),
    E2(0x6200, TEXT_SUBH, M_MAM,  M_NARP),
    E2(0x6300, TEXT_SUBS, M_MAM,  M_NARP),
    E2(0x6400, TEXT_SUBC, M_MAM,  M_NARP),
    E2(0x6500, TEXT_ZALH, M_MAM,  M_NARP),
    E2(0x6600, TEXT_ZALS, M_MAM,  M_NARP),
    E2(0x6700, TEXT_TBLR, M_MAM,  M_NARP),
    E1(0x6880, TEXT_LARP, M_ARK),
    E2(0x6800, TEXT_MAR,  M_MAM,  M_NARP),
    E2(0x6900, TEXT_DMOV, M_MAM,  M_NARP),
    E2(0x6A00, TEXT_LT,   M_MAM,  M_NARP),
    E2(0x6B00, TEXT_LTD,  M_MAM,  M_NARP),
    E2(0x6C00, TEXT_LTA,  M_MAM,  M_NARP),
    E2(0x6D00, TEXT_MPY,  M_MAM,  M_NARP),
    E1(0x6E00, TEXT_LDPK, M_DPK),
    E2(0x6F00, TEXT_LDP,  M_MAM,  M_NARP),
    E2(0x7000, TEXT_LARK, M_AR,   M_IM8),
    E2(0x7800, TEXT_XOR,  M_MAM,  M_NARP),
    E2(0x7900, TEXT_AND,  M_MAM,  M_NARP),
    E2(0x7A00, TEXT_OR,   M_MAM,  M_NARP),
    E2(0x7B00, TEXT_LST,  M_MAM,  M_NARP),
    E2(0x7C00, TEXT_SST,  M_MAM,  M_NARP),
    E2(0x7D00, TEXT_TBLW, M_MAM,  M_NARP),
    E1(0x7E00, TEXT_LACK, M_IM8),
    E0(0x7F80, TEXT_NOP),
    E0(0x7F81, TEXT_DINT),
    E0(0x7F82, TEXT_EINT),
    E0(0x7F88, TEXT_ABS),
    E0(0x7F89, TEXT_ZAC),
    E0(0x7F8A, TEXT_ROVM),
    E0(0x7F8B, TEXT_SOVM),
    E0(0x7F8C, TEXT_CALA),
    E0(0x7F8D, TEXT_RET),
    E0(0x7F8E, TEXT_PAC),
    E0(0x7F8F, TEXT_APAC),
    E0(0x7F90, TEXT_SPAC),
    E0(0x7F9C, TEXT_PUSH),
    E0(0x7F9D, TEXT_POP),
    E1(0x8000, TEXT_MPYK, M_IM13),
    E1(0xF400, TEXT_BANZ, M_PMA),
    E1(0xF500, TEXT_BV,   M_PMA),
    E1(0xF600, TEXT_BIOZ, M_PMA),
    E1(0xF800, TEXT_CALL, M_PMA),
    E1(0xF900, TEXT_B,    M_PMA),
    E1(0xFA00, TEXT_BLZ,  M_PMA),
    E1(0xFB00, TEXT_BLEZ, M_PMA),
    E1(0xFC00, TEXT_BGZ,  M_PMA),
    E1(0xFD00, TEXT_BGEZ, M_PMA),
    E1(0xFE00, TEXT_BNZ,  M_PMA),
    E1(0xFF00, TEXT_BZ,   M_PMA),
};
// clang-format on

static bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table)
        return true;
    if (opr == M_LS0)
        return table == M_LS4 || table == M_NO;
    if (opr == M_LS3)
        return table == M_LS4 || table == M_NO;
    if (table == M_LS4 || table == M_LS3 || table == M_LS0 || table == M_NARP)
        return opr == M_NO;
    return false;
}

static bool acceptModes(Entry::Flags flags, const Entry *entry) {
    const Entry::Flags table = entry->flags();
    return acceptMode(flags.op1(), table.op1()) && acceptMode(flags.op2(), table.op2()) &&
           acceptMode(flags.op3(), table.op3());
}

Error TableTms32010::searchName(InsnTms32010 &insn) const {
    uint8_t count = 0;
    const Entry *entry = TableBase::searchName<Entry, Entry::Flags>(
            insn.name(), insn.flags(), ARRAY_RANGE(TABLE_TMS32010), acceptModes, count);
    if (entry) {
        insn.setOpCode(entry->opCode());
        insn.setFlags(entry->flags());
        return setOK();
    }
    return setError(count == 0 ? UNKNOWN_INSTRUCTION : OPERAND_NOT_ALLOWED);
}

static Config::opcode_t tableCode(Config::opcode_t opCode, const Entry *entry) {
    const AddrMode op1 = entry->flags().op1();
    const AddrMode op2 = entry->flags().op2();
    Config::opcode_t mask = 0;
    if (op1 == M_IM8 || op2 == M_IM8)
        mask |= 0xFF;
    if (op1 == M_MAM || op2 == M_MAM) {
        if ((opCode & (1 << 7)) == 0) {
            mask |= 0x7F;       // Direct addressing
        } else {
            mask |= 0xB9;       // Indirect addressing
        }
    }
    if (op1 == M_IM13)
        mask |= 0x1FFF;
    if (op1 == M_AR)
        mask |= 1 << 8;
    if (op1 == M_ARK || op1 == M_DPK)
        mask |= (1 << 0);
    if (op2 == M_LS4)
        mask |= 0xF << 8;
    if (op2 == M_PA || op2 == M_LS3 || op2 == M_LS0)
        mask |= 7 << 8;
    return opCode & ~mask;
}

Error TableTms32010::searchOpCode(InsnTms32010 &insn) const {
    const Entry *entry = TableBase::searchCode<Entry, Config::opcode_t>(
            insn.opCode(), ARRAY_RANGE(TABLE_TMS32010), tableCode);
    if (entry) {
        insn.setFlags(entry->flags());
        insn.setName_P(entry->name());
        return setOK();
    }
    return setError(UNKNOWN_INSTRUCTION);
}

const char *TableTms32010::listCpu() const {
    return TEXT_CPU_LIST;
}

const char *TableTms32010::getCpu() const {
    return TEXT_CPU_32010;
}

bool TableTms32010::setCpu(const char *cpu) {
    if (strncasecmp_P(cpu, TEXT_CPU_TMS, 3) == 0)
        cpu += 3;
    return strcmp_P(cpu, TEXT_CPU_32010) == 0;
}

class TableTms32010 TableTms32010;

}  // namespace tms32010
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
