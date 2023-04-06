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

#include "reg_tms9900.h"

using namespace libasm::reg;

namespace libasm {
namespace tms9900 {
namespace reg {

RegName parseRegName(StrScanner &scan) {
    auto p = scan;
    if (p.iexpect('R')) {
        const auto num = parseRegNumber(p, 16);
        if (num >= 0) {
            scan = p;
            return RegName(num);
        }
    }
    return REG_UNDEF;
}

StrBuffer &outRegName(StrBuffer &out, uint8_t num) {
    num &= 0x0f;
    return outRegNumber(out.letter('R'), num);
}

uint8_t encodeRegNumber(RegName name) {
    return uint8_t(name);
}

}  // namespace reg
}  // namespace tms9900
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
