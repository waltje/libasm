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

#include "asm_mc6809.h"
#include "test_asm_helper.h"

using namespace libasm;
using namespace libasm::mc6809;
using namespace libasm::test;

AsmMc6809 as6809;
Assembler &assembler(as6809);

static bool is6309() {
    return strcmp(assembler.getCpu(), "6309") == 0;
}

static void set_up() {
    assembler.reset();
}

static void tear_down() {
    symtab.reset();
}

static void test_cpu() {
    EQUALS("cpu 6809", true,   assembler.setCpu("6809"));
    EQUALS("cpu 6809", "6809", assembler.getCpu());

    EQUALS("cpu 6309", true,   assembler.setCpu("6309"));
    EQUALS("cpu 6309", "6309", assembler.getCpu());

    EQUALS("cpu mc6809", true,   assembler.setCpu("mc6809"));
    EQUALS("cpu mc6809", "6809", assembler.getCpu());

    EQUALS("cpu hd6309", true,   assembler.setCpu("hd6309"));
    EQUALS("cpu hd6309", "6309", assembler.getCpu());
}

static void test_inherent() {
    TEST("NOP",  0x12);
    TEST("SYNC", 0x13);
    TEST("DAA",  0x19);
    TEST("SEX",  0x1D);
    TEST("RTS",  0x39);
    TEST("ABX",  0x3A);
    TEST("RTI",  0x3B);
    TEST("MUL",  0x3D);
    TEST("SWI",  0x3F);
    TEST("SWI2", 0x10, 0x3F);
    TEST("SWI3", 0x11, 0x3F);

    TEST("NEGA", 0x40);
    TEST("COMA", 0x43);
    TEST("LSRA", 0x44);
    TEST("RORA", 0x46);
    TEST("ASRA", 0x47);
    TEST("ASLA", 0x48);
    TEST("ROLA", 0x49);
    TEST("DECA", 0x4A);
    TEST("INCA", 0x4C);
    TEST("TSTA", 0x4D);
    TEST("CLRA", 0x4F);

    TEST("NEGB", 0x50);
    TEST("COMB", 0x53);
    TEST("LSRB", 0x54);
    TEST("RORB", 0x56);
    TEST("ASRB", 0x57);
    TEST("ASLB", 0x58);
    TEST("ROLB", 0x59);
    TEST("DECB", 0x5A);
    TEST("INCB", 0x5C);
    TEST("TSTB", 0x5D);
    TEST("CLRB", 0x5F);

    if (is6309()) {
        // HD6309
        TEST("SEXW",  0x14);
        TEST("PSHSW", 0x10, 0x38);
        TEST("PULSW", 0x10, 0x39);
        TEST("PSHUW", 0x10, 0x3A);
        TEST("PULUW", 0x10, 0x3B);

        TEST("NEGD",  0x10, 0x40);
        TEST("COMD",  0x10, 0x43);
        TEST("LSRD",  0x10, 0x44);
        TEST("RORD",  0x10, 0x46);
        TEST("ASRD",  0x10, 0x47);
        TEST("ASLD",  0x10, 0x48);
        TEST("ROLD",  0x10, 0x49);
        TEST("DECD",  0x10, 0x4A);
        TEST("INCD",  0x10, 0x4C);
        TEST("TSTD",  0x10, 0x4D);
        TEST("CLRD",  0x10, 0x4F);

        TEST("COME",  0x11, 0x43);
        TEST("DECE",  0x11, 0x4A);
        TEST("INCE",  0x11, 0x4C);
        TEST("TSTE",  0x11, 0x4D);
        TEST("CLRE",  0x11, 0x4F);

        TEST("COMF",  0x11, 0x53);
        TEST("DECF",  0x11, 0x5A);
        TEST("INCF",  0x11, 0x5C);
        TEST("TSTF",  0x11, 0x5D);
        TEST("CLRF",  0x11, 0x5F);
    } else {
        ETEST(UNKNOWN_INSTRUCTION, "SEXW");
        ETEST(UNKNOWN_INSTRUCTION, "PSHSW");
        ETEST(UNKNOWN_INSTRUCTION, "PULSW");
        ETEST(UNKNOWN_INSTRUCTION, "PSHUW");
        ETEST(UNKNOWN_INSTRUCTION, "PULUW");
        ETEST(UNKNOWN_INSTRUCTION, "NEGD");
        ETEST(UNKNOWN_INSTRUCTION, "COMD");
        ETEST(UNKNOWN_INSTRUCTION, "LSRD");
        ETEST(UNKNOWN_INSTRUCTION, "RORD");
        ETEST(UNKNOWN_INSTRUCTION, "ASRD");
        ETEST(UNKNOWN_INSTRUCTION, "ASLD");
        ETEST(UNKNOWN_INSTRUCTION, "ROLD");
        ETEST(UNKNOWN_INSTRUCTION, "DECD");
        ETEST(UNKNOWN_INSTRUCTION, "INCD");
        ETEST(UNKNOWN_INSTRUCTION, "TSTD");
        ETEST(UNKNOWN_INSTRUCTION, "CLRD");
        ETEST(UNKNOWN_INSTRUCTION, "COME");
        ETEST(UNKNOWN_INSTRUCTION, "DECE");
        ETEST(UNKNOWN_INSTRUCTION, "INCE");
        ETEST(UNKNOWN_INSTRUCTION, "TSTE");
        ETEST(UNKNOWN_INSTRUCTION, "CLRE");
        ETEST(UNKNOWN_INSTRUCTION, "COMF");
        ETEST(UNKNOWN_INSTRUCTION, "DECF");
        ETEST(UNKNOWN_INSTRUCTION, "INCF");
        ETEST(UNKNOWN_INSTRUCTION, "TSTF");
        ETEST(UNKNOWN_INSTRUCTION, "CLRF");
    }
}

static void test_stack() {
    TEST("PSHS CC", 0x34, 0x01);
    TEST("PSHS A",  0x34, 0x02);
    TEST("PSHS B",  0x34, 0x04);
    TEST("PSHS DP", 0x34, 0x08);
    TEST("PSHS X",  0x34, 0x10);
    TEST("PSHS Y",  0x34, 0x20);
    TEST("PSHS U",  0x34, 0x40);
    TEST("PSHS PC", 0x34, 0x80);
    TEST("PSHS D",  0x34, 0x06);

    TEST("PSHU CC", 0x36, 0x01);
    TEST("PSHU A",  0x36, 0x02);
    TEST("PSHU B",  0x36, 0x04);
    TEST("PSHU DP", 0x36, 0x08);
    TEST("PSHU X",  0x36, 0x10);
    TEST("PSHU Y",  0x36, 0x20);
    TEST("PSHU S",  0x36, 0x40);
    TEST("PSHU PC", 0x36, 0x80);
    TEST("PSHU D",  0x36, 0x06);

    TEST("PULS CC", 0x35, 0x01);
    TEST("PULS A",  0x35, 0x02);
    TEST("PULS B",  0x35, 0x04);
    TEST("PULS DP", 0x35, 0x08);
    TEST("PULS X",  0x35, 0x10);
    TEST("PULS Y",  0x35, 0x20);
    TEST("PULS U",  0x35, 0x40);
    TEST("PULS PC", 0x35, 0x80);
    TEST("PULS D",  0x35, 0x06);

    TEST("PULU CC", 0x37, 0x01);
    TEST("PULU A",  0x37, 0x02);
    TEST("PULU B",  0x37, 0x04);
    TEST("PULU DP", 0x37, 0x08);
    TEST("PULU X",  0x37, 0x10);
    TEST("PULU Y",  0x37, 0x20);
    TEST("PULU S",  0x37, 0x40);
    TEST("PULU PC", 0x37, 0x80);
    TEST("PULU D",  0x37, 0x06);

    TEST("PSHU A,X",     0x36, 0x12);
    TEST("PSHU A,X,B,Y", 0x36, 0x36);

    TEST("PSHS CC,A,B,DP,X,Y,U,PC", 0x34, 0xFF);
    TEST("PULS PC,A,DP,B,X,U,Y,CC", 0x35, 0xFF);
    TEST("PSHU PC,S,Y,X,DP,B,A,CC", 0x36, 0xFF);
    TEST("PULU DP,CC,PC,A,B,S,Y,X", 0x37, 0xFF);
    TEST("PSHS PC,U,Y,X,DP,D,CC",   0x34, 0xFF);
    TEST("PULS CC,D,DP,X,U,Y,PC",   0x35, 0xFF);

    TEST("PSHS",      0x34, 0x00);
    TEST("PULS #$00", 0x35, 0x00);
    TEST("PSHU #$AA", 0x36, 0xAA);
    TEST("PULU #$FF", 0x37, 0xFF);

    ETEST(REGISTER_NOT_ALLOWED, "PSHS S");
    ETEST(REGISTER_NOT_ALLOWED, "PULS A,S,B");
    ETEST(REGISTER_NOT_ALLOWED, "PSHU CC,X,U,PC");
    ETEST(REGISTER_NOT_ALLOWED, "PULU U");

    ETEST(DUPLICATE_REGISTER, "PSHS A,D");
    ETEST(DUPLICATE_REGISTER, "PULS D,B");
    ETEST(DUPLICATE_REGISTER, "PSHU X,D,X");
    ETEST(DUPLICATE_REGISTER, "PSHS U,D,U");
    ETEST(DUPLICATE_REGISTER, "PSHU S,D,S");

    ETEST(REGISTER_NOT_ALLOWED, "PULS E");
    ETEST(REGISTER_NOT_ALLOWED, "PSHU F");
    ETEST(REGISTER_NOT_ALLOWED, "PSHS V");
    ETEST(REGISTER_NOT_ALLOWED, "PULU W");
    ETEST(REGISTER_NOT_ALLOWED, "PULS E,X");
    ETEST(REGISTER_NOT_ALLOWED, "PSHU F,Y");
    ETEST(REGISTER_NOT_ALLOWED, "PSHS V,U");
}

static void test_register() {
    TEST("EXG A,B", 0x1E, 0x89);
    TEST("EXG A,A", 0x1E, 0x88);
    TEST("TFR X,Y", 0x1F, 0x12);
    TEST("TFR X,X", 0x1F, 0x11);

    TEST("TFR A,B",  0x1F, 0x89);
    TEST("TFR A,CC", 0x1F, 0x8A);
    TEST("TFR A,DP", 0x1F, 0x8B);
    TEST("TFR B,A",  0x1F, 0x98);
    TEST("TFR CC,A", 0x1F, 0xA8);
    TEST("TFR DP,A", 0x1F, 0xB8);

    TEST("TFR D,X",  0x1F, 0x01);
    TEST("TFR D,Y",  0x1F, 0x02);
    TEST("TFR D,U",  0x1F, 0x03);
    TEST("TFR D,S",  0x1F, 0x04);
    TEST("TFR D,PC", 0x1F, 0x05);
    TEST("TFR X,D",  0x1F, 0x10);
    TEST("TFR Y,D",  0x1F, 0x20);
    TEST("TFR U,D",  0x1F, 0x30);
    TEST("TFR S,D",  0x1F, 0x40);
    TEST("TFR PC,D", 0x1F, 0x50);

    ETEST(OPERAND_NOT_ALLOWED, "EXG A");
    ETEST(OPERAND_NOT_ALLOWED, "EXG A,");
    ETEST(ILLEGAL_SIZE,    "EXG A,X");
    ETEST(OPERAND_NOT_ALLOWED, "EXG A,X,Y");

    if (is6309()) {
        // HD6309
        TEST("ADDR A,B", 0x10, 0x30, 0x89);
        TEST("ADCR A,B", 0x10, 0x31, 0x89);
        TEST("SUBR A,B", 0x10, 0x32, 0x89);
        TEST("SBCR A,B", 0x10, 0x33, 0x89);
        TEST("ANDR A,B", 0x10, 0x34, 0x89);
        TEST("ORR  A,B", 0x10, 0x35, 0x89);
        TEST("EORR A,B", 0x10, 0x36, 0x89);
        TEST("CMPR A,B", 0x10, 0x37, 0x89);

        TEST("TFR A,E",  0x1F, 0x8E);
        TEST("TFR A,F",  0x1F, 0x8F);
        TEST("TFR E,A",  0x1F, 0xE8);
        TEST("TFR F,A",  0x1F, 0xF8);
        TEST("TFR A,Z",  0x1F, 0x8C);
        TEST("TFR Z,A",  0x1F, 0xC8);
        TEST("TFR A,0",  0x1F, 0x8C);
        TEST("TFR 0,A",  0x1F, 0xC8);
        TEST("TFR CC,0", 0x1F, 0xAC);
        TEST("TFR 0,CC", 0x1F, 0xCA);
        TEST("TFR D,W",  0x1F, 0x06);
        TEST("TFR D,V",  0x1F, 0x07);
        TEST("TFR W,D",  0x1F, 0x60);
        TEST("TFR V,D",  0x1F, 0x70);
        TEST("TFR W,PC", 0x1F, 0x65);
        TEST("TFR S,W",  0x1F, 0x46);
        TEST("TFR Z,V",  0x1F, 0xC7);
        TEST("TFR X,Z",  0x1F, 0x1C);
        TEST("TFR 0,V",  0x1F, 0xC7);
        TEST("TFR X,0",  0x1F, 0x1C);

        ETEST(OPERAND_NOT_ALLOWED, "ADDR A");
        ETEST(OPERAND_NOT_ALLOWED, "ADDR A,");
        ETEST(ILLEGAL_SIZE,        "ADDR A,X");
        ETEST(OPERAND_NOT_ALLOWED, "ADDR A,X,Y");

        ETEST(OPERAND_NOT_ALLOWED, "TFR E");
        ETEST(OPERAND_NOT_ALLOWED, "TFR E,");
        ETEST(ILLEGAL_SIZE,        "TFR E,W");
        ETEST(OPERAND_NOT_ALLOWED, "TFR E,X,F");
    } else {
        ETEST(UNKNOWN_INSTRUCTION, "ADDR A,B");
        ETEST(UNKNOWN_INSTRUCTION, "ADCR A,B");
        ETEST(UNKNOWN_INSTRUCTION, "SUBR A,B");
        ETEST(UNKNOWN_INSTRUCTION, "SBCR A,B");
        ETEST(UNKNOWN_INSTRUCTION, "ANDR A,B");
        ETEST(UNKNOWN_INSTRUCTION, "ORR  A,B");
        ETEST(UNKNOWN_INSTRUCTION, "EORR A,B");
        ETEST(UNKNOWN_INSTRUCTION, "CMPR A,B");
        ETEST(UNKNOWN_REGISTER, "TFR A,E");
        ETEST(UNKNOWN_REGISTER, "TFR A,F");
        ETEST(UNKNOWN_REGISTER, "TFR E,A");
        ETEST(UNKNOWN_REGISTER, "TFR F,A");
        ETEST(UNKNOWN_REGISTER, "TFR A,Z");
        ETEST(UNKNOWN_REGISTER, "TFR Z,A");
        ETEST(UNKNOWN_REGISTER, "TFR A,0");
        ETEST(UNKNOWN_REGISTER, "TFR 0,A");
        ETEST(UNKNOWN_REGISTER, "TFR CC,0");
        ETEST(UNKNOWN_REGISTER, "TFR 0,CC");
        ETEST(UNKNOWN_REGISTER, "TFR D,W");
        ETEST(UNKNOWN_REGISTER, "TFR D,V");
        ETEST(UNKNOWN_REGISTER, "TFR W,D");
        ETEST(UNKNOWN_REGISTER, "TFR V,D");
        ETEST(UNKNOWN_REGISTER, "TFR W,PC");
        ETEST(UNKNOWN_REGISTER, "TFR S,W");
        ETEST(UNKNOWN_REGISTER, "TFR Z,V");
        ETEST(UNKNOWN_REGISTER, "TFR X,Z");
        ETEST(UNKNOWN_REGISTER, "TFR 0,V");
        ETEST(UNKNOWN_REGISTER, "TFR X,0");
    }
}

static void test_relative() {
    ATEST(0x1000, "BRA $1002", 0x20, 0x00);
    ATEST(0x1000, "BRN $1000", 0x21, 0xFE);
    ATEST(0x1000, "BHI $1004", 0x22, 0x02);
    ATEST(0x1000, "BLS $1002", 0x23, 0x00);
    ATEST(0x1000, "BHS $1002", 0x24, 0x00);
    ATEST(0x1000, "BLO $1002", 0x25, 0x00);
    ATEST(0x1000, "BNE $1002", 0x26, 0x00);
    ATEST(0x1000, "BEQ $1002", 0x27, 0x00);
    ATEST(0x1000, "BVC $1002", 0x28, 0x00);
    ATEST(0x1000, "BVS $1002", 0x29, 0x00);
    ATEST(0x1000, "BPL $1002", 0x2A, 0x00);
    ATEST(0x1000, "BMI $1002", 0x2B, 0x00);
    ATEST(0x1000, "BGE $1002", 0x2C, 0x00);
    ATEST(0x1000, "BLT $1002", 0x2D, 0x00);
    ATEST(0x1000, "BGT $1002", 0x2E, 0x00);
    ATEST(0x1000, "BLE $1002", 0x2F, 0x00);

    ATEST(0x1000, "LBRA $1003", 0x16, 0x00, 0x00);
    ATEST(0x1000, "LBRN $1000", 0x10, 0x21, 0xFF, 0xFC);
    ATEST(0x1000, "LBHI $1008", 0x10, 0x22, 0x00, 0x04);
    ATEST(0x1000, "LBLS $1004", 0x10, 0x23, 0x00, 0x00);
    ATEST(0x1000, "LBHS $1004", 0x10, 0x24, 0x00, 0x00);
    ATEST(0x1000, "LBLO $1004", 0x10, 0x25, 0x00, 0x00);
    ATEST(0x1000, "LBNE $1004", 0x10, 0x26, 0x00, 0x00);
    ATEST(0x1000, "LBEQ $1004", 0x10, 0x27, 0x00, 0x00);
    ATEST(0x1000, "LBVC $1004", 0x10, 0x28, 0x00, 0x00);
    ATEST(0x1000, "LBVS $1004", 0x10, 0x29, 0x00, 0x00);
    ATEST(0x1000, "LBPL $1004", 0x10, 0x2A, 0x00, 0x00);
    ATEST(0x1000, "LBMI $1004", 0x10, 0x2B, 0x00, 0x00);
    ATEST(0x1000, "LBGE $1004", 0x10, 0x2C, 0x00, 0x00);
    ATEST(0x1000, "LBLT $1004", 0x10, 0x2D, 0x00, 0x00);
    ATEST(0x1000, "LBGT $1004", 0x10, 0x2E, 0x00, 0x00);
    ATEST(0x1000, "LBLE $1004", 0x10, 0x2F, 0x00, 0x00);

    ATEST(0x1000, "BSR  $1042", 0x8D, 0x40);
    ATEST(0x1000, "LBSR $1043", 0x17, 0x00, 0x40);

    symtab.intern(0x0F82, "sub0F82");
    symtab.intern(0x1081, "sub1081");
    symtab.intern(0x9002, "sub9002");
    symtab.intern(0x9003, "sub9003");

    ATEST(0x1000, "BSR  sub1081", 0x8D, 0x7F);
    ATEST(0x1000, "BSR  sub0F82", 0x8D, 0x80);
    ATEST(0x1000, "LBSR sub9002", 0x17, 0x7F, 0xFF);
    ATEST(0x1000, "LBSR sub9003", 0x17, 0x80, 0x00);
}

static void test_immediate() {
    TEST("ORCC  #%10", 0x1A, 0x02);
    TEST("ANDCC #~1",  0x1C, 0xFE);
    TEST("CWAI  #$EF", 0x3C, 0xEF);

    TEST("SUBA #$90", 0x80, 0x90);
    TEST("CMPA #-1",  0x81, 0xFF);
    TEST("SBCA #+$00", 0x82, 0x00);
    TEST("ANDA #@177", 0x84, 0x7F);
    TEST("BITA #$90", 0x85, 0x90);
    TEST("LDA  #$90", 0x86, 0x90);
    TEST("EORA #$90", 0x88, 0x90);
    TEST("ADCA #$90", 0x89, 0x90);
    TEST("ORA  #$90", 0x8A, 0x90);
    TEST("ADDA #$90", 0x8B, 0x90);

    TEST("SUBB #$90", 0xC0, 0x90);
    TEST("CMPB #$90", 0xC1, 0x90);
    TEST("SBCB #$90", 0xC2, 0x90);
    TEST("ANDB #$90", 0xC4, 0x90);
    TEST("BITB #$90", 0xC5, 0x90);
    TEST("LDB  #$90", 0xC6, 0x90);
    TEST("EORB #$90", 0xC8, 0x90);
    TEST("ADCB #$90", 0xC9, 0x90);
    TEST("ORB  #$90", 0xCA, 0x90);
    TEST("ADDB #$90", 0xCB, 0x90);

    TEST("SUBD #$90A0", 0x83, 0x90, 0xA0);
    TEST("ADDD #$90A0", 0xC3, 0x90, 0xA0);
    TEST("LDD  #$90A0", 0xCC, 0x90, 0xA0);
    TEST("CMPD #$90A0", 0x10, 0x83, 0x90, 0xA0);

    TEST("CMPX #$90A0", 0x8C, 0x90, 0xA0);
    TEST("LDX  #$90A0", 0x8E, 0x90, 0xA0);

    TEST("CMPY #$90A0", 0x10, 0x8C, 0x90, 0xA0);
    TEST("LDY  #$90A0", 0x10, 0x8E, 0x90, 0xA0);

    TEST("CMPU #$90A0", 0x11, 0x83, 0x90, 0xA0);
    TEST("LDU  #$90A0", 0xCE, 0x90, 0xA0);

    TEST("CMPS #$90A0", 0x11, 0x8C, 0x90, 0xA0);
    TEST("LDS  #$90A0", 0x10, 0xCE, 0x90, 0xA0);

    if (is6309()) {
        // HD6309
        TEST("LDMD  #$01", 0x11, 0x3D, 0x01);
        TEST("BITMD #$80", 0x11, 0x3C, 0x80);

        TEST("SBCD #$90A0", 0x10, 0x82, 0x90, 0xA0);
        TEST("ANDD #$90A0", 0x10, 0x84, 0x90, 0xA0);
        TEST("BITD #$90A0", 0x10, 0x85, 0x90, 0xA0);
        TEST("EORD #$90A0", 0x10, 0x88, 0x90, 0xA0);
        TEST("ADCD #$90A0", 0x10, 0x89, 0x90, 0xA0);
        TEST("ORD  #$90A0", 0x10, 0x8A, 0x90, 0xA0);

        TEST("SUBE #$90", 0x11, 0x80, 0x90);
        TEST("ADDE #$90", 0x11, 0x8B, 0x90);
        TEST("LDE  #$90", 0x11, 0x86, 0x90);
        TEST("CMPE #$90", 0x11, 0x81, 0x90);

        TEST("SUBF #$90", 0x11, 0xC0, 0x90);
        TEST("ADDF #$90", 0x11, 0xCB, 0x90);
        TEST("LDF  #$90", 0x11, 0xC6, 0x90);
        TEST("CMPF #$90", 0x11, 0xC1, 0x90);

        TEST("SUBW #$90A0", 0x10, 0x80, 0x90, 0xA0);
        TEST("ADDW #$90A0", 0x10, 0x8B, 0x90, 0xA0);
        TEST("LDW  #$90A0", 0x10, 0x86, 0x90, 0xA0);
        TEST("CMPW #$90A0", 0x10, 0x81, 0x90, 0xA0);

        TEST("LDQ  #$12345678", 0xCD, 0x12, 0x34, 0x56, 0x78);

        TEST("MULD #$90A0", 0x11, 0x8F, 0x90, 0xA0);
        TEST("DIVD #$90A0", 0x11, 0x8D, 0x90, 0xA0);
        TEST("DIVQ #$90A0", 0x11, 0x8E, 0x90, 0xA0);
    } else {
        ETEST(UNKNOWN_INSTRUCTION, "LDMD  #$01");
        ETEST(UNKNOWN_INSTRUCTION, "BITMD #$80");
        ETEST(UNKNOWN_INSTRUCTION, "SBCD #$90A0");
        ETEST(UNKNOWN_INSTRUCTION, "ANDD #$90A0");
        ETEST(UNKNOWN_INSTRUCTION, "BITD #$90A0");
        ETEST(UNKNOWN_INSTRUCTION, "EORD #$90A0");
        ETEST(UNKNOWN_INSTRUCTION, "ADCD #$90A0");
        ETEST(UNKNOWN_INSTRUCTION, "ORD  #$90A0");
        ETEST(UNKNOWN_INSTRUCTION, "SUBE #$90");
        ETEST(UNKNOWN_INSTRUCTION, "ADDE #$90");
        ETEST(UNKNOWN_INSTRUCTION, "LDE  #$90");
        ETEST(UNKNOWN_INSTRUCTION, "CMPE #$90");
        ETEST(UNKNOWN_INSTRUCTION, "SUBF #$90");
        ETEST(UNKNOWN_INSTRUCTION, "ADDF #$90");
        ETEST(UNKNOWN_INSTRUCTION, "LDF  #$90");
        ETEST(UNKNOWN_INSTRUCTION, "CMPF #$90");
        ETEST(UNKNOWN_INSTRUCTION, "SUBW #$90A0");
        ETEST(UNKNOWN_INSTRUCTION, "ADDW #$90A0");
        ETEST(UNKNOWN_INSTRUCTION, "LDW  #$90A0");
        ETEST(UNKNOWN_INSTRUCTION, "CMPW #$90A0");
        ETEST(UNKNOWN_INSTRUCTION, "LDQ  #$12345678");
        ETEST(UNKNOWN_INSTRUCTION, "MULD #$90A0");
        ETEST(UNKNOWN_INSTRUCTION, "DIVD #$90A0");
    }

    symtab.intern(0x90, "dir90");
    symtab.intern(0x90A0, "sym90A0");
    symtab.intern(-2, "minus2");

    TEST("LDA  #dir90",   0x86, 0x90);
    TEST("CMPX #sym90A0", 0x8C, 0x90, 0xA0);
    TEST("LDY  #sym90A0", 0x10, 0x8E, 0x90, 0xA0);
    TEST("LDS  #sym90A0", 0x10, 0xCE, 0x90, 0xA0);
    TEST("LDA  # dir90",  0x86, 0x90);

    if (is6309()) {
        // HD6309
        TEST("SBCD #sym90A0", 0x10, 0x82, 0x90, 0xA0);
        TEST("LDE  #dir90",   0x11, 0x86, 0x90);

        TEST("LDQ  #sym90A0", 0xCD, 0x00, 0x00, 0x90, 0xA0);
        TEST("LDQ  #minus2",  0xCD, 0xFF, 0xFF, 0xFF, 0xFE);
        TEST("MULD #dir90",   0x11, 0x8F, 0x00, 0x90);
    }
}

static void test_direct() {
    TEST("NEG $10", 0x00, 0x10);
    TEST("COM $10", 0x03, 0x10);
    TEST("LSR $10", 0x04, 0x10);
    TEST("ROR $10", 0x06, 0x10);
    TEST("ASR $10", 0x07, 0x10);
    TEST("ASL $10", 0x08, 0x10);
    TEST("ROL $10", 0x09, 0x10);
    TEST("DEC $10", 0x0A, 0x10);
    TEST("INC $10", 0x0C, 0x10);
    TEST("TST $10", 0x0D, 0x10);
    TEST("CLR $10", 0x0F, 0x10);

    TEST("SUBA $90", 0x90, 0x90);
    TEST("CMPA $90", 0x91, 0x90);
    TEST("SBCA $90", 0x92, 0x90);
    TEST("ANDA $90", 0x94, 0x90);
    TEST("BITA $90", 0x95, 0x90);
    TEST("LDA  $90", 0x96, 0x90);
    TEST("STA  $90", 0x97, 0x90);
    TEST("EORA $90", 0x98, 0x90);
    TEST("ADCA $90", 0x99, 0x90);
    TEST("ORA  $90", 0x9A, 0x90);
    TEST("ADDA $90", 0x9B, 0x90);

    TEST("SUBB <$1290", 0xD0, 0x90);
    TEST("CMPB <$1290", 0xD1, 0x90);
    TEST("SBCB <$1290", 0xD2, 0x90);
    TEST("ANDB <$1290", 0xD4, 0x90);
    TEST("BITB <$1290", 0xD5, 0x90);
    TEST("LDB  <$1290", 0xD6, 0x90);
    TEST("STB  <$1290", 0xD7, 0x90);
    TEST("EORB <$1290", 0xD8, 0x90);
    TEST("ADCB <$1290", 0xD9, 0x90);
    TEST("ORB  <$1290", 0xDA, 0x90);
    TEST("ADDB <$1290", 0xDB, 0x90);

    TEST("SUBD $90",    0x93, 0x90);
    TEST("ADDD $90",    0xD3, 0x90);
    TEST("LDD  $90",    0xDC, 0x90);
    TEST("STD  <$1290", 0xDD, 0x90);
    TEST("CMPD <$1290", 0x10, 0x93, 0x90);

    TEST("CMPX $90",    0x9C, 0x90);
    TEST("LDX  $90",    0x9E, 0x90);
    TEST("STX  <$1290", 0x9F, 0x90);

    TEST("CMPY $90",    0x10, 0x9C, 0x90);
    TEST("LDY  $90",    0x10, 0x9E, 0x90);
    TEST("STY  <$1290", 0x10, 0x9F, 0x90);

    TEST("CMPU $90",    0x11, 0x93, 0x90);
    TEST("LDU  $90",    0xDE, 0x90);
    TEST("STU  <$1290", 0xDF, 0x90);

    TEST("CMPS $90",    0x11, 0x9C, 0x90);
    TEST("LDS  $90",    0x10, 0xDE, 0x90);
    TEST("STS  <$1290", 0x10, 0xDF, 0x90);

    TEST("JMP $10",    0x0E, 0x10);
    TEST("JSR <$1290", 0x9D, 0x90);

    if (is6309()) {
        // HD6309
        TEST("SBCD $90",    0x10, 0x92, 0x90);
        TEST("ANDD $90",    0x10, 0x94, 0x90);
        TEST("BITD $90",    0x10, 0x95, 0x90);
        TEST("EORD <$1290", 0x10, 0x98, 0x90);
        TEST("ADCD <$1290", 0x10, 0x99, 0x90);
        TEST("ORD  <$1290", 0x10, 0x9A, 0x90);

        TEST("SUBE $90", 0x11, 0x90, 0x90);
        TEST("ADDE $90", 0x11, 0x9B, 0x90);
        TEST("LDE  $90", 0x11, 0x96, 0x90);
        TEST("STE  $90", 0x11, 0x97, 0x90);
        TEST("CMPE $90", 0x11, 0x91, 0x90);

        TEST("SUBF <$1290", 0x11, 0xD0, 0x90);
        TEST("ADDF <$1290", 0x11, 0xDB, 0x90);
        TEST("LDF  <$1290", 0x11, 0xD6, 0x90);
        TEST("STF  <$1290", 0x11, 0xD7, 0x90);
        TEST("CMPF <$1290", 0x11, 0xD1, 0x90);

        TEST("SUBW $90",    0x10, 0x90, 0x90);
        TEST("ADDW $90",    0x10, 0x9B, 0x90);
        TEST("LDW  <$1290", 0x10, 0x96, 0x90);
        TEST("STW  <$1290", 0x10, 0x97, 0x90);
        TEST("CMPW <$1290", 0x10, 0x91, 0x90);

        TEST("LDQ  $90",    0x10, 0xDC, 0x90);
        TEST("STQ  <$1290", 0x10, 0xDD, 0x90);

        TEST("MULD <$1290", 0x11, 0x9F, 0x90);
        TEST("DIVD $90",    0x11, 0x9D, 0x90);
        TEST("DIVQ <$1290", 0x11, 0x9E, 0x90);

        TEST("OIM #$30,$10",    0x01, 0x30, 0x10);
        TEST("AIM #$30,$10",    0x02, 0x30, 0x10);
        TEST("EIM #$30,<$1290", 0x05, 0x30, 0x90);
        TEST("TIM #$30,<$1290", 0x0B, 0x30, 0x90);

        TEST("OIM #$30,,W",     0x61, 0x30, 0x8F);
        TEST("AIM #$30,W,X",    0x62, 0x30, 0x8E);
        TEST("AIM #$30,[W,X]",  0x62, 0x30, 0x9E);
        TEST("EIM #$30,,--X",   0x65, 0x30, 0x83);
        TEST("TIM #$30,*+$20,PCR",    0x6B, 0x30, 0x8C, 0x1C);
        TEST("TIM #$30,*+$120,PCR",   0x6B, 0x30, 0x8D, 0x01, 0x1B);
        TEST("TIM #$30,[*+$20,PCR]",  0x6B, 0x30, 0x9C, 0x1C);
        TEST("TIM #$30,[*+$120,PCR]", 0x6B, 0x30, 0x9D, 0x01, 0x1B);
    } else {
        ETEST(UNKNOWN_INSTRUCTION, "SBCD $90");
        ETEST(UNKNOWN_INSTRUCTION, "ANDD $90");
        ETEST(UNKNOWN_INSTRUCTION, "BITD $90");
        ETEST(UNKNOWN_INSTRUCTION, "EORD <$1290");
        ETEST(UNKNOWN_INSTRUCTION, "ADCD <$1290");
        ETEST(UNKNOWN_INSTRUCTION, "ORD  <$1290");
        ETEST(UNKNOWN_INSTRUCTION, "SUBE $90");
        ETEST(UNKNOWN_INSTRUCTION, "ADDE $90");
        ETEST(UNKNOWN_INSTRUCTION, "LDE  $90");
        ETEST(UNKNOWN_INSTRUCTION, "STE  $90");
        ETEST(UNKNOWN_INSTRUCTION, "CMPE $90");
        ETEST(UNKNOWN_INSTRUCTION, "SUBF <$1290");
        ETEST(UNKNOWN_INSTRUCTION, "ADDF <$1290");
        ETEST(UNKNOWN_INSTRUCTION, "LDF  <$1290");
        ETEST(UNKNOWN_INSTRUCTION, "STF  <$1290");
        ETEST(UNKNOWN_INSTRUCTION, "CMPF <$1290");
        ETEST(UNKNOWN_INSTRUCTION, "SUBW $90");
        ETEST(UNKNOWN_INSTRUCTION, "ADDW $90");
        ETEST(UNKNOWN_INSTRUCTION, "LDW  <$1290");
        ETEST(UNKNOWN_INSTRUCTION, "STW  <$1290");
        ETEST(UNKNOWN_INSTRUCTION, "CMPW <$1290");
        ETEST(UNKNOWN_INSTRUCTION, "LDQ  $90");
        ETEST(UNKNOWN_INSTRUCTION, "STQ  <$1290");
        ETEST(UNKNOWN_INSTRUCTION, "MULD <$1290");
        ETEST(UNKNOWN_INSTRUCTION, "DIVD $90");
        ETEST(UNKNOWN_INSTRUCTION, "OIM #$30,$10");
        ETEST(UNKNOWN_INSTRUCTION, "AIM #$30,$10");
        ETEST(UNKNOWN_INSTRUCTION, "EIM #$30,<$1290");
        ETEST(UNKNOWN_INSTRUCTION, "TIM #$30,<$1290");
    }

    symtab.intern(0x10, "dir10");
    symtab.intern(0x90, "dir90");
    symtab.intern(0x1290, "sym1290");
    symtab.intern(0x90A0, "sym90A0");

    TEST("NEG  dir10",    0x00, 0x10);
    TEST("LDA  dir10",    0x96, 0x10);
    TEST("STB  <sym1290", 0xD7, 0x90);
    TEST("CMPX <sym1290", 0x9C, 0x90);
    TEST("STU  <dir10",   0xDF, 0x10);
    TEST("LDY  <dir10",   0x10, 0x9E, 0x10);
    TEST("LDS  <sym1290", 0x10, 0xDE, 0x90);
    TEST("JMP  <sym1290", 0x0E, 0x90);
    TEST("JSR  dir90",    0x9D, 0x90);

    if (is6309()) {
        // HD6309
        TEST("SBCD <sym90A0", 0x10, 0x92, 0xA0);
        TEST("LDE  dir90",    0x11, 0x96, 0x90);

        TEST("LDQ  <sym1290", 0x10, 0xDC, 0x90);
        TEST("MULD dir90",    0x11, 0x9F, 0x90);

        TEST("OIM #$30,dir10",    0x01, 0x30, 0x10);
        TEST("OIM #$30,<sym1290", 0x01, 0x30, 0x90);

        TEST("SETDP sym1290>>8");
        TEST("NEG dir10",    0x70, 0x00, 0x10);
        TEST("LDA dir10",    0xB6, 0x00, 0x10);
        TEST("LDA <dir10",   0x96, 0x10);
        TEST("NEG sym1290",  0x00, 0x90);
        TEST("LDA sym1290",  0x96, 0x90);
        TEST("LDA >sym1290", 0xB6, 0x12, 0x90);

        // HD6309
        TEST("LDE dir90",        0x11, 0xB6, 0x00, 0x90);
        TEST("LDQ sym1290",      0x10, 0xDC, 0x90);
        TEST("OIM #$30,sym1290", 0x01, 0x30, 0x90);

        TEST("SETDP sym90A0>>8");
        TEST("STU dir90",    0xFF, 0x00, 0x90);
        TEST("LDY dir90",    0x10, 0xBE, 0x00, 0x90);
        TEST("LDY <dir90",   0x10, 0x9E, 0x90);
        TEST("LDS sym90A0",  0x10, 0xDE, 0xA0);
        TEST("JMP sym90A0",  0x0E, 0xA0);
        TEST("JMP >sym90A0", 0x7E, 0x90, 0xA0);

        TEST("SBCD sym90A0",     0x10, 0x92, 0xA0);
        TEST("MULD dir90",       0x11, 0xBF, 0x00, 0x90);
        TEST("OIM  #$30,<dir10", 0x01, 0x30, 0x10);
    }
}

static void test_extended() {
    TEST("NEG $1234", 0x70, 0x12, 0x34);
    TEST("COM $1234", 0x73, 0x12, 0x34);
    TEST("LSR $1234", 0x74, 0x12, 0x34);
    TEST("ROR $1234", 0x76, 0x12, 0x34);
    TEST("ASR $1234", 0x77, 0x12, 0x34);
    TEST("ASL $1234", 0x78, 0x12, 0x34);
    TEST("ROL $1234", 0x79, 0x12, 0x34);
    TEST("DEC $1234", 0x7A, 0x12, 0x34);
    TEST("INC $1234", 0x7C, 0x12, 0x34);
    TEST("TST $1234", 0x7D, 0x12, 0x34);
    TEST("CLR $1234", 0x7F, 0x12, 0x34);

    TEST("SUBA >$90", 0xB0, 0x00, 0x90);
    TEST("CMPA >$90", 0xB1, 0x00, 0x90);
    TEST("SBCA >$90", 0xB2, 0x00, 0x90);
    TEST("ANDA >$90", 0xB4, 0x00, 0x90);
    TEST("BITA >$90", 0xB5, 0x00, 0x90);
    TEST("LDA  >$90", 0xB6, 0x00, 0x90);
    TEST("STA  >$90", 0xB7, 0x00, 0x90);
    TEST("EORA >$90", 0xB8, 0x00, 0x90);
    TEST("ADCA >$90", 0xB9, 0x00, 0x90);
    TEST("ORA  >$90", 0xBA, 0x00, 0x90);
    TEST("ADDA >$90", 0xBB, 0x00, 0x90);

    TEST("SUBB $9ABC", 0xF0, 0x9A, 0xBC);
    TEST("CMPB $9ABC", 0xF1, 0x9A, 0xBC);
    TEST("SBCB $9ABC", 0xF2, 0x9A, 0xBC);
    TEST("ANDB $9ABC", 0xF4, 0x9A, 0xBC);
    TEST("BITB $9ABC", 0xF5, 0x9A, 0xBC);
    TEST("LDB  $9ABC", 0xF6, 0x9A, 0xBC);
    TEST("STB  $9ABC", 0xF7, 0x9A, 0xBC);
    TEST("EORB $9ABC", 0xF8, 0x9A, 0xBC);
    TEST("ADCB $9ABC", 0xF9, 0x9A, 0xBC);
    TEST("ORB  $9ABC", 0xFA, 0x9A, 0xBC);
    TEST("ADDB $9ABC", 0xFB, 0x9A, 0xBC);

    TEST("SUBD $9ABC", 0xB3, 0x9A, 0xBC);
    TEST("ADDD $9ABC", 0xF3, 0x9A, 0xBC);
    TEST("LDD  $9ABC", 0xFC, 0x9A, 0xBC);
    TEST("STD  $9ABC", 0xFD, 0x9A, 0xBC);
    TEST("CMPD >$90",  0x10, 0xB3, 0x00, 0x90);

    TEST("CMPX $9ABC", 0xBC, 0x9A, 0xBC);
    TEST("LDX  >$90",  0xBE, 0x00, 0x90);
    TEST("STX  $9ABC", 0xBF, 0x9A, 0xBC);

    TEST("CMPY $9ABC", 0x10, 0xBC, 0x9A, 0xBC);
    TEST("LDY  >$90",  0x10, 0xBE, 0x00, 0x90);
    TEST("STY  $9ABC", 0x10, 0xBF, 0x9A, 0xBC);

    TEST("CMPU $9ABC", 0x11, 0xB3, 0x9A, 0xBC);
    TEST("LDU  >$90",  0xFE, 0x00, 0x90);
    TEST("STU  $9ABC", 0xFF, 0x9A, 0xBC);

    TEST("CMPS $9ABC", 0x11, 0xBC, 0x9A, 0xBC);
    TEST("LDS  >$90",  0x10, 0xFE, 0x00, 0x90);
    TEST("STS  $9ABC", 0x10, 0xFF, 0x9A, 0xBC);

    TEST("JMP $1234", 0x7E, 0x12, 0x34);
    TEST("JSR >$90",  0xBD, 0x00, 0x90);

    if (is6309()) {
        // HD6309
        TEST("SBCD $9ABC", 0x10, 0xB2, 0x9A, 0xBC);
        TEST("ANDD $9ABC", 0x10, 0xB4, 0x9A, 0xBC);
        TEST("BITD $9ABC", 0x10, 0xB5, 0x9A, 0xBC);
        TEST("EORD $9ABC", 0x10, 0xB8, 0x9A, 0xBC);
        TEST("ORD  >$90",  0x10, 0xBA, 0x00, 0x90);
        TEST("ADCD $9ABC", 0x10, 0xB9, 0x9A, 0xBC);

        TEST("SUBE $9ABC", 0x11, 0xB0, 0x9A, 0xBC);
        TEST("ADDE $9ABC", 0x11, 0xBB, 0x9A, 0xBC);
        TEST("LDE  >$90",  0x11, 0xB6, 0x00, 0x90);
        TEST("STE  $9ABC", 0x11, 0xB7, 0x9A, 0xBC);
        TEST("CMPE $9ABC", 0x11, 0xB1, 0x9A, 0xBC);

        TEST("SUBF $9ABC", 0x11, 0xF0, 0x9A, 0xBC);
        TEST("ADDF $9ABC", 0x11, 0xFB, 0x9A, 0xBC);
        TEST("LDF  >$90",  0x11, 0xF6, 0x00, 0x90);
        TEST("STF  $9ABC", 0x11, 0xF7, 0x9A, 0xBC);
        TEST("CMPF $9ABC", 0x11, 0xF1, 0x9A, 0xBC);

        TEST("SUBW $9ABC", 0x10, 0xB0, 0x9A, 0xBC);
        TEST("ADDW $9ABC", 0x10, 0xBB, 0x9A, 0xBC);
        TEST("LDW  >$90",  0x10, 0xB6, 0x00, 0x90);
        TEST("STW  $9ABC", 0x10, 0xB7, 0x9A, 0xBC);
        TEST("CMPW $9ABC", 0x10, 0xB1, 0x9A, 0xBC);

        TEST("LDQ  >$90",  0x10, 0xFC, 0x00, 0x90);
        TEST("STQ  $1290", 0x10, 0xFD, 0x12, 0x90);

        TEST("MULD $1290", 0x11, 0xBF, 0x12, 0x90);
        TEST("DIVD >$90",  0x11, 0xBD, 0x00, 0x90);
        TEST("DIVQ $1290", 0x11, 0xBE, 0x12, 0x90);

        TEST("OIM #$30,>$10",  0x71, 0x30, 0x00, 0x10);
        TEST("AIM #$30,>$10",  0x72, 0x30, 0x00, 0x10);
        TEST("EIM #$30,$1290", 0x75, 0x30, 0x12, 0x90);
        TEST("TIM #$30,$1290", 0x7B, 0x30, 0x12, 0x90);
    } else {
        ETEST(UNKNOWN_INSTRUCTION, "SBCD $9ABC");
        ETEST(UNKNOWN_INSTRUCTION, "ANDD $9ABC");
        ETEST(UNKNOWN_INSTRUCTION, "BITD $9ABC");
        ETEST(UNKNOWN_INSTRUCTION, "EORD $9ABC");
        ETEST(UNKNOWN_INSTRUCTION, "ORD  >$90");
        ETEST(UNKNOWN_INSTRUCTION, "ADCD $9ABC");
        ETEST(UNKNOWN_INSTRUCTION, "SUBE $9ABC");
        ETEST(UNKNOWN_INSTRUCTION, "ADDE $9ABC");
        ETEST(UNKNOWN_INSTRUCTION, "LDE  >$90");
        ETEST(UNKNOWN_INSTRUCTION, "STE  $9ABC");
        ETEST(UNKNOWN_INSTRUCTION, "CMPE $9ABC");
        ETEST(UNKNOWN_INSTRUCTION, "SUBF $9ABC");
        ETEST(UNKNOWN_INSTRUCTION, "ADDF $9ABC");
        ETEST(UNKNOWN_INSTRUCTION, "LDF  >$90");
        ETEST(UNKNOWN_INSTRUCTION, "STF  $9ABC");
        ETEST(UNKNOWN_INSTRUCTION, "CMPF $9ABC");
        ETEST(UNKNOWN_INSTRUCTION, "SUBW $9ABC");
        ETEST(UNKNOWN_INSTRUCTION, "ADDW $9ABC");
        ETEST(UNKNOWN_INSTRUCTION, "LDW  >$90");
        ETEST(UNKNOWN_INSTRUCTION, "STW  $9ABC");
        ETEST(UNKNOWN_INSTRUCTION, "CMPW $9ABC");
        ETEST(UNKNOWN_INSTRUCTION, "LDQ  >$90");
        ETEST(UNKNOWN_INSTRUCTION, "STQ  $1290");
        ETEST(UNKNOWN_INSTRUCTION, "MULD $1290");
        ETEST(UNKNOWN_INSTRUCTION, "DIVD >$90");
        ETEST(UNKNOWN_INSTRUCTION, "OIM #$30,>$10");
        ETEST(UNKNOWN_INSTRUCTION, "AIM #$30,>$10");
        ETEST(UNKNOWN_INSTRUCTION, "EIM #$30,$1290");
        ETEST(UNKNOWN_INSTRUCTION, "TIM #$30,$1290");
    }

    symtab.intern(0x0090, "dir90");
    symtab.intern(0x1290, "sym1290");

    TEST("NEG >dir90",   0x70, 0x00, 0x90);
    TEST("LDA  sym1290", 0xB6, 0x12, 0x90);
    TEST("STB >dir90",   0xF7, 0x00, 0x90);
    TEST("CMPX sym1290", 0xBC, 0x12, 0x90);
    TEST("STU >dir90",   0xFF, 0x00, 0x90);
    TEST("LDY  sym1290", 0x10, 0xBE, 0x12, 0x90);
    TEST("LDS >dir90",   0x10, 0xFE, 0x00, 0x90);
    TEST("JMP  sym1290", 0x7E, 0x12, 0x90);
    TEST("JSR >dir90",   0xBD, 0x00, 0x90);

    if (is6309()) {
        // HD6309
        TEST("SBCD sym1290", 0x10, 0xB2, 0x12, 0x90);
        TEST("LDE  >dir90",  0x11, 0xB6, 0x00, 0x90);

        TEST("LDQ  sym1290",  0x10, 0xFC, 0x12, 0x90);
        TEST("MULD >dir90",   0x11, 0xBF, 0x00, 0x90);

        TEST("OIM #$30,>dir90",  0x71, 0x30, 0x00, 0x90);
        TEST("OIM #$30,sym1290", 0x71, 0x30, 0x12, 0x90);
    }
}

static void test_indexed() {
    TEST("LEAX ,Y", 0x30, 0xA4);
    TEST("LEAY ,U", 0x31, 0xC4);
    TEST("LEAU ,S", 0x33, 0xE4);
    TEST("LEAS ,X", 0x32, 0x84);

    TEST("NEG  ,X", 0x60, 0x84);
    TEST("COM  ,X", 0x63, 0x84);
    TEST("LSR  ,X", 0x64, 0x84);
    TEST("ROR  ,X", 0x66, 0x84);
    TEST("ASR  ,X", 0x67, 0x84);
    TEST("ASL  ,X", 0x68, 0x84);
    TEST("ROL  ,X", 0x69, 0x84);
    TEST("DEC  ,X", 0x6A, 0x84);
    TEST("INC  ,X", 0x6C, 0x84);
    TEST("TST  ,X", 0x6D, 0x84);
    TEST("CLR  ,X", 0x6F, 0x84);

    TEST("SUBA ,Y", 0xA0, 0xA4);
    TEST("CMPA ,Y", 0xA1, 0xA4);
    TEST("SBCA ,Y", 0xA2, 0xA4);
    TEST("ANDA ,Y", 0xA4, 0xA4);
    TEST("BITA ,Y", 0xA5, 0xA4);
    TEST("LDA  ,Y", 0xA6, 0xA4);
    TEST("STA  ,Y", 0xA7, 0xA4);
    TEST("EORA ,Y", 0xA8, 0xA4);
    TEST("ADCA ,Y", 0xA9, 0xA4);
    TEST("ORA  ,Y", 0xAA, 0xA4);
    TEST("ADDA ,Y", 0xAB, 0xA4);

    TEST("SUBB ,U", 0xE0, 0xC4);
    TEST("CMPB ,U", 0xE1, 0xC4);
    TEST("SBCB ,U", 0xE2, 0xC4);
    TEST("ANDB ,U", 0xE4, 0xC4);
    TEST("BITB ,U", 0xE5, 0xC4);
    TEST("LDB  ,U", 0xE6, 0xC4);
    TEST("STB  ,U", 0xE7, 0xC4);
    TEST("EORB ,U", 0xE8, 0xC4);
    TEST("ADCB ,U", 0xE9, 0xC4);
    TEST("ORB  ,U", 0xEA, 0xC4);
    TEST("ADDB ,U", 0xEB, 0xC4);

    TEST("SUBD ,S", 0xA3, 0xE4);
    TEST("ADDD ,S", 0xE3, 0xE4);
    TEST("LDD  ,S", 0xEC, 0xE4);
    TEST("STD  ,S", 0xED, 0xE4);
    TEST("CMPD ,S", 0x10, 0xA3, 0xE4);

    TEST("CMPX 2,Y", 0xAC, 0x22);
    TEST("LDX  2,Y", 0xAE, 0x22);
    TEST("STX  2,Y", 0xAF, 0x22);

    TEST("CMPY -2,X", 0x10, 0xAC, 0x1E);
    TEST("LDY  -2,X", 0x10, 0xAE, 0x1E);
    TEST("STY  -2,X", 0x10, 0xAF, 0x1E);

    TEST("CMPU ,X++", 0x11, 0xA3, 0x81);
    TEST("LDU  ,X++", 0xEE, 0x81);
    TEST("STU  ,X++", 0xEF, 0x81);

    TEST("CMPS ,--Y", 0x11, 0xAC, 0xA3);
    TEST("LDS  ,--Y", 0x10, 0xEE, 0xA3);
    TEST("STS  ,--Y", 0x10, 0xEF, 0xA3);

    TEST("JMP  [,X]",   0x6E, 0x94);
    TEST("JSR  [,X++]", 0xAD, 0x91);

    if (is6309()) {
        // HD6309
        TEST("SBCD ,X",   0x10, 0xA2, 0x84);
        TEST("ANDD ,X++", 0x10, 0xA4, 0x81);
        TEST("BITD ,--X", 0x10, 0xA5, 0x83);
        TEST("EORD [,X]", 0x10, 0xA8, 0x94);
        TEST("ADCD [,X++]", 0x10, 0xA9, 0x91);
        TEST("ORD  [,--X]", 0x10, 0xAA, 0x93);

        TEST("SUBE ,X", 0x11, 0xA0, 0x84);
        TEST("ADDE ,X", 0x11, 0xAB, 0x84);
        TEST("LDE  ,X", 0x11, 0xA6, 0x84);
        TEST("STE  ,X", 0x11, 0xA7, 0x84);
        TEST("CMPE ,X", 0x11, 0xA1, 0x84);

        TEST("SUBF ,X", 0x11, 0xE0, 0x84);
        TEST("ADDF ,X", 0x11, 0xEB, 0x84);
        TEST("LDF  ,X", 0x11, 0xE6, 0x84);
        TEST("STF  ,X", 0x11, 0xE7, 0x84);
        TEST("CMPF ,X", 0x11, 0xE1, 0x84);

        TEST("SUBW ,X", 0x10, 0xA0, 0x84);
        TEST("CMPW ,X", 0x10, 0xA1, 0x84);
        TEST("LDW  ,X", 0x10, 0xA6, 0x84);
        TEST("STW  ,X", 0x10, 0xA7, 0x84);
        TEST("ADDW ,X", 0x10, 0xAB, 0x84);

        TEST("LDQ  ,X", 0x10, 0xEC, 0x84);
        TEST("STQ  ,X", 0x10, 0xED, 0x84);

        TEST("MULD ,X",   0x11, 0xAF, 0x84);
        TEST("DIVD -1,X", 0x11, 0xAD, 0x1F);
        TEST("DIVQ 1,X",  0x11, 0xAE, 0x01);

        TEST("OIM #$30,,X",     0x61, 0x30, 0x84);
        TEST("AIM #$30,,X+",    0x62, 0x30, 0x80);
        TEST("EIM #$30,[,--X]", 0x65, 0x30, 0x93);
        TEST("TIM #$30,-2,X",   0x6B, 0x30, 0x1E);
    } else {
        ETEST(UNKNOWN_INSTRUCTION, "SBCD ,X");
        ETEST(UNKNOWN_INSTRUCTION, "ANDD ,X++");
        ETEST(UNKNOWN_INSTRUCTION, "BITD ,--X");
        ETEST(UNKNOWN_INSTRUCTION, "EORD [,X]");
        ETEST(UNKNOWN_INSTRUCTION, "ADCD [,X++]");
        ETEST(UNKNOWN_INSTRUCTION, "ORD  [,--X]");
        ETEST(UNKNOWN_INSTRUCTION, "SUBE ,X");
        ETEST(UNKNOWN_INSTRUCTION, "ADDE ,X");
        ETEST(UNKNOWN_INSTRUCTION, "LDE  ,X");
        ETEST(UNKNOWN_INSTRUCTION, "STE  ,X");
        ETEST(UNKNOWN_INSTRUCTION, "CMPE ,X");
        ETEST(UNKNOWN_INSTRUCTION, "SUBF ,X");
        ETEST(UNKNOWN_INSTRUCTION, "ADDF ,X");
        ETEST(UNKNOWN_INSTRUCTION, "LDF  ,X");
        ETEST(UNKNOWN_INSTRUCTION, "STF  ,X");
        ETEST(UNKNOWN_INSTRUCTION, "CMPF ,X");
        ETEST(UNKNOWN_INSTRUCTION, "SUBW ,X");
        ETEST(UNKNOWN_INSTRUCTION, "CMPW ,X");
        ETEST(UNKNOWN_INSTRUCTION, "LDW  ,X");
        ETEST(UNKNOWN_INSTRUCTION, "STW  ,X");
        ETEST(UNKNOWN_INSTRUCTION, "ADDW ,X");
        ETEST(UNKNOWN_INSTRUCTION, "LDQ  ,X");
        ETEST(UNKNOWN_INSTRUCTION, "STQ  ,X");
        ETEST(UNKNOWN_INSTRUCTION, "MULD ,X");
        ETEST(UNKNOWN_INSTRUCTION, "DIVD -1,X");
        ETEST(UNKNOWN_INSTRUCTION, "OIM #$30,,X");
        ETEST(UNKNOWN_INSTRUCTION, "AIM #$30,,X+");
        ETEST(UNKNOWN_INSTRUCTION, "EIM #$30,[,--X]");
        ETEST(UNKNOWN_INSTRUCTION, "TIM #$30,-2,X");
    }
}

static void test_indexed_mode() {
    TEST("LDA ,X", 0xA6, 0x84);
    TEST("LDA ,Y", 0xA6, 0xA4);
    TEST("LDA ,U", 0xA6, 0xC4);
    TEST("LDA ,S", 0xA6, 0xE4);
    TEST("LDA 0,X",  0xA6, 0x84);
    TEST("LDA 0,Y",  0xA6, 0xA4);
    TEST("LDA 0,U",  0xA6, 0xC4);
    TEST("LDA 0,S",  0xA6, 0xE4);
    TEST("LDA [,X]", 0xA6, 0x94);
    TEST("LDA [,Y]", 0xA6, 0xB4);
    TEST("LDA [,U]", 0xA6, 0xD4);
    TEST("LDA [,S]", 0xA6, 0xF4);
    TEST("LDA [0,X]", 0xA6, 0x94);
    TEST("LDA [0,Y]", 0xA6, 0xB4);
    TEST("LDA [0,U]", 0xA6, 0xD4);
    TEST("LDA [0,S]", 0xA6, 0xF4);

    TEST("LDA <<0,X", 0xA6, 0x00);
    TEST("LDA <<0,Y", 0xA6, 0x20);
    TEST("LDA <<0,U", 0xA6, 0x40);
    TEST("LDA <<0,S", 0xA6, 0x60);
    TEST("LDA -1,X", 0xA6, 0x1F);
    TEST("LDA -1,Y", 0xA6, 0x3F);
    TEST("LDA -1,U", 0xA6, 0x5F);
    TEST("LDA -1,S", 0xA6, 0x7F);
    TEST("LDA 15,X", 0xA6, 0x0F);
    TEST("LDA 15,Y", 0xA6, 0x2F);
    TEST("LDA 15,U", 0xA6, 0x4F);
    TEST("LDA 15,S", 0xA6, 0x6F);
    TEST("LDA -16,X", 0xA6, 0x10);
    TEST("LDA -16,Y", 0xA6, 0x30);
    TEST("LDA -16,U", 0xA6, 0x50);
    TEST("LDA -16,S", 0xA6, 0x70);

    TEST("LDA <0,X",     0xA6, 0x88, 0x00);
    TEST("LDA 16,X",     0xA6, 0x88, 0x10);
    TEST("LDA -17,X",    0xA6, 0x88, 0xEF);
    TEST("LDA 127,X",    0xA6, 0x88, 0x7F);
    TEST("LDA -128,X",   0xA6, 0x88, 0x80);
    TEST("LDA [<0,X]",   0xA6, 0x98, 0x00);
    TEST("LDA [15,X]",   0xA6, 0x98, 0x0F);
    TEST("LDA [127,X]",  0xA6, 0x98, 0x7F);
    TEST("LDA [-16,X]",  0xA6, 0x98, 0xF0);
    TEST("LDA [-128,X]", 0xA6, 0x98, 0x80);
    TEST("LDA <0,Y",     0xA6, 0xA8, 0x00);
    TEST("LDA 127,Y",    0xA6, 0xA8, 0x7F);
    TEST("LDA -128,Y",   0xA6, 0xA8, 0x80);
    TEST("LDA [<0,Y]",   0xA6, 0xB8, 0x00);
    TEST("LDA [15,Y]",   0xA6, 0xB8, 0x0F);
    TEST("LDA [127,Y]",  0xA6, 0xB8, 0x7F);
    TEST("LDA [-16,Y]",  0xA6, 0xB8, 0xF0);
    TEST("LDA [-128,Y]", 0xA6, 0xB8, 0x80);
    TEST("LDA <0,U",     0xA6, 0xC8, 0x00);
    TEST("LDA 127,U",    0xA6, 0xC8, 0x7F);
    TEST("LDA -128,U",   0xA6, 0xC8, 0x80);
    TEST("LDA [<0,U]",   0xA6, 0xD8, 0x00);
    TEST("LDA [15,U]",   0xA6, 0xD8, 0x0F);
    TEST("LDA [127,U]",  0xA6, 0xD8, 0x7F);
    TEST("LDA [-16,U]",  0xA6, 0xD8, 0xF0);
    TEST("LDA [-128,U]", 0xA6, 0xD8, 0x80);
    TEST("LDA <0,S",     0xA6, 0xE8, 0x00);
    TEST("LDA 127,S",    0xA6, 0xE8, 0x7F);
    TEST("LDA -128,S",   0xA6, 0xE8, 0x80);
    TEST("LDA [<0,S]",   0xA6, 0xF8, 0x00);
    TEST("LDA [15,S]",   0xA6, 0xF8, 0x0F);
    TEST("LDA [127,S]",  0xA6, 0xF8, 0x7F);
    TEST("LDA [-16,S]",  0xA6, 0xF8, 0xF0);
    TEST("LDA [-128,S]", 0xA6, 0xF8, 0x80);
    TEST("LDA 0,PC",     0xA6, 0x8C, 0x00);
    TEST("LDA 127,PC",   0xA6, 0x8C, 0x7F);
    TEST("LDA -128,PC",  0xA6, 0x8C, 0x80);
    TEST("LDA [0,PC]",   0xA6, 0x9C, 0x00);
    TEST("LDA [127,PC]", 0xA6, 0x9C, 0x7F);
    TEST("LDA [-128,PC]",0xA6, 0x9C, 0x80);

    TEST("LDA >0,X",       0xA6, 0x89, 0x00, 0x00);
    TEST("LDA >127,X",     0xA6, 0x89, 0x00, 0x7F);
    TEST("LDA >-128,X",    0xA6, 0x89, 0xFF, 0x80);
    TEST("LDA 128,X",      0xA6, 0x89, 0x00, 0x80);
    TEST("LDA -129,X",     0xA6, 0x89, 0xFF, 0x7F);
    TEST("LDA 32767,X",    0xA6, 0x89, 0x7F, 0xFF);
    TEST("LDA -32768,X",   0xA6, 0x89, 0x80, 0x00);
    TEST("LDA [>0,X]",     0xA6, 0x99, 0x00, 0x00);
    TEST("LDA [>127,X]",   0xA6, 0x99, 0x00, 0x7F);
    TEST("LDA [>-128,X]",  0xA6, 0x99, 0xFF, 0x80);
    TEST("LDA [128,X]",    0xA6, 0x99, 0x00, 0x80);
    TEST("LDA [-129,X]",   0xA6, 0x99, 0xFF, 0x7F);
    TEST("LDA [32767,X]",  0xA6, 0x99, 0x7F, 0xFF);
    TEST("LDA [-32768,X]", 0xA6, 0x99, 0x80, 0x00);
    TEST("LDA >0,Y",       0xA6, 0xA9, 0x00, 0x00);
    TEST("LDA 32767,Y",    0xA6, 0xA9, 0x7F, 0xFF);
    TEST("LDA -32768,Y",   0xA6, 0xA9, 0x80, 0x00);
    TEST("LDA [>0,Y]",     0xA6, 0xB9, 0x00, 0x00);
    TEST("LDA [32767,Y]",  0xA6, 0xB9, 0x7F, 0xFF);
    TEST("LDA [-32768,Y]", 0xA6, 0xB9, 0x80, 0x00);
    TEST("LDA >0,U",       0xA6, 0xC9, 0x00, 0x00);
    TEST("LDA 32767,U",    0xA6, 0xC9, 0x7F, 0xFF);
    TEST("LDA -32768,U",   0xA6, 0xC9, 0x80, 0x00);
    TEST("LDA [>0,U]",     0xA6, 0xD9, 0x00, 0x00);
    TEST("LDA [32767,U]",  0xA6, 0xD9, 0x7F, 0xFF);
    TEST("LDA [-32768,U]", 0xA6, 0xD9, 0x80, 0x00);
    TEST("LDA >0,S",       0xA6, 0xE9, 0x00, 0x00);
    TEST("LDA 32767,S",    0xA6, 0xE9, 0x7F, 0xFF);
    TEST("LDA -32768,S",   0xA6, 0xE9, 0x80, 0x00);
    TEST("LDA [>0,S]",     0xA6, 0xF9, 0x00, 0x00);
    TEST("LDA [32767,S]",  0xA6, 0xF9, 0x7F, 0xFF);
    TEST("LDA [-32768,S]", 0xA6, 0xF9, 0x80, 0x00);
    TEST("LDA >0,PC",      0xA6, 0x8D, 0x00, 0x00);
    TEST("LDA 32767,PC",   0xA6, 0x8D, 0x7F, 0xFF);
    TEST("LDA -32768,PC",  0xA6, 0x8D, 0x80, 0x00);
    TEST("LDA [>0,PC]",    0xA6, 0x9D, 0x00, 0x00);
    TEST("LDA [32767,PC]", 0xA6, 0x9D, 0x7F, 0xFF);
    TEST("LDA [-32768,PC]",0xA6, 0x9D, 0x80, 0x00);

    TEST("LDA <<0,X",  0xA6, 0x00);
    TEST("LDA <0,X",   0xA6, 0x88, 0x00);
    TEST("LDA >0,X",   0xA6, 0x89, 0x00, 0x00);
    TEST("LDA [<0,X]", 0xA6, 0x98, 0x00);
    TEST("LDA [>0,X]", 0xA6, 0x99, 0x00, 0x00);

    TEST("LDA A,X",   0xA6, 0x86);
    TEST("LDA B,X",   0xA6, 0x85);
    TEST("LDA D,X",   0xA6, 0x8B);
    TEST("LDA [A,X]", 0xA6, 0x96);
    TEST("LDA [B,X]", 0xA6, 0x95);
    TEST("LDA [D,X]", 0xA6, 0x9B);

    TEST("LDA A,Y",   0xA6, 0xA6);
    TEST("LDA B,Y",   0xA6, 0xA5);
    TEST("LDA D,Y",   0xA6, 0xAB);
    TEST("LDA [A,Y]", 0xA6, 0xB6);
    TEST("LDA [B,Y]", 0xA6, 0xB5);
    TEST("LDA [D,Y]", 0xA6, 0xBB);

    TEST("LDA A,U",   0xA6, 0xC6);
    TEST("LDA B,U",   0xA6, 0xC5);
    TEST("LDA D,U",   0xA6, 0xCB);
    TEST("LDA [A,U]", 0xA6, 0xD6);
    TEST("LDA [B,U]", 0xA6, 0xD5);
    TEST("LDA [D,U]", 0xA6, 0xDB);

    TEST("LDA A,S",   0xA6, 0xE6);
    TEST("LDA B,S",   0xA6, 0xE5);
    TEST("LDA D,S",   0xA6, 0xEB);
    TEST("LDA [A,S]", 0xA6, 0xF6);
    TEST("LDA [B,S]", 0xA6, 0xF5);
    TEST("LDA [D,S]", 0xA6, 0xFB);

    TEST("LDA ,X+",    0xA6, 0x80);
    TEST("LDA ,X++",   0xA6, 0x81);
    TEST("LDA ,-X",    0xA6, 0x82);
    TEST("LDA ,--X",   0xA6, 0x83);
    TEST("LDA [,X++]", 0xA6, 0x91);
    TEST("LDA [,--X]", 0xA6, 0x93);

    TEST("LDA ,Y+",    0xA6, 0xA0);
    TEST("LDA ,Y++",   0xA6, 0xA1);
    TEST("LDA ,-Y",    0xA6, 0xA2);
    TEST("LDA ,--Y",   0xA6, 0xA3);
    TEST("LDA [,Y++]", 0xA6, 0xB1);
    TEST("LDA [,--Y]", 0xA6, 0xB3);

    TEST("LDA ,U+",    0xA6, 0xC0);
    TEST("LDA ,U++",   0xA6, 0xC1);
    TEST("LDA ,-U",    0xA6, 0xC2);
    TEST("LDA ,--U",   0xA6, 0xC3);
    TEST("LDA [,U++]", 0xA6, 0xD1);
    TEST("LDA [,--U]", 0xA6, 0xD3);

    TEST("LDA ,S+",    0xA6, 0xE0);
    TEST("LDA ,S++",   0xA6, 0xE1);
    TEST("LDA ,-S",    0xA6, 0xE2);
    TEST("LDA ,--S",   0xA6, 0xE3);
    TEST("LDA [,S++]", 0xA6, 0xF1);
    TEST("LDA [,--S]", 0xA6, 0xF3);

    ATEST(0x1000, "LDA $1003,PCR",   0xA6, 0x8C, 0x00);
    ATEST(0x1000, "LDA $1082,PCR",   0xA6, 0x8C, 0x7F);
    ATEST(0x1000, "LDA $0F83,PCR",   0xA6, 0x8C, 0x80);
    ATEST(0x1000, "LDA [$1003,PCR]", 0xA6, 0x9C, 0x00);
    ATEST(0x1000, "LDA [$1082,PCR]", 0xA6, 0x9C, 0x7F);
    ATEST(0x1000, "LDA [$0F83,PCR]", 0xa6, 0x9C, 0x80);

    ATEST(0x1000, "LDA $1004,PCR",   0xA6, 0x8C, 0x01);
    ATEST(0x1000, "LDA $9003,PCR",   0xA6, 0x8D, 0x7F, 0xFF);
    ATEST(0x1000, "LDA $9004,PCR",   0xA6, 0x8D, 0x80, 0x00);
    ATEST(0x1000, "LDA [$1004,PCR]", 0xA6, 0x9C, 0x01);
    ATEST(0x1000, "LDA [$9003,PCR]", 0xA6, 0x9D, 0x7F, 0xFF);
    ATEST(0x1000, "LDA [$9004,PCR]", 0xA6, 0x9D, 0x80, 0x00);

    TEST("LDA [$1234]", 0xA6, 0x9F, 0x12, 0x34);

    if (is6309()) {
        // HD6309
        TEST("LDA E,X", 0xA6, 0x87);
        TEST("LDA F,X", 0xA6, 0x8A);
        TEST("LDA W,X", 0xA6, 0x8E);
        TEST("LDA [E,X]", 0xA6, 0x97);
        TEST("LDA [F,X]", 0xA6, 0x9A);
        TEST("LDA [W,X]", 0xA6, 0x9E);

        TEST("LDA E,Y", 0xA6, 0xA7);
        TEST("LDA F,Y", 0xA6, 0xAA);
        TEST("LDA W,Y", 0xA6, 0xAE);
        TEST("LDA [E,Y]", 0xA6, 0xB7);
        TEST("LDA [F,Y]", 0xA6, 0xBA);
        TEST("LDA [W,Y]", 0xA6, 0xBE);

        TEST("LDA E,U", 0xA6, 0xC7);
        TEST("LDA F,U", 0xA6, 0xCA);
        TEST("LDA W,U", 0xA6, 0xCE);
        TEST("LDA [E,U]", 0xA6, 0xD7);
        TEST("LDA [F,U]", 0xA6, 0xDA);
        TEST("LDA [W,U]", 0xA6, 0xDE);

        TEST("LDA E,S", 0xA6, 0xE7);
        TEST("LDA F,S", 0xA6, 0xEA);
        TEST("LDA W,S", 0xA6, 0xEE);
        TEST("LDA [E,S]", 0xA6, 0xF7);
        TEST("LDA [F,S]", 0xA6, 0xFA);
        TEST("LDA [W,S]", 0xA6, 0xFE);

        TEST("LDA ,W",        0xA6, 0x8F);
        TEST("LDA 0,W",       0xA6, 0x8F);
        TEST("LDA >0,W",      0xA6, 0xAF, 0x00, 0x00);
        TEST("LDA 32767,W",   0xA6, 0xAF, 0x7F, 0xFF);
        TEST("LDA -32768,W",  0xA6, 0xAF, 0x80, 0x00);
        TEST("LDA ,W++",      0xA6, 0xCF);
        TEST("LDA ,--W",      0xA6, 0xEF);
        TEST("LDA [,W]",      0xA6, 0x90);
        TEST("LDA [0,W]",     0xA6, 0x90);
        TEST("LDA [>0,W]",    0xA6, 0xB0, 0x00, 0x00);
        TEST("LDA [32767,W]", 0xA6, 0xB0, 0x7F, 0xFF);
        TEST("LDA [-32768,W]",0xA6, 0xB0, 0x80, 0x00);
        TEST("LDA [,W++]",    0xA6, 0xD0);
        TEST("LDA [,--W]",    0xA6, 0xF0);
    } else {
        ETEST(UNKNOWN_OPERAND, "LDA E,X");
        ETEST(UNKNOWN_OPERAND, "LDA F,X");
        ETEST(UNKNOWN_OPERAND, "LDA W,X");
        ETEST(UNKNOWN_OPERAND, "LDA [E,X]");
        ETEST(UNKNOWN_OPERAND, "LDA [F,X]");
        ETEST(UNKNOWN_OPERAND, "LDA [W,X]");
        ETEST(UNKNOWN_OPERAND, "LDA E,Y");
        ETEST(UNKNOWN_OPERAND, "LDA F,Y");
        ETEST(UNKNOWN_OPERAND, "LDA W,Y");
        ETEST(UNKNOWN_OPERAND, "LDA [E,Y]");
        ETEST(UNKNOWN_OPERAND, "LDA [F,Y]");
        ETEST(UNKNOWN_OPERAND, "LDA [W,Y]");
        ETEST(UNKNOWN_OPERAND, "LDA E,U");
        ETEST(UNKNOWN_OPERAND, "LDA F,U");
        ETEST(UNKNOWN_OPERAND, "LDA W,U");
        ETEST(UNKNOWN_OPERAND, "LDA [E,U]");
        ETEST(UNKNOWN_OPERAND, "LDA [F,U]");
        ETEST(UNKNOWN_OPERAND, "LDA [W,U]");
        ETEST(UNKNOWN_OPERAND, "LDA E,S");
        ETEST(UNKNOWN_OPERAND, "LDA F,S");
        ETEST(UNKNOWN_OPERAND, "LDA W,S");
        ETEST(UNKNOWN_OPERAND, "LDA [E,S]");
        ETEST(UNKNOWN_OPERAND, "LDA [F,S]");
        ETEST(UNKNOWN_OPERAND, "LDA [W,S]");
        ETEST(UNKNOWN_OPERAND, "LDA ,W");
        ETEST(UNKNOWN_OPERAND, "LDA 0,W");
        ETEST(UNKNOWN_OPERAND, "LDA >0,W");
        ETEST(UNKNOWN_OPERAND, "LDA 32767,W");
        ETEST(UNKNOWN_OPERAND, "LDA -32768,W");
        ETEST(UNKNOWN_OPERAND, "LDA ,W++");
        ETEST(UNKNOWN_OPERAND, "LDA ,--W");
        ETEST(UNKNOWN_OPERAND, "LDA [,W]");
        ETEST(UNKNOWN_OPERAND, "LDA [0,W]");
        ETEST(UNKNOWN_OPERAND, "LDA [>0,W]");
        ETEST(UNKNOWN_OPERAND, "LDA [32767,W]");
        ETEST(UNKNOWN_OPERAND, "LDA [-32768,W]");
        ETEST(UNKNOWN_OPERAND, "LDA [,W++]");
        ETEST(UNKNOWN_OPERAND, "LDA [,--W]");
    }

    symtab.intern(0x0F83, "label0F83");
    symtab.intern(0x1003, "label1003");
    symtab.intern(0x1004, "label1004");
    symtab.intern(0x1082, "label1082");
    symtab.intern(0x1234, "label1234");
    symtab.intern(0x9003, "label9003");
    symtab.intern(0x9004, "label9004");

    ATEST(0x1000, "LDA label1003,PCR",   0xA6, 0x8C, 0x00);
    ATEST(0x1000, "LDA label1082,PCR",   0xA6, 0x8C, 0x7F);
    ATEST(0x1000, "LDA label0F83,PCR",   0xA6, 0x8C, 0x80);
    ATEST(0x1000, "LDA [label1003,PCR]", 0xA6, 0x9C, 0x00);
    ATEST(0x1000, "LDA [label1082,PCR]", 0xA6, 0x9C, 0x7F);
    ATEST(0x1000, "LDA [label0F83,PCR]", 0xA6, 0x9C, 0x80);

    ATEST(0x1000, "LDA label1004,PCR",   0xA6, 0x8C, 0x01);
    ATEST(0x1000, "LDA label9003,PCR",   0xA6, 0x8D, 0x7F, 0xFF);
    ATEST(0x1000, "LDA label9004,PCR",   0xA6, 0x8D, 0x80, 0x00);
    ATEST(0x1000, "LDA [label1004,PCR]", 0xA6, 0x9C, 0x01);
    ATEST(0x1000, "LDA [label9003,PCR]", 0xA6, 0x9D, 0x7F, 0xFF);
    ATEST(0x1000, "LDA [label9004,PCR]", 0xA6, 0x9D, 0x80, 0x00);

    ATEST(0x1000, "LDA [label1234]", 0xA6, 0x9F, 0x12, 0x34);
}

static void test_transfer() {
    if (is6309()) {
        // HD6309
        TEST("TFM X+,Y+", 0x11, 0x38, 0x12);
        TEST("TFM X-,Y-", 0x11, 0x39, 0x12);
        TEST("TFM X+,Y",  0x11, 0x3A, 0x12);
        TEST("TFM X,Y+",  0x11, 0x3B, 0x12);

        TEST("TFM D+,X+", 0x11, 0x38, 0x01);
        TEST("TFM D-,X-", 0x11, 0x39, 0x01);
        TEST("TFM D+,X",  0x11, 0x3A, 0x01);
        TEST("TFM D,X+",  0x11, 0x3B, 0x01);

        TEST("TFM D+,X+", 0x11, 0x38, 0x01);
        TEST("TFM Y+,U+", 0x11, 0x38, 0x23);
        TEST("TFM U+,S+", 0x11, 0x38, 0x34);
        TEST("TFM S+,D+", 0x11, 0x38, 0x40);

        ETEST(OPERAND_NOT_ALLOWED, "TFM X+,W");
        ETEST(OPERAND_NOT_ALLOWED, "TFM W+,X");
        ETEST(OPERAND_NOT_ALLOWED, "TFM W-,X-");
    } else {
        ETEST(UNKNOWN_INSTRUCTION, "TFM D,Y");
        ETEST(UNKNOWN_INSTRUCTION, "TFM X+,Y+");
    }
}

static void test_bit_position() {
    if (is6309()) {
        // HD6309
        TEST("BAND  A.1,$34.2", 0x11, 0x30, 0x51, 0x34);
        TEST("BIAND A.1,$34.2", 0x11, 0x31, 0x51, 0x34);
        TEST("BOR   A.1,$34.2", 0x11, 0x32, 0x51, 0x34);
        // ',' can be accepted as bit number separator.
        TEST("BIOR  A,1,$34,2", 0x11, 0x33, 0x51, 0x34);
        TEST("BEOR  A,1,$34,2", 0x11, 0x34, 0x51, 0x34);
        TEST("SETDP $12");
        TEST("BIEOR A.1,$1234.2", 0x11, 0x35, 0x51, 0x34);
        TEST("LDBT  A,1,$1234,2", 0x11, 0x36, 0x51, 0x34);
        TEST("STBT  A.1,<$5634.2", 0x11, 0x37, 0x51, 0x34);
        TEST("STBT  B,0,<$5634.2", 0x11, 0x37, 0x90, 0x34);

        TEST("SETDP 0");
        TEST("LDBT CC,0,$34.7", 0x11, 0x36, 0x38, 0x34);
        TEST("LDBT CC.1,$34.7", 0x11, 0x36, 0x39, 0x34);
        TEST("LDBT CC.2,$34.7", 0x11, 0x36, 0x3A, 0x34);
        // ',' can be accepted as bit number separator.
        TEST("LDBT CC,3,$34.7", 0x11, 0x36, 0x3B, 0x34);
        TEST("LDBT CC,4,$34,7", 0x11, 0x36, 0x3C, 0x34);
        TEST("SETDP $12");
        TEST("LDBT CC.5,$1234.7", 0x11, 0x36, 0x3D, 0x34);
        TEST("LDBT CC,6,$1234,7", 0x11, 0x36, 0x3E, 0x34);
        TEST("LDBT CC.7,<$5634.7", 0x11, 0x36, 0x3F, 0x34);
        TEST("LDBT A,7,$1234,0",  0x11, 0x36, 0x47, 0x34);
        TEST("LDBT B,2,<$5634,4",  0x11, 0x36, 0xA2, 0x34);

        symtab.intern(0x0034, "dir34");
        symtab.intern(0x9030, "sym9030");

        TEST("SETDP 0");
        TEST("LDBT  CC.0,dir34.7",   0x11, 0x36, 0x38, 0x34);
        TEST("LDBT  B.2,<sym9030.4", 0x11, 0x36, 0xA2, 0x30);
        TEST("SETDP $90");
        TEST("LDBT  B.2,sym9030.4",  0x11, 0x36, 0xA2, 0x30);
    } else {
        ETEST(UNKNOWN_INSTRUCTION, "BAND  A.1,$34.2");
        ETEST(UNKNOWN_INSTRUCTION, "BIAND A.1,$34.2");
        ETEST(UNKNOWN_INSTRUCTION, "BOR   A.1,$34.2");
        ETEST(UNKNOWN_INSTRUCTION, "BIOR  A,1,$34,2");
        ETEST(UNKNOWN_INSTRUCTION, "BEOR  A,1,$34,2");
        ETEST(UNKNOWN_INSTRUCTION, "LDBT  A,1,$34,2");
        ETEST(UNKNOWN_INSTRUCTION, "STBT  A.1,$34.2");
    }
}

static void test_comment() {
    TEST("NOP      ; comment", 0x12);
    TEST("PSHS A   ; comment", 0x34, 0x02);
    TEST("PSHS CC , A , B , DP , X , Y , U , PC ; comment", 0x34, 0xFF);
    TEST("TFR  D , X   ; comment", 0x1F, 0x01);
    TEST("SUBB # $90   ; comment", 0xC0, 0x90);
    TEST("NEG  $10     ; comment", 0x00, 0x10);
    ATEST(0x1000, "BRA $1002           ; comment", 0x20, 0x00);
    ATEST(0x1000, "LDA [ $1004 , PCR ] ; comment", 0xA6, 0x9C, 0x01);

    TEST("LDA ,S      ; comment", 0xA6, 0xE4);
    TEST("LDA <<0 , S ; comment", 0xA6, 0x60);
    TEST("LDA <0 , X  ; comment", 0xA6, 0x88, 0x00);
    TEST("LDA >0 , X  ; comment", 0xA6, 0x89, 0x00, 0x00);
    TEST("LDA D , X   ; comment", 0xA6, 0x8B);
    TEST("LDA - 1 , S ; comment", 0xA6, 0x7F);
    TEST("LDA 15 , S  ; comment", 0xA6, 0x6F);
    TEST("LDA [ ,S ]          ; comment", 0xA6, 0xF4);
    TEST("LDA [ 0 , S ]       ; comment", 0xA6, 0xF4);
    TEST("LDA [ <0 , X ]      ; comment", 0xA6, 0x98, 0x00);
    TEST("LDA [ >0 , X ]      ; comment", 0xA6, 0x99, 0x00, 0x00);
    TEST("LDA [ -32768 , PC ] ; comment", 0xA6, 0x9D, 0x80, 0x00);
    TEST("LDA [ D , X ]       ; comment", 0xA6, 0x9B);
    ATEST(0x1000, "LDA $0F83 , PCR     ; comment", 0xA6, 0x8C, 0x80);
    ATEST(0x1000, "LDA [ $0F83 , PCR ] ; comment", 0xa6, 0x9C, 0x80);

    if (is6309()) {
        // HD6309
        TEST("TFM S+ , D+  ; comment", 0x11, 0x38, 0x40);
        TEST("TFM X- , Y-  ; comment", 0x11, 0x39, 0x12);
        TEST("TFM X+ , Y   ; comment", 0x11, 0x3A, 0x12);
        TEST("TFM X , Y+   ; comment", 0x11, 0x3B, 0x12);
        TEST("BOR A.1   , $34.2   ; comment", 0x11, 0x32, 0x51, 0x34);
        TEST("BOR A , 1 , $34 , 2 ; comment", 0x11, 0x32, 0x51, 0x34);
    }
}

static void test_error() {
    ETEST(UNKNOWN_OPERAND, "LDA , S");
    ETEST(UNKNOWN_OPERAND, "LDA , X+");
    ETEST(UNKNOWN_OPERAND, "LDA , X++");
    ETEST(UNKNOWN_OPERAND, "LDA , -X");
    ETEST(UNKNOWN_OPERAND, "LDA , --X");
    ETEST(UNKNOWN_OPERAND, "LDA [ , X++ ]");
    ETEST(UNKNOWN_OPERAND, "LDA [ , --X ]");
    ETEST(GARBAGE_AT_END,  "LDA ,X +");
    ETEST(GARBAGE_AT_END,  "LDA ,X ++");
    ETEST(GARBAGE_AT_END,  "LDA ,X+ +");
    ETEST(UNKNOWN_OPERAND, "LDA ,- X");
    ETEST(UNKNOWN_OPERAND, "LDA ,-- X");
    ETEST(UNKNOWN_OPERAND, "LDA ,- -X");
    ETEST(GARBAGE_AT_END,  "LDA ,X]");
    ETEST(MISSING_CLOSING_PAREN, "LDA [,X");

    if (is6309()) {
        // HD6309
        ETEST(UNKNOWN_OPERAND, "LDA ,W+");
        ETEST(UNKNOWN_OPERAND, "LDA ,-W");
        ETEST(UNKNOWN_OPERAND, "LDA , W++");
        ETEST(UNKNOWN_OPERAND, "LDA , --W");
        ETEST(UNKNOWN_OPERAND, "LDA [ , W++ ]");
        ETEST(UNKNOWN_OPERAND, "LDA [ , --W ]");

        ETEST(OPERAND_NOT_ALLOWED, "TFM D+,W+");
        ETEST(GARBAGE_AT_END,      "TFM D + , X+");
        ETEST(GARBAGE_AT_END,      "TFM D+ , X +");
        ETEST(GARBAGE_AT_END,      "TFM X - , Y-");
        ETEST(GARBAGE_AT_END,      "TFM X- , Y -");
        ETEST(GARBAGE_AT_END,      "BOR A .1  , $34.2");
        ETEST(GARBAGE_AT_END,      "BOR A. 1  , $34.2");
        TEST(     "BOR A.1   , $34 .2", 0x11, 0x32, 0x51, 0x34);
        ETEST(GARBAGE_AT_END,      "BOR A.1   , $34. 2");
        TEST(                      "BOR A , 1 , $34 , 2", 0x11, 0x32, 0x51, 0x34);
    }
}

static void test_undefined_symbol() {
    ETEST(UNDEFINED_SYMBOL, "LDA  #UNDEF",  0x86, 0x00);
    ETEST(UNDEFINED_SYMBOL, "SUBD #UNDEF",  0x83, 0x00, 0x00);
    ETEST(UNDEFINED_SYMBOL, "NEG  UNDEF",   0x00, 0x00);
    ETEST(UNDEFINED_SYMBOL, "SUBA <UNDEF",  0x90, 0x00);
    ETEST(UNDEFINED_SYMBOL, "SUBB >UNDEF",  0xF0, 0x00, 0x00);

    ETEST(UNDEFINED_SYMBOL, "LDA  UNDEF,Y",    0xA6, 0xA9, 0x00, 0x00);
    ETEST(UNDEFINED_SYMBOL, "LDA  <<UNDEF,Y",  0xA6, 0x20);
    ETEST(UNDEFINED_SYMBOL, "LDA  <UNDEF,Y",   0xA6, 0xA8, 0x00);
    ETEST(UNDEFINED_SYMBOL, "LDA  >UNDEF,Y",   0xA6, 0xA9, 0x00, 0x00);
    ETEST(UNDEFINED_SYMBOL, "LDA  [UNDEF,Y]",  0xA6, 0xB9, 0x00, 0x00);
    ETEST(UNDEFINED_SYMBOL, "LDA  [<UNDEF,Y]", 0xA6, 0xB8, 0x00);
    ETEST(UNDEFINED_SYMBOL, "LDA  [>UNDEF,Y]", 0xA6, 0xB9, 0x00, 0x00);
    ETEST(UNDEFINED_SYMBOL, "LDA  [UNDEF]",    0xA6, 0x9F, 0x00, 0x00);
    ETEST(UNDEFINED_SYMBOL, "LDA  [>UNDEF]",   0xA6, 0x9F, 0x00, 0x00);

    ETEST(UNDEFINED_SYMBOL, "BRA  UNDEF", 0x20, 0x00);
    ETEST(UNDEFINED_SYMBOL, "LBRA UNDEF", 0x16, 0x00, 0x00);
    ETEST(UNDEFINED_SYMBOL, "LBCC UNDEF", 0x10, 0x24, 0x00, 0x00);
    ETEST(UNDEFINED_SYMBOL, "LDA  UNDEF,PCR",    0xA6, 0x8D, 0x00, 0x00);
    ETEST(UNDEFINED_SYMBOL, "LDA  <UNDEF,PCR",   0xA6, 0x8C, 0x00);
    ETEST(UNDEFINED_SYMBOL, "LDA  >UNDEF,PCR",   0xA6, 0x8D, 0x00, 0x00);
    ETEST(UNDEFINED_SYMBOL, "LDA  [UNDEF,PCR]",  0xA6, 0x9D, 0x00, 0x00);
    ETEST(UNDEFINED_SYMBOL, "LDA  [<UNDEF,PCR]", 0xA6, 0x9C, 0x00);
    ETEST(UNDEFINED_SYMBOL, "LDA  [>UNDEF,PCR]", 0xA6, 0x9D, 0x00, 0x00);

    if (is6309()) {
        // HD6309
        ETEST(UNDEFINED_SYMBOL, "BITMD #UNDEF",  0x11, 0x3C, 0x00);
        ETEST(UNDEFINED_SYMBOL, "LDQ   #UNDEF",  0xCD, 0x00, 0x00, 0x00, 0x00);
        ETEST(UNDEFINED_SYMBOL, "MULD  #UNDEF",  0x11, 0x8F, 0x00, 0x00);
        ETEST(UNDEFINED_SYMBOL, "LDA UNDEF,W",    0xA6, 0xAF, 0x00, 0x00);
        ETEST(UNDEFINED_SYMBOL, "LDA >UNDEF,W",   0xA6, 0xAF, 0x00, 0x00);
        ETEST(UNDEFINED_SYMBOL, "LDA [UNDEF,W]",  0xA6, 0xB0, 0x00, 0x00);
        ETEST(UNDEFINED_SYMBOL, "LDA [>UNDEF,W]", 0xA6, 0xB0, 0x00, 0x00);
        ETEST(UNDEFINED_SYMBOL, "OIM #UNDEF,$10",       0x01, 0x00, 0x10);
        ETEST(UNDEFINED_SYMBOL, "OIM #UNDEF,$1000",     0x71, 0x00, 0x10, 0x00);
        ETEST(UNDEFINED_SYMBOL, "OIM #UNDEF,UNDEF,X",   0x61, 0x00, 0x89, 0x00, 0x00);
        ETEST(UNDEFINED_SYMBOL, "OIM #UNDEF,[$1000,X]", 0x61, 0x00, 0x99, 0x10, 0x00);
        ETEST(UNDEFINED_SYMBOL, "OIM #$30,UNDEF",       0x01, 0x30, 0x00);
        ETEST(UNDEFINED_SYMBOL, "OIM #$30,>UNDEF",      0x71, 0x30, 0x00, 0x00);
        ETEST(UNDEFINED_SYMBOL, "OIM #$30,UNDEF,X",     0x61, 0x30, 0x89, 0x00, 0x00);
        ETEST(UNDEFINED_SYMBOL, "OIM #$30,[UNDEF,X]",   0x61, 0x30, 0x99, 0x00, 0x00);
        ETEST(UNDEFINED_SYMBOL, "BOR A.UNDEF,$34.2", 0x11, 0x32, 0x50, 0x34);
        ETEST(UNDEFINED_SYMBOL, "BOR A.1,UNDEF.2",   0x11, 0x32, 0x51, 0x00);
        ETEST(UNDEFINED_SYMBOL, "BOR A.1,$34.UNDEF", 0x11, 0x32, 0x41, 0x34);
    }
}

void run_tests() {
    RUN_TEST(test_cpu);
    static const char *cpus[] = {
        "6809", "6309",
    };
    for (size_t i = 0; i < sizeof(cpus)/sizeof(cpus[0]); i++) {
        const char *cpu = cpus[i];
        assembler.setCpu(cpu);
        printf("  TEST CPU %s\n", cpu);
        RUN_TEST(test_inherent);
        RUN_TEST(test_immediate);
        RUN_TEST(test_direct);
        RUN_TEST(test_extended);
        RUN_TEST(test_indexed);
        RUN_TEST(test_indexed_mode);
        RUN_TEST(test_relative);
        RUN_TEST(test_stack);
        RUN_TEST(test_register);
        RUN_TEST(test_transfer);
        RUN_TEST(test_bit_position);
        RUN_TEST(test_comment);
        RUN_TEST(test_error);
        RUN_TEST(test_undefined_symbol);
    }
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
