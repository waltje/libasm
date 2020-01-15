#include "asm_hd6309.h"
#include "test_asm_helper.h"

TestAsserter asserter;
TestSymtab symtab;
AsmMc6809 assembler;

static void set_up() {
    TEST("SETDP 0");
}

static void tear_down() {
    symtab.reset();
}

static void test_cpu() {
    asserter.equals(
        "cpu 6309", false, assembler.acceptCpu("6309"));
    asserter.equals(
        "cpu 6809", true, assembler.acceptCpu("6809"));
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
}

static void test_stack() {
    TEST("PSHS A", 0x34, 0x02);
    TEST("PULS A", 0x35, 0x02);
    TEST("PSHU A", 0x36, 0x02);
    TEST("PULU A", 0x37, 0x02);

    TEST("PSHS CC", 0x34, 0x01);
    TEST("PSHS A",  0x34, 0x02);
    TEST("PSHS B",  0x34, 0x04);
    TEST("PSHS DP", 0x34, 0x08);
    TEST("PSHS X",  0x34, 0x10);
    TEST("PSHS Y",  0x34, 0x20);
    TEST("PSHS U",  0x34, 0x40);
    TEST("PSHS PC", 0x34, 0x80);
    TEST("PSHS D",  0x34, 0x06);

    TEST("PULU CC", 0x37, 0x01);
    TEST("PULU A",  0x37, 0x02);
    TEST("PULU B",  0x37, 0x04);
    TEST("PULU DP", 0x37, 0x08);
    TEST("PULU X",  0x37, 0x10);
    TEST("PULU Y",  0x37, 0x20);
    TEST("PULU S",  0x37, 0x40);
    TEST("PULU PC", 0x37, 0x80);
    TEST("PULU D",  0x37, 0x06);

    TEST("PSHS CC,A,B,DP,X,Y,U,PC", 0x34, 0xFF);
    TEST("PULS PC,A,DP,B,X,U,Y,CC", 0x35, 0xFF);
    TEST("PSHU PC,S,Y,X,DP,B,A,CC", 0x36, 0xFF);
    TEST("PULU DP,CC,PC,A,B,S,Y,X", 0x37, 0xFF);
    TEST("PSHS CC,A,B,DP,X,Y,U,PC", 0x34, 0xFF);
    TEST("PSHS PC,U,Y,X,DP,D,CC",   0x34, 0xFF);
    TEST("PULS CC,D,DP,X,U,Y,PC",   0x35, 0xFF);

    TEST("PSHS", 0x34, 0x00);
}

static void test_register() {
    TEST("EXG A,B", 0x1E, 0x89);
    TEST("TFR X,Y", 0x1F, 0x12);

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

    symtab.intern(0x90, "dir90");
    symtab.intern(0x90A0, "dir90A0");

    TEST("LDA  #dir90",   0x86, 0x90);
    TEST("CMPX #dir90A0", 0x8C, 0x90, 0xA0);
    TEST("LDY  #dir90A0", 0x10, 0x8E, 0x90, 0xA0);
    TEST("LDS  #dir90A0", 0x10, 0xCE, 0x90, 0xA0);
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

    symtab.intern(0x10, "dir10");
    symtab.intern(0x90, "dir90");
    symtab.intern(0x1290, "sym1290");
    symtab.intern(0x90A0, "sym90A0");

    TEST("NEG  dir10",    0x00, 0x10);
    TEST("LDA  dir10",    0x96, 0x10);
    TEST("STB  <sym1290", 0xD7, 0x90);
    TEST("CMPX <sym1290", 0x9C, 0x90);
    TEST("STU  dir90",    0xDF, 0x90);
    TEST("LDY  dir90",    0x10, 0x9E, 0x90);
    TEST("LDS  <sym90A0", 0x10, 0xDE, 0xA0);
    TEST("JMP  <sym90A0", 0x0E, 0xA0);
    TEST("JSR  dir90",    0x9D, 0x90);

    TEST("SETDP sym1290>>8");
    TEST("NEG   dir10",    0x70, 0x00, 0x10);
    TEST("LDA   dir10",    0xB6, 0x00, 0x10);
    TEST("LDA   <dir10",   0x96, 0x10);
    TEST("NEG   sym1290",  0x00, 0x90);
    TEST("LDA   sym1290",  0x96, 0x90);
    TEST("LDA   >sym1290", 0xB6, 0x12, 0x90);

    TEST("ASSUME DPR:sym90A0>>8");
    TEST("STU   dir90",    0xFF, 0x00, 0x90);
    TEST("LDY   dir90",    0x10, 0xBE, 0x00, 0x90);
    TEST("LDY   <dir90",   0x10, 0x9E, 0x90);
    TEST("LDS   sym90A0",  0x10, 0xDE, 0xA0);
    TEST("JMP   sym90A0",  0x0E, 0xA0);
    TEST("JMP   >sym90A0", 0x7E, 0x90, 0xA0);
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

    symtab.intern(0x0090, "dir90");
    symtab.intern(0x9ABC, "ext9ABC");

    TEST("NEG >dir90",   0x70, 0x00, 0x90);
    TEST("LDA  ext9ABC", 0xB6, 0x9A, 0xBC);
    TEST("STB >dir90",   0xF7, 0x00, 0x90);
    TEST("CMPX ext9ABC", 0xBC, 0x9A, 0xBC);
    TEST("STU >dir90",   0xFF, 0x00, 0x90);
    TEST("LDY  ext9ABC", 0x10, 0xBE, 0x9A, 0xBC);
    TEST("LDS >dir90",   0x10, 0xFE, 0x00, 0x90);
    TEST("JMP  ext9ABC", 0x7E, 0x9A, 0xBC);
    TEST("JSR >dir90",   0xBD, 0x00, 0x90);
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
}

static void test_indexed_mode() {
    TEST("LDA ,X", 0xA6, 0x84);
    TEST("LDA ,Y", 0xA6, 0xA4);
    TEST("LDA ,U", 0xA6, 0xC4);
    TEST("LDA ,S", 0xA6, 0xE4);
    TEST("LDA [,X]", 0xA6, 0x94);
    TEST("LDA [,Y]", 0xA6, 0xB4);
    TEST("LDA [,U]", 0xA6, 0xD4);
    TEST("LDA [,S]", 0xA6, 0xF4);

    TEST("LDA 0,X",  0xA6, 0x00);
    TEST("LDA 0,Y",  0xA6, 0x20);
    TEST("LDA 0,U",  0xA6, 0x40);
    TEST("LDA 0,S",  0xA6, 0x60);
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

    TEST("LDA 0,X",      0xA6, 0x00);
    TEST("LDA 127,X",    0xA6, 0x88, 0x7F);
    TEST("LDA -128,X",   0xA6, 0x88, 0x80);
    TEST("LDA [0,X]",    0xA6, 0x98, 0x00);
    TEST("LDA [127,X]",  0xA6, 0x98, 0x7F);
    TEST("LDA [-128,X]", 0xA6, 0x98, 0x80);
    TEST("LDA 0,Y",      0xA6, 0x20);
    TEST("LDA 127,Y",    0xA6, 0xA8, 0x7F);
    TEST("LDA -128,Y",   0xA6, 0xA8, 0x80);
    TEST("LDA [0,Y]",    0xA6, 0xB8, 0x00);
    TEST("LDA [127,Y]",  0xA6, 0xB8, 0x7F);
    TEST("LDA [-128,Y]", 0xA6, 0xB8, 0x80);
    TEST("LDA 0,U",      0xA6, 0x40);
    TEST("LDA 127,U",    0xA6, 0xC8, 0x7F);
    TEST("LDA -128,U",   0xA6, 0xC8, 0x80);
    TEST("LDA [0,U]",    0xA6, 0xD8, 0x00);
    TEST("LDA [127,U]",  0xA6, 0xD8, 0x7F);
    TEST("LDA [-128,U]", 0xA6, 0xD8, 0x80);
    TEST("LDA 0,S",      0xA6, 0x60);
    TEST("LDA 127,S",    0xA6, 0xE8, 0x7F);
    TEST("LDA -128,S",   0xA6, 0xE8, 0x80);
    TEST("LDA [0,S]",    0xA6, 0xF8, 0x00);
    TEST("LDA [127,S]",  0xA6, 0xF8, 0x7F);
    TEST("LDA [-128,S]", 0xA6, 0xF8, 0x80);
    TEST("LDA 0,PC",      0xA6, 0x8C, 0x00);
    TEST("LDA 127,PC",    0xA6, 0x8C, 0x7F);
    TEST("LDA -128,PC",   0xA6, 0x8C, 0x80);
    TEST("LDA [0,PC]",    0xA6, 0x9C, 0x00);
    TEST("LDA [127,PC]",  0xA6, 0x9C, 0x7F);
    TEST("LDA [-128,PC]", 0xA6, 0x9C, 0x80);

    TEST("LDA 0,X",        0xA6, 0x00);
    TEST("LDA 32767,X",    0xA6, 0x89, 0x7F, 0xFF);
    TEST("LDA -32768,X",   0xA6, 0x89, 0x80, 0x00);
    TEST("LDA [0,X]",      0xA6, 0x98, 0x00);
    TEST("LDA [32767,X]",  0xA6, 0x99, 0x7F, 0xFF);
    TEST("LDA [-32768,X]", 0xA6, 0x99, 0x80, 0x00);
    TEST("LDA 0,Y",        0xA6, 0x20);
    TEST("LDA 32767,Y",    0xA6, 0xA9, 0x7F, 0xFF);
    TEST("LDA -32768,Y",   0xA6, 0xA9, 0x80, 0x00);
    TEST("LDA [0,Y]",      0xA6, 0xB8, 0x00);
    TEST("LDA [32767,Y]",  0xA6, 0xB9, 0x7F, 0xFF);
    TEST("LDA [-32768,Y]", 0xA6, 0xB9, 0x80, 0x00);
    TEST("LDA 0,U",        0xA6, 0x40);
    TEST("LDA 32767,U",    0xA6, 0xC9, 0x7F, 0xFF);
    TEST("LDA -32768,U",   0xA6, 0xC9, 0x80, 0x00);
    TEST("LDA [0,U]",      0xA6, 0xD8, 0x00);
    TEST("LDA [32767,U]",  0xA6, 0xD9, 0x7F, 0xFF);
    TEST("LDA [-32768,U]", 0xA6, 0xD9, 0x80, 0x00);
    TEST("LDA 0,S",        0xA6, 0x60);
    TEST("LDA 32767,S",    0xA6, 0xE9, 0x7F, 0xFF);
    TEST("LDA -32768,S",   0xA6, 0xE9, 0x80, 0x00);
    TEST("LDA [0,S]",      0xA6, 0xF8, 0x00);
    TEST("LDA [32767,S]",  0xA6, 0xF9, 0x7F, 0xFF);
    TEST("LDA [-32768,S]", 0xA6, 0xF9, 0x80, 0x00);
    TEST("LDA 0,PC",        0xA6, 0x8C, 0x00);
    TEST("LDA 32767,PC",    0xA6, 0x8D, 0x7F, 0xFF);
    TEST("LDA -32768,PC",   0xA6, 0x8D, 0x80, 0x00);
    TEST("LDA [0,PC]",      0xA6, 0x9C, 0x00);
    TEST("LDA [32767,PC]",  0xA6, 0x9D, 0x7F, 0xFF);
    TEST("LDA [-32768,PC]", 0xA6, 0x9D, 0x80, 0x00);

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

static void run_test(void (*test)(), const char *test_name) {
    asserter.clear(test_name);
    set_up();
    test();
    tear_down();
    asserter.check();
}

int main(int argc, char **argv) {
    RUN_TEST(test_cpu);
    RUN_TEST(test_inherent);
    RUN_TEST(test_immediate);
    RUN_TEST(test_direct);
    RUN_TEST(test_extended);
    RUN_TEST(test_indexed);
    RUN_TEST(test_indexed_mode);
    RUN_TEST(test_relative);
    RUN_TEST(test_stack);
    RUN_TEST(test_register);
    return 0;
}
