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

#ifndef __REG_I8080_H__
#define __REG_I8080_H__

#include "reg_base.h"

namespace libasm {
namespace i8080 {

enum RegName : int8_t {
    REG_UNDEF = -1,
    // Data registers.
    REG_B = 0,
    REG_C = 1,
    REG_D = 2,
    REG_E = 3,
    REG_H = 4,
    REG_L = 5,
    REG_M = 6,
    REG_A = 7,
    // Other registers
    REG_SP = 0 + 8,
    REG_PSW = 1 + 8,
};

namespace reg {

RegName parseRegName(StrScanner &scan);
StrBuffer &outRegName(StrBuffer &out, RegName name);

bool isPointerReg(RegName name);
uint8_t encodePointerReg(RegName name);
bool isStackReg(RegName name);
uint8_t encodeStackReg(RegName name);
bool isIndexReg(RegName name);
uint8_t encodeIndexReg(RegName name);
bool isDataReg(RegName name);
uint8_t encodeDataReg(RegName name);

RegName decodePointerReg(uint8_t num);
RegName decodeStackReg(uint8_t num);
RegName decodeIndexReg(uint8_t num);
RegName decodeDataReg(uint8_t num);

}  // namespace reg
}  // namespace i8080
}  // namespace libasm

#endif  // __REG_I8080_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
