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

#ifndef __GEN_FORMATTER_H__
#define __GEN_FORMATTER_H__

#include <cstdio>
#include <string>

#include "dis_base.h"
#include "dis_formatter.h"

namespace libasm {
namespace gen {

class GenFormatter : public libasm::driver::DisFormatter {
public:
    GenFormatter(Disassembler &disassembler) : DisFormatter(disassembler, "<TestGenerator>") {}

    const char *name() const { return _insn.name(); }
    const char *operand() const { return _operands; }
    int length() const { return _insn.length(); }
};

}  // namespace gen
}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
