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

#include "reg_mos6502.h"

#include "reg_base.h"

using namespace libasm::reg;

namespace libasm {
namespace mos6502 {
namespace reg {

RegName parseRegName(StrScanner &scan) {
    auto p = scan;
    auto reg = REG_UNDEF;
    if (p.iexpect('A')) {
        reg = REG_A;
    } else if (p.iexpect('X')) {
        reg = REG_X;
    } else if (p.iexpect('Y')) {
        reg = REG_Y;
    } else if (p.iexpect('S')) {
        reg = REG_S;
    } else {
        return REG_UNDEF;
    }
    if (isidchar(*p))
        return REG_UNDEF;
    scan = p;
    return reg;
}

StrBuffer &outRegName(StrBuffer &out, RegName name) {
    return out.letter(char(name));
}

}  // namespace reg
}  // namespace mos6502
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
