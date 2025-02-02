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

#ifndef __ENTRY_Z8000_H__
#define __ENTRY_Z8000_H__

#include <stdint.h>

#include "config_z8000.h"
#include "entry_base.h"

namespace libasm {
namespace z8000 {

enum OprSize : uint8_t {
    SZ_NONE = 0,
    SZ_BYTE = 1,
    SZ_WORD = 2,
    SZ_LONG = 3,
    SZ_QUAD = 4,
    SZ_ADDR = 5,
};

enum AddrMode : uint8_t {
    M_NONE = 0,
    M_R = 1,      // Register: Rn/RHn/RLn/RRn/RQn
    M_IM = 2,     // Immediate: #nn
    M_IR = 3,     // Indirect Register: @Rn/@RRn
    M_DA = 4,     // Direct Address: nnnn
    M_X = 5,      // Index: nnnn(Rn)
    M_RA = 6,     // 16-bit Relative: nnnn -32768~+32767
    M_BA = 7,     // Base Address: Rn(#nnnn)/RRn(#nnnn)
    M_BX = 8,     // Base Index: Rn(Rn)/RRn(Rn)
    M_GENI = 9,   // Generic and Immediate: IM/IR/DA/X/R
    M_GEND = 10,  // Generic Data: IR/DA/X/R
    M_GENA = 11,  // Generic Address: IR/DA/X
    M_IO = 12,    // IO Address: nnnn/#nnnn
    M_IM8 = 13,   // System call: #nn
    M_BIT = 14,   // Bit number: #0~15
    M_CNT = 15,   // Count: #1~16
    M_QCNT = 16,  // Quick Count: #1/2
    M_SCNT = 17,  // Signed Count: #-32~32
    M_NCNT = 18,  // Signed Negative Count: #-32~32
    M_CC = 19,    // Condition Code:
                  // F/Z/NZ/C/NC/PL/MI/NE/EQ/OV/NOV/PE/PO/GE/LT/GT/LE/UGE/ULT/UGT/ULE
    M_INTR = 20,  // Interrupt type: VI/NVI
    M_CTL = 21,   // Control Register:
                  // FCW/REFRESH/NSPSRG/NSPOFF/PSAPSEG/PSAPOFF/FLAGS
    M_FLAG = 22,  // Flags: C/Z/S/P/V
    M_RA12 = 23,  // 12-bit Relative: (-2048~+2047)*22
    M_RA8 = 24,   // 8-bit Relative: (-128~+127)*2
    M_RA7 = 25,   // 7-bit Positive Relative: -(0~127)*2
    M_DR = 26,    // Double-Sized Register: WORD:RRn LONG:RQn
    M_WR07 = 27,  // R0~R7
    M_WR = 28,    // R0-R15
    M_IRIO = 29,  // Indirect Register IO Address: @R1~R15
    M_ERROR = 31,
};

enum Ex1Mode : uint8_t {
    E1_NONE = 0,
    E1_CNT = 1,
    E1_WR = 2,
    E1_ERROR = 3,
};
enum Ex2Mode : uint8_t {
    E2_NONE = 0,
    E2_CC = 1,
    E2_ERROR = 2,
};

enum PostFormat : uint8_t {
    PF_NONE = 0,  // No Post Word
    PF_0XX0 = 1,  // 0 is part of instruction
    PF_0XX8 = 2,  // 8 is part of instruction
    PF_0XXE = 3,  // E is part of instruction
    PF_0X0X = 4,  // Any post word
    PF_0X00 = 5,  // Any post word
    PF_0XXX = 6,  // Any post word
};

enum CodeMask : uint8_t {
    CM_0x0000 = 0,
    CM_0x0003 = 1,
    CM_0x000F = 2,
    CM_0x00F0 = 3,
    CM_0x00F2 = 4,
    CM_0x00F7 = 5,
    CM_0x00FF = 6,
    CM_0x0F7F = 7,
    CM_0x0FFF = 8,
    CM_0xC0F0 = 9,
    CM_0xC0FF = 10,
};

enum ModeField : uint8_t {
    MF_NO = 0,
    MF_C0 = 1,
    MF_C4 = 2,
    MF_C8 = 3,
    MF_P0 = 4,
    MF_P4 = 5,
    MF_P8 = 6,
};

struct Entry final : entry::Base<Config::opcode_t> {
    struct Flags final {
        uint8_t _dst;
        uint8_t _src;
        uint8_t _ext;
        uint8_t _size;

        static constexpr Flags create(AddrMode dst, ModeField dstField, AddrMode src,
                ModeField srcField, Ex1Mode ex1, Ex2Mode ex2, PostFormat post, CodeMask mask,
                OprSize size) {
            return Flags{Entry::opr(dst, dstField), Entry::opr(src, srcField),
                    Entry::ext(ex1, ex2, post), Entry::size(mask, size)};
        }

        static const Flags create(AddrMode dst, AddrMode src, AddrMode ex1, AddrMode ex2) {
            return Flags{Entry::opr(dst, MF_NO), Entry::opr(src, MF_NO),
                    Entry::ext(toEx1Mode(ex1), toEx2Mode(ex2), PF_NONE),
                    Entry::size(CM_0x0000, SZ_NONE)};
        }

        Flags read() const {
            return Flags{pgm_read_byte(&_dst), pgm_read_byte(&_src), pgm_read_byte(&_ext),
                    pgm_read_byte(&_size)};
        }
        AddrMode dst() const { return Entry::mode(_dst); }
        AddrMode src() const { return Entry::mode(_src); }
        AddrMode ex1() const { return toAddrMode(Ex1Mode((_ext >> ex1Mode_gp) & ex1Mode_gm)); }
        AddrMode ex2() const { return toAddrMode(Ex2Mode((_ext >> ex2Mode_gp) & ex2Mode_gm)); }
        ModeField dstField() const { return Entry::field(_dst); }
        ModeField srcField() const { return Entry::field(_src); }
        PostFormat postFormat() const { return PostFormat((_ext >> postVal_gp) & postVal_gm); }
        OprSize size() const { return OprSize((_size >> size_gp) & size_gm); }
        uint8_t postMask() const { return Entry::postMask(postFormat()); }
        uint8_t postVal() const { return Entry::postVal(postFormat()); }
        uint16_t codeMask() const { return Entry::codeMask(_size); }
    };

    constexpr Entry(Config::opcode_t opCode, Flags flags, const char *name)
        : Base(name, opCode), _flags(flags) {}

    Flags flags() const { return _flags.read(); }

private:
    Flags _flags;

    static constexpr uint8_t opr(AddrMode mode, ModeField field) {
        return (static_cast<uint8_t>(mode) << mode_gp) |
               (static_cast<uint8_t>(field) << modeField_gp);
    }

    static constexpr uint8_t ext(Ex1Mode ex1, Ex2Mode ex2, PostFormat post) {
        return (static_cast<uint8_t>(ex1) << ex1Mode_gp) |
               (static_cast<uint8_t>(ex2) << ex2Mode_gp) |
               (static_cast<uint8_t>(post) << postVal_gp);
    }

    static constexpr uint8_t size(CodeMask mask, OprSize size) {
        return (static_cast<uint8_t>(mask) << codeMask_gp) |
               (static_cast<uint8_t>(size) << size_gp);
    }

    static inline AddrMode mode(uint8_t opr) { return AddrMode((opr >> mode_gp) & mode_gm); }
    static inline ModeField field(uint8_t opr) {
        return ModeField((opr >> modeField_gp) & modeField_gm);
    }

    static AddrMode toAddrMode(Ex1Mode mode);
    static inline Ex1Mode toEx1Mode(AddrMode mode) {
        switch (mode) {
        case M_NONE:
            return E1_NONE;
        case M_CNT:
        case M_IM:
            return E1_CNT;
        case M_WR:
        case M_R:
            return E1_WR;
        default:
            return E1_ERROR;
        }
    }

    static AddrMode toAddrMode(Ex2Mode mode);
    static inline Ex2Mode toEx2Mode(AddrMode mode) {
        switch (mode) {
        case M_NONE:
            return E2_NONE;
        case M_CC:
            return E2_CC;
        default:
            return E2_ERROR;
        }
    }

    static uint8_t postVal(PostFormat post);
    static uint8_t postMask(PostFormat post);
    static uint16_t codeMask(uint8_t size);

    // |dst|, |src|
    static constexpr int mode_gp = 0;
    static constexpr int modeField_gp = 5;
    static constexpr uint8_t mode_gm = 0x1f;
    static constexpr uint8_t modeField_gm = 0x7;
    // |ext|
    static constexpr int ex1Mode_gp = 0;
    static constexpr int ex2Mode_gp = 2;
    static constexpr int postVal_gp = 4;
    static constexpr uint8_t ex1Mode_gm = 0x3;
    static constexpr uint8_t ex2Mode_gm = 0x3;
    static constexpr uint8_t postVal_gm = 0x7;
    // |size|
    static constexpr int codeMask_gp = 0;
    static constexpr int size_gp = 4;
    static constexpr uint8_t codeMask_gm = 0xf;
    static constexpr uint8_t size_gm = 0x7;
};

}  // namespace z8000
}  // namespace libasm

#endif  // __ENTRY_Z8000_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
