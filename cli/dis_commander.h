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

#ifndef __DIS_COMMANDER_H__
#define __DIS_COMMANDER_H__

#include "bin_memory.h"
#include "dis_base.h"
#include "dis_driver.h"
#include "file_reader.h"

#include <map>

namespace libasm {
namespace cli {

class DisCommander {
public:
    DisCommander(Disassembler **begin, Disassembler **end);

    int parseArgs(int argc, const char **argv);
    int usage();
    int disassemble();

private:
    driver::DisDriver _driver;
    // command line arguments
    const char *_prog_name;
    const char *_input_name;
    const char *_output_name;
    const char *_list_name;
    const char *_cpu;
    bool _upper_hex;
    bool _uppercase;
    bool _verbose;
    uint32_t _dis_start;
    uint32_t _dis_end;
    std::map<std::string, std::string> _options;

    static constexpr const char *PROG_PREFIX = "dis";
    Disassembler *defaultDisassembler();
    int readBinary(FileReader &input, driver::BinMemory &memory);
    int parseOptionValue(const char *option);
};

}  // namespace cli
}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
