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

#ifndef __CONFIG_MC6809_H__
#define __CONFIG_MC6809_H__

#include "config_host.h"

namespace target
{
    typedef uint16_t uintptr_t;
    typedef int16_t  ptrdiff_t;
    typedef uint8_t  opcode_t;
    typedef uint16_t insn_t;
} // namespace target

enum CpuType : host::uint_t {
    MC6809,
    HD6309,
};

enum OprSize : host::uint_t {
    SZ_NONE = 0,
    SZ_BYTE = 1,                // 8 bit operation
    SZ_WORD = 2,                // 16 bit operation
    SZ_LONG = 3,                // 32 bit operation
};

enum AddrMode : host::uint_t {
    INHR,
    DIRP,
    EXTD,
    INDX,
    REL,
    IMM,
    STKOP,
    REGS,
    // HD6309
    IMMDIR,
    IMMEXT,
    IMMIDX,
    BITOP,
    TFRM,
    // Pseudo instruction
    PSEUDO,
};

#endif // __CONFIG_MC6809_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
