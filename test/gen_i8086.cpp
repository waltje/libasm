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

#include "dis_i8086.h"
#include "gen_driver.h"

using namespace libasm::i8086;
using namespace libasm::gen;

int main(int argc, const char **argv) {
    DisI8086 dis8086;
    GenDriver driver(dis8086);
    if (driver.main(argc, argv))
        return 1;

    dis8086.setOption("relative", "enable");
    dis8086.setOption("string-insn", "enable");

    TestGenerator generator(driver, dis8086, 0x1000);
    generator.generate();

    return driver.close();
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
