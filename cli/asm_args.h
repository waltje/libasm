/*
 * Copyright 2022 Tadashi G. Takaoka
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

#ifndef __ASM_ARGS_H__
#define __ASM_ARGS_H__

#include <stddef.h>

#include <string>

namespace libasm {
namespace cli {

struct AsmArgs {
    std::string prog_name;
    std::string input_name;
    std::string output_name;
    std::string list_name;
    std::string cpu;
    char encoder;
    size_t record_bytes;
    bool uppercase;
    bool line_number;
    bool verbose;
    AsmArgs()
        : prog_name(),
          input_name(),
          output_name(),
          list_name(),
          cpu(),
          encoder(0),
          record_bytes(32),
          uppercase(false),
          line_number(false),
          verbose(false) {}
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
