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

#ifndef __REGISTER_Z80_H__
#define __REGISTER_Z80_H__

#include "insn_z80.h"

enum RegName : char {
    REG_UNDEF = 0,
    REG_HL = 'h',
    REG_BC = 'b',
    REG_DE = 'd',
    REG_SP = 's',
    REG_AF = 'a',
    REG_AFP = 'f',              // AF'
    REG_IX = 'x',
    REG_IY = 'y',
    REG_B = 'B',
    REG_C = 'C',
    REG_D = 'D',
    REG_E = 'E',
    REG_H = 'H',
    REG_L = 'L',
    REG_A = 'A',
    REG_I = 'I',
    REG_R = 'R',
};

enum CcName : char {
    CC_UNDEF = 0,
    CC_Z  = 'Z',
    CC_NZ = 'z',
    CC_C  = 'C',
    CC_NC = 'c',
    CC_PO = 'o',
    CC_PE = 'e',
    CC_P  = 'P',
    CC_M  = 'M',
};

#include "reg_base.h"

class RegZ80 : public RegBase {
public:
    CcName parseCc4Name(const char *line) const;
    CcName parseCc8Name(const char *line) const;
    bool compareRegName(const char *line, RegName regName) const;
    static host::uint_t ccNameLen(const CcName ccName);
    static host::int_t encodeCcName(CcName ccName);

    RegName parseRegister(const char *line) const;
    static OprSize registerSize(const RegName regName);
    static host::uint_t regNameLen(const RegName regName);
    static host::int_t encodePointerReg(RegName regName);
    static host::int_t encodePointerRegIx(RegName regName, RegName ix);
    static host::int_t encodeStackReg(RegName regName);
    static host::int_t encodeIndirectBase(RegName regName);
    static void encodeIndexReg(Insn &insn, RegName ixReg);
    static host::int_t encodeIrReg(RegName regName);
    static host::int_t encodeDataReg(RegName regName);

    static RegName decodePointerReg(
        uint8_t regNum, const Insn *insn = nullptr);
    static RegName decodeStackReg(uint8_t regNum);
    static RegName decodeIndirectBase(uint8_t regNum);
    static RegName decodeIndexReg(const Insn &insn);
    static RegName decodeIrReg(uint8_t regNum);
    static RegName decodeDataReg(uint8_t regNum);

    char *outRegName(char *out, const RegName regName) const;
    char *outCc4Name(char *out, target::opcode_t cc4) const;
    char *outCc8Name(char *out, target::opcode_t cc8) const;

private:
    RegName parseRegName(
        const char *line, const RegName *table, const RegName *end) const;
    char regName1stChar(const RegName regName) const;
    char regName2ndChar(const RegName regName) const;
    char regName3rdChar(const RegName regName) const;
    char ccName1stChar(const CcName ccName) const;
    char ccName2ndChar(const CcName ccName) const;
    char *outCcName(char *out, CcName ccName) const;
    bool compareCcName(const char *line, CcName ccName) const;
    CcName parseCcName(const char *line, host::int_t max) const;
};

#endif // __REGISTER_Z80_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
