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

#include "table_tms32010.h"

#include "entry_table.h"
#include "entry_tms32010.h"
#include "text_tms32010.h"

using namespace libasm::text::tms32010;

namespace libasm {
namespace tms32010 {

#define E3(_opc, _name, _opr1, _opr2, _opr3) \
    { _opc, Entry::Flags::create(_opr1, _opr2, _opr3), _name }
#define E2(_opc, _name, _opr1, _opr2) E3(_opc, _name, _opr1, _opr2, M_NONE)
#define E1(_opc, _name, _opr1) E2(_opc, _name, _opr1, M_NONE)
#define E0(_opc, _name) E1(_opc, _name, M_NONE)

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

static constexpr uint8_t INDEX_TMS32010[] PROGMEM = {
     37,  // TEXT_ABS
      0,  // TEXT_ADD
      9,  // TEXT_ADDH
     10,  // TEXT_ADDS
     28,  // TEXT_AND
     44,  // TEXT_APAC
     53,  // TEXT_B
     49,  // TEXT_BANZ
     57,  // TEXT_BGEZ
     56,  // TEXT_BGZ
     51,  // TEXT_BIOZ
     55,  // TEXT_BLEZ
     54,  // TEXT_BLZ
     58,  // TEXT_BNZ
     50,  // TEXT_BV
     59,  // TEXT_BZ
     41,  // TEXT_CALA
     52,  // TEXT_CALL
     35,  // TEXT_DINT
     19,  // TEXT_DMOV
     36,  // TEXT_EINT
      5,  // TEXT_IN
      2,  // TEXT_LAC
     33,  // TEXT_LACK
      4,  // TEXT_LAR
     26,  // TEXT_LARK
     17,  // TEXT_LARP
     25,  // TEXT_LDP
     24,  // TEXT_LDPK
     30,  // TEXT_LST
     20,  // TEXT_LT
     22,  // TEXT_LTA
     21,  // TEXT_LTD
     18,  // TEXT_MAR
     23,  // TEXT_MPY
     48,  // TEXT_MPYK
     34,  // TEXT_NOP
     29,  // TEXT_OR
      6,  // TEXT_OUT
     43,  // TEXT_PAC
     47,  // TEXT_POP
     46,  // TEXT_PUSH
     42,  // TEXT_RET
     39,  // TEXT_ROVM
      8,  // TEXT_SACH
      7,  // TEXT_SACL
      3,  // TEXT_SAR
     40,  // TEXT_SOVM
     45,  // TEXT_SPAC
     31,  // TEXT_SST
      1,  // TEXT_SUB
     13,  // TEXT_SUBC
     11,  // TEXT_SUBH
     12,  // TEXT_SUBS
     16,  // TEXT_TBLR
     32,  // TEXT_TBLW
     27,  // TEXT_XOR
     38,  // TEXT_ZAC
     14,  // TEXT_ZALH
     15,  // TEXT_ZALS
};
// clang-format on

using EntryPage = entry::TableBase<Entry>;

static constexpr EntryPage TMS32010_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_TMS32010), ARRAY_RANGE(INDEX_TMS32010)},
};

using Cpu = entry::CpuBase<CpuType, EntryPage>;

static constexpr Cpu CPU_TABLE[] PROGMEM = {
        {TMS32010, TEXT_CPU_32010, ARRAY_RANGE(TMS32010_PAGES)},
        {TMS32015, TEXT_CPU_32015, ARRAY_RANGE(TMS32010_PAGES)},
};

static const Cpu *cpu(CpuType cpuType) {
    return Cpu::search(cpuType, ARRAY_RANGE(CPU_TABLE));
}

static bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table)
        return true;
    if (opr == M_NONE)  // These can be ommitted.
        return table == M_LS0 || table == M_LS3 || table == M_LS4 || table == M_NARP;
    if (opr == M_AR)  // ARn can be used as 0,1
        return table == M_ARK || table == M_NARP;
    if (opr == M_PA)  // PAn can not match other than itself.
        return false;
    if (opr == M_ARP || opr == M_INC || opr == M_DEC)
        return table == M_MAM;
    if (opr == M_IM13 && table == M_PMA)
        return true;
    // Compare constant range.
    auto tv = uint8_t(table);
    if (tv >= uint8_t(M_LS0) && tv <= uint8_t(M_IM13)) {
        const uint8_t ov = uint8_t(opr);
        return tv >= ov;
    }
    return false;
}

static bool acceptModes(AsmInsn &insn, const Entry *entry) {
    auto flags = insn.flags();
    auto table = entry->flags();
    return acceptMode(flags.mode1(), table.mode1()) && acceptMode(flags.mode2(), table.mode2()) &&
           acceptMode(flags.mode3(), table.mode3());
}

Error TableTms32010::searchName(CpuType cpuType, AsmInsn &insn) const {
    cpu(cpuType)->searchName(insn, acceptModes);
    return insn.getError();
}

static bool matchOpCode(DisInsn &insn, const Entry *entry, const EntryPage *page) {
    auto opCode = insn.opCode();
    const auto flags = entry->flags();
    const auto mode1 = flags.mode1();
    const auto mode2 = flags.mode2();
    if (mode1 == M_IM8 || mode2 == M_IM8) {
        opCode &= ~0xFF;
    } else if (mode1 == M_MAM || mode2 == M_MAM) {
        if ((opCode & (1 << 7)) == 0) {
            opCode &= ~0x7F;  // Direct addressing
        } else {
            opCode &= ~0xB9;  // Indirect addressing
        }
    } else if (mode1 == M_IM13) {
        opCode &= ~0x1FFF;
    }
    if (mode1 == M_AR)
        opCode &= ~(1 << 8);
    if (mode1 == M_ARK || mode1 == M_DPK)
        opCode &= ~(1 << 0);
    if (mode2 == M_LS4)
        opCode &= ~(0xF << 8);
    if (mode2 == M_PA || mode2 == M_LS3 || mode2 == M_LS0)
        opCode &= ~(7 << 8);
    return opCode == entry->opCode();
}

Error TableTms32010::searchOpCode(CpuType cpuType, DisInsn &insn, StrBuffer &out) const {
    cpu(cpuType)->searchOpCode(insn, out, matchOpCode);
    return insn.getError();
}

const /*PROGMEM*/ char *TableTms32010::listCpu_P() const {
    return TEXT_CPU_LIST;
}

const /*PROGMEM*/ char *TableTms32010::cpuName_P(CpuType cpuType) const {
    return cpu(cpuType)->name_P();
}

Error TableTms32010::searchCpuName(StrScanner &name, CpuType &cpuType) const {
    if (name.istarts_P(TEXT_CPU_LIST, 3))
        name += 3;
    if (name.iequals_P(TEXT_CPU_32010)) {
        cpuType = TMS32010;
    } else if (name.iequals_P(TEXT_CPU_32015)) {
        cpuType = TMS32015;
    } else {
        return UNSUPPORTED_CPU;
    }
    return OK;
}

const TableTms32010 TABLE;

}  // namespace tms32010
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
