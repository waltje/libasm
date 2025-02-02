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

#include "table_z8000.h"

#include "entry_table.h"
#include "entry_z8000.h"
#include "text_z8000.h"

using namespace libasm::text::z8000;

namespace libasm {
namespace z8000 {

static constexpr AddrMode ex1Modes[] PROGMEM = {
        M_NONE,   // E1_NONE
        M_CNT,    // E1_CNT
        M_WR,     // E1_WR
        M_ERROR,  // E1_ERROR
};
AddrMode Entry::toAddrMode(Ex1Mode mode) {
    return AddrMode(pgm_read_byte(ex1Modes + mode));
}

static constexpr AddrMode ex2Modes[] PROGMEM = {
        M_NONE,   // E2_NONE
        M_CC,     // E2_CC
        M_ERROR,  // E2_ERROR
};
AddrMode Entry::toAddrMode(Ex2Mode mode) {
    return AddrMode(pgm_read_byte(ex2Modes + mode));
}

static constexpr uint8_t postVals[] PROGMEM = {
        0x0,  // PF_NONE
        0x0,  // PF_0XX0
        0x8,  // PF_0XX8
        0xE,  // PF_0XXE
        0x0,  // PF_0X0X
        0x0,  // PF_0X00
        0x0,  // PF_0XXX
};
uint8_t Entry::postVal(PostFormat post) {
    return pgm_read_byte(postVals + post);
}

static constexpr uint8_t postMasks[] PROGMEM = {
        0x0,  // PF_NONE
        0xF,  // PF_0XX0
        0xF,  // PF_0XX8
        0xF,  // PF_0XXE
        0x0,  // PF_0X0X
        0x0,  // PF_0X00
        0x0,  // PF_0XXX
};
uint8_t Entry::postMask(PostFormat post) {
    return pgm_read_byte(postMasks + post);
}

static constexpr uint16_t codeMasks[] PROGMEM = {
        (uint16_t)~0x0000,  // CM_0x0000
        (uint16_t)~0x0003,  // CM_0x0003
        (uint16_t)~0x000F,  // CM_0x000F
        (uint16_t)~0x00F0,  // CM_0x00F0
        (uint16_t)~0x00F2,  // CM_0x00F2
        (uint16_t)~0x00F7,  // CM_0x00F7
        (uint16_t)~0x00FF,  // CM_0x00FF
        (uint16_t)~0x0F7F,  // CM_0x0F7F
        (uint16_t)~0x0FFF,  // CM_0x0FFF
        (uint16_t)~0xC0F0,  // CM_0xC0F0
        (uint16_t)~0xC0FF,  // CM_0xC0FF
};
uint16_t Entry::codeMask(uint8_t size) {
    const CodeMask cm = CodeMask((size >> codeMask_gp) & codeMask_gm);
    return pgm_read_word(codeMasks + cm);
}

#define X4(_opc, _name, _cm, _sz, _dst, _src, _dstf, _srcf, _ex1, _ex2, _post)                   \
    {                                                                                            \
        _opc,                                                                                    \
                Entry::Flags::create(_dst, MF_##_dstf, _src, MF_##_srcf, _ex1, _ex2, PF_##_post, \
                        CM_##_cm, SZ_##_sz),                                                     \
                _name                                                                            \
    }
#define X3(_opc, _name, _cm, _sz, _dst, _src, _dstf, _srcf, _ex1, _post) \
    X4(_opc, _name, _cm, _sz, _dst, _src, _dstf, _srcf, _ex1, E2_NONE, _post)
#define X2(_opc, _name, _cm, _sz, _dst, _src, _dstf, _srcf, _post) \
    X3(_opc, _name, _cm, _sz, _dst, _src, _dstf, _srcf, E1_NONE, _post)
#define E2(_opc, _name, _cm, _sz, _dst, _src, _dstf, _srcf) \
    X2(_opc, _name, _cm, _sz, _dst, _src, _dstf, _srcf, NONE)
#define E1(_opc, _name, _cm, _sz, _dst, _dstf) E2(_opc, _name, _cm, _sz, _dst, M_NONE, _dstf, NO)
#define E0(_opc, _name, _cm) E1(_opc, _name, _cm, NONE, M_NONE, NO)

// clang-format off
static constexpr Entry TABLE_Z8000[] PROGMEM = {
    E2(0xC000, TEXT_LDB,    0x0FFF, BYTE, M_R,    M_IM8,  C8, C0),
    E1(0xD000, TEXT_CALR,   0x0FFF, NONE, M_RA12, C0),
    E2(0xE000, TEXT_JR,     0x0FFF, NONE, M_CC,   M_RA8,  C8, C0),
    E1(0xE800, TEXT_JR,     0x00FF, NONE, M_RA8, C0),
    E2(0xF000, TEXT_DBJNZ,  0x0F7F, BYTE, M_R,    M_RA7,  C8, C0),
    E2(0xF080, TEXT_DJNZ,   0x0F7F, WORD, M_R,    M_RA7,  C8, C0),
    E2(0x3000, TEXT_LDRB,   0x000F, BYTE, M_R,    M_RA,   C0, NO),
    E2(0x3000, TEXT_LDB,    0x00FF, BYTE, M_R,    M_BA,   C0, C4),
    E2(0x3100, TEXT_LDR,    0x000F, WORD, M_R,    M_RA,   C0, NO),
    E2(0x3100, TEXT_LD,     0x00FF, WORD, M_R,    M_BA,   C0, C4),
    E2(0x3200, TEXT_LDRB,   0x000F, BYTE, M_RA,   M_R,    NO, C0),
    E2(0x3200, TEXT_LDB,    0x00FF, BYTE, M_BA,   M_R,    C4, C0),
    E2(0x3300, TEXT_LDR,    0x000F, WORD, M_RA,   M_R,    NO, C0),
    E2(0x3300, TEXT_LD,     0x00FF, WORD, M_BA,   M_R,    C4, C0),
    E2(0x3400, TEXT_LDAR,   0x000F, ADDR, M_R,    M_RA,   C0, NO),
    E2(0x3400, TEXT_LDA,    0x00FF, ADDR, M_R,    M_BA,   C0, C4),
    E2(0x3500, TEXT_LDRL,   0x000F, LONG, M_R,    M_RA,   C0, NO),
    E2(0x3500, TEXT_LDL,    0x00FF, LONG, M_R,    M_BA,   C0, C4),
    E2(0x3700, TEXT_LDRL,   0x000F, LONG, M_RA,   M_R,    NO, C0),
    E2(0x3700, TEXT_LDL,    0x00FF, LONG, M_BA,   M_R,    C4, C0),
    E2(0x3C00, TEXT_INB,    0x00FF, BYTE, M_R,    M_IRIO, C0, C4),
    E2(0x3D00, TEXT_IN,     0x00FF, WORD, M_R,    M_IRIO, C0, C4),
    E2(0x3E00, TEXT_OUTB,   0x00FF, BYTE, M_IRIO, M_R,    C4, C0),
    E2(0x3F00, TEXT_OUT,    0x00FF, WORD, M_IRIO, M_R,    C4, C0),
    X2(0x7000, TEXT_LDB,    0x00FF, BYTE, M_R,    M_BX,   C0, C4, 0X00),
    X2(0x7100, TEXT_LD,     0x00FF, WORD, M_R,    M_BX,   C0, C4, 0X00),
    X2(0x7200, TEXT_LDB,    0x00FF, BYTE, M_BX,   M_R,    C4, C0, 0X00),
    X2(0x7300, TEXT_LD,     0x00FF, WORD, M_BX,   M_R,    C4, C0, 0X00),
    X2(0x7400, TEXT_LDA,    0x00FF, ADDR, M_R,    M_BX,   C0, C4, 0X00),
    X2(0x7500, TEXT_LDL,    0x00FF, LONG, M_R,    M_BX,   C0, C4, 0X00),
    E2(0x7600, TEXT_LDA,    0x00FF, ADDR, M_R,    M_GENA, C0, C4),
    X2(0x7700, TEXT_LDL,    0x00FF, LONG, M_BX,   M_R,    C4, C0, 0X00),
    E0(0x7A00, TEXT_HALT,   0x0000),
    E0(0x7B00, TEXT_IRET,   0x0000),
    E0(0x7B08, TEXT_MSET,   0x0000),
    E0(0x7B09, TEXT_MRES,   0x0000),
    E0(0x7B0A, TEXT_MBIT,   0x0000),
    E1(0x7B0D, TEXT_MREQ,   0x00F0, WORD, M_R, C4),
    E1(0x7C00, TEXT_DI,     0x0003, NONE, M_INTR, C0),
    E1(0x7C04, TEXT_EI,     0x0003, NONE, M_INTR, C0),
    E2(0x7D00, TEXT_LDCTL,  0x00F7, WORD, M_R,    M_CTL,  C4, C0),
    E2(0x7D08, TEXT_LDCTL,  0x00F7, WORD, M_CTL,  M_R,    C0, C4),
    E2(0x7F00, TEXT_SC,     0x00FF, NONE, M_IM8,  M_NONE, C0, C0),
    E2(0x8C01, TEXT_LDCTLB, 0x00F0, BYTE, M_R,    M_CTL,  C4, C0),
    E2(0x8C09, TEXT_LDCTLB, 0x00F0, BYTE, M_CTL,  M_R,    C0, C4),
    E1(0x0C00, TEXT_COMB,   0xC0F0, BYTE, M_GEND, C4),
    E2(0x0C01, TEXT_CPB,    0xC0F0, BYTE, M_GENA, M_IM,   C4, NO),
    E1(0x0C02, TEXT_NEGB,   0xC0F0, BYTE, M_GEND, C4), // NEGB/LDCTLB share same bit pattern
    E1(0x0C04, TEXT_TESTB,  0xC0F0, BYTE, M_GEND, C4),
    E2(0x0C05, TEXT_LDB,    0xC0F0, BYTE, M_GENA, M_IM,   C4, NO),
    E1(0x0C06, TEXT_TSETB,  0xC0F0, BYTE, M_GEND, C4),
    E1(0x0C08, TEXT_CLRB,   0xC0F0, BYTE, M_GEND, C4), // CLRB/LDCTLB share smae bit pattern
    E1(0x8D01, TEXT_SETFLG, 0x00F0, NONE, M_FLAG, C4),
    E1(0x8D03, TEXT_RESFLG, 0x00F0, NONE, M_FLAG, C4),
    E1(0x8D05, TEXT_COMFLG, 0x00F0, NONE, M_FLAG, C4),
    E0(0x8D07, TEXT_NOP,    0x0000),
    E1(0x9E00, TEXT_RET,    0x000F, NONE, M_CC, C0),
    E2(0xAE00, TEXT_TCCB,   0x00FF, BYTE, M_CC,   M_R,    C0, C4),
    E1(0xAE08, TEXT_TCCB,   0x00F0, BYTE, M_R, C4),
    E2(0xAF00, TEXT_TCC,    0x00FF, WORD, M_CC,   M_R,    C0, C4),
    E1(0xAF08, TEXT_TCC,    0x00F0, WORD, M_R, C4),
    E1(0xB000, TEXT_DAB,    0x00F0, BYTE, M_R, C4),
    E1(0xB100, TEXT_EXTSB,  0x00F0, BYTE, M_DR, C4),
    E1(0xB107, TEXT_EXTSL,  0x00F0, LONG, M_DR, C4),
    E1(0xB10A, TEXT_EXTS,   0x00F0, WORD, M_DR, C4),
    E2(0xB200, TEXT_RLB,    0x00F2, BYTE, M_R,    M_QCNT, C4, C0),
    E2(0xB201, TEXT_SLLB,   0x00F0, BYTE, M_R,    M_SCNT, C4, NO),
    E2(0xB201, TEXT_SRLB,   0x00F0, BYTE, M_R,    M_NCNT, C4, NO),
    X2(0xB203, TEXT_SDLB,   0x00F0, BYTE, M_R,    M_WR,   C4, P8, 0X00),
    E2(0xB204, TEXT_RRB,    0x00F2, BYTE, M_R,    M_QCNT, C4, C0),
    E2(0xB208, TEXT_RLCB,   0x00F2, BYTE, M_R,    M_QCNT, C4, C0),
    E2(0xB209, TEXT_SLAB,   0x00F0, BYTE, M_R,    M_SCNT, C4, NO),
    E2(0xB209, TEXT_SRAB,   0x00F0, BYTE, M_R,    M_NCNT, C4, NO),
    X2(0xB20B, TEXT_SDAB,   0x00F0, BYTE, M_R,    M_WR,   C4, P8, 0X00),
    E2(0xB20C, TEXT_RRCB,   0x00F2, BYTE, M_R,    M_QCNT, C4, C0),
    E2(0xB300, TEXT_RL,     0x00F2, WORD, M_R,    M_QCNT, C4, C0),
    E2(0xB301, TEXT_SLL,    0x00F0, WORD, M_R,    M_SCNT, C4, NO),
    E2(0xB301, TEXT_SRL,    0x00F0, WORD, M_R,    M_NCNT, C4, NO),
    X2(0xB303, TEXT_SDL,    0x00F0, WORD, M_R,    M_WR,   C4, P8, 0X00),
    E2(0xB304, TEXT_RR,     0x00F2, WORD, M_R,    M_QCNT, C4, C0),
    E2(0xB305, TEXT_SLLL,   0x00F0, LONG, M_R,    M_SCNT, C4, NO),
    E2(0xB305, TEXT_SRLL,   0x00F0, LONG, M_R,    M_NCNT, C4, NO),
    X2(0xB307, TEXT_SDLL,   0x00F0, LONG, M_R,    M_WR,   C4, P8, 0X00),
    E2(0xB308, TEXT_RLC,    0x00F2, WORD, M_R,    M_QCNT, C4, C0),
    E2(0xB309, TEXT_SLA,    0x00F0, WORD, M_R,    M_SCNT, C4, NO),
    E2(0xB309, TEXT_SRA,    0x00F0, WORD, M_R,    M_NCNT, C4, NO),
    X2(0xB30B, TEXT_SDA,    0x00F0, WORD, M_R,    M_WR,   C4, P8, 0X00),
    E2(0xB30C, TEXT_RRC,    0x00F2, WORD, M_R,    M_QCNT, C4, C0),
    E2(0xB30D, TEXT_SLAL,   0x00F0, LONG, M_R,    M_SCNT, C4, NO),
    E2(0xB30D, TEXT_SRAL,   0x00F0, LONG, M_R,    M_NCNT, C4, NO),
    X2(0xB30F, TEXT_SDAL,   0x00F0, LONG, M_R,    M_WR,   C4, P8, 0X00),
    E2(0xB400, TEXT_ADCB,   0x00FF, BYTE, M_R,    M_R,    C0, C4),
    E2(0xB500, TEXT_ADC,    0x00FF, WORD, M_R,    M_R,    C0, C4),
    E2(0xB600, TEXT_SBCB,   0x00FF, BYTE, M_R,    M_R,    C0, C4),
    E2(0xB700, TEXT_SBC,    0x00FF, WORD, M_R,    M_R,    C0, C4),
    E2(0xBC00, TEXT_RRDB,   0x00FF, BYTE, M_R,    M_R,    C0, C4),
    E2(0xBD00, TEXT_LDK,    0x00FF, WORD, M_R,    M_BIT,  C4, C0),
    E2(0xBE00, TEXT_RLDB,   0x00FF, BYTE, M_R,    M_R,    C0, C4),
    E2(0x3A04, TEXT_INB,    0x00F0, BYTE, M_R,    M_IO,   C4, NO),
    E2(0x3A05, TEXT_SINB,   0x00F0, BYTE, M_R,    M_IO,   C4, NO),
    E2(0x3A06, TEXT_OUTB,   0x00F0, BYTE, M_IO,   M_R,    NO, C4),
    E2(0x3A07, TEXT_SOUTB,  0x00F0, BYTE, M_IO,   M_R,    NO, C4),
    E2(0x3B04, TEXT_IN,     0x00F0, WORD, M_R,    M_IO,   C4, NO),
    E2(0x3B05, TEXT_SIN,    0x00F0, WORD, M_R,    M_IO,   C4, NO),
    E2(0x3B06, TEXT_OUT,    0x00F0, WORD, M_IO,   M_R,    NO, C4),
    E2(0x3B07, TEXT_SOUT,   0x00F0, WORD, M_IO,   M_R,    NO, C4),
    X3(0x3A00, TEXT_INIRB,  0x00F0, BYTE, M_IR,   M_IRIO, P4, C4, E1_WR, 0XX0),
    X3(0x3A00, TEXT_INIB,   0x00F0, BYTE, M_IR,   M_IRIO, P4, C4, E1_WR, 0XX8),
    X3(0x3A01, TEXT_SINIRB, 0x00F0, BYTE, M_IR,   M_IRIO, P4, C4, E1_WR, 0XX0),
    X3(0x3A01, TEXT_SINIB,  0x00F0, BYTE, M_IR,   M_IRIO, P4, C4, E1_WR, 0XX8),
    X3(0x3A02, TEXT_OTIRB,  0x00F0, BYTE, M_IRIO, M_IR,   P4, C4, E1_WR, 0XX0),
    X3(0x3A02, TEXT_OUTIB,  0x00F0, BYTE, M_IRIO, M_IR,   P4, C4, E1_WR, 0XX8),
    X3(0x3A03, TEXT_SOTIRB, 0x00F0, BYTE, M_IRIO, M_IR,   P4, C4, E1_WR, 0XX0),
    X3(0x3A03, TEXT_SOUTIB, 0x00F0, BYTE, M_IRIO, M_IR,   P4, C4, E1_WR, 0XX8),
    X3(0x3A08, TEXT_INDRB,  0x00F0, BYTE, M_IR,   M_IRIO, P4, C4, E1_WR, 0XX0),
    X3(0x3A08, TEXT_INDB,   0x00F0, BYTE, M_IR,   M_IRIO, P4, C4, E1_WR, 0XX8),
    X3(0x3A09, TEXT_SINDRB, 0x00F0, BYTE, M_IR,   M_IRIO, P4, C4, E1_WR, 0XX0),
    X3(0x3A09, TEXT_SINDB,  0x00F0, BYTE, M_IR,   M_IRIO, P4, C4, E1_WR, 0XX8),
    X3(0x3A0A, TEXT_OTDRB,  0x00F0, BYTE, M_IRIO, M_IR,   P4, C4, E1_WR, 0XX0),
    X3(0x3A0A, TEXT_OUTDB,  0x00F0, BYTE, M_IRIO, M_IR,   P4, C4, E1_WR, 0XX8),
    X3(0x3A0B, TEXT_SOTDRB, 0x00F0, BYTE, M_IRIO, M_IR,   P4, C4, E1_WR, 0XX0),
    X3(0x3A0B, TEXT_SOUTDB, 0x00F0, BYTE, M_IRIO, M_IR,   P4, C4, E1_WR, 0XX8),
    X3(0x3B00, TEXT_INIR,   0x00F0, WORD, M_IR,   M_IRIO, P4, C4, E1_WR, 0XX0),
    X3(0x3B00, TEXT_INI,    0x00F0, WORD, M_IR,   M_IRIO, P4, C4, E1_WR, 0XX8),
    X3(0x3B01, TEXT_SINIR,  0x00F0, WORD, M_IR,   M_IRIO, P4, C4, E1_WR, 0XX0),
    X3(0x3B01, TEXT_SINI,   0x00F0, WORD, M_IR,   M_IRIO, P4, C4, E1_WR, 0XX8),
    X3(0x3B02, TEXT_OTIR,   0x00F0, WORD, M_IRIO, M_IR,   P4, C4, E1_WR, 0XX0),
    X3(0x3B02, TEXT_OUTI,   0x00F0, WORD, M_IRIO, M_IR,   P4, C4, E1_WR, 0XX8),
    X3(0x3B03, TEXT_SOTIR,  0x00F0, WORD, M_IRIO, M_IR,   P4, C4, E1_WR, 0XX0),
    X3(0x3B03, TEXT_SOUTI,  0x00F0, WORD, M_IRIO, M_IR,   P4, C4, E1_WR, 0XX8),
    X3(0x3B08, TEXT_INDR,   0x00F0, WORD, M_IR,   M_IRIO, P4, C4, E1_WR, 0XX0),
    X3(0x3B08, TEXT_IND,    0x00F0, WORD, M_IR,   M_IRIO, P4, C4, E1_WR, 0XX8),
    X3(0x3B09, TEXT_SINDR,  0x00F0, WORD, M_IR,   M_IRIO, P4, C4, E1_WR, 0XX0),
    X3(0x3B09, TEXT_SIND,   0x00F0, WORD, M_IR,   M_IRIO, P4, C4, E1_WR, 0XX8),
    X3(0x3B0A, TEXT_OTDR,   0x00F0, WORD, M_IRIO, M_IR,   P4, C4, E1_WR, 0XX0),
    X3(0x3B0A, TEXT_OUTD,   0x00F0, WORD, M_IRIO, M_IR,   P4, C4, E1_WR, 0XX8),
    X3(0x3B0B, TEXT_SOTDR,  0x00F0, WORD, M_IRIO, M_IR,   P4, C4, E1_WR, 0XX0),
    X3(0x3B0B, TEXT_SOUTD,  0x00F0, WORD, M_IRIO, M_IR,   P4, C4, E1_WR, 0XX8),
    X3(0xB800, TEXT_TRIB,   0x00F0, BYTE, M_IR,   M_IR,   C4, P4, E1_WR, 0XX0),
    X3(0xB802, TEXT_TRTIB,  0x00F0, BYTE, M_IR,   M_IR,   C4, P4, E1_WR, 0XX0),
    X3(0xB804, TEXT_TRIRB,  0x00F0, BYTE, M_IR,   M_IR,   C4, P4, E1_WR, 0XX0),
    X3(0xB806, TEXT_TRTIRB, 0x00F0, BYTE, M_IR,   M_IR,   C4, P4, E1_WR, 0XXE),
    X3(0xB808, TEXT_TRDB,   0x00F0, BYTE, M_IR,   M_IR,   C4, P4, E1_WR, 0XX0),
    X3(0xB80A, TEXT_TRTDB,  0x00F0, BYTE, M_IR,   M_IR,   C4, P4, E1_WR, 0XX0),
    X3(0xB80C, TEXT_TRDRB,  0x00F0, BYTE, M_IR,   M_IR,   C4, P4, E1_WR, 0XX0),
    X3(0xB80E, TEXT_TRTDRB, 0x00F0, BYTE, M_IR,   M_IR,   C4, P4, E1_WR, 0XXE),
    X4(0xBA00, TEXT_CPIB,   0x00F0, BYTE, M_R,    M_IR,   P4, C4, E1_WR, E2_CC, 0XXX),
    X3(0xBA01, TEXT_LDIRB,  0x00F0, BYTE, M_IR,   M_IR,   P4, C4, E1_WR, 0XX0),
    X3(0xBA01, TEXT_LDIB,   0x00F0, BYTE, M_IR,   M_IR,   P4, C4, E1_WR, 0XX8),
    X4(0xBA02, TEXT_CPSIB,  0x00F0, BYTE, M_IR,   M_IR,   P4, C4, E1_WR, E2_CC, 0XXX),
    X4(0xBA04, TEXT_CPIRB,  0x00F0, BYTE, M_R,    M_IR,   P4, C4, E1_WR, E2_CC, 0XXX),
    X4(0xBA06, TEXT_CPSIRB, 0x00F0, BYTE, M_IR,   M_IR,   P4, C4, E1_WR, E2_CC, 0XXX),
    X4(0xBA08, TEXT_CPDB,   0x00F0, BYTE, M_R,    M_IR,   P4, C4, E1_WR, E2_CC, 0XXX),
    X3(0xBA09, TEXT_LDDRB,  0x00F0, BYTE, M_IR,   M_IR,   P4, C4, E1_WR, 0XX0),
    X3(0xBA09, TEXT_LDDB,   0x00F0, BYTE, M_IR,   M_IR,   P4, C4, E1_WR, 0XX8),
    X4(0xBA0A, TEXT_CPSDB,  0x00F0, BYTE, M_IR,   M_IR,   P4, C4, E1_WR, E2_CC, 0XXX),
    X4(0xBA0C, TEXT_CPDRB,  0x00F0, BYTE, M_R,    M_IR,   P4, C4, E1_WR, E2_CC, 0XXX),
    X4(0xBA0E, TEXT_CPSDRB, 0x00F0, BYTE, M_IR,   M_IR,   P4, C4, E1_WR, E2_CC, 0XXX),
    X4(0xBB00, TEXT_CPI,    0x00F0, WORD, M_R,    M_IR,   P4, C4, E1_WR, E2_CC, 0XXX),
    X3(0xBB01, TEXT_LDIR,   0x00F0, WORD, M_IR,   M_IR,   P4, C4, E1_WR, 0XX0),
    X3(0xBB01, TEXT_LDI,    0x00F0, WORD, M_IR,   M_IR,   P4, C4, E1_WR, 0XX8),
    X4(0xBB02, TEXT_CPSI,   0x00F0, WORD, M_IR,   M_IR,   P4, C4, E1_WR, E2_CC, 0XXX),
    X4(0xBB04, TEXT_CPIR,   0x00F0, WORD, M_R,    M_IR,   P4, C4, E1_WR, E2_CC, 0XXX),
    X4(0xBB06, TEXT_CPSIR,  0x00F0, WORD, M_IR,   M_IR,   P4, C4, E1_WR, E2_CC, 0XXX),
    X4(0xBB08, TEXT_CPD,    0x00F0, WORD, M_R,    M_IR,   P4, C4, E1_WR, E2_CC, 0XXX),
    X3(0xBB09, TEXT_LDDR,   0x00F0, WORD, M_IR,   M_IR,   P4, C4, E1_WR, 0XX0),
    X3(0xBB09, TEXT_LDD,    0x00F0, WORD, M_IR,   M_IR,   P4, C4, E1_WR, 0XX8),
    X4(0xBB0A, TEXT_CPSD,   0x00F0, WORD, M_IR,   M_IR,   P4, C4, E1_WR, E2_CC, 0XXX),
    X4(0xBB0C, TEXT_CPDR,   0x00F0, WORD, M_R,    M_IR,   P4, C4, E1_WR, E2_CC, 0XXX),
    X4(0xBB0E, TEXT_CPSDR,  0x00F0, WORD, M_IR,   M_IR,   P4, C4, E1_WR, E2_CC, 0XXX),
    E2(0x0000, TEXT_ADDB,   0xC0FF, BYTE, M_R,    M_GENI, C0, C4),
    E2(0x0100, TEXT_ADD,    0xC0FF, WORD, M_R,    M_GENI, C0, C4),
    E2(0x0200, TEXT_SUBB,   0xC0FF, BYTE, M_R,    M_GENI, C0, C4),
    E2(0x0300, TEXT_SUB,    0xC0FF, WORD, M_R,    M_GENI, C0, C4),
    E2(0x0400, TEXT_ORB,    0xC0FF, BYTE, M_R,    M_GENI, C0, C4),
    E2(0x0500, TEXT_OR,     0xC0FF, WORD, M_R,    M_GENI, C0, C4),
    E2(0x0600, TEXT_ANDB,   0xC0FF, BYTE, M_R,    M_GENI, C0, C4),
    E2(0x0700, TEXT_AND,    0xC0FF, WORD, M_R,    M_GENI, C0, C4),
    E2(0x0800, TEXT_XORB,   0xC0FF, BYTE, M_R,    M_GENI, C0, C4),
    E2(0x0900, TEXT_XOR,    0xC0FF, WORD, M_R,    M_GENI, C0, C4),
    E2(0x0A00, TEXT_CPB,    0xC0FF, BYTE, M_R,    M_GENI, C0, C4),
    E2(0x0B00, TEXT_CP,     0xC0FF, WORD, M_R,    M_GENI, C0, C4),
    E1(0x0D00, TEXT_COM,    0xC0F0, WORD, M_GEND, C4),
    E2(0x0D01, TEXT_CP,     0xC0F0, WORD, M_GENA, M_IM,   C4, NO),
    E1(0x0D02, TEXT_NEG,    0xC0F0, WORD, M_GEND, C4),
    E1(0x0D04, TEXT_TEST,   0xC0F0, WORD, M_GEND, C4),
    E2(0x0D05, TEXT_LD,     0xC0F0, WORD, M_GENA, M_IM,   C4, NO),
    E1(0x0D06, TEXT_TSET,   0xC0F0, WORD, M_GEND, C4),
    E1(0x0D08, TEXT_CLR,    0xC0F0, WORD, M_GEND, C4),
    E2(0x0D09, TEXT_PUSH,   0xC0F0, WORD, M_IR,   M_IM,   C4, NO),
    E2(0x1000, TEXT_CPL,    0xC0FF, LONG, M_R,    M_GENI, C0, C4),
    E2(0x1100, TEXT_PUSHL,  0xC0FF, LONG, M_IR,   M_GEND, C4, C0),
    E2(0x1200, TEXT_SUBL,   0xC0FF, LONG, M_R,    M_GENI, C0, C4),
    E2(0x1300, TEXT_PUSH,   0xC0FF, WORD, M_IR,   M_GEND, C4, C0),
    E2(0x1400, TEXT_LDL,    0xC0FF, LONG, M_R,    M_GENI, C0, C4),
    E2(0x1500, TEXT_POPL,   0xC0FF, LONG, M_GEND, M_IR,   C0, C4),
    E2(0x1600, TEXT_ADDL,   0xC0FF, LONG, M_R,    M_GENI, C0, C4),
    E2(0x1700, TEXT_POP,    0xC0FF, WORD, M_GEND, M_IR,   C0, C4),
    E2(0x1800, TEXT_MULTL,  0xC0FF, LONG, M_DR,   M_GENI, C0, C4),
    E2(0x1900, TEXT_MULT,   0xC0FF, WORD, M_DR,   M_GENI, C0, C4),
    E2(0x1A00, TEXT_DIVL,   0xC0FF, LONG, M_DR,   M_GENI, C0, C4),
    E2(0x1B00, TEXT_DIV,    0xC0FF, WORD, M_DR,   M_GENI, C0, C4),
    X3(0x1C01, TEXT_LDM,    0xC0F0, WORD, M_R,    M_GENA, P8, C4, E1_CNT, 0X0X),
    E1(0x1C08, TEXT_TESTL,  0xC0F0, LONG, M_GEND, C4),
    X3(0x1C09, TEXT_LDM,    0xC0F0, WORD, M_GENA, M_R,    C4, P8, E1_CNT, 0X0X),
    E2(0x1D00, TEXT_LDL,    0xC0FF, LONG, M_GENA, M_R,    C4, C0),
    E2(0x1E00, TEXT_JP,     0xC0FF, NONE, M_CC,   M_GENA, C0, C4),
    E1(0x1E08, TEXT_JP,     0xC0F0, NONE, M_GENA, C4),
    E1(0x1F00, TEXT_CALL,   0xC0FF, NONE, M_GENA, C4),
    E2(0x2000, TEXT_LDB,    0xC0FF, BYTE, M_R,    M_GENI, C0, C4),
    E2(0x2100, TEXT_LD,     0xC0FF, WORD, M_R,    M_GENI, C0, C4),
    X2(0x2200, TEXT_RESB,   0x000F, BYTE, M_R,    M_WR07, P8, C0, 0X00),
    E2(0x2200, TEXT_RESB,   0xC0FF, BYTE, M_GEND, M_BIT,  C4, C0),
    X2(0x2300, TEXT_RES,    0x000F, WORD, M_R,    M_R,    P8, C0, 0X00),
    E2(0x2300, TEXT_RES,    0xC0FF, WORD, M_GEND, M_BIT,  C4, C0),
    X2(0x2400, TEXT_SETB,   0x000F, BYTE, M_R,    M_WR07, P8, C0, 0X00),
    E2(0x2400, TEXT_SETB,   0xC0FF, BYTE, M_GEND, M_BIT,  C4, C0),
    X2(0x2500, TEXT_SET,    0x000F, WORD, M_R,    M_R,    P8, C0, 0X00),
    E2(0x2500, TEXT_SET,    0xC0FF, WORD, M_GEND, M_BIT,  C4, C0),
    X2(0x2600, TEXT_BITB,   0x000F, BYTE, M_R,    M_WR07, P8, C0, 0X00),
    E2(0x2600, TEXT_BITB,   0xC0FF, BYTE, M_GEND, M_BIT,  C4, C0),
    X2(0x2700, TEXT_BIT,    0x000F, WORD, M_R,    M_R,    P8, C0, 0X00),
    E2(0x2700, TEXT_BIT,    0xC0FF, WORD, M_GEND, M_BIT,  C4, C0),
    E2(0x2800, TEXT_INCB,   0xC0FF, BYTE, M_GEND, M_CNT,  C4, C0),
    E2(0x2900, TEXT_INC,    0xC0FF, WORD, M_GEND, M_CNT,  C4, C0),
    E2(0x2A00, TEXT_DECB,   0xC0FF, BYTE, M_GEND, M_CNT,  C4, C0),
    E2(0x2B00, TEXT_DEC,    0xC0FF, WORD, M_GEND, M_CNT,  C4, C0),
    E2(0x2C00, TEXT_EXB,    0xC0FF, BYTE, M_R,    M_GEND, C0, C4),
    E2(0x2D00, TEXT_EX,     0xC0FF, WORD, M_R,    M_GEND, C0, C4),
    E2(0x2E00, TEXT_LDB,    0xC0FF, BYTE, M_GENA, M_R,    C4, C0),
    E2(0x2F00, TEXT_LD,     0xC0FF, WORD, M_GENA, M_R,    C4, C0),
    E1(0x3900, TEXT_LDPS,   0xC0F0, WORD, M_GENA, C4),
};
static constexpr uint8_t INDEX_Z8000[] PROGMEM = {
     92,  // TEXT_ADC
     91,  // TEXT_ADCB
    171,  // TEXT_ADD
    170,  // TEXT_ADDB
    196,  // TEXT_ADDL
    177,  // TEXT_AND
    176,  // TEXT_ANDB
    221,  // TEXT_BIT
    222,  // TEXT_BIT
    219,  // TEXT_BITB
    220,  // TEXT_BITB
    208,  // TEXT_CALL
      1,  // TEXT_CALR
    188,  // TEXT_CLR
     51,  // TEXT_CLRB
    182,  // TEXT_COM
     45,  // TEXT_COMB
     54,  // TEXT_COMFLG
    181,  // TEXT_CP
    183,  // TEXT_CP
     46,  // TEXT_CPB
    180,  // TEXT_CPB
    164,  // TEXT_CPD
    152,  // TEXT_CPDB
    168,  // TEXT_CPDR
    156,  // TEXT_CPDRB
    158,  // TEXT_CPI
    146,  // TEXT_CPIB
    162,  // TEXT_CPIR
    150,  // TEXT_CPIRB
    190,  // TEXT_CPL
    167,  // TEXT_CPSD
    155,  // TEXT_CPSDB
    169,  // TEXT_CPSDR
    157,  // TEXT_CPSDRB
    161,  // TEXT_CPSI
    149,  // TEXT_CPSIB
    163,  // TEXT_CPSIR
    151,  // TEXT_CPSIRB
     61,  // TEXT_DAB
      4,  // TEXT_DBJNZ
    226,  // TEXT_DEC
    225,  // TEXT_DECB
     38,  // TEXT_DI
    201,  // TEXT_DIV
    200,  // TEXT_DIVL
      5,  // TEXT_DJNZ
     39,  // TEXT_EI
    228,  // TEXT_EX
    227,  // TEXT_EXB
     64,  // TEXT_EXTS
     62,  // TEXT_EXTSB
     63,  // TEXT_EXTSL
     32,  // TEXT_HALT
     21,  // TEXT_IN
    102,  // TEXT_IN
     20,  // TEXT_INB
     98,  // TEXT_INB
    224,  // TEXT_INC
    223,  // TEXT_INCB
    131,  // TEXT_IND
    115,  // TEXT_INDB
    130,  // TEXT_INDR
    114,  // TEXT_INDRB
    123,  // TEXT_INI
    107,  // TEXT_INIB
    122,  // TEXT_INIR
    106,  // TEXT_INIRB
     33,  // TEXT_IRET
    206,  // TEXT_JP
    207,  // TEXT_JP
      2,  // TEXT_JR
      3,  // TEXT_JR
      9,  // TEXT_LD
     13,  // TEXT_LD
     25,  // TEXT_LD
     27,  // TEXT_LD
    186,  // TEXT_LD
    210,  // TEXT_LD
    230,  // TEXT_LD
     15,  // TEXT_LDA
     28,  // TEXT_LDA
     30,  // TEXT_LDA
     14,  // TEXT_LDAR
      0,  // TEXT_LDB
      7,  // TEXT_LDB
     11,  // TEXT_LDB
     24,  // TEXT_LDB
     26,  // TEXT_LDB
     49,  // TEXT_LDB
    209,  // TEXT_LDB
    229,  // TEXT_LDB
     40,  // TEXT_LDCTL
     41,  // TEXT_LDCTL
     43,  // TEXT_LDCTLB
     44,  // TEXT_LDCTLB
    166,  // TEXT_LDD
    154,  // TEXT_LDDB
    165,  // TEXT_LDDR
    153,  // TEXT_LDDRB
    160,  // TEXT_LDI
    148,  // TEXT_LDIB
    159,  // TEXT_LDIR
    147,  // TEXT_LDIRB
     96,  // TEXT_LDK
     17,  // TEXT_LDL
     19,  // TEXT_LDL
     29,  // TEXT_LDL
     31,  // TEXT_LDL
    194,  // TEXT_LDL
    205,  // TEXT_LDL
    202,  // TEXT_LDM
    204,  // TEXT_LDM
    231,  // TEXT_LDPS
      8,  // TEXT_LDR
     12,  // TEXT_LDR
      6,  // TEXT_LDRB
     10,  // TEXT_LDRB
     16,  // TEXT_LDRL
     18,  // TEXT_LDRL
     36,  // TEXT_MBIT
     37,  // TEXT_MREQ
     35,  // TEXT_MRES
     34,  // TEXT_MSET
    199,  // TEXT_MULT
    198,  // TEXT_MULTL
    184,  // TEXT_NEG
     47,  // TEXT_NEGB
     55,  // TEXT_NOP
    175,  // TEXT_OR
    174,  // TEXT_ORB
    134,  // TEXT_OTDR
    118,  // TEXT_OTDRB
    126,  // TEXT_OTIR
    110,  // TEXT_OTIRB
     23,  // TEXT_OUT
    104,  // TEXT_OUT
     22,  // TEXT_OUTB
    100,  // TEXT_OUTB
    135,  // TEXT_OUTD
    119,  // TEXT_OUTDB
    127,  // TEXT_OUTI
    111,  // TEXT_OUTIB
    197,  // TEXT_POP
    195,  // TEXT_POPL
    189,  // TEXT_PUSH
    193,  // TEXT_PUSH
    191,  // TEXT_PUSHL
    213,  // TEXT_RES
    214,  // TEXT_RES
    211,  // TEXT_RESB
    212,  // TEXT_RESB
     53,  // TEXT_RESFLG
     56,  // TEXT_RET
     75,  // TEXT_RL
     65,  // TEXT_RLB
     83,  // TEXT_RLC
     70,  // TEXT_RLCB
     97,  // TEXT_RLDB
     79,  // TEXT_RR
     69,  // TEXT_RRB
     87,  // TEXT_RRC
     74,  // TEXT_RRCB
     95,  // TEXT_RRDB
     94,  // TEXT_SBC
     93,  // TEXT_SBCB
     42,  // TEXT_SC
     86,  // TEXT_SDA
     73,  // TEXT_SDAB
     90,  // TEXT_SDAL
     78,  // TEXT_SDL
     68,  // TEXT_SDLB
     82,  // TEXT_SDLL
    217,  // TEXT_SET
    218,  // TEXT_SET
    215,  // TEXT_SETB
    216,  // TEXT_SETB
     52,  // TEXT_SETFLG
    103,  // TEXT_SIN
     99,  // TEXT_SINB
    133,  // TEXT_SIND
    117,  // TEXT_SINDB
    132,  // TEXT_SINDR
    116,  // TEXT_SINDRB
    125,  // TEXT_SINI
    109,  // TEXT_SINIB
    124,  // TEXT_SINIR
    108,  // TEXT_SINIRB
     84,  // TEXT_SLA
     71,  // TEXT_SLAB
     88,  // TEXT_SLAL
     76,  // TEXT_SLL
     66,  // TEXT_SLLB
     80,  // TEXT_SLLL
    136,  // TEXT_SOTDR
    120,  // TEXT_SOTDRB
    128,  // TEXT_SOTIR
    112,  // TEXT_SOTIRB
    105,  // TEXT_SOUT
    101,  // TEXT_SOUTB
    137,  // TEXT_SOUTD
    121,  // TEXT_SOUTDB
    129,  // TEXT_SOUTI
    113,  // TEXT_SOUTIB
     85,  // TEXT_SRA
     72,  // TEXT_SRAB
     89,  // TEXT_SRAL
     77,  // TEXT_SRL
     67,  // TEXT_SRLB
     81,  // TEXT_SRLL
    173,  // TEXT_SUB
    172,  // TEXT_SUBB
    192,  // TEXT_SUBL
     59,  // TEXT_TCC
     60,  // TEXT_TCC
     57,  // TEXT_TCCB
     58,  // TEXT_TCCB
    185,  // TEXT_TEST
     48,  // TEXT_TESTB
    203,  // TEXT_TESTL
    142,  // TEXT_TRDB
    144,  // TEXT_TRDRB
    138,  // TEXT_TRIB
    140,  // TEXT_TRIRB
    143,  // TEXT_TRTDB
    145,  // TEXT_TRTDRB
    139,  // TEXT_TRTIB
    141,  // TEXT_TRTIRB
    187,  // TEXT_TSET
     50,  // TEXT_TSETB
    179,  // TEXT_XOR
    178,  // TEXT_XORB
};
// clang-format on

using EntryPage = entry::TableBase<Entry>;

static constexpr EntryPage Z8000_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_Z8000), ARRAY_RANGE(INDEX_Z8000)},
};

using Cpu = entry::CpuBase<CpuType, EntryPage>;

static constexpr Cpu CPU_TABLE[] PROGMEM = {
        {Z8001, TEXT_CPU_Z8001, ARRAY_RANGE(Z8000_PAGES)},
        {Z8002, TEXT_CPU_Z8002, ARRAY_RANGE(Z8000_PAGES)},
};

static const Cpu *cpu(CpuType cpuType) {
    return Cpu::search(cpuType, ARRAY_RANGE(CPU_TABLE));
}

static bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table)
        return true;
    if (opr == M_R)
        return table == M_GENI || table == M_GEND || table == M_WR07 || table == M_WR ||
               table == M_DR;
    if (opr == M_IM)
        return table == M_GENI || table == M_IM8 || table == M_BIT || table == M_CNT ||
               table == M_QCNT || table == M_SCNT || table == M_NCNT || table == M_IO;
    if (opr == M_IR)
        return table == M_GENI || table == M_GEND || table == M_GENA || table == M_IRIO;
    if (opr == M_DA)
        return table == M_GENI || table == M_GEND || table == M_GENA || table == M_RA ||
               table == M_RA12 || table == M_RA8 || table == M_RA7 || table == M_IO;
    if (opr == M_X)
        return table == M_GENI || table == M_GEND || table == M_GENA;
    if (opr == M_CC)
        return table == M_FLAG;  // C & Z
    if (opr == M_NONE)
        return table == M_CC || table == M_INTR || table == M_FLAG;
    return false;
}

static bool acceptModes(AsmInsn &insn, const Entry *entry) {
    auto flags = insn.flags();
    auto table = entry->flags();
    return acceptMode(flags.dst(), table.dst()) && acceptMode(flags.src(), table.src()) &&
           acceptMode(flags.ex1(), table.ex1()) && acceptMode(flags.ex2(), table.ex2());
}

Error TableZ8000::searchName(CpuType cpuType, AsmInsn &insn) const {
    cpu(cpuType)->searchName(insn, acceptModes);
    return insn.getError();
}

static bool matchOpCode(DisInsn &insn, const Entry *entry, const EntryPage *page) {
    const auto flags = entry->flags();
    if ((insn.opCode() & flags.codeMask()) != entry->opCode())
        return false;
    if (flags.postFormat() != PF_NONE) {
        if (insn.length() < 4)
            insn.readPost();
        if ((insn.post() & flags.postMask()) != flags.postVal())
            return false;
    }
    return true;
}

Error TableZ8000::searchOpCode(CpuType cpuType, DisInsn &insn, StrBuffer &out) const {
    cpu(cpuType)->searchOpCode(insn, out, matchOpCode);
    return insn.getError();
}

Error TableZ8000::searchOpCodeAlias(CpuType cpuType, DisInsn &insn, StrBuffer &out) const {
    auto entry = cpu(cpuType)->searchOpCode(insn, out, matchOpCode);
    if (entry) {
        entry++;
        insn.clearNameBuffer();
        Cpu::defaultReadEntryName(insn, entry, out, nullptr);
    }
    return insn.getError();
}

const /*PROGMEM*/ char *TableZ8000::listCpu_P() const {
    return TEXT_CPU_LIST;
}

const /*PROGMEM*/ char *TableZ8000::cpuName_P(CpuType cpuType) const {
    return cpu(cpuType)->name_P();
}

Error TableZ8000::searchCpuName(StrScanner &name, CpuType &cpuType) const {
    if (name.iequals(TEXT_CPU_Z8001)) {
        cpuType = Z8001;
    } else if (name.iequals_P(TEXT_CPU_Z8002)) {
        cpuType = Z8002;
    } else {
        return UNSUPPORTED_CPU;
    }
    return OK;
}

const TableZ8000 TABLE;

}  // namespace z8000
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
