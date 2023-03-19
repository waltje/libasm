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

#include "reg_i8051.h"

namespace libasm {
namespace i8051 {

// clang-format off
static constexpr char TEXT_REG_A[]    PROGMEM = "A";
static constexpr char TEXT_REG_C[]    PROGMEM = "C";
static constexpr char TEXT_REG_AB[]   PROGMEM = "AB";
static constexpr char TEXT_REG_R0[]   PROGMEM = "R0";
static constexpr char TEXT_REG_R1[]   PROGMEM = "R1";
static constexpr char TEXT_REG_R2[]   PROGMEM = "R2";
static constexpr char TEXT_REG_R3[]   PROGMEM = "R3";
static constexpr char TEXT_REG_R4[]   PROGMEM = "R4";
static constexpr char TEXT_REG_R5[]   PROGMEM = "R5";
static constexpr char TEXT_REG_R6[]   PROGMEM = "R6";
static constexpr char TEXT_REG_R7[]   PROGMEM = "R7";
static constexpr char TEXT_REG_PC[]   PROGMEM = "PC";
static constexpr char TEXT_REG_DPTR[] PROGMEM = "DPTR";
// clang-format on
static constexpr RegBase::NameEntry REG_TABLE[] PROGMEM = {
        NAME_ENTRY(REG_A),
        NAME_ENTRY(REG_C),
        NAME_ENTRY(REG_AB),
        NAME_ENTRY(REG_R0),
        NAME_ENTRY(REG_R1),
        NAME_ENTRY(REG_R2),
        NAME_ENTRY(REG_R3),
        NAME_ENTRY(REG_R4),
        NAME_ENTRY(REG_R5),
        NAME_ENTRY(REG_R6),
        NAME_ENTRY(REG_R7),
        NAME_ENTRY(REG_PC),
        NAME_ENTRY(REG_DPTR),
};

RegName RegI8051::parseRegName(StrScanner &scan) {
    const auto *entry = searchText(scan, ARRAY_RANGE(REG_TABLE));
    return entry ? RegName(entry->name()) : REG_UNDEF;
}

StrBuffer &RegI8051::outRegName(StrBuffer &out, const RegName name) const {
    const auto *entry = searchName(uint8_t(name), ARRAY_RANGE(REG_TABLE));
    if (entry)
        out.text_P(entry->text_P(), isUppercase());
    return out;
}

bool RegI8051::isRReg(RegName name) {
    const auto num = int8_t(name);
    return num >= 0 && num < 8;
}

uint8_t RegI8051::encodeRReg(RegName name) {
    return uint8_t(name);
}

RegName RegI8051::decodeRReg(const uint8_t num) {
    return RegName(num);
}

}  // namespace i8051
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
