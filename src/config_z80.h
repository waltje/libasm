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

#ifndef __CONFIG_Z80_H__
#define __CONFIG_Z80_H__

#include "config_host.h"

namespace target
{
    typedef uint16_t uintptr_t;
    typedef int16_t  ptrdiff_t;
    typedef uint8_t  opcode_t;
    typedef uint16_t insn_t;
} // namespace target

enum CpuType : host::uint_t {
    Z80,
    I8080,
};

enum OprSize : host::uint_t {
    SZ_NONE,                    // unknown
    SZ_BYTE = 1,
    SZ_WORD = 2,
};

enum AddrMode : host::uint_t {
    INHR,
    IMM8,
    IMM16,
    DIRECT,
    IOADR,
    REL8,
    INDX,
    INDX_IMM8,
};

enum InsnFormat : host::uint_t {
    NO_FMT,
    PTR_FMT,                            // **PP_****: BC/DE/HL/SP
                                        // **PP_****: BC/DE/HL/AF
    CC4_FMT,                            // ***C_C***: NZ/Z/NC/C
    IDX_FMT,                            // ***I_****: BC/DE, 0/1
    IR_FMT,                             // ****_R***: I/R
    DST_FMT,                            // **DD_D***: B/C/D/E/H/L/(HL)/A
                                        // **VV_V***: [0123][08]H
                                        // **CC_C***: NZ/Z/NC/C/PO/PE/P/M
    SRC_FMT,                            // ****_*SSS: B/C/D/E/H/L/(HL)/A
    DST_SRC_FMT,                        // **DD_DSSS: B/C/D/E/H/L/(HL)/A
                                        // **BB_BSSS: 0-7, B/C/D?E/H/L/(HL)/A
};

enum OprFormat : host::uint_t {
    NO_OPR,
    REG_8,                      // B/C/D/E/H/L/(HL)/A
    REG_16,                     // BC/DE/HL/SP
    REG_16X,                    // BC/DE/IX/SP, BC/DE/IY/SP
    A_REG,                      // A
    BC_REG,                     // BC
    DE_REG,                     // DE
    HL_REG,                     // HL
    SP_REG,                     // SP
    IX_REG,                     // IX/IY
    IR_REG,                     // I/R
    AF_REG,                     // AF
    AFPREG,                     // AF'
    STK_16,                     // BC/DE/HL/AF
    BC_PTR,                     // (BC)/(DE)
    HL_PTR,                     // (HL)
    IX_PTR,                     // (IX)/(IY)
    SP_PTR,                     // (SP)
    C_PTR,                      // (C)
    IMM_8,                      // nn
    IMM_16,                     // nnnn
    ADDR_8,                     // (nn)
    ADDR_16,                    // (nnnn)
    COND_4,                     // Z/NZ/C/NC
    COND_8,                     // Z/NZ/C/NC/PO/PE/P/M
    BIT_NO,                     // 0~7
    IMM_NO,                     // 0,1,2
    VEC_NO,                     // [0123][08]H
    IX_OFF,                     // (IX/IY+nn)
    IX_BIT,                     // DD/FD CB nn xx
};

#endif // __CONFIG_Z80_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
