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

#include "table_ins8060.h"

#include <ctype.h>
#include <string.h>

#include "config_ins8060.h"
#include "entry_ins8060.h"
#include "text_ins8060.h"

namespace libasm {
namespace ins8060 {

#define E(_opc, _name, _mode) \
    { _opc, Entry::Flags::create(_mode), _name }

// clang-format off
static constexpr Entry TABLE_INS8060[] PROGMEM = {
    E(0x00, TEXT_HALT, M_INHR),
    E(0x01, TEXT_XAE,  M_INHR),
    E(0x02, TEXT_CCL,  M_INHR),
    E(0x03, TEXT_SCL,  M_INHR),
    E(0x04, TEXT_DINT, M_INHR),
    E(0x05, TEXT_IEN,  M_INHR),
    E(0x06, TEXT_CSA,  M_INHR),
    E(0x07, TEXT_CAS,  M_INHR),
    E(0x08, TEXT_NOP,  M_INHR),
    E(0x19, TEXT_SIO,  M_INHR),
    E(0x1C, TEXT_SR,   M_INHR),
    E(0x1D, TEXT_SRL,  M_INHR),
    E(0x1E, TEXT_RR,   M_INHR),
    E(0x1F, TEXT_RRL,  M_INHR),
    E(0x30, TEXT_XPAL, M_PNTR),
    E(0x34, TEXT_XPAH, M_PNTR),
    E(0x3C, TEXT_XPPC, M_PNTR),
    E(0x40, TEXT_LDE,  M_INHR),
    E(0x50, TEXT_ANE,  M_INHR),
    E(0x58, TEXT_ORE,  M_INHR),
    E(0x60, TEXT_XRE,  M_INHR),
    E(0x68, TEXT_DAE,  M_INHR),
    E(0x70, TEXT_ADE,  M_INHR),
    E(0x78, TEXT_CAE,  M_INHR),
    E(0x8F, TEXT_DLY,  M_IMM8),
    E(0x90, TEXT_JMP,  M_REL8),
    E(0x94, TEXT_JP,   M_REL8),
    E(0x98, TEXT_JZ,   M_REL8),
    E(0x9C, TEXT_JNZ,  M_REL8),
    E(0xA8, TEXT_ILD,  M_DISP),
    E(0xB8, TEXT_DLD,  M_DISP),
    E(0xC4, TEXT_LDI,  M_IMM8),
    E(0xC0, TEXT_LD,   M_INDX),
    E(0xCC, TEXT_null, M_UNDEF),  // undefined ST immediate instruction
    E(0xC8, TEXT_ST,   M_INDX),
    E(0xD4, TEXT_ANI,  M_IMM8),
    E(0xD0, TEXT_AND,  M_INDX),
    E(0xDC, TEXT_ORI,  M_IMM8),
    E(0xD8, TEXT_OR,   M_INDX),
    E(0xE4, TEXT_XRI,  M_IMM8),
    E(0xE0, TEXT_XOR,  M_INDX),
    E(0xEC, TEXT_DAI,  M_IMM8),
    E(0xE8, TEXT_DAD,  M_INDX),
    E(0xF4, TEXT_ADI,  M_IMM8),
    E(0xF0, TEXT_ADD,  M_INDX),
    E(0xFC, TEXT_CAI,  M_IMM8),
    E(0xF8, TEXT_CAD,  M_INDX),
};

static constexpr uint8_t INDEX_INS8060[] PROGMEM = {
     33,  // TEXT_0
     44,  // TEXT_ADD
     22,  // TEXT_ADE
     43,  // TEXT_ADI
     36,  // TEXT_AND
     18,  // TEXT_ANE
     35,  // TEXT_ANI
     46,  // TEXT_CAD
     23,  // TEXT_CAE
     45,  // TEXT_CAI
      7,  // TEXT_CAS
      2,  // TEXT_CCL
      6,  // TEXT_CSA
     42,  // TEXT_DAD
     21,  // TEXT_DAE
     41,  // TEXT_DAI
      4,  // TEXT_DINT
     30,  // TEXT_DLD
     24,  // TEXT_DLY
      0,  // TEXT_HALT
      5,  // TEXT_IEN
     29,  // TEXT_ILD
     25,  // TEXT_JMP
     28,  // TEXT_JNZ
     26,  // TEXT_JP
     27,  // TEXT_JZ
     32,  // TEXT_LD
     17,  // TEXT_LDE
     31,  // TEXT_LDI
      8,  // TEXT_NOP
     38,  // TEXT_OR
     19,  // TEXT_ORE
     37,  // TEXT_ORI
     12,  // TEXT_RR
     13,  // TEXT_RRL
      3,  // TEXT_SCL
      9,  // TEXT_SIO
     10,  // TEXT_SR
     11,  // TEXT_SRL
     34,  // TEXT_ST
      1,  // TEXT_XAE
     40,  // TEXT_XOR
     15,  // TEXT_XPAH
     14,  // TEXT_XPAL
     16,  // TEXT_XPPC
     20,  // TEXT_XRE
     39,  // TEXT_XRI
};
// clang-format on

struct TableIns8060::EntryPage : EntryPageBase<Entry> {
    constexpr EntryPage(
            const Entry *table, const Entry *end, const uint8_t *index, const uint8_t *iend)
        : EntryPageBase(table, end, index, iend) {}
};

static constexpr TableIns8060::EntryPage INS8060_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_INS8060), ARRAY_RANGE(INDEX_INS8060)},
};

static bool acceptAddrMode(Entry::Flags flags, const Entry *entry) {
    auto opr = flags.mode();
    auto table = entry->flags().mode();
    if (opr == table)
        return true;
    if (opr == M_REL8)
        return table == M_IMM8 || table == M_DISP || table == M_INDX;
    if (opr == M_DISP)
        return table == M_REL8 || table == M_INDX;
    return false;
}

Error TableIns8060::searchName(InsnIns8060 &insn) const {
    uint8_t count = 0;
    auto entry = TableBase::searchName<EntryPage, Entry, Entry::Flags>(
            insn.name(), insn.flags(), INS8060_PAGES, acceptAddrMode, count);
    if (entry) {
        insn.setOpCode(entry->opCode());
        insn.setFlags(entry->flags());
        return setOK();
    }
    return setError(count == 0 ? UNKNOWN_INSTRUCTION : OPERAND_NOT_ALLOWED);
}

static Config::opcode_t tableCode(Config::opcode_t opCode, const Entry *entry) {
    switch (entry->flags().mode()) {
    case M_PNTR:
    case M_REL8:
    case M_DISP:
        return opCode & ~0x03;
    case M_INDX:
        return opCode & ~0x07;
    default:
        return opCode;
    }
}

Error TableIns8060::searchOpCode(InsnIns8060 &insn) const {
    auto entry = TableBase::searchCode<Entry, Config::opcode_t>(
            insn.opCode(), ARRAY_RANGE(TABLE_INS8060), tableCode);
    if (!entry)
        return setError(UNKNOWN_INSTRUCTION);
    insn.setFlags(entry->flags());
    if (insn.addrMode() == M_UNDEF)
        return setError(UNKNOWN_INSTRUCTION);
    insn.setName_P(entry->name_P());
    return setOK();
}

const /* PROGMEM */ char *TableIns8060::listCpu_P() const {
    return TEXT_CPU_INS8060;
}

const /* PROGMEM */ char *TableIns8060::cpu_P() const {
    return TEXT_CPU_SCMP;
}

bool TableIns8060::setCpu(const char *cpu) {
    return strcasecmp_P(cpu, TEXT_CPU_SCMP) == 0 || strcasecmp_P(cpu, TEXT_CPU_INS8060) == 0 ||
           strcasecmp_P(cpu, TEXT_CPU_8060) == 0;
}

class TableIns8060 TableIns8060;

}  // namespace ins8060
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
