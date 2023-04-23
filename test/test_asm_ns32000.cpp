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

#include "asm_ns32000.h"
#include "test_asm_helper.h"

using namespace libasm;
using namespace libasm::ns32000;
using namespace libasm::test;

AsmNs32000 asm32k;
Assembler &assembler(asm32k);

static void set_up() {
    assembler.reset();
}

static void tear_down() {
    symtab.reset();
}

// clang-format off
 void test_cpu() {
    EQUALS("cpu NS32032", true,    assembler.setCpu("NS32032"));
    EQUALS_P("cpu NS32032", "32032", assembler.cpu_P());

    EQUALS("cpu 32032", true,    assembler.setCpu("32032"));
    EQUALS_P("cpu 32032", "32032", assembler.cpu_P());

    TEST("fpu ns32081");
    TEST("fpu none");
    ERRT("fpu ns32082", UNKNOWN_OPERAND, "ns32082");

    TEST("pmmu ns32082");
    TEST("pmmu none");
    ERRT("pmmu ns32081", UNKNOWN_OPERAND, "ns32081");
}

static void test_format_0() {
    ATEST(0x100000, "BEQ .",            0x0A, 0x00);
    ATEST(0x100000, "BNE .+63",         0x1A, 0x3F);
    ATEST(0x100000, "BCS .-64",         0x2A, 0x40);
    ATEST(0x100000, "BCC .+8191",       0x3A, 0x9F, 0xFF);
    ATEST(0x100000, "BHI .-8192",       0x4A, 0xA0, 0x00);
    AERRT(0x000000, "BLS .+536870911",  OVERFLOW_RANGE, ".+536870911",  0x5A, 0xDF, 0xFF, 0xFF, 0xFF);
    AERRT(0xFFFFF0, "BGT .-520093696",  OVERFLOW_RANGE, ".-520093696",  0x6A, 0xE1, 0x00, 0x00, 0x00);
    AERRT(0x100000, "BLE .+0x1FFFFFFF", OVERFLOW_RANGE, ".+0x1FFFFFFF", 0x7A, 0xDF, 0xFF, 0xFF, 0xFF);
    AERRT(0x100000, "BFS .-0x1F000000", OVERFLOW_RANGE, ".-0x1F000000", 0x8A, 0xE1, 0x00, 0x00, 0x00);
    AERRT(0x100000, "BLE .+0x20000000", OVERFLOW_RANGE, ".+0x20000000", 0x7A, 0xDF, 0xFF, 0xFF, 0xFF);
    AERRT(0x100000, "BFS .-0x1F000001", OVERFLOW_RANGE, ".-0x1F000001", 0x8A, 0xE1, 0x00, 0x00, 0x00);
    ATEST(0x100000, "BFC .+2",          0x9A, 0x02);
    ATEST(0x100000, "BLO .-0x9A",       0xAA, 0xBF, 0x66);
    ATEST(0x100000, "BHS .+0x3F",       0xBA, 0x3F);
    ATEST(0x100000, "BLT .-0x80",       0xCA, 0xBF, 0x80);
    ATEST(0x100000, "BGE .+0x100",      0xDA, 0x81, 0x00);
    ATEST(0x100000, "BR  .+10",         0xEA, 0x0A);
    ATEST(0x100000, "BR  .-0x9A",       0xEA, 0xBF, 0x66);
}

static void test_format_1() {
    TEST("BSR .+16", 0x02, 0x10);

    TEST("RET  16", 0x12, 0x10);
    TEST("RETT 16", 0x42, 0x10);
    TEST("RETI ",   0x52);
    TEST("NOP  ",   0xA2);
    TEST("WAIT ",   0xB2);
    TEST("DIA  ",   0xC2);
    TEST("FLAG ",   0xD2);
    TEST("SVC  ",   0xE2);
    TEST("BPT  ",   0xF2);

    TEST("CXP 1",  0x22, 0x01);
    TEST("RXP 16", 0x32, 0x10);

    TEST("save    [r0,r2,r7]",     0x62, 0x85);
    TEST("RESTORE [R0,R2,R7]",     0x72, 0xA1);
    TEST("ENTER   [R0,R2,R7], 16", 0x82, 0x85, 0x10);
    TEST("ENTER   [], 0",          0x82, 0x00, 0x00);
    TEST("EXIT    [R0,R2,R7]",     0x92, 0xA1);
    TEST("EXIT    []",             0x92, 0x00);
    ERRT("SAVE    []",     OPCODE_HAS_NO_EFFECT, "[]",  0x62, 0x00);
    ERRT("RESTORE [ ]",    OPCODE_HAS_NO_EFFECT, "[ ]", 0x72, 0x00);

    // Various displacement.
    TEST("RET 0",          0x12, 0x00);
    TEST("RET 63",         0x12, 0x3F);
    TEST("RET -64",        0x12, 0x40);
    TEST("RET 8191",       0x12, 0x9F, 0xFF);
    TEST("RET -8192",      0x12, 0xA0, 0x00);
    TEST("RET 536870911",  0x12, 0xDF, 0xFF, 0xFF, 0xFF);
    TEST("RET -520093696", 0x12, 0xE1, 0x00, 0x00, 0x00);
    ERRT("RET 536870912",  OVERFLOW_RANGE, "536870912",  0x12, 0xDF, 0xFF, 0xFF, 0xFF);
    ERRT("RET -520093697", OVERFLOW_RANGE, "-520093697", 0x12, 0xE1, 0x00, 0x00, 0x00);
}

static void test_format_2() {
    TEST("ADDQB -8,R0", 0x0C, 0x04);
    TEST("ADDQW 7,R1",  0x8D, 0x0B);
    TEST("ADDQD -8,R2", 0x0F, 0x14);
    TEST("CMPQB -8,R0", 0x1C, 0x04);
    TEST("CMPQW 7,R1",  0x9D, 0x0B);
    TEST("CMPQD -8,R2", 0x1F, 0x14);
    TEST("CMPQB -8,0x12",       0x1C, 0xA4, 0x12);
    TEST("CMPQW 7,0x1234",      0x9D, 0xA3, 0x12, 0x34);
    TEST("CMPQD -8,0x12345678", 0x1F, 0xA4, 0x12, 0x34, 0x56, 0x78);
    TEST("MOVQB 0,R0",  0x5C, 0x00);
    TEST("MOVQW 7,TOS", 0xDD, 0xBB);
    TEST("MOVQD -8,R2", 0x5F, 0x14);

    ATEST(0x1000, "ACBB  -1,R0,.-3", 0xCC, 0x07, 0x7D);
    ATEST(0x1000, "ACBW  -2,R1,.-2", 0x4D, 0x0F, 0x7E);
    ATEST(0x1000, "ACBD  4,R2,.+2",  0x4F, 0x12, 0x02);

    TEST("lprb upsr,r0",    0x6C, 0x00);
    TEST("LPRD FP,R0",      0x6F, 0x04);
    TEST("LPRD SP,R6",      0xEF, 0x34);
    TEST("LPRD SB,R7",      0x6F, 0x3D);
    TEST("LPRW PSR,R2",     0xED, 0x16);
    TEST("LPRD INTBASE,R3", 0x6F, 0x1F);
    TEST("LPRW MOD,4(SB)",  0xED, 0xD7, 0x04);
    TEST("SPRB UPSR,R5",    0x2C, 0x28);
    TEST("SPRD FP,R0",      0x2F, 0x04);
    TEST("SPRD SP,R6",      0xAF, 0x34);
    TEST("SPRD SB,R7",      0x2F, 0x3D);
    TEST("SPRW PSR,R2",     0xAD, 0x16);
    TEST("SPRD INTBASE,R3", 0x2F, 0x1F);
    TEST("SPRW MOD,4(SB)",  0xAD, 0xD7, 0x04);

    TEST("SEQB R0",           0x3C, 0x00);
    TEST("SNEB R1",           0xBC, 0x08);
    TEST("SCSB R2",           0x3C, 0x11);
    TEST("SCCB R3",           0xBC, 0x19);
    TEST("SHIB R4",           0x3C, 0x22);
    TEST("SLSB R5",           0xBC, 0x2A);
    TEST("SGTB R6",           0x3C, 0x33);
    TEST("SLEB R7",           0xBC, 0x3B);
    TEST("SFSB 0(R0)",        0x3C, 0x44, 0x00);
    TEST("SFCB 1(R1)",        0xBC, 0x4C, 0x01);
    TEST("SLOB 2(R2)",        0x3C, 0x55, 0x02);
    TEST("SHSB 3(R3)",        0xBC, 0x5D, 0x03);
    TEST("SLTB 4(R4)",        0x3C, 0x66, 0x04);
    TEST("SGEB 5(R5)",        0xBC, 0x6E, 0x05);
    TEST("SEQW 6(R6)",        0x3D, 0x70, 0x06);
    TEST("SNEW 7(R7)",        0xBD, 0x78, 0x07);
    TEST("SCSW 8(9(FP))",     0x3D, 0x81, 0x09, 0x08);
    TEST("SCCW 10(11(SP))",   0xBD, 0x89, 0x0B, 0x0A);
    TEST("SHIW 12(13(SB))",   0x3D, 0x92, 0x0D, 0x0C);
    TEST("SLSW @14",          0xBD, 0xAA, 0x0E);
    TEST("SGTW EXT(15)+16",   0x3D, 0xB3, 0x0F, 0x10);
    TEST("SGTW 16(15(EXT))",  0x3D, 0xB3, 0x0F, 0x10);
    TEST("SGTW EXT(15)-16",   0x3D, 0xB3, 0x0F, 0x70);
    TEST("SGTW -16(15(EXT))", 0x3D, 0xB3, 0x0F, 0x70);
    TEST("SGTW EXT(15)",      0x3D, 0xB3, 0x0F, 0x00);
    TEST("SGTW 15(EXT)",      0x3D, 0xB3, 0x0F, 0x00);
    TEST("SLEW TOS",          0xBD, 0xBB);
    TEST("SFSW 12(FP)",       0x3D, 0xC4, 0x0C);
    TEST("SFCW 13(SP)",       0xBD, 0xCC, 0x0D);
    TEST("SLOW 10(SB)",       0x3D, 0xD5, 0x0A);
    ATEST(0x10000, "SHSW .+15",        0xBD, 0xDD, 0x0F);
    ATEST(0x10000, "SHSW 0x1000F(PC)", 0xBD, 0xDD, 0x0F);
    ATEST(0x10000, "SHSW x'1000F(PC)", 0xBD, 0xDD, 0x0F);
    TEST("SLTW 2(R0)[R7:B]",  0x3D, 0xE6, 0x47, 0x02);
    TEST("SGEW 3(R1)[R6:W]",  0xBD, 0xEE, 0x4E, 0x03);
    TEST("SEQD 4(R2)[R5:D]",  0x3F, 0xF0, 0x55, 0x04);
    TEST("SNED 5(R3)[R4:Q]",  0xBF, 0xF8, 0x5C, 0x05);
    TEST("SCSD -1(R5)",       0x3F, 0x69, 0x7F);
    TEST("SCCD -2(R6)",       0xBF, 0x71, 0x7E);
    TEST("SHID TOS",          0x3F, 0xBA);
    TEST("SLSD -4(-5(FP))",   0xBF, 0x82, 0x7B, 0x7C);
    TEST("SGTD -6(-7(SP))",   0x3F, 0x8B, 0x79, 0x7A);
    TEST("SLED -8(-9(SB))",   0xBF, 0x93, 0x77, 0x78);
    TEST("SFSD @0x1234",      0x3F, 0xAC, 0x92, 0x34);
    TEST("SFSD @H'1234",      0x3F, 0xAC, 0x92, 0x34);
    TEST("SFCD EXT(-9)+(-8)", 0xBF, 0xB4, 0x77, 0x78);
    TEST("SFCD EXT(-9)-8",    0xBF, 0xB4, 0x77, 0x78);
    TEST("SFCD -8(-9(EXT))",  0xBF, 0xB4, 0x77, 0x78);
    TEST("SLOD -12(FP)",      0x3F, 0xC5, 0x74);
    TEST("SHSD -13(SP)",      0xBF, 0xCD, 0x73);
    TEST("SLTD -14(SB)",      0x3F, 0xD6, 0x72);
    ATEST(0x10000, "SGED .-15",       0xBF, 0xDE, 0x71);
    ATEST(0x10000, "SGED 0xFFF1(PC)", 0xBF, 0xDE, 0x71);
}

static void test_format_3() {
    TEST("BICPSRB 0xA2",  0x7C, 0xA1, 0xA2);
    TEST("BISPSRB 0xA2",  0x7C, 0xA3, 0xA2);
    TEST("ADJSPD -4(FP)", 0x7F, 0xC5, 0x7C);
    ATEST(0x20000, "CASEB .+4[R7:B]", 0x7C, 0xE7, 0xDF, 0x04);
    TEST("CXPD 8(SB)",     0x7F, 0xD0, 0x08);
    TEST("JSR  0(4(SB))",  0x7F, 0x96, 0x04, 0x00);
    TEST("JUMP 0(-8(FP))", 0x7F, 0x82, 0x78, 0x00);
}

static void test_format_4() {
    TEST("MOVB  R0,R1",    0x54, 0x00);
    TEST("MOVW  R2,R3",    0xD5, 0x10);
    TEST("MOVD  R0,8(SB)", 0x97, 0x06, 0x08);

    TEST("ADDB  R0,R1",        0x40, 0x00);
    TEST("ADDW  R2,R3",        0xC1, 0x10);
    TEST("ADDD  4(SB),-4(FP)", 0x03, 0xD6, 0x04, 0x7C);
    TEST("ADDCB 0x20,R0",  0x10, 0xA0, 0x20);
    TEST("ADDCW R2,R3",    0xD1, 0x10);
    TEST("ADDCD 8(SB),R0", 0x13, 0xD0, 0x08);
    TEST("SUBB  R0,R1",          0x60, 0x00);
    TEST("SUBW  R2,R3",          0xE1, 0x10);
    TEST("SUBD  4(SB),0x14(SB)", 0xA3, 0xD6, 0x04, 0x14);
    TEST("SUBCB 0x20,R1",    0x70, 0xA0, 0x20);
    TEST("SUBCW TOS,-8(FP)", 0x31, 0xBE, 0x78);
    TEST("SUBCD R4,R5",      0x73, 0x21);
    TEST("CMPB  7(SB),4(R0)", 0x04, 0xD2, 0x07, 0x04);
    TEST("CMPW  R2,R3",       0xC5, 0x10);
    TEST("CMPD  R4,R5",       0x47, 0x21);
    TEST("CMPB  0x12,0x34",             0x04, 0xA5, 0x12, 0x34);
    TEST("CMPW  0x1234,0x5678",         0x05, 0xA5, 0x12, 0x34, 0x56, 0x78);
    TEST("CMPW  0x1234,X'5678",         0x05, 0xA5, 0x12, 0x34, 0x56, 0x78);
    TEST("CMPW  H'1234,0x5678",         0x05, 0xA5, 0x12, 0x34, 0x56, 0x78);
    TEST("CMPW  X'1234,H'5678",         0x05, 0xA5, 0x12, 0x34, 0x56, 0x78);
    TEST("CMPD  0x12345678,0x9ABCDEF0", 0x07, 0xA5, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0);

    TEST("ANDB  R0,R1", 0x68, 0x00);
    TEST("ANDW  R2,R3", 0xE9, 0x10);
    TEST("ANDD  R4,R5", 0x6B, 0x21);
    TEST("ORB   -6(FP),11(SB)", 0x98, 0xC6, 0x7A, 0x0B);
    TEST("ORW   R2,R3",         0xD9, 0x10);
    TEST("ORD   R4,R5",         0x5B, 0x21);
    TEST("BICB  R0,3(SB)", 0x88, 0x06, 0x03);
    TEST("BICW  R2,R3",    0xC9, 0x10);
    TEST("BICD  R4,R5",    0x4B, 0x21);
    TEST("XORB  -8(FP),-4(FP)", 0x38, 0xC6, 0x78, 0x7C);
    TEST("XORW  R2,R3",         0xF9, 0x10);
    TEST("XORD  R4,R5",         0x7B, 0x21);
    TEST("TBITB R0,R1",    0x74, 0x00);
    TEST("TBITW R0,0(R1)", 0x75, 0x02, 0x00);
    TEST("TBITD R4,R5",    0x77, 0x21);

    TEST("ADDR  4(FP),R0", 0x27, 0xC0, 0x04);
}

static void test_format_5() {
    TEST("CMPSB []",    0x0E, 0x04, 0x00);
    TEST("CMPSB   ",    0x0E, 0x04, 0x00);
    TEST("MOVST [ ]",   0x0E, 0x80, 0x00);
    TEST("SKPSB [U]",   0x0E, 0x0C, 0x06);
    TEST("SKPSB U",     0x0E, 0x0C, 0x06);
    TEST("SKPSB [W]",   0x0E, 0x0C, 0x02);
    TEST("SKPSB W",     0x0E, 0x0C, 0x02);
    TEST("SKPSB [B,U]", 0x0E, 0x0C, 0x07);
    TEST("SKPSB B,U",   0x0E, 0x0C, 0x07);
    TEST("SKPSB B/U",   0x0E, 0x0C, 0x07);
    TEST("SKPSB [W,B]", 0x0E, 0x0C, 0x03);
    TEST("SKPSB W,B",   0x0E, 0x0C, 0x03);
    TEST("SKPSB W/B",   0x0E, 0x0C, 0x03);

    TEST("SETCFG [I,F,M]", 0x0E, 0x8B, 0x03);
    TEST("SETCFG []",      0x0E, 0x0B, 0x00);
}

static void test_format_6() {
    TEST("ABSB R5,R6",    0x4E, 0xB0, 0x29);
    TEST("ABSD 8(SP),R7", 0x4E, 0xF3, 0xC9, 0x08);
    TEST("ADDPB 5(SB),TOS", 0x4E, 0xFC, 0xD5, 0x05);
    TEST("ADDPD R0,R1",     0x4E, 0x7F, 0x00);
    TEST("ASHB  2,16(SB)",   0x4E, 0x84, 0xA6, 0x02, 0x10);
    TEST("ASHW  TOS,16(SB)", 0x4E, 0x85, 0xBE, 0x10);
    TEST("CBITW R0,0(R1)", 0x4E, 0x49, 0x02, 0x00);
    TEST("COMB R0,-4(FP)", 0x4E, 0x34, 0x06, 0x7C);
    TEST("IBITW R0,1(R1)", 0x4E, 0x79, 0x02, 0x01);
    TEST("LSHB 4,8(SB)",      0x4E, 0x94, 0xA6, 0x04, 0x08);
    TEST("LSHW -4(FP),8(SB)", 0x4E, 0x95, 0xC6, 0x7C, 0x08);
    TEST("NEGB R5,R6",       0x4E, 0xA0, 0x29);
    TEST("NEGW 4(SB),6(SB)", 0x4E, 0xA1, 0xD6, 0x04, 0x06);
    TEST("NOTB R0,R0",      0x4E, 0x24, 0x00);
    TEST("NOTW 10(R1),TOS", 0x4E, 0xE5, 0x4D, 0x0A);
    TEST("ROTB 4,R5",      0x4E, 0x40, 0xA1, 0x04);
    TEST("ROTW -3,16(SP)", 0x4E, 0x41, 0xA6, 0xFD, 0x10);
    TEST("SBITW R0,1(R1)", 0x4E, 0x59, 0x02, 0x01);
    TEST("SUBPB -8(FP),16(FP)", 0x4E, 0x2C, 0xC6, 0x78, 0x10);
    TEST("SUBPD 0x00000099,R1", 0x4E, 0x6F, 0xA0, 0x00, 0x00, 0x00, 0x99);
}

static void test_format_7() {
    TEST("CMPMW 10(R0),16(R1),4", 0xCE, 0x45, 0x42, 0x0A, 0x10, 0x06);
    TEST("DEIB R1,R0", 0xCE, 0x2C, 0x08);
    TEST("DEIW R3,R2", 0xCE, 0xAD, 0x18);
    TEST("DEID R5,R4", 0xCE, 0x2F, 0x29);
    TEST("DIVW 10(SP),4(SP)",  0xCE, 0x7D, 0xCE, 0x0A, 0x04);
    TEST("DIVD -6(FP),12(SB)", 0xCE, 0xBF, 0xC6, 0x7A, 0x0C);
    TEST("EXTSW 16(SB),R2,4,7", 0xCE, 0x8D, 0xD0, 0x10, 0x86);
    TEST("INSSW R2,16(SB),4,7", 0xCE, 0x89, 0x16, 0x10, 0x86);
    TEST("MEIB R1,R0", 0xCE, 0x24, 0x08);
    TEST("MEIW R3,R2", 0xCE, 0xA5, 0x18);
    TEST("MEID R5,R4", 0xCE, 0x27, 0x29);
    TEST("MODB 4(SB),8(SB)", 0xCE, 0xB8, 0xD6, 0x04, 0x08);
    TEST("MOVMW 10(R0),16(R1),4", 0xCE, 0x41, 0x42, 0x0A, 0x10, 0x06);
    TEST("MOVXBW 2(SB),R0",  0xCE, 0x10, 0xD0, 0x02);
    TEST("MOVZBW -4(FP),R0", 0xCE, 0x14, 0xC0, 0x7C);
    TEST("MULW 5,R0",            0xCE, 0x21, 0xA0, 0x00, 0x05);
    TEST("MULD 4(-4(FP)),3(SB)", 0xCE, 0xA3, 0x86, 0x7C, 0x04, 0x03);
    TEST("QUOB R0,R7",       0xCE, 0xF0, 0x01);
    TEST("QUOW 4(SB),8(SB)", 0xCE, 0xB1, 0xD6, 0x04, 0x08);
    TEST("REMB 4(SB),8(SB)", 0xCE, 0xB4, 0xD6, 0x04, 0x08);

    ERRT("DEIB R1,R3", REGISTER_NOT_ALLOWED, "R3", 0xCE, 0xEC, 0x08);
    ERRT("DEIW R3,R1", REGISTER_NOT_ALLOWED, "R1", 0xCE, 0x6D, 0x18);
    ERRT("DEID R5,R7", REGISTER_NOT_ALLOWED, "R7", 0xCE, 0xEF, 0x29);
    ERRT("MEIB R1,R3", REGISTER_NOT_ALLOWED, "R3", 0xCE, 0xE4, 0x08);
    ERRT("MEIW R3,R1", REGISTER_NOT_ALLOWED, "R1", 0xCE, 0x65, 0x18);
    ERRT("MEID R5,R7", REGISTER_NOT_ALLOWED, "R7", 0xCE, 0xE7, 0x29);
}

static void test_format_8() {
    TEST("CHECKB R0,4(SB),R2",        0xEE, 0x80, 0xD0, 0x04);
    TEST("CVTP   R0,0x20(SB),R2",     0x6E, 0x83, 0xD0, 0x20);
    TEST("EXTW   R0,0(R1),R2,7",      0x2E, 0x81, 0x48, 0x00, 0x07);
    TEST("FFSB   -4(FP),TOS",         0x6E, 0xC4, 0xC5, 0x7C);
    TEST("FFSW   8(SB),R0",           0x6E, 0x05, 0xD0, 0x08);
    TEST("INDEXB R0,0x14(SB),-4(FP)", 0x2E, 0x04, 0xD6, 0x14, 0x7C);
    TEST("INSW   R0,R2,0(R1),7",      0xAE, 0x41, 0x12, 0x00, 0x07);
}

static void test_format_9() {
    TEST("FPU NS32081");

    TEST("MOVF F1,8(SB)",   0xBE, 0x85, 0x0E, 0x08);
    TEST("MOVL F2,8(SB)",   0xBE, 0x84, 0x16, 0x08);
    TEST("MOVF 1,F3",       0xBE, 0xC5, 0xA0, 0x3F, 0x80, 0x00, 0x00);
    TEST("MOVF -1,F3",      0xBE, 0xC5, 0xA0, 0xBF, 0x80, 0x00, 0x00);
    TEST("MOVF 0x7FFFFFFF,F3",   0xBE, 0xC5, 0xA0, 0x4F, 0x00, 0x00, 0x00);
    TEST("MOVF 0x80000000,F3",   0xBE, 0xC5, 0xA0, 0x4F, 0x00, 0x00, 0x00);
    TEST("MOVF 0xFFFFFFFF,F3",   0xBE, 0xC5, 0xA0, 0x4F, 0x80, 0x00, 0x00);
    TEST("MOVF +99999999999,F3", 0xBE, 0xC5, 0xA0, 0x51, 0xBA, 0x43, 0xB7);
    TEST("MOVF -99999999999,F3", 0xBE, 0xC5, 0xA0, 0xD1, 0xBA, 0x43, 0xB7);
    TEST("MOVF NAN,F1",     0xBE, 0x45, 0xA0, 0x7F, 0xC0, 0x00, 0x00);
    TEST("MOVF +INF,F2",    0xBE, 0x85, 0xA0, 0x7F, 0x80, 0x00, 0x00);
    TEST("MOVF -INF,F2",    0xBE, 0x85, 0xA0, 0xFF, 0x80, 0x00, 0x00);
    TEST("MOVF +0.0,F3",    0xBE, 0xC5, 0xA0, 0x00, 0x00, 0x00, 0x00);
    TEST("MOVF -0.0,F3",    0xBE, 0xC5, 0xA0, 0x80, 0x00, 0x00, 0x00);
    TEST("MOVL 1,F4",       0xBE, 0x04, 0xA1, 0x3F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
    TEST("MOVL -1,F4",      0xBE, 0x04, 0xA1, 0xBF, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
    TEST("MOVL 0x7FFFFFFFFFFFFFFF,F4",
         0xBE, 0x04, 0xA1, 0x43, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
    TEST("MOVL 0x8000000000000000,F4",
         0xBE, 0x04, 0xA1, 0x43, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
    TEST("MOVL 0xFFFFFFFFFFFFFFFF,F4",
         0xBE, 0x04, 0xA1, 0x43, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
    TEST("MOVL +999999999999999999999,F4",
         0xBE, 0x04, 0xA1, 0x44, 0x4B, 0x1A, 0xE4, 0xD6, 0xE2, 0xEF, 0x50);
    TEST("MOVL -999999999999999999999,F4",
         0xBE, 0x04, 0xA1, 0xC4, 0x4B, 0x1A, 0xE4, 0xD6, 0xE2, 0xEF, 0x50);
    TEST("MOVL NAN,F0",     0xBE, 0x04, 0xA0, 0x7F, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
    TEST("MOVL +INF,F2",    0xBE, 0x84, 0xA0, 0x7F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
    TEST("MOVL -INF,F2",    0xBE, 0x84, 0xA0, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
    TEST("MOVL +0.0,F4",    0xBE, 0x04, 0xA1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
    TEST("MOVL -0.0,F4",    0xBE, 0x04, 0xA1, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

    TEST("MOVBF 2,F0",      0x3E, 0x04, 0xA0, 0x02);
    TEST("MOVBL R1,F2",     0x3E, 0x80, 0x08);
    TEST("MOVWL R3,F2",     0x3E, 0x81, 0x18);
    TEST("MOVDL R5,F2",     0x3E, 0x83, 0x28);
    TEST("MOVDL 16(SB),F2", 0x3E, 0x83, 0xD0, 0x10);
    TEST("MOVFL 8(SB),F0",  0x3E, 0x1B, 0xD0, 0x08);
    TEST("MOVFL F1,F2",     0x3E, 0x9B, 0x08);
    TEST("MOVLF F0,12(SB)", 0x3E, 0x96, 0x06, 0x0C);
    TEST("MOVLF F0,F3",     0x3E, 0xD6, 0x00);
    TEST("FLOORFB F1,R3",   0x3E, 0xFC, 0x08);
    TEST("FLOORLD F2,R5",   0x3E, 0x7B, 0x11);
    TEST("ROUNDFB F1,R5",   0x3E, 0x64, 0x09);
    TEST("ROUNDLD F2,R5",   0x3E, 0x63, 0x11);
    TEST("TRUNCFB F1,R3",   0x3E, 0xEC, 0x08);
    TEST("TRUNCLD F2,R5",   0x3E, 0x6B, 0x11);
    TEST("LFSR R0",  0x3E, 0x0F, 0x00);
    TEST("SFSR TOS", 0x3E, 0xF7, 0x05);

    ERRT("MOVL F3,8(SB)",   REGISTER_NOT_ALLOWED, "F3,8(SB)",  0xBE, 0x84, 0x1E, 0x08);
    ERRT("MOVBL R1,F5",     REGISTER_NOT_ALLOWED, "F5",        0x3E, 0x40, 0x09);
    ERRT("MOVWL R3,F7",     REGISTER_NOT_ALLOWED, "F7",        0x3E, 0xC1, 0x19);
    ERRT("MOVDL R5,F1",     REGISTER_NOT_ALLOWED, "F1",        0x3E, 0x43, 0x28);
    ERRT("MOVDL 16(SB),F3", REGISTER_NOT_ALLOWED, "F3",        0x3E, 0xC3, 0xD0, 0x10);
    ERRT("MOVFL 8(SB),F5",  REGISTER_NOT_ALLOWED, "F5",        0x3E, 0x5B, 0xD1, 0x08);
    ERRT("MOVFL F1,F7",     REGISTER_NOT_ALLOWED, "F7",        0x3E, 0xDB, 0x09);
    ERRT("MOVLF F1,12(SB)", REGISTER_NOT_ALLOWED, "F1,12(SB)", 0x3E, 0x96, 0x0E, 0x0C);
    ERRT("MOVLF F3,F3",     REGISTER_NOT_ALLOWED, "F3,F3",     0x3E, 0xD6, 0x18);
    ERRT("FLOORLD F5,R5",   REGISTER_NOT_ALLOWED, "F5,R5",     0x3E, 0x7B, 0x29);
    ERRT("ROUNDLD F7,R5",   REGISTER_NOT_ALLOWED, "F7,R5",     0x3E, 0x63, 0x39);
    ERRT("TRUNCLD F1,R5",   REGISTER_NOT_ALLOWED, "F1,R5",     0x3E, 0x6B, 0x09);

    TEST("FPU NONE");

    ERUI("MOVF F1,8(SB)");
    ERUI("MOVL F2,8(SB)");
    ERUI("MOVBF 2,F0");
    ERUI("MOVBL R1,F2");
    ERUI("MOVWL R1,F2");
    ERUI("MOVDL R1,F2");
    ERUI("MOVFL 8(SB),F0");
    ERUI("MOVLF F0,12(SB)");
    ERUI("FLOORFB F0,R0");
    ERUI("FLOORLD F2,16(SB)");
    ERUI("ROUNDFB F0,R0");
    ERUI("ROUNDLD F2,12(SB)");
    ERUI("TRUNCFB F0,R0");
    ERUI("TRUNCLD F2,8(SB)");
    ERUI("LFSR R0");
    ERUI("SFSR TOS");
}

static void test_format_11() {
    TEST("FPU NS32081");

    TEST("ABSF F1,F5",     0xBE, 0x75, 0x09);
    TEST("ABSL F2,F4",     0xBE, 0x34, 0x11);
    TEST("ADDF F3,F7",     0xBE, 0xC1, 0x19);
    TEST("ADDL F2,16(SB)", 0xBE, 0x80, 0x16, 0x10);
    TEST("ADDL 16(SB),F4", 0xBE, 0x00, 0xD1, 0x10);
    TEST("CMPF F1,F3",     0xBE, 0xC9, 0x08);
    TEST("CMPL F6,F4",     0xBE, 0x08, 0x31);
    TEST("CMPF 1.25, 3.75",
         0xBE, 0x09, 0xA5,
         0x3F, 0xA0, 0x00, 0x00,
         0x40, 0x70, 0x00, 0x00);
    TEST("CMPL 1.25, 3.75",
         0xBE, 0x08, 0xA5,
         0x3F, 0xF4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x40, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
    TEST("DIVF F1,F7",     0xBE, 0xE1, 0x09);
    TEST("DIVL F2,16(SB)", 0xBE, 0xA0, 0x16, 0x10);
    TEST("DIVL -8(FP),F4", 0xBE, 0x20, 0xC1, 0x78);
    TEST("MULF F5,F7",     0xBE, 0xF1, 0x29);
    TEST("MULL F6,8(SB)",  0xBE, 0xB0, 0x36, 0x08);
    TEST("MULL -8(FP),F0", 0xBE, 0x30, 0xC0, 0x78);
    TEST("NEGF F3,F5",     0xBE, 0x55, 0x19);
    TEST("NEGL F2,F4",     0xBE, 0x14, 0x11);
    TEST("SUBF F1,F7",     0xBE, 0xD1, 0x09);
    TEST("SUBL F2,16(SB)", 0xBE, 0x90, 0x16, 0x10);
    TEST("SUBL 16(SB),F4", 0xBE, 0x10, 0xD1, 0x10);

    TEST("FPU NONE");

    ERUI("ABSF F0,F2");
    ERUI("ADDF F0,F7");
    ERUI("ADDL F2,16(SB)");
    ERUI("CMPF F0,F2");
    ERUI("DIVF F0,F7");
    ERUI("divl -8(fp),16(sb)");
    ERUI("MULF F0,F7");
    ERUI("MULL -8(FP),8(SB)");
    ERUI("NEGF F0,F2");
    ERUI("SUBF F0,F7");
    ERUI("SUBL F2,16(SB)");
}

static void test_format_8_mmu() {
    TEST("PMMU NS32082");

    TEST("MOVSUB 5(SP),9(SB)", 0xAE, 0x8C, 0xCE, 0x05, 0x09);
    TEST("MOVUSB 9(SB),5(SP)", 0xAE, 0x5C, 0xD6, 0x09, 0x05);

    TEST("PMMU NONE");

    ERUI("MOVSUB 5(SP),9(SB)");
    ERUI("MOVUSB 9(SB),5(SP)");
}

static void test_format_14() {
    TEST("PMMU NS32082");

    TEST("LMR PTB1,R0", 0x1E, 0x8B, 0x06);
    TEST("SMR PTB0,R0", 0x1E, 0x0F, 0x06);
    TEST("RDVAL 0x200(R0)", 0x1E, 0x03, 0x40, 0x82, 0x00);
    TEST("WRVAL 0x200(R0)", 0x1E, 0x07, 0x40, 0x82, 0x00);

    TEST("PMMU NONE");

    ERUI("LMR PTB1,R0");
    ERUI("SMR PTB0,R0");
    ERUI("RDVAL 0x200(R0)");
    ERUI("WRVAL 0x200(R0)");
}

static void test_generic_addressing() {
    TEST("FPU NS32081");

    // Register
    TEST("ADDW R1, R2", 0x81, 0x08);
    TEST("ADDF F1, F2", 0xBE, 0x81, 0x08);
    // Register Relative
    TEST("ADDW 4(R1), R2",     0x81, 0x48, 0x04);
    TEST("ADDW 4(R1), 64(R2)", 0x81, 0x4A, 0x04, 0x80, 0x40);
    TEST("ADDW R1,    64(R2)", 0x81, 0x0A, 0x80, 0x40);
    // Memory Relative
    TEST("ADDW 6(4(FP)), R2",         0x81, 0x80, 0x04, 0x06);
    TEST("ADDW 6(4(FP)), 32(64(SB))", 0x81, 0x84, 0x04, 0x06, 0x80, 0x40, 0x20);
    TEST("ADDW R2,       32(64(SB))", 0x81, 0x14, 0x80, 0x40, 0x20);
    // Immediate
    TEST("ADDB 0x56,       R1", 0x40, 0xA0, 0x56);
    TEST("ADDW 0x1234,     R1", 0x41, 0xA0, 0x12, 0x34);
    TEST("ADDD 0x12345678, R1", 0x43, 0xA0, 0x12, 0x34, 0x56, 0x78);
    TEST("ADDF 3.14159, F1",
         0xBE, 0x41, 0xA0, 0x40, 0x49, 0x0F, 0xD0);
    TEST("ADDF 299792000, F3",
         0xBE, 0xC1, 0xA0, 0x4D, 0x8E, 0xF3, 0xB4);
    TEST("ADDL 2.718281828459045, F2",
         0xBE, 0x80, 0xA0, 0x40, 0x05, 0xBF, 0x0A, 0x8B, 0x14, 0x57, 0x69);
    TEST("ADDL 6.62607015e-34, F4",
         0xBE, 0x00, 0xA1, 0x39, 0x0B, 0x86, 0x0B, 0xDE, 0x02, 0x31, 0x11);
    // Absolute
    TEST("ADDW @0x1234, 4(R2)",   0x81, 0xAA, 0x92, 0x34, 0x4);
    TEST("ADDW @0x1234, @0x5678", 0x41, 0xAD, 0x92, 0x34, 0xC0, 0x00, 0x56, 0x78);
    TEST("ADDW 4(R2),   @0x5678", 0x41, 0x55, 0x04, 0xC0, 0x00, 0x56, 0x78);
    TEST("MOVW @0x00003F, R0", 0x15, 0xA8, 0x3F);
    TEST("MOVW @0x001FFF, R0", 0x15, 0xA8, 0x9F, 0xFF);
    TEST("MOVW @0x7FFFFF, R0", 0x15, 0xA8, 0xC0, 0x7F, 0xFF, 0xFF);
    TEST("MOVW @0x800000, R0", 0x15, 0xA8, 0xFF, 0x80, 0x00, 0x00);
    TEST("MOVW @0xFFE000, R0", 0x15, 0xA8, 0xA0, 0x00);
    TEST("MOVW @0xFFFFC0, R0", 0x15, 0xA8, 0x40);
    TEST("MOVB @0xFFFFFF, R0", 0x14, 0xA8, 0x7F);
    ERRT("MOVW @0x1000000, R0", OVERFLOW_RANGE, "@0x1000000, R0", 0x15, 0xA8, 0x00);
    // External
    TEST("ADDW EXT(2)+4,     10(R2)",       0x81, 0xB2, 0x02, 0x04, 0x0A);
    TEST("ADDW EXT(-2)+(-4), 10(R2)",       0x81, 0xB2, 0x7E, 0x7C, 0x0A);
    TEST("ADDW EXT(-2)+4,    EXT(-6)+8",    0x81, 0xB5, 0x7E, 0x04, 0x7A, 0x08);
    TEST("ADDW EXT(2)+(-4),  EXT(6)+(-8)",  0x81, 0xB5, 0x02, 0x7C, 0x06, 0x78);
    TEST("ADDW EXT(2),       EXT(6)",       0x81, 0xB5, 0x02, 0x00, 0x06, 0x00);
    TEST("ADDW 10(R2),       EXT(6)+8",     0x81, 0x55, 0x0A, 0x06, 0x08);
    TEST("ADDW 10(R2),       EXT(-6)+(-8)", 0x81, 0x55, 0x0A, 0x7A, 0x78);
    TEST("ADDW 4(2(EXT)),    10(R2)",       0x81, 0xB2, 0x02, 0x04, 0x0A);
    TEST("ADDW -4(-2(EXT)),  10(R2)",       0x81, 0xB2, 0x7E, 0x7C, 0x0A);
    TEST("ADDW 4(-2(EXT)),   8(-6(EXT))",   0x81, 0xB5, 0x7E, 0x04, 0x7A, 0x08);
    TEST("ADDW -4(2(EXT)),   -8(6(EXT))",   0x81, 0xB5, 0x02, 0x7C, 0x06, 0x78);
    TEST("ADDW 2(EXT),       6(EXT)",       0x81, 0xB5, 0x02, 0x00, 0x06, 0x00);
    TEST("ADDW 10(R2),       8(6(EXT))",    0x81, 0x55, 0x0A, 0x06, 0x08);
    TEST("ADDW 10(R2),       -8(-6(EXT))",  0x81, 0x55, 0x0A, 0x7A, 0x78);
    // Top of Stack
    TEST("ADDW TOS,   4(R2)", 0x81, 0xBA, 0x04);
    TEST("ADDW TOS,   TOS",   0xC1, 0xBD);
    TEST("ADDW 4(R2), TOS",   0xC1, 0x55, 0x04);

    symtab.intern(0x1004, "label4");
    symtab.intern(0x1004, ".label_4");
    symtab.intern(0x1004, "_label.4");

    // Memory Space
    TEST("ADDW 4(FP), 6(R2)", 0x81, 0xC2, 0x04, 0x06);
    TEST("ADDW 4(SP), 6(R2)", 0x81, 0xCA, 0x04, 0x06);
    TEST("ADDW 4(SB), 6(R2)", 0x81, 0xD2, 0x04, 0x06);
    ATEST(0x1000, "ADDW .+4,        6(R2)", 0x81, 0xDA, 0x04, 0x06);
    ATEST(0x1000, "ADDW label4(PC), 6(R2)", 0x81, 0xDA, 0x04, 0x06);
    TEST("ADDW 6(R2), 4(FP)", 0x01, 0x56, 0x06, 0x04);
    TEST("ADDW 6(R2), 4(SP)", 0x41, 0x56, 0x06, 0x04);
    TEST("ADDW 6(R2), 4(SB)", 0x81, 0x56, 0x06, 0x04);
    ATEST(0x1000, "ADDW 6(R2), .+4",          0xC1, 0x56, 0x06, 0x04);
    ATEST(0x1000, "ADDW 6(R2), .label_4(PC)", 0xC1, 0x56, 0x06, 0x04);
    TEST("ADDW 4(FP), 6(SB)", 0x81, 0xC6, 0x04, 0x06);
    TEST("ADDW 6(SB), 8(SP)", 0x41, 0xD6, 0x06, 0x08);
    ATEST(0x1000, "ADDW 8(SP), .+10",        0xC1, 0xCE, 0x08, 0x0A);
    ATEST(0x1000, "ADDW 8(SP), 0x100A(PC)",  0xC1, 0xCE, 0x08, 0x0A);
    ATEST(0x1000, "ADDW .+2,   .+12",        0xC1, 0xDE, 0x02, 0x0C);
    ATEST(0x1000, "ADDW .+2,   0x100C(PC)",  0xC1, 0xDE, 0x02, 0x0C);
    // Scaled Indexing
    TEST("ADDW R1[R3:B],8(R2)",             0x81, 0xE2, 0x0B, 0x08);
    TEST("ADDW 4(R1)[R3:B],8(R2)",          0x81, 0xE2, 0x4B, 0x04, 0x08);
    TEST("ADDW 6(4(FP))[R3:B],8(R2)",       0x81, 0xE2, 0x83, 0x04, 0x06, 0x08);
    TEST("ADDW @0x1234[R3:B],8(R2)",        0x81, 0xE2, 0xAB, 0x92, 0x34, 0x08);
    TEST("addw ext(4)+6[r3:b],8(r2)",       0x81, 0xE2, 0xB3, 0x04, 0x06, 0x08);
    TEST("ADDW EXT(4)-6[R3:B],8(R2)",       0x81, 0xE2, 0xB3, 0x04, 0x7A, 0x08);
    TEST("ADDW EXT(4)[R3:B],8(R2)",         0x81, 0xE2, 0xB3, 0x04, 0x00, 0x08);
    TEST("ADDW 6(4(EXT))[R3:B],8(R2)",      0x81, 0xE2, 0xB3, 0x04, 0x06, 0x08);
    TEST("ADDW -6(4(EXT))[R3:B],8(R2)",     0x81, 0xE2, 0xB3, 0x04, 0x7A, 0x08);
    TEST("ADDW 4(EXT)[R3:B],8(R2)",         0x81, 0xE2, 0xB3, 0x04, 0x00, 0x08);
    TEST("ADDW TOS[R3:B],8(R2)",            0x81, 0xE2, 0xBB, 0x08);
    TEST("ADDW 4(SP)[R3:B],8(R2)",          0x81, 0xE2, 0xCB, 0x04, 0x08);
    ATEST(0x1000, "ADDW .+4[R3:B],8(R2)",        0x81, 0xE2, 0xDB, 0x04, 0x08);
    ATEST(0x1000, "ADDW label4(PC)[R3:B],8(R2)", 0x81, 0xE2, 0xDB, 0x04, 0x08);
    TEST("ADDW 8(R2),R1[R3:W]",             0x41, 0x57, 0x0B, 0x08);
    TEST("ADDW 8(R2),4(R1)[R3:W]",          0x41, 0x57, 0x4B, 0x08, 0x04);
    TEST("ADDW 8(R2),6(4(FP))[R3:W]",       0x41, 0x57, 0x83, 0x08, 0x04, 0x06);
    TEST("ADDW 8(R2),@0x1234[R3:W]",        0x41, 0x57, 0xAB, 0x08, 0x92, 0x34);
    TEST("ADDW 8(R2),EXT(4)+6[R3:W]",       0x41, 0x57, 0xB3, 0x08, 0x04, 0x06);
    TEST("ADDW 8(R2),6(4(EXT))[R3:W]",      0x41, 0x57, 0xB3, 0x08, 0x04, 0x06);
    TEST("ADDW 8(R2),TOS[R3:W]",            0x41, 0x57, 0xBB, 0x08);
    TEST("ADDW 8(R2),4(SP)[R3:W]",          0x41, 0x57, 0xCB, 0x08, 0x04);
    ATEST(0x1000, "ADDW 8(R2), *+4[R3:W]",          0x41, 0x57, 0xDB, 0x08, 0x04);
    ATEST(0x1000, "ADDW 8(R2), _label.4(PC)[R3:W]", 0x41, 0x57, 0xDB, 0x08, 0x04);
    TEST("ADDW 8(R2)[R3:D],R1[R5:W]",       0x41, 0xF7, 0x53, 0x0D, 0x08);
    TEST("ADDW 8(R2)[R3:D],4(R1)[R5:W]",    0x41, 0xF7, 0x53, 0x4D, 0x08, 0x04);
    TEST("ADDW 8(R2)[R3:D],6(4(FP))[R5:W]", 0x41, 0xF7, 0x53, 0x85, 0x08, 0x04, 0x06);
    TEST("ADDW 8(R2)[R3:D],@0x1234[R5:W]",  0x41, 0xF7, 0x53, 0xAD, 0x08, 0x92, 0x34);
    TEST("ADDW 8(R2)[R3:Q],EXT(4)+6[R5:W]", 0x41, 0xFF, 0x53, 0xB5, 0x08, 0x04, 0x06);
    TEST("ADDW 8(R2)[R3:Q],6(4(EXT))[R5:W]",0x41, 0xFF, 0x53, 0xB5, 0x08, 0x04, 0x06);
    TEST("ADDW 8(R2)[R3:Q],TOS[R5:W]",      0x41, 0xFF, 0x53, 0xBD, 0x08);
    TEST("ADDW 8(R2)[R3:Q],4(SP)[R5:W]",    0x41, 0xFF, 0x53, 0xCD, 0x08, 0x04);
    ATEST(0x100000, "ADDW 8(R2)[R3:Q],.+4[R5:W]",
                                            0x41, 0xFF, 0x53, 0xDD, 0x08, 0x04);
    ATEST(0x100000, "ADDW 8(R2)[R3:Q], 0x100004(PC)[R5:W]",
                                            0x41, 0xFF, 0x53, 0xDD, 0x08, 0x04);

    TEST("EXTD R1,0x112233(0x334455(FP))[R3:W],0x556677(0x778899(SB))[R4:W],32",
         // 11101|111 01|001|0|11 00101111
         // 10000 011 10010 100
         0x2E, 0x4B, 0xEF,
         0x83, 0x94,
         0xC0, 0x33, 0x44, 0x55,
         0xC0, 0x11, 0x22, 0x33,
         0xC0, 0x77, 0x88, 0x99,
         0xC0, 0x55, 0x66, 0x77,
         0x20);

    TEST("INSW R0, *  , 0(R1)[R5:W], 7", 0xAE, 0x41, 0xDF, 0x4D, 0x00, 0x00, 0x07);
    TEST("INSB R0, *+3, 0(R0)[R2:B], 4", 0xAE, 0x00, 0xDF, 0x42, 0x03, 0x00, 0x04);
}

static void test_comment() {
    TEST("FPU NS32081");
    TEST("PMMU NS32082");

    TEST("ADDB R1 , R0           ; comment",     0x00, 0x08);
    TEST("ADDB 2 (R2) , R0       ; comment",     0x00, 0x50, 0x02);
    TEST("ADDB 2 ( 4 (FP) ) , R0 ; comment",     0x00, 0x80, 0x04, 0x02);
    TEST("ADDB 0x123 , R0        ; comment",     0x00, 0xA0, 0x23);
    TEST("ADDB @0x123 , R0       ; comment",     0x00, 0xA8, 0x81, 0x23);
    TEST("ADDB EXT( 2 ) + ( -4 ), R0 ; comment", 0x00, 0xB0, 0x02, 0x7C);
    TEST("ADDB - 4 ( 2 (EXT)),    R0 ; comment", 0x00, 0xB0, 0x02, 0x7C);
    TEST("ADDB TOS , R0          ; comment",     0x00, 0xB8);
    TEST("ADDB 2 (SP) , R0       ; comment",     0x00, 0xC8, 0x02);
    TEST("ADDB R1[R3:B] , R0           ; comment",     0x00, 0xE0, 0x0B);
    TEST("ADDB 2 (R2)[R3:B] , R0       ; comment",     0x00, 0xE0, 0x53, 0x02);
    TEST("ADDB 2 ( 4 (FP) )[R3:B] , R0 ; comment",     0x00, 0xE0, 0x83, 0x04, 0x02);
    TEST("ADDB @0x123[R3:B] , R0       ; comment",     0x00, 0xE0, 0xAB, 0x81, 0x23);
    TEST("ADDB EXT( 2 ) + ( -4 )[R3:B], R0 ; comment", 0x00, 0xE0, 0xB3, 0x02, 0x7C);
    TEST("ADDB - 4 ( 2 (EXT)) [R3:B],   R0 ; comment", 0x00, 0xE0, 0xB3, 0x02, 0x7C);
    TEST("ADDB TOS[R3:B] , R0          ; comment",     0x00, 0xE0, 0xBB);
    TEST("ADDB 2 (SP)[R3:B] , R0       ; comment",     0x00, 0xE0, 0xCB, 0x02);
    ATEST(0x100, "ADDW 8 (SP) , . + 10   ; comment", 0xC1, 0xCE, 0x08, 0x0A);
    ATEST(0x100, "ADDW 8 (SP) , * - 10   ; comment", 0xC1, 0xCE, 0x08, 0x76);
    ATEST(0x100, "ADDW 8 (SP) , 0x10A (PC)   ; comment", 0xC1, 0xCE, 0x08, 0x0A);

    TEST("ADDL 137.03599908421, F6",
         0xBE, 0x80, 0xA1, 0x40, 0x61, 0x21, 0x26, 0xE7, 0x8D, 0x2B, 0xC6);

    TEST("LPRB UPSR , R0 ; comment", 0x6C, 0x00);
    TEST("LMR  PTB0 , R0 ; comment", 0x1E, 0x0B, 0x06);

    TEST("SETCFG [ I , F , M , C ] ; comment", 0x0E, 0x8B, 0x07);

    TEST("ENTER [ R0 , R1 ] , 10   ; comment", 0x82, 0x03, 0x0A);
    TEST("EXIT  [ R0 , R1 ]        ; comment", 0x92, 0xC0);

    TEST("MOVSB [ ]       ; comment", 0x0E, 0x00, 0x00);
    TEST("MOVSB [ B ]     ; comment", 0x0E, 0x00, 0x01);
    TEST("MOVSB [ B , W ] ; comment", 0x0E, 0x00, 0x03);
    TEST("MOVSB [ U , B ] ; comment", 0x0E, 0x00, 0x07);
    ERRT("MOVSB [ U , W ] ; comment", ILLEGAL_OPERAND,  "[ U , W ] ; comment");
}

static void test_undefined_symbol() {
    ERUS("ADDB UNDEF(R2), R0",          "UNDEF(R2), R0",           0x00, 0x50, 0x00);
    ERUS("ADDB UNDEF(4(FP)), R0",       "UNDEF(4(FP)), R0",        0x00, 0x80, 0x04, 0x00);
    ERUS("ADDB 2(UNDEF(FP)), R0",       "UNDEF(FP)), R0",          0x00, 0x80, 0x00, 0x02);
    ERUS("ADDB UNDEF(UNDEF(FP)), R0",   "UNDEF(UNDEF(FP)), R0",    0x00, 0x80, 0x00, 0x00);
    ERUS("ADDB UNDEF, R0",              "UNDEF, R0",               0x00, 0xA0, 0x00);
    ERUS("ADDB @UNDEF, R0",             "UNDEF, R0",               0x00, 0xA8, 0x00);
    ERUS("ADDB EXT(UNDEF)+(-4), R0",    "UNDEF)+(-4), R0",         0x00, 0xB0, 0x00, 0x7C);
    ERUS("ADDB EXT(2)+(UNDEF), R0",     "UNDEF), R0",              0x00, 0xB0, 0x02, 0x00);
    ERUS("ADDB EXT(UNDEF)+(UNDEF), R0", "UNDEF)+(UNDEF), R0",      0x00, 0xB0, 0x00, 0x00);
    ERUS("ADDB -4(UNDEF(EXT)), R0",     "UNDEF(EXT)), R0",         0x00, 0xB0, 0x00, 0x7C);
    ERUS("ADDB UNDEF(2(EXT)), R0",      "UNDEF(2(EXT)), R0",       0x00, 0xB0, 0x02, 0x00);
    ERUS("ADDB UNDEF(UNDEF(EXT)), R0",  "UNDEF(UNDEF(EXT)), R0",   0x00, 0xB0, 0x00, 0x00);
    ERUS("ADDB UNDEF(SP), R0",          "UNDEF(SP), R0",           0x00, 0xC8, 0x00);
    ERUS("ADDB R0, UNDEF(R2)",          "UNDEF(R2)",               0x80, 0x02, 0x00);
    ERUS("ADDB R0, UNDEF(4(FP))",       "UNDEF(4(FP))",            0x00, 0x04, 0x04, 0x00);
    ERUS("ADDB R0, 2(UNDEF(FP))",       "UNDEF(FP))",              0x00, 0x04, 0x00, 0x02);
    ERUS("ADDB R0, UNDEF(UNDEF(FP))",   "UNDEF(UNDEF(FP))",        0x00, 0x04, 0x00, 0x00);
    ERUS("ADDB R0, @UNDEF",             "UNDEF",                   0x40, 0x05, 0x00);
    ERUS("ADDB R0, EXT(UNDEF)+(-4)",    "UNDEF)+(-4)",             0x80, 0x05, 0x00, 0x7C);
    ERUS("ADDB R0, EXT(2)+(UNDEF)",     "UNDEF)",                  0x80, 0x05, 0x02, 0x00);
    ERUS("ADDB R0, EXT(UNDEF)+(UNDEF)", "UNDEF)+(UNDEF)",          0x80, 0x05, 0x00, 0x00);
    ERUS("ADDB R0, UNDEF(SP)",          "UNDEF(SP)",               0x40, 0x06, 0x00);
    AERUS(0x100, "ADDW 8(SP) , *+UNDEF",      "UNDEF",                0xC1, 0xCE, 0x08, 0x00);
    AERUS(0x100, "ADDW 8(SP) , UNDEF(PC)",    "UNDEF(PC)",            0xC1, 0xCE, 0x08, 0x00);
    AERUS(0x100, "ADDW *-UNDEF, 6(R2)",       "UNDEF, 6(R2)",         0x81, 0xDA, 0x00, 0x06);
    AERUS(0x100, "ADDW UNDEF(PC), 6(R2)",     "UNDEF(PC), 6(R2)",     0x81, 0xDA, 0x00, 0x06);
    AERUS(0x100, "ADDW UNDEF(PC), UNDEF(R2)", "UNDEF(PC), UNDEF(R2)", 0x81, 0xDA, 0x00, 0x00);

    ERUS("MOVQB UNDEF,R0", "UNDEF,R0", 0x5C, 0x00);

    ERUS("BR   UNDEF",          "UNDEF",          0xEA, 0x00);
    ERUS("ACBB UNDEF,R0,.",     "UNDEF,R0,.",     0x4C, 0x00, 0x00);
    ERUS("ACBB -1,   R0,UNDEF", "UNDEF",          0xCC, 0x07, 0x00);
    ERUS("ACBB UNDEF,R0,UNDEF", "UNDEF,R0,UNDEF", 0x4C, 0x00, 0x00);

    ERUS("MOVMB 2(R0),4(R1),UNDEF", "UNDEF", 0xCE, 0x40, 0x42, 0x02, 0x04, 0x00);

    ERUS("INSSB 16, 2(R0),2, UNDEF",      "UNDEF",                   0xCE, 0x08, 0xA2, 0x10, 0x02, 0x40);
    ERUS("INSSB 16, 2(R0),UNDEF,6",       "UNDEF,6",                 0xCE, 0x08, 0xA2, 0x10, 0x02, 0x05);
    ERUS("INSSB 16, 2(R0),UNDEF,UNDEF",   "UNDEF,UNDEF",             0xCE, 0x08, 0xA2, 0x10, 0x02, 0x00);
    ERUS("INSSB UNDEF,2(R0),2,6",         "UNDEF,2(R0),2,6",         0xCE, 0x08, 0xA2, 0x00, 0x02, 0x45);
    ERUS("INSSB UNDEF,2(R0),2,UNDEF",     "UNDEF,2(R0),2,UNDEF",     0xCE, 0x08, 0xA2, 0x00, 0x02, 0x40);
    ERUS("INSSB UNDEF,2(R0),UNDEF,6",     "UNDEF,2(R0),UNDEF,6",     0xCE, 0x08, 0xA2, 0x00, 0x02, 0x05);
    ERUS("INSSB UNDEF,2(R0),UNDEF,UNDEF", "UNDEF,2(R0),UNDEF,UNDEF", 0xCE, 0x08, 0xA2, 0x00, 0x02, 0x00);

    ERUS("INSB R1,UNDEF,2(R0),6",     "UNDEF,2(R0),6",     0xAE, 0x08, 0xA2, 0x00, 0x02, 0x06);
    ERUS("INSB R1,16,   2(R0),UNDEF", "UNDEF",             0xAE, 0x08, 0xA2, 0x10, 0x02, 0x00);
    ERUS("INSB R1,UNDEF,2(R0),UNDEF", "UNDEF,2(R0),UNDEF", 0xAE, 0x08, 0xA2, 0x00, 0x02, 0x00);

    TEST("FPU NS32081");
    ERUS("ADDF UNDEF, F0", "UNDEF, F0", 0xBE, 0x01, 0xA0, 0x00, 0x00, 0x00, 0x00);
    ERUS("ADDL UNDEF, F0", "UNDEF, F0", 0xBE, 0x00, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
    ERUS("CMPF F0, UNDEF", "UNDEF",     0xBE, 0x09, 0x05, 0x00, 0x00, 0x00, 0x00);
    ERUS("CMPL F0, UNDEF", "UNDEF",     0xBE, 0x08, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
}

static void test_data_constant() {
    TEST(".byte   -128, 255",    0x80, 0xFF);
    TEST(".byte   'A', '\"'",    0x41, 0x22);
    TEST(".byte   '9'-'0'",      0x09);
    TEST(".byte   '\\''",        0x27);
    ERRT(".byte   '''",          ILLEGAL_CONSTANT, "'''");
    ERRT(".byte   ''",           ILLEGAL_CONSTANT, "''");
    TEST(".byte   \"A\\\"B\",0", 0x41, 0x22, 0x42, 0x00);
    ERRT(".byte   \"A\\\"B,0",   MISSING_CLOSING_DQUOTE, "\"A\\\"B,0");
    TEST(".byte   \"\\x12\"",    0x78, 0x31, 0x32);
    TEST(".ascii  \"A'B\\\"C\"", 0x41, 0x27, 0x42, 0x22, 0x43);
    TEST(".ascii  \"A\", \"C\"", 0x41, 0x43);
    TEST(".ascii   \"\\x12\"",   0x78, 0x31, 0x32);
    TEST(".word   -128, 255",    0xFF, 0x80, 0x00, 0xFF);
    TEST(".word   'X'",          0x00, 0x58);
    TEST(".word   'X'+0",        0x00, 0x58);
    TEST(".word   \"X\"",        0x58, 0x00);
    TEST(".word   \"A'B\"",      0x41, 0x27, 0x42, 0x00);
    ERRT(".word   \"A'B",        MISSING_CLOSING_DQUOTE, "\"A'B");
    TEST(".double x'1234",       0x00, 0x00, 0x12, 0x34);
    TEST(".double x'12345678",   0x12, 0x34, 0x56, 0x78);
    TEST(".double 'X'",          0x00, 0x00, 0x00, 0x58);
    TEST(".double \"X\"",        0x58, 0x00, 0x00, 0x00);
    TEST(".double \"A'B\\\"C\"", 0x41, 0x27, 0x42, 0x22, 0x43, 0x00, 0x00, 0x00);
    ERRT(".double \"A'B\\\"C",   MISSING_CLOSING_DQUOTE, "\"A'B\\\"C");
}

// clang-format on

void run_tests(const char *cpu) {
    assembler.setCpu(cpu);
    RUN_TEST(test_format_0);
    RUN_TEST(test_format_1);
    RUN_TEST(test_format_2);
    RUN_TEST(test_format_3);
    RUN_TEST(test_format_4);
    RUN_TEST(test_format_5);
    RUN_TEST(test_format_6);
    RUN_TEST(test_format_7);
    RUN_TEST(test_format_8);
    RUN_TEST(test_format_9);
    RUN_TEST(test_format_11);
    RUN_TEST(test_format_8_mmu);
    RUN_TEST(test_format_14);
    RUN_TEST(test_generic_addressing);
    RUN_TEST(test_comment);
    RUN_TEST(test_undefined_symbol);
    RUN_TEST(test_data_constant);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
