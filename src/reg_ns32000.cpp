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

#include "reg_ns32000.h"

#include "text_ns32000.h"

using namespace libasm::text::ns32000;
using namespace libasm::reg;

namespace libasm {
namespace ns32000 {
namespace reg {

static constexpr NameEntry REG_TABLE[] PROGMEM = {
        NAME_ENTRY(REG_R0),
        NAME_ENTRY(REG_R1),
        NAME_ENTRY(REG_R2),
        NAME_ENTRY(REG_R3),
        NAME_ENTRY(REG_R4),
        NAME_ENTRY(REG_R5),
        NAME_ENTRY(REG_R6),
        NAME_ENTRY(REG_R7),
        NAME_ENTRY(REG_FP),
        NAME_ENTRY(REG_SP),
        NAME_ENTRY(REG_SB),
        NAME_ENTRY(REG_PC),
        NAME_ENTRY(REG_TOS),

        NAME_ENTRY(REG_F0),
        NAME_ENTRY(REG_F1),
        NAME_ENTRY(REG_F2),
        NAME_ENTRY(REG_F3),
        NAME_ENTRY(REG_F4),
        NAME_ENTRY(REG_F5),
        NAME_ENTRY(REG_F6),
        NAME_ENTRY(REG_F7),

        NAME_ENTRY(REG_EXT),
};

RegName parseRegName(StrScanner &scan) {
    const auto *entry = searchText(scan, ARRAY_RANGE(REG_TABLE));
    return entry ? RegName(entry->name()) : REG_UNDEF;
}

StrBuffer &outRegName(StrBuffer &out, RegName name) {
    const auto *entry = searchName(uint8_t(name), ARRAY_RANGE(REG_TABLE));
    if (entry)
        out.text_P(entry->text_P());
    return out;
}

RegName decodeRegName(uint8_t num, bool floating) {
    return RegName((num & 7) + (floating ? 8 : 0));
}

uint8_t encodeRegName(RegName name) {
    if (isFloat(name))
        return uint8_t(name) - 8;
    return uint8_t(name);
}

bool isGeneric(RegName name) {
    const auto num = int8_t(name);
    return num >= 0 && num < 8;
}

bool isFloat(RegName name) {
    const auto num = int8_t(name) - 8;
    return num >= 0 && num < 8;
}

bool isRegPair(RegName name) {
    const auto num = int8_t(name);
    return num >= 0 && num < 16 && num % 2 == 0;
}

static constexpr NameEntry PREG_TABLE[] PROGMEM = {
        NAME_ENTRY(PREG_UPSR),
        {PREG_UPSR, TEXT_PREG_US},
        {PREG_FP, TEXT_REG_FP},
        {PREG_SP, TEXT_REG_SP},
        {PREG_SB, TEXT_REG_SB},
        NAME_ENTRY(PREG_PSR),
        NAME_ENTRY(PREG_INTBASE),
        NAME_ENTRY(PREG_MOD),
};

PregName parsePregName(StrScanner &scan) {
    const auto *entry = searchText(scan, ARRAY_RANGE(PREG_TABLE));
    return entry ? PregName(entry->name()) : PREG_UNDEF;
}

StrBuffer &outPregName(StrBuffer &out, PregName name) {
    const auto *entry = searchName(uint8_t(name), ARRAY_RANGE(PREG_TABLE));
    if (entry)
        out.text_P(entry->text_P());
    return out;
}

PregName decodePregName(uint8_t num) {
    num &= 0xF;
    if (num >= 1 && num < 8)
        return PREG_UNDEF;
    if (num == 11 || num == 12)
        return PREG_UNDEF;
    return PregName(num);
}

uint8_t encodePregName(PregName name) {
    return uint8_t(name);
}

static constexpr NameEntry MREG_TABLE[] PROGMEM = {
        NAME_ENTRY(MREG_BPR0),
        NAME_ENTRY(MREG_BPR1),
        NAME_ENTRY(MREG_MSR),
        NAME_ENTRY(MREG_BCNT),
        NAME_ENTRY(MREG_PTB0),
        NAME_ENTRY(MREG_PTB1),
        NAME_ENTRY(MREG_EIA),
};

MregName parseMregName(StrScanner &scan) {
    const auto *entry = searchText(scan, ARRAY_RANGE(MREG_TABLE));
    return entry ? MregName(entry->name()) : MREG_UNDEF;
}

StrBuffer &outMregName(StrBuffer &out, MregName name) {
    const auto *entry = searchName(uint8_t(name), ARRAY_RANGE(MREG_TABLE));
    if (entry)
        out.text_P(entry->text_P());
    return out;
}

MregName decodeMregName(uint8_t num) {
    num &= 0xF;
    if (num == 14 || (num >= 2 && num < 10))
        return MREG_UNDEF;
    return MregName(num);
}

uint8_t encodeMregName(MregName name) {
    return uint8_t(name);
}

static constexpr NameEntry CONFIG_TABLE[] PROGMEM = {
        NAME_ENTRY(CONFIG_I),
        NAME_ENTRY(CONFIG_M),
        NAME_ENTRY(CONFIG_F),
        NAME_ENTRY(CONFIG_C),
};

ConfigName parseConfigName(StrScanner &scan) {
    const auto *entry = searchText(scan, ARRAY_RANGE(CONFIG_TABLE));
    return entry ? ConfigName(entry->name()) : CONFIG_UNDEF;
}

StrBuffer &outConfigNames(StrBuffer &out, uint8_t configs) {
    out.letter('[');
    char sep = 0;
    for (uint8_t mask = 0x01; mask < 0x10; mask <<= 1) {
        if (configs & mask) {
            if (sep)
                out.letter(sep);
            const auto *entry = searchName(mask, ARRAY_RANGE(CONFIG_TABLE));
            out.text_P(entry->text_P());
            sep = ',';
        }
    }
    return out.letter(']');
}

static constexpr NameEntry STROPT_TABLE[] PROGMEM = {
        NAME_ENTRY(STROPT_B),
        NAME_ENTRY(STROPT_W),
        NAME_ENTRY(STROPT_U),
};

StrOptName parseStrOptName(StrScanner &scan) {
    const auto *entry = searchText(scan, ARRAY_RANGE(STROPT_TABLE));
    return entry ? StrOptName(entry->name()) : STROPT_UNDEF;
}

StrBuffer &outStrOptNames(StrBuffer &out, uint8_t strOpts) {
    char sep = 0;
    if (strOpts & uint8_t(STROPT_B)) {
        out.text_P(TEXT_STROPT_B);
        sep = ',';
    }
    const auto *entry = searchName(strOpts & 0xC, ARRAY_RANGE(STROPT_TABLE));
    if (entry) {
        if (sep)
            out.letter(sep);
        out.text_P(entry->text_P());
    }
    return out;
}

OprSize parseIndexSize(StrScanner &scan) {
    auto p = scan;
    auto size = SZ_NONE;
    if (p.iexpect('B')) {
        size = SZ_BYTE;
    } else if (p.iexpect('W')) {
        size = SZ_WORD;
    } else if (p.iexpect('D')) {
        size = SZ_DOUBLE;
    } else if (p.iexpect('Q')) {
        size = SZ_QUAD;
    }
    if (size == SZ_NONE || isidchar(*p))
        return SZ_NONE;
    scan = p;
    return size;
}

char indexSizeChar(OprSize size) {
    switch (size) {
    case SZ_BYTE:
        return 'B';
    case SZ_WORD:
        return 'W';
    case SZ_DOUBLE:
        return 'D';
    case SZ_QUAD:
        return 'Q';
    default:
        return 0;
    }
}

}  // namespace reg
}  // namespace ns32000
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
