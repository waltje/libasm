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

#ifndef __ENTRY_TMS32010_H__
#define __ENTRY_TMS32010_H__

#include "config_tms32010.h"
#include "entry_base.h"

namespace libasm {
namespace tms32010 {

enum CpuType : uint8_t {
    TMS32010,
    TMS32015,
};

enum AddrMode : uint8_t {
    // T=table, P=parser. Sorted by constant range from M_LS0 to M_IM13.
    M_NO = 0,     // TP: No operand
    M_LS0 = 1,    // TP: [0] 0 left shift (SACL)     ---- -000 ---- ----
    M_DPK = 2,    // TP: [0-1] Data memory page      ---- ---- ---- ---d
    M_ARK = 3,    // T_: [0-1] Auxiliary register    ---- ---- ---- ---a
    M_AR = 4,     // TP: AR[0-1] Auxiliary register  ---- ---a ---- ----
    M_NARP = 5,   // T_: AR[0-1] Next ARP            ---- ---- 10id n00a
    M_LS3 = 6,    // TP: [0-1,4] 3-bit left shift    ---- -XXX ---- ----
    M_IM3 = 7,    // _P: [0-7] 3-bit unsigned
    M_PA = 8,     // TP: PA[0-7] Port Address        ---- -AAA ---- ----
    M_LS4 = 9,    // TP: [0-15]  4-bit left shift    ---- SSSS ---- ----
    M_IM8 = 10,   // TP: [0-255] 8-bit unsigned      ---- ---- KKKK KKKK
    M_MAM = 11,   // T_: [0-255] Memory addressing mode      ---- ---- Immm mmmm
    M_PMA = 12,   // TP: [0-4095] Programm address   ---- PPPP PPPP PPPP
    M_IM13 = 13,  // TP: [-4096-4095] 13-bit signed  ---K KKKK KKKK KKKK
    M_ARP = 14,   // _P: "*"   Indirect addressing
    M_INC = 15,   // _P: "*+"  Indirect then auto increment addressing
    M_DEC = 16,   // _P: "*-"  Indirect then auto decrement addressing
};

class Entry : public EntryBase<Config> {
public:
    struct Flags {
        uint16_t _attr;

        static constexpr Flags create(AddrMode op1, AddrMode op2, AddrMode op3) {
            return Flags{static_cast<uint16_t>((static_cast<uint16_t>(op1) << op1_gp) |
                                               (static_cast<uint16_t>(op2) << op2_gp) |
                                               (static_cast<uint16_t>(op3) << op3_gp))};
        }
        Flags read() const { return Flags{pgm_read_word(&_attr)}; }

        AddrMode op1() const { return AddrMode((_attr >> op1_gp) & mode_gm); }
        AddrMode op2() const { return AddrMode((_attr >> op2_gp) & mode_gm); }
        AddrMode op3() const { return AddrMode((_attr >> op3_gp) & mode_gm); }
    };

    constexpr Entry(Config::opcode_t opCode, Flags flags, const char *name)
        : EntryBase(name, opCode), _flags(flags) {}

    Flags flags() const { return _flags.read(); }

private:
    Flags _flags;

    static constexpr uint8_t mode_gm = 0x1f;
    static constexpr int op1_gp = 0;
    static constexpr int op2_gp = 5;
    static constexpr int op3_gp = 10;
};

}  // namespace tms32010
}  // namespace libasm

#endif  // __ENTRY_TMS32010_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
