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

#include "reg_mc68000.h"

#include "text_mc68000.h"

using namespace libasm::text::mc68000;
using namespace libasm::reg;

namespace libasm {
namespace mc68000 {
namespace reg {

static constexpr NameEntry REG_TABLE[] PROGMEM = {
        NAME_ENTRY(REG_D0),
        NAME_ENTRY(REG_D1),
        NAME_ENTRY(REG_D2),
        NAME_ENTRY(REG_D3),
        NAME_ENTRY(REG_D4),
        NAME_ENTRY(REG_D5),
        NAME_ENTRY(REG_D6),
        NAME_ENTRY(REG_D7),
        NAME_ENTRY(REG_A0),
        NAME_ENTRY(REG_A1),
        NAME_ENTRY(REG_A2),
        NAME_ENTRY(REG_A3),
        NAME_ENTRY(REG_A4),
        NAME_ENTRY(REG_A5),
        NAME_ENTRY(REG_A6),
        NAME_ENTRY(REG_A7),
        NAME_ENTRY(REG_PC),
        NAME_ENTRY(REG_SR),
        NAME_ENTRY(REG_CCR),
        NAME_ENTRY(REG_USP),
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

bool isDataReg(RegName name) {
    const auto num = int8_t(name);
    return num >= 0 && num < 8;
}

bool isAddrReg(RegName name) {
    const auto num = int8_t(name) - 8;
    return num >= 0 && num < 8;
}

bool isGeneralReg(RegName name) {
    const auto num = int8_t(name);
    return num >= 0 && num < 16;
}

Config::opcode_t encodeGeneralRegNo(RegName name) {
    return int8_t(name) & 7;
}

uint8_t encodeGeneralRegPos(RegName name) {
    return uint8_t(name);
}

RegName decodeGeneralReg(uint8_t regno) {
    return RegName(regno & 0xF);
}

RegName decodeDataReg(uint8_t regno) {
    return RegName(regno & 7);
}

RegName decodeAddrReg(uint8_t regno) {
    return RegName((regno & 7) + 8);
}

OprSize parseSize(StrScanner &scan) {
    auto p = scan;
    if (p.expect('.')) {
        auto size = SZ_ERROR;
        if (p.iexpect('B')) {
            size = SZ_BYTE;
        } else if (p.iexpect('W')) {
            size = SZ_WORD;
        } else if (p.iexpect('L')) {
            size = SZ_LONG;
        }
        if (size == SZ_ERROR || isidchar(*p))
            return SZ_ERROR;
        scan = p;
        return size;
    }
    return SZ_NONE;
}

uint8_t sizeNameLen(OprSize size) {
    return size == SZ_NONE || size == SZ_ERROR ? 0 : 2;
}

char sizeSuffix(OprSize size) {
    switch (size) {
    case SZ_BYTE:
        return 'B';
    case SZ_WORD:
        return 'W';
        break;
    case SZ_LONG:
        return 'L';
    default:
        return 0;
    }
}

}  // namespace reg

Config::opcode_t EaMc68000::encodeMode(AddrMode mode) {
    const auto m = static_cast<uint8_t>(mode);
    return m >= 8 ? 7 : m;
}

Config::opcode_t EaMc68000::encodeRegNo(AddrMode mode, RegName reg) {
    const auto m = static_cast<uint8_t>(mode);
    if (m < 8)
        return reg::encodeGeneralRegNo(reg);
    return m - 8;
}

EaMc68000::EaMc68000(OprSize size_, uint8_t raw_mode, uint8_t regno) {
    regno &= 7;
    size = size_;
    if ((raw_mode &= 7) == 7) {
        mode = (regno < 5) ? AddrMode(regno + 8) : M_ERROR;
        reg = REG_UNDEF;
    } else {
        mode = AddrMode(raw_mode);
        reg = (mode == M_DREG) ? reg::decodeDataReg(regno) : reg::decodeAddrReg(regno);
    }
}

OprSize BriefExt::indexSize() const {
    return (word & 0x800) ? SZ_LONG : SZ_WORD;
}

RegName BriefExt::index() const {
    return reg::decodeGeneralReg(word >> 12);
}

uint8_t BriefExt::disp() const {
    return static_cast<uint8_t>(word);
}

}  // namespace mc68000
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
