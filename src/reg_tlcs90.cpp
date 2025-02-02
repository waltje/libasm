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

#include "text_tlcs90.h"

using namespace libasm::text::tlcs90;
using namespace libasm::reg;

namespace libasm {
namespace tlcs90 {
namespace reg {

static constexpr NameEntry REG_TABLE[] PROGMEM = {
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

bool isReg16(RegName name) {
    return uint8_t(name) <= uint8_t(REG_SP);
}

bool isRegIndex(RegName name) {
    return isReg16(name) && uint8_t(name) >= uint8_t(REG_IX);
}

uint8_t encodeReg8(RegName name) {
    return int8_t(name) - 16;
}

RegName decodeReg8(uint8_t num) {
    num &= 7;
    if (num == 7)
        return REG_UNDEF;
    return RegName(num + 16);
}

uint8_t encodeReg16(RegName name) {
    return int8_t(name);
}

RegName decodeReg16(uint8_t num) {
    num &= 7;
    if (num == 3 || num == 7)
        return REG_UNDEF;
    return RegName(num);
}

uint8_t encodeStackReg(RegName name) {
    if (name == REG_AF)
        return 6;
    return uint8_t(name);
}

RegName decodeStackReg(uint8_t num) {
    num &= 7;
    if (num == 3 || num == 7)
        return REG_UNDEF;
    if (num == 6)
        return REG_AF;
    return RegName(num);
}

uint8_t encodeIndexReg(RegName name) {
    return int8_t(name) - 4;
}

RegName decodeIndexReg(uint8_t num) {
    num &= 3;
    if (num == 3)
        return REG_UNDEF;
    return RegName(num + 4);
}

static constexpr NameEntry CC_TABLE[] PROGMEM = {
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

CcName parseCcName(StrScanner &scan) {
    const auto *entry = searchText(scan, ARRAY_RANGE(CC_TABLE));
    const auto name = entry ? CcName(entry->name()) : CC_UNDEF;
    return name == CC_T ? CC_UNDEF : name;
}

StrBuffer &outCcName(StrBuffer &out, const CcName name) {
    const auto *entry = searchName(uint8_t(name), ARRAY_RANGE(CC_TABLE));
    if (entry)
        out.text_P(entry->text_P());
    return out;
}

uint8_t encodeCcName(const CcName name) {
    const auto cc = uint8_t(name);
    return cc >= 16 ? cc - 16 : cc;
}

CcName decodeCcName(uint8_t num) {
    return CcName(num & 0xF);
}

}  // namespace reg
}  // namespace tlcs90
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
