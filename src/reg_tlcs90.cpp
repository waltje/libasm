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

#include "reg_tlcs90.h"

namespace libasm {
namespace tlcs90 {

// clang-format off
static constexpr char TEXT_REG_BC[]  PROGMEM = "BC";
static constexpr char TEXT_REG_DE[]  PROGMEM = "DE";
static constexpr char TEXT_REG_HL[]  PROGMEM = "HL";
static constexpr char TEXT_REG_IX[]  PROGMEM = "IX";
static constexpr char TEXT_REG_IY[]  PROGMEM = "IY";
static constexpr char TEXT_REG_SP[]  PROGMEM = "SP";
static constexpr char TEXT_REG_AFP[] PROGMEM = "AF'";
static constexpr char TEXT_REG_AF[]  PROGMEM = "AF";
static constexpr char TEXT_REG_A[]   PROGMEM = "A";
static constexpr char TEXT_REG_B[]   PROGMEM = "B";
static constexpr char TEXT_REG_C[]   PROGMEM = "C";
static constexpr char TEXT_REG_D[]   PROGMEM = "D";
static constexpr char TEXT_REG_E[]   PROGMEM = "E";
static constexpr char TEXT_REG_H[]   PROGMEM = "H";
static constexpr char TEXT_REG_L[]   PROGMEM = "L";
// clang-format on
static constexpr RegBase::NameEntry REG_TABLE[] PROGMEM = {
        NAME_ENTRY(REG_BC),
        NAME_ENTRY(REG_DE),
        NAME_ENTRY(REG_HL),
        NAME_ENTRY(REG_IX),
        NAME_ENTRY(REG_IY),
        NAME_ENTRY(REG_SP),
        NAME_ENTRY(REG_AFP),
        NAME_ENTRY(REG_AF),
        NAME_ENTRY(REG_A),
        NAME_ENTRY(REG_B),
        NAME_ENTRY(REG_C),
        NAME_ENTRY(REG_D),
        NAME_ENTRY(REG_E),
        NAME_ENTRY(REG_H),
        NAME_ENTRY(REG_L),
};

RegName RegTlcs90::parseRegName(StrScanner &scan) {
    const auto *entry = searchText(scan, ARRAY_RANGE(REG_TABLE));
    return entry ? RegName(entry->name()) : REG_UNDEF;
}

StrBuffer &RegTlcs90::outRegName(StrBuffer &out, RegName name) const {
    const auto *entry = searchName(uint8_t(name), ARRAY_RANGE(REG_TABLE));
    if (entry)
        out.text_P(entry->text_P(), isUppercase());
    return out;
}

bool RegTlcs90::isReg16(RegName name) {
    return uint8_t(name) <= uint8_t(REG_SP);
}

bool RegTlcs90::isRegIndex(RegName name) {
    return isReg16(name) && uint8_t(name) >= uint8_t(REG_IX);
}

uint8_t RegTlcs90::encodeReg8(RegName name) {
    return int8_t(name) - 16;
}

RegName RegTlcs90::decodeReg8(uint8_t num) {
    num &= 7;
    if (num == 7)
        return REG_UNDEF;
    return RegName(num + 16);
}

uint8_t RegTlcs90::encodeReg16(RegName name) {
    return int8_t(name);
}

RegName RegTlcs90::decodeReg16(uint8_t num) {
    num &= 7;
    if (num == 3 || num == 7)
        return REG_UNDEF;
    return RegName(num);
}

uint8_t RegTlcs90::encodeStackReg(RegName name) {
    if (name == REG_AF)
        return 6;
    return uint8_t(name);
}

RegName RegTlcs90::decodeStackReg(uint8_t num) {
    num &= 7;
    if (num == 3 || num == 7)
        return REG_UNDEF;
    if (num == 6)
        return REG_AF;
    return RegName(num);
}

uint8_t RegTlcs90::encodeIndexReg(RegName name) {
    return int8_t(name) - 4;
}

RegName RegTlcs90::decodeIndexReg(uint8_t num) {
    num &= 3;
    if (num == 3)
        return REG_UNDEF;
    return RegName(num + 4);
}

// clang-format off
static constexpr char TEXT_CC_F[]   PROGMEM = "F";
static constexpr char TEXT_CC_LT[]  PROGMEM = "LT";
static constexpr char TEXT_CC_LE[]  PROGMEM = "LE";
static constexpr char TEXT_CC_ULE[] PROGMEM = "ULE";
static constexpr char TEXT_CC_OV[]  PROGMEM = "OV";
static constexpr char TEXT_CC_MI[]  PROGMEM = "MI";
static constexpr char TEXT_CC_Z[]   PROGMEM = "Z";
static constexpr char TEXT_CC_C[]   PROGMEM = "C";
static constexpr char TEXT_CC_T[]   PROGMEM = "";
static constexpr char TEXT_CC_GE[]  PROGMEM = "GE";
static constexpr char TEXT_CC_GT[]  PROGMEM = "GT";
static constexpr char TEXT_CC_UGT[] PROGMEM = "UGT";
static constexpr char TEXT_CC_NOV[] PROGMEM = "NOV";
static constexpr char TEXT_CC_PL[]  PROGMEM = "PL";
static constexpr char TEXT_CC_NZ[]  PROGMEM = "NZ";
static constexpr char TEXT_CC_NC[]  PROGMEM = "NC";
static constexpr char TEXT_CC_PE[]  PROGMEM = "PE";
static constexpr char TEXT_CC_M[]   PROGMEM = "M";
static constexpr char TEXT_CC_EQ[]  PROGMEM = "EQ";
static constexpr char TEXT_CC_ULT[] PROGMEM = "ULT";
static constexpr char TEXT_CC_PO[]  PROGMEM = "PO";
static constexpr char TEXT_CC_P[]   PROGMEM = "P";
static constexpr char TEXT_CC_NE[]  PROGMEM = "NE";
static constexpr char TEXT_CC_UGE[] PROGMEM = "UGE";
// clang-format on
static constexpr RegBase::NameEntry CC_TABLE[] PROGMEM = {
        NAME_ENTRY(CC_F),
        NAME_ENTRY(CC_LT),
        NAME_ENTRY(CC_LE),
        NAME_ENTRY(CC_ULE),
        NAME_ENTRY(CC_OV),
        NAME_ENTRY(CC_MI),
        NAME_ENTRY(CC_Z),
        NAME_ENTRY(CC_C),
        NAME_ENTRY(CC_GE),
        NAME_ENTRY(CC_GT),
        NAME_ENTRY(CC_UGT),
        NAME_ENTRY(CC_NOV),
        NAME_ENTRY(CC_PL),
        NAME_ENTRY(CC_NZ),
        NAME_ENTRY(CC_NC),
        // Aliases
        NAME_ENTRY(CC_PE),
        NAME_ENTRY(CC_M),
        NAME_ENTRY(CC_EQ),
        NAME_ENTRY(CC_ULT),
        NAME_ENTRY(CC_PO),
        NAME_ENTRY(CC_P),
        NAME_ENTRY(CC_NE),
        NAME_ENTRY(CC_UGE),
        // Empty text
        NAME_ENTRY(CC_T),
};

CcName RegTlcs90::parseCcName(StrScanner &scan) {
    const auto *entry = searchText(scan, ARRAY_RANGE(CC_TABLE));
    const auto name = entry ? CcName(entry->name()) : CC_UNDEF;
    return name == CC_T ? CC_UNDEF : name;
}

StrBuffer &RegTlcs90::outCcName(StrBuffer &out, const CcName name) const {
    const auto *entry = searchName(uint8_t(name), ARRAY_RANGE(CC_TABLE));
    if (entry)
        out.text_P(entry->text_P(), isUppercase());
    return out;
}

uint8_t RegTlcs90::encodeCcName(const CcName name) {
    const auto cc = uint8_t(name);
    return cc >= 16 ? cc - 16 : cc;
}

CcName RegTlcs90::decodeCcName(uint8_t num) {
    return CcName(num & 0xF);
}

}  // namespace tlcs90
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
