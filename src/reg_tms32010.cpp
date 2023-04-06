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

#include "reg_tms32010.h"

namespace libasm {
namespace tms32010 {

RegName RegTms32010::parseRegName(StrScanner &scan) {
    auto p = scan;
    if (p.iexpect('A') && p.iexpect('R')) {
        const auto num = parseRegNumber(p, 2);
        if (num >= 0) {
            scan = p;
            return RegName(num);
        }
    } else if (p.iexpect('P') && p.iexpect('A')) {
        const auto num = parseRegNumber(p, 8);
        if (num >= 0) {
            scan = p;
            return RegName(num + int8_t(REG_PA0));
        }
    }
    return REG_UNDEF;
}

StrBuffer &RegTms32010::outRegName(StrBuffer &out, RegName name) const {
    if (isAuxiliary(name))
        return outRegNumber(out.text_P(PSTR("AR"), isUppercase()), int8_t(name));

    const int8_t num = int8_t(name) - int8_t(REG_PA0);
    return outRegNumber(out.text_P(PSTR("PA"), isUppercase()), num);
}

bool RegTms32010::isAuxiliary(RegName name) {
    return name == REG_AR0 || name == REG_AR1;
}

bool RegTms32010::isPortAddress(RegName name) {
    const auto num = int8_t(name);
    return num >= int8_t(REG_PA0) && num <= int8_t(REG_PA7);
}

}  // namespace tms32010
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
