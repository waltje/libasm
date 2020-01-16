#include "dis_mc6809.h"
#include "test_dis_helper.h"

TestAsserter asserter;
TestMemory memory;
TestSymtab symtab;
DisMc6809 dis6809;
Disassembler<target::uintptr_t> &disassembler(dis6809);

static void set_up() {
    dis6809.acceptCpu(McuType::MC6809);
}

static void tear_down() {
    symtab.reset();
}

static void test_inherent() {
    TEST(NOP,  "", 0x12);
    TEST(SYNC, "", 0x13);
    TEST(DAA,  "", 0x19);
    TEST(SEX,  "", 0x1D);
    TEST(RTS,  "", 0x39);
    TEST(ABX,  "", 0x3A);
    TEST(RTI,  "", 0x3B);
    TEST(MUL,  "", 0x3D);
    TEST(SWI,  "", 0x3F);
    TEST(SWI2, "", 0x10, 0x3F);
    TEST(SWI3, "", 0x11, 0x3F);

    TEST(NEGA, "", 0x40);
    TEST(COMA, "", 0x43);
    TEST(LSRA, "", 0x44);
    TEST(RORA, "", 0x46);
    TEST(ASRA, "", 0x47);
    TEST(ASLA, "", 0x48);
    TEST(ROLA, "", 0x49);
    TEST(DECA, "", 0x4A);
    TEST(INCA, "", 0x4C);
    TEST(TSTA, "", 0x4D);
    TEST(CLRA, "", 0x4F);

    TEST(NEGB, "", 0x50);
    TEST(COMB, "", 0x53);
    TEST(LSRB, "", 0x54);
    TEST(RORB, "", 0x56);
    TEST(ASRB, "", 0x57);
    TEST(ASLB, "", 0x58);
    TEST(ROLB, "", 0x59);
    TEST(DECB, "", 0x5A);
    TEST(INCB, "", 0x5C);
    TEST(TSTB, "", 0x5D);
    TEST(CLRB, "", 0x5F);

    // HD6309
    dis6809.acceptCpu(McuType::HD6309);
    TEST(SEXW,  "", 0x14);
    TEST(PSHSW, "", 0x10, 0x38);
    TEST(PULSW, "", 0x10, 0x39);
    TEST(PSHUW, "", 0x10, 0x3A);
    TEST(PULUW, "", 0x10, 0x3B);

    TEST(NEGD, "", 0x10, 0x40);
    TEST(COMD, "", 0x10, 0x43);
    TEST(LSRD, "", 0x10, 0x44);
    TEST(RORD, "", 0x10, 0x46);
    TEST(ASRD, "", 0x10, 0x47);
    TEST(ASLD, "", 0x10, 0x48);
    TEST(ROLD, "", 0x10, 0x49);
    TEST(DECD, "", 0x10, 0x4A);
    TEST(INCD, "", 0x10, 0x4C);
    TEST(TSTD, "", 0x10, 0x4D);
    TEST(CLRD, "", 0x10, 0x4F);

    TEST(COME, "", 0x11, 0x43);
    TEST(DECE, "", 0x11, 0x4A);
    TEST(INCE, "", 0x11, 0x4C);
    TEST(TSTE, "", 0x11, 0x4D);
    TEST(CLRE, "", 0x11, 0x4F);

    TEST(COMF, "", 0x11, 0x53);
    TEST(DECF, "", 0x11, 0x5A);
    TEST(INCF, "", 0x11, 0x5C);
    TEST(TSTF, "", 0x11, 0x5D);
    TEST(CLRF, "", 0x11, 0x5F);
}

static void test_immediate() {
    TEST(ORCC, "#1",   0x1A, 0x01);
    TEST(ORCC, "#$10", 0x1A, 0x10);
    TEST(ORCC, "#$FF", 0x1A, 0xFF);
    TEST(ANDCC,"#$FF", 0x1C, 0xFF);
    TEST(ANDCC,"#$EF", 0x1C, 0xEF);
    TEST(ANDCC,"#$0A", 0x1C, 0x0A);
    TEST(CWAI, "#$FF", 0x3C, 0xFF);
    TEST(CWAI, "#$EF", 0x3C, 0xEF);
    TEST(CWAI, "#0",   0x3C, 0x00);

    TEST(SUBA, "#$90", 0x80, 0x90);
    TEST(CMPA, "#$90", 0x81, 0x90);
    TEST(SBCA, "#$90", 0x82, 0x90);
    TEST(ANDA, "#$90", 0x84, 0x90);
    TEST(BITA, "#$90", 0x85, 0x90);
    TEST(LDA,  "#$90", 0x86, 0x90);
    TEST(EORA, "#$90", 0x88, 0x90);
    TEST(ADCA, "#$90", 0x89, 0x90);
    TEST(ORA,  "#$90", 0x8A, 0x90);
    TEST(ADDA, "#$90", 0x8B, 0x90);

    TEST(SUBB, "#$90", 0xC0, 0x90);
    TEST(CMPB, "#$90", 0xC1, 0x90);
    TEST(SBCB, "#$90", 0xC2, 0x90);
    TEST(ANDB, "#$90", 0xC4, 0x90);
    TEST(BITB, "#$90", 0xC5, 0x90);
    TEST(LDB,  "#$90", 0xC6, 0x90);
    TEST(EORB, "#$90", 0xC8, 0x90);
    TEST(ADCB, "#$90", 0xC9, 0x90);
    TEST(ORB,  "#$90", 0xCA, 0x90);
    TEST(ADDB, "#$90", 0xCB, 0x90);

    TEST(SUBD, "#$90A0", 0x83, 0x90, 0xA0);
    TEST(ADDD, "#$90A0", 0xC3, 0x90, 0xA0);
    TEST(LDD,  "#$90A0", 0xCC, 0x90, 0xA0);
    TEST(CMPD, "#$90A0", 0x10, 0x83, 0x90, 0xA0);

    TEST(CMPX, "#$90A0", 0x8C, 0x90, 0xA0);
    TEST(LDX,  "#$90A0", 0x8E, 0x90, 0xA0);

    TEST(CMPY, "#$90A0", 0x10, 0x8C, 0x90, 0xA0);
    TEST(LDY,  "#$90A0", 0x10, 0x8E, 0x90, 0xA0);

    TEST(CMPU, "#$90A0", 0x11, 0x83, 0x90, 0xA0);
    TEST(LDU,  "#$90A0", 0xCE, 0x90, 0xA0);

    TEST(CMPS, "#$90A0", 0x11, 0x8C, 0x90, 0xA0);
    TEST(LDS,  "#$90A0", 0x10, 0xCE, 0x90, 0xA0);

    // HD6309
    dis6809.acceptCpu(McuType::HD6309);
    TEST(LDMD,  "#1",   0x11, 0x3D, 0x01);
    TEST(BITMD, "#$80", 0x11, 0x3C, 0x80);

    TEST(SBCD, "#$90A0", 0x10, 0x82, 0x90, 0xA0);
    TEST(ANDD, "#$90A0", 0x10, 0x84, 0x90, 0xA0);
    TEST(BITD, "#$90A0", 0x10, 0x85, 0x90, 0xA0);
    TEST(EORD, "#$90A0", 0x10, 0x88, 0x90, 0xA0);
    TEST(ADCD, "#$90A0", 0x10, 0x89, 0x90, 0xA0);
    TEST(ORD,  "#$90A0", 0x10, 0x8A, 0x90, 0xA0);

    TEST(SUBE, "#$90", 0x11, 0x80, 0x90);
    TEST(ADDE, "#$90", 0x11, 0x8B, 0x90);
    TEST(LDE,  "#$90", 0x11, 0x86, 0x90);
    TEST(CMPE, "#$90", 0x11, 0x81, 0x90);

    TEST(SUBF, "#$90", 0x11, 0xC0, 0x90);
    TEST(ADDF, "#$90", 0x11, 0xCB, 0x90);
    TEST(LDF,  "#$90", 0x11, 0xC6, 0x90);
    TEST(CMPF, "#$90", 0x11, 0xC1, 0x90);

    TEST(SUBW, "#$90A0", 0x10, 0x80, 0x90, 0xA0);
    TEST(ADDW, "#$90A0", 0x10, 0x8B, 0x90, 0xA0);
    TEST(LDW,  "#$90A0", 0x10, 0x86, 0x90, 0xA0);
    TEST(CMPW, "#$90A0", 0x10, 0x81, 0x90, 0xA0);

    TEST(LDQ,  "#$12345678", 0xCD, 0x12, 0x34, 0x56, 0x78);

    TEST(MULD, "#$90A0", 0x11, 0x8F, 0x90, 0xA0);
    TEST(DIVD, "#$90A0", 0x11, 0x8D, 0x90, 0xA0);
    TEST(DIVQ, "#$90A0", 0x11, 0x8E, 0x90, 0xA0);

    symtab.intern(0x90, "dir90");
    symtab.intern(0x90A0, "dir90A0");

    dis6809.acceptCpu(McuType::MC6809);
    TEST(LDA,  "#$90", 0x86, 0x90);
    TEST(CMPX, "#dir90A0", 0x8C, 0x90, 0xA0);
    TEST(LDY,  "#dir90A0", 0x10, 0x8E, 0x90, 0xA0);
    TEST(LDS,  "#dir90A0", 0x10, 0xCE, 0x90, 0xA0);
}

static void test_direct() {
    TEST(NEG,  "$00", 0x00, 0x00);
    TEST(COM,  "$09", 0x03, 0x09);
    TEST(LSR,  "$10", 0x04, 0x10);
    TEST(ROR,  "$10", 0x06, 0x10);
    TEST(ASR,  "$10", 0x07, 0x10);
    TEST(ASL,  "$10", 0x08, 0x10);
    TEST(ROL,  "$10", 0x09, 0x10);
    TEST(DEC,  "$10", 0x0A, 0x10);
    TEST(INC,  "$10", 0x0C, 0x10);
    TEST(TST,  "$10", 0x0D, 0x10);
    TEST(CLR,  "$10", 0x0F, 0x10);

    TEST(SUBA, "$90", 0x90, 0x90);
    TEST(CMPA, "$90", 0x91, 0x90);
    TEST(SBCA, "$90", 0x92, 0x90);
    TEST(ANDA, "$90", 0x94, 0x90);
    TEST(BITA, "$90", 0x95, 0x90);
    TEST(LDA,  "$90", 0x96, 0x90);
    TEST(STA,  "$90", 0x97, 0x90);
    TEST(EORA, "$90", 0x98, 0x90);
    TEST(ADCA, "$90", 0x99, 0x90);
    TEST(ORA,  "$90", 0x9A, 0x90);
    TEST(ADDA, "$90", 0x9B, 0x90);

    TEST(SUBB, "$90", 0xD0, 0x90);
    TEST(CMPB, "$90", 0xD1, 0x90);
    TEST(SBCB, "$90", 0xD2, 0x90);
    TEST(ANDB, "$90", 0xD4, 0x90);
    TEST(BITB, "$90", 0xD5, 0x90);
    TEST(LDB,  "$90", 0xD6, 0x90);
    TEST(STB,  "$90", 0xD7, 0x90);
    TEST(EORB, "$90", 0xD8, 0x90);
    TEST(ADCB, "$90", 0xD9, 0x90);
    TEST(ORB,  "$90", 0xDA, 0x90);
    TEST(ADDB, "$90", 0xDB, 0x90);

    TEST(SUBD, "$90", 0x93, 0x90);
    TEST(ADDD, "$90", 0xD3, 0x90);
    TEST(LDD,  "$90", 0xDC, 0x90);
    TEST(STD,  "$90", 0xDD, 0x90);
    TEST(CMPD, "$90", 0x10, 0x93, 0x90);

    TEST(CMPX, "$90", 0x9C, 0x90);
    TEST(LDX,  "$90", 0x9E, 0x90);
    TEST(STX,  "$90", 0x9F, 0x90);

    TEST(CMPY, "$90", 0x10, 0x9C, 0x90);
    TEST(LDY,  "$90", 0x10, 0x9E, 0x90);
    TEST(STY,  "$90", 0x10, 0x9F, 0x90);

    TEST(CMPU, "$90", 0x11, 0x93, 0x90);
    TEST(LDU,  "$90", 0xDE, 0x90);
    TEST(STU,  "$90", 0xDF, 0x90);

    TEST(CMPS, "$90", 0x11, 0x9C, 0x90);
    TEST(LDS,  "$90", 0x10, 0xDE, 0x90);
    TEST(STS,  "$90", 0x10, 0xDF, 0x90);

    TEST(JMP,  "$10", 0x0E, 0x10);
    TEST(JSR,  "$10", 0x9D, 0x10);

    // HD6309
    dis6809.acceptCpu(McuType::HD6309);
    TEST(SBCD, "$90", 0x10, 0x92, 0x90);
    TEST(ANDD, "$90", 0x10, 0x94, 0x90);
    TEST(BITD, "$90", 0x10, 0x95, 0x90);
    TEST(EORD, "$90", 0x10, 0x98, 0x90);
    TEST(ADCD, "$90", 0x10, 0x99, 0x90);
    TEST(ORD,  "$90", 0x10, 0x9A, 0x90);

    TEST(SUBE, "$90", 0x11, 0x90, 0x90);
    TEST(ADDE, "$90", 0x11, 0x9B, 0x90);
    TEST(LDE,  "$90", 0x11, 0x96, 0x90);
    TEST(STE,  "$90", 0x11, 0x97, 0x90);
    TEST(CMPE, "$90", 0x11, 0x91, 0x90);

    TEST(SUBF, "$90", 0x11, 0xD0, 0x90);
    TEST(ADDF, "$90", 0x11, 0xDB, 0x90);
    TEST(LDF,  "$90", 0x11, 0xD6, 0x90);
    TEST(STF,  "$90", 0x11, 0xD7, 0x90);
    TEST(CMPF, "$90", 0x11, 0xD1, 0x90);

    TEST(SUBW, "$90", 0x10, 0x90, 0x90);
    TEST(ADDW, "$90", 0x10, 0x9B, 0x90);
    TEST(LDW,  "$90", 0x10, 0x96, 0x90);
    TEST(STW,  "$90", 0x10, 0x97, 0x90);
    TEST(CMPW, "$90", 0x10, 0x91, 0x90);

    TEST(LDQ,  "$90", 0x10, 0xDC, 0x90);
    TEST(STQ,  "$90", 0x10, 0xDD, 0x90);

    TEST(MULD, "$90", 0x11, 0x9F, 0x90);
    TEST(DIVD, "$90", 0x11, 0x9D, 0x90);
    TEST(DIVQ, "$90", 0x11, 0x9E, 0x90);

    TEST(OIM,  "#$30,$00", 0x01, 0x30, 0x00);
    TEST(AIM,  "#$30,$09", 0x02, 0x30, 0x09);
    TEST(EIM,  "#$30,$10", 0x05, 0x30, 0x10);
    TEST(TIM,  "#$30,$10", 0x0B, 0x30, 0x10);

    symtab.intern(0x10, "dir10");
    symtab.intern(0x22, "dir22");
    symtab.intern(0x90, "dir90");

    dis6809.acceptCpu(McuType::MC6809);
    TEST(NEG,  "<dir10", 0x00, 0x10);
    TEST(LDA,  "<dir90", 0x96, 0x90);
    TEST(STB,  "<dir90", 0xD7, 0x90);
    TEST(CMPX, "<dir22", 0x9C, 0x22);
    TEST(STU,  "<dir22", 0xDF, 0x22);
    TEST(LDY,  "<dir22", 0x10, 0x9E, 0x22);
    TEST(LDS,  "<dir90", 0x10, 0xDE, 0x90);
    TEST(JMP,  "<dir90", 0x0E, 0x90);

    // HD6309
    dis6809.acceptCpu(McuType::HD6309);
    TEST(OIM,  "#$30,<dir10", 0x01, 0x30, 0x10);
}

static void test_extended() {
    TEST(NEG,  "$0000", 0x70, 0x00, 0x00);
    TEST(COM,  "$0009", 0x73, 0x00, 0x09);
    TEST(LSR,  "$0034", 0x74, 0x00, 0x34);
    TEST(ROR,  "$1234", 0x76, 0x12, 0x34);
    TEST(ASR,  "$1234", 0x77, 0x12, 0x34);
    TEST(ASL,  "$1234", 0x78, 0x12, 0x34);
    TEST(ROL,  "$1234", 0x79, 0x12, 0x34);
    TEST(DEC,  "$1234", 0x7A, 0x12, 0x34);
    TEST(INC,  "$1234", 0x7C, 0x12, 0x34);
    TEST(TST,  "$1234", 0x7D, 0x12, 0x34);
    TEST(CLR,  "$1234", 0x7F, 0x12, 0x34);

    TEST(SUBA, "$0090", 0xB0, 0x00, 0x90);
    TEST(CMPA, "$0090", 0xB1, 0x00, 0x90);
    TEST(SBCA, "$0090", 0xB2, 0x00, 0x90);
    TEST(ANDA, "$0090", 0xB4, 0x00, 0x90);
    TEST(BITA, "$0090", 0xB5, 0x00, 0x90);
    TEST(LDA,  "$0090", 0xB6, 0x00, 0x90);
    TEST(STA,  "$0090", 0xB7, 0x00, 0x90);
    TEST(EORA, "$0090", 0xB8, 0x00, 0x90);
    TEST(ADCA, "$0090", 0xB9, 0x00, 0x90);
    TEST(ORA,  "$0090", 0xBA, 0x00, 0x90);
    TEST(ADDA, "$0090", 0xBB, 0x00, 0x90);

    TEST(SUBB, "$9ABC", 0xF0, 0x9A, 0xBC);
    TEST(CMPB, "$9ABC", 0xF1, 0x9A, 0xBC);
    TEST(SBCB, "$9ABC", 0xF2, 0x9A, 0xBC);
    TEST(ANDB, "$9ABC", 0xF4, 0x9A, 0xBC);
    TEST(BITB, "$9ABC", 0xF5, 0x9A, 0xBC);
    TEST(LDB,  "$9ABC", 0xF6, 0x9A, 0xBC);
    TEST(STB,  "$9ABC", 0xF7, 0x9A, 0xBC);
    TEST(EORB, "$9ABC", 0xF8, 0x9A, 0xBC);
    TEST(ADCB, "$9ABC", 0xF9, 0x9A, 0xBC);
    TEST(ORB,  "$9ABC", 0xFA, 0x9A, 0xBC);
    TEST(ADDB, "$9ABC", 0xFB, 0x9A, 0xBC);

    TEST(SUBD, "$9ABC", 0xB3, 0x9A, 0xBC);
    TEST(ADDD, "$9ABC", 0xF3, 0x9A, 0xBC);
    TEST(LDD,  "$9ABC", 0xFC, 0x9A, 0xBC);
    TEST(STD,  "$9ABC", 0xFD, 0x9A, 0xBC);
    TEST(CMPD, "$9ABC", 0x10, 0xB3, 0x9A, 0xBC);

    TEST(CMPX, "$9ABC", 0xBC, 0x9A, 0xBC);
    TEST(LDX,  "$9ABC", 0xBE, 0x9A, 0xBC);
    TEST(STX,  "$9ABC", 0xBF, 0x9A, 0xBC);

    TEST(CMPY, "$9ABC", 0x10, 0xBC, 0x9A, 0xBC);
    TEST(LDY,  "$9ABC", 0x10, 0xBE, 0x9A, 0xBC);
    TEST(STY,  "$9ABC", 0x10, 0xBF, 0x9A, 0xBC);

    TEST(CMPU, "$9ABC", 0x11, 0xB3, 0x9A, 0xBC);
    TEST(LDU,  "$9ABC", 0xFE, 0x9A, 0xBC);
    TEST(STU,  "$9ABC", 0xFF, 0x9A, 0xBC);

    TEST(CMPS, "$9ABC", 0x11, 0xBC, 0x9A, 0xBC);
    TEST(LDS,  "$9ABC", 0x10, 0xFE, 0x9A, 0xBC);
    TEST(STS,  "$9ABC", 0x10, 0xFF, 0x9A, 0xBC);

    TEST(JMP,  "$1234", 0x7E, 0x12, 0x34);
    TEST(JSR,  "$1234", 0xBD, 0x12, 0x34);

    // HD6309
    dis6809.acceptCpu(McuType::HD6309);
    TEST(SBCD, "$9ABC", 0x10, 0xB2, 0x9A, 0xBC);
    TEST(ANDD, "$9ABC", 0x10, 0xB4, 0x9A, 0xBC);
    TEST(BITD, "$9ABC", 0x10, 0xB5, 0x9A, 0xBC);
    TEST(EORD, "$9ABC", 0x10, 0xB8, 0x9A, 0xBC);
    TEST(ADCD, "$9ABC", 0x10, 0xB9, 0x9A, 0xBC);
    TEST(ORD,  "$9ABC", 0x10, 0xBA, 0x9A, 0xBC);

    TEST(SUBE, "$9ABC", 0x11, 0xB0, 0x9A, 0xBC);
    TEST(ADDE, "$9ABC", 0x11, 0xBB, 0x9A, 0xBC);
    TEST(LDE,  "$9ABC", 0x11, 0xB6, 0x9A, 0xBC);
    TEST(STE,  "$9ABC", 0x11, 0xB7, 0x9A, 0xBC);
    TEST(CMPE, "$9ABC", 0x11, 0xB1, 0x9A, 0xBC);

    TEST(SUBF, "$9ABC", 0x11, 0xF0, 0x9A, 0xBC);
    TEST(ADDF, "$9ABC", 0x11, 0xFB, 0x9A, 0xBC);
    TEST(LDF,  "$9ABC", 0x11, 0xF6, 0x9A, 0xBC);
    TEST(STF,  "$9ABC", 0x11, 0xF7, 0x9A, 0xBC);
    TEST(CMPF, "$9ABC", 0x11, 0xF1, 0x9A, 0xBC);

    TEST(SUBW, "$9ABC", 0x10, 0xB0, 0x9A, 0xBC);
    TEST(ADDW, "$9ABC", 0x10, 0xBB, 0x9A, 0xBC);
    TEST(LDW,  "$9ABC", 0x10, 0xB6, 0x9A, 0xBC);
    TEST(STW,  "$9ABC", 0x10, 0xB7, 0x9A, 0xBC);
    TEST(CMPW, "$9ABC", 0x10, 0xB1, 0x9A, 0xBC);

    TEST(LDQ,  "$9ABC", 0x10, 0xFC, 0x9A, 0xBC);
    TEST(STQ,  "$9ABC", 0x10, 0xFD, 0x9A, 0xBC);

    TEST(MULD, "$9ABC", 0x11, 0xBF, 0x9A, 0xBC);
    TEST(DIVD, "$9ABC", 0x11, 0xBD, 0x9A, 0xBC);
    TEST(DIVQ, "$9ABC", 0x11, 0xBE, 0x9A, 0xBC);

    TEST(OIM,  "#$30,$9ABC", 0x71, 0x30, 0x9A, 0xBC);
    TEST(AIM,  "#$30,$9ABC", 0x72, 0x30, 0x9A, 0xBC);
    TEST(EIM,  "#$30,$9ABC", 0x75, 0x30, 0x9A, 0xBC);
    TEST(TIM,  "#$30,$9ABC", 0x7B, 0x30, 0x9A, 0xBC);

    symtab.intern(0x0090, "ext0090");
    symtab.intern(0x9ABC, "ext9ABC");

    dis6809.acceptCpu(McuType::MC6809);
    TEST(NEG,  ">ext0090", 0x70, 0x00, 0x90);
    TEST(LDA,  "ext9ABC",  0xB6, 0x9A, 0xBC);
    TEST(STB,  ">ext0090", 0xF7, 0x00, 0x90);
    TEST(CMPX, "ext9ABC",  0xBC, 0x9A, 0xBC);
    TEST(STU,  ">ext0090", 0xFF, 0x00, 0x90);
    TEST(LDY,  "ext9ABC",  0x10, 0xBE, 0x9A, 0xBC);
    TEST(LDS,  ">ext0090", 0x10, 0xFE, 0x00, 0x90);
    TEST(JMP,  "ext9ABC",  0x7E, 0x9A, 0xBC);
    TEST(JSR,  ">ext0090", 0xBD, 0x00, 0x90);

    // HD6309
    dis6809.acceptCpu(McuType::HD6309);
    TEST(OIM,  "#$30,ext9ABC",  0x71, 0x30, 0x9A, 0xBC);
    TEST(OIM,  "#$30,>ext0090", 0x71, 0x30, 0x00, 0x90);
}

static void test_indexed() {
    TEST(LEAX, ",Y", 0x30, 0xA4);
    TEST(LEAY, ",U", 0x31, 0xC4);
    TEST(LEAU, ",S", 0x33, 0xE4);
    TEST(LEAS, ",X", 0x32, 0x84);

    TEST(NEG,  ",X", 0x60, 0x84);
    TEST(COM,  ",X", 0x63, 0x84);
    TEST(LSR,  ",X", 0x64, 0x84);
    TEST(ROR,  ",X", 0x66, 0x84);
    TEST(ASR,  ",X", 0x67, 0x84);
    TEST(ASL,  ",X", 0x68, 0x84);
    TEST(ROL,  ",X", 0x69, 0x84);
    TEST(DEC,  ",X", 0x6A, 0x84);
    TEST(INC,  ",X", 0x6C, 0x84);
    TEST(TST,  ",X", 0x6D, 0x84);
    TEST(CLR,  ",X", 0x6F, 0x84);

    TEST(SUBA, ",Y", 0xA0, 0xA4);
    TEST(CMPA, ",Y", 0xA1, 0xA4);
    TEST(SBCA, ",Y", 0xA2, 0xA4);
    TEST(ANDA, ",Y", 0xA4, 0xA4);
    TEST(BITA, ",Y", 0xA5, 0xA4);
    TEST(LDA,  ",Y", 0xA6, 0xA4);
    TEST(STA,  ",Y", 0xA7, 0xA4);
    TEST(EORA, ",Y", 0xA8, 0xA4);
    TEST(ADCA, ",Y", 0xA9, 0xA4);
    TEST(ORA,  ",Y", 0xAA, 0xA4);
    TEST(ADDA, ",Y", 0xAB, 0xA4);

    TEST(SUBB, ",U", 0xE0, 0xC4);
    TEST(CMPB, ",U", 0xE1, 0xC4);
    TEST(SBCB, ",U", 0xE2, 0xC4);
    TEST(ANDB, ",U", 0xE4, 0xC4);
    TEST(BITB, ",U", 0xE5, 0xC4);
    TEST(LDB,  ",U", 0xE6, 0xC4);
    TEST(STB,  ",U", 0xE7, 0xC4);
    TEST(EORB, ",U", 0xE8, 0xC4);
    TEST(ADCB, ",U", 0xE9, 0xC4);
    TEST(ORB,  ",U", 0xEA, 0xC4);
    TEST(ADDB, ",U", 0xEB, 0xC4);

    TEST(SUBD, ",S", 0xA3, 0xE4);
    TEST(ADDD, ",S", 0xE3, 0xE4);
    TEST(LDD,  ",S", 0xEC, 0xE4);
    TEST(STD,  ",S", 0xED, 0xE4);
    TEST(CMPD, ",S", 0x10, 0xA3, 0xE4);

    TEST(CMPX, "2,Y", 0xAC, 0x22);
    TEST(LDX,  "15,Y", 0xAE, 0x2F);
    TEST(STX,  "-16,Y", 0xAF, 0x30);

    TEST(CMPY, "-2,X", 0x10, 0xAC, 0x1E);
    TEST(LDY,  "15,X", 0x10, 0xAE, 0x0F);
    TEST(STY,  "-16,X", 0x10, 0xAF, 0x10);

    TEST(CMPU, ",X++", 0x11, 0xA3, 0x81);
    TEST(LDU,  ",X++", 0xEE, 0x81);
    TEST(STU,  ",X++", 0xEF, 0x81);

    TEST(CMPS, ",--Y", 0x11, 0xAC, 0xA3);
    TEST(LDS,  ",--Y", 0x10, 0xEE, 0xA3);
    TEST(STS,  ",--Y", 0x10, 0xEF, 0xA3);

    TEST(JMP,  "[,X]", 0x6E, 0x94);
    TEST(JSR,  "[,X++]", 0xAD, 0x91);

    // HD6309
    dis6809.acceptCpu(McuType::HD6309);
    TEST(SBCD, ",W", 0x10, 0xA2, 0x8F);
    TEST(ANDD, ",W++", 0x10, 0xA4, 0xCF);
    TEST(BITD, ",--W", 0x10, 0xA5, 0xEF);
    TEST(EORD, "[,W]", 0x10, 0xA8, 0x90);
    TEST(ADCD, "[,W++]", 0x10, 0xA9, 0xD0);
    TEST(ORD,  "[,--W]", 0x10, 0xAA, 0xF0);

    TEST(SUBE, ",X", 0x11, 0xA0, 0x84);
    TEST(ADDE, ",X", 0x11, 0xAB, 0x84);
    TEST(LDE,  ",X", 0x11, 0xA6, 0x84);
    TEST(STE,  ",X", 0x11, 0xA7, 0x84);
    TEST(CMPE, ",X", 0x11, 0xA1, 0x84);

    TEST(SUBF, ",X", 0x11, 0xE0, 0x84);
    TEST(ADDF, ",X", 0x11, 0xEB, 0x84);
    TEST(LDF,  ",X", 0x11, 0xE6, 0x84);
    TEST(STF,  ",X", 0x11, 0xE7, 0x84);
    TEST(CMPF, ",X", 0x11, 0xE1, 0x84);

    TEST(SUBW, ",X", 0x10, 0xA0, 0x84);
    TEST(CMPW, ",X", 0x10, 0xA1, 0x84);
    TEST(LDW,  ",X", 0x10, 0xA6, 0x84);
    TEST(STW,  ",X", 0x10, 0xA7, 0x84);
    TEST(ADDW, ",X", 0x10, 0xAB, 0x84);

    TEST(LDQ,  ",X", 0x10, 0xEC, 0x84);
    TEST(STQ,  ",X", 0x10, 0xED, 0x84);

    TEST(MULD, ",X",   0x11, 0xAF, 0x84);
    TEST(DIVD, "-1,X", 0x11, 0xAD, 0x1F);
    TEST(DIVQ, "1,X",  0x11, 0xAE, 0x01);

    TEST(OIM,  "#$30,,X", 0x61, 0x30, 0x84);
    TEST(AIM,  "#$30,,X+", 0x62, 0x30, 0x80);
    TEST(EIM,  "#$30,[,--X]", 0x65, 0x30, 0x93);
    TEST(TIM,  "#$30,-2,X", 0x6B, 0x30, 0x1E);
}

static void test_indexed_mode() {
    TEST(LDA, ",X", 0xA6, 0x84);
    TEST(LDA, ",Y", 0xA6, 0xA4);
    TEST(LDA, ",U", 0xA6, 0xC4);
    TEST(LDA, ",S", 0xA6, 0xE4);
    TEST(LDA, "[,X]", 0xA6, 0x94);
    TEST(LDA, "[,Y]", 0xA6, 0xB4);
    TEST(LDA, "[,U]", 0xA6, 0xD4);
    TEST(LDA, "[,S]", 0xA6, 0xF4);

    TEST(LDA, "0,X",  0xA6, 0x00);
    TEST(LDA, "0,Y",  0xA6, 0x20);
    TEST(LDA, "0,U",  0xA6, 0x40);
    TEST(LDA, "0,S",  0xA6, 0x60);
    TEST(LDA, "-1,X", 0xA6, 0x1F);
    TEST(LDA, "-1,Y", 0xA6, 0x3F);
    TEST(LDA, "-1,U", 0xA6, 0x5F);
    TEST(LDA, "-1,S", 0xA6, 0x7F);
    TEST(LDA, "15,X", 0xA6, 0x0F);
    TEST(LDA, "15,Y", 0xA6, 0x2F);
    TEST(LDA, "15,U", 0xA6, 0x4F);
    TEST(LDA, "15,S", 0xA6, 0x6F);
    TEST(LDA, "-16,X", 0xA6, 0x10);
    TEST(LDA, "-16,Y", 0xA6, 0x30);
    TEST(LDA, "-16,U", 0xA6, 0x50);
    TEST(LDA, "-16,S", 0xA6, 0x70);

    TEST(LDA, "0,X",     0xA6, 0x88, 0x00);
    TEST(LDA, "127,X",   0xA6, 0x88, 0x7F);
    TEST(LDA, "-128,X",  0xA6, 0x88, 0x80);
    TEST(LDA, "[0,X]",   0xA6, 0x98, 0x00);
    TEST(LDA, "[127,X]",  0xA6, 0x98, 0x7F);
    TEST(LDA, "[-128,X]", 0xA6, 0x98, 0x80);
    TEST(LDA, "0,Y",    0xA6, 0xA8, 0x00);
    TEST(LDA, "127,Y",  0xA6, 0xA8, 0x7F);
    TEST(LDA, "-128,Y", 0xA6, 0xA8, 0x80);
    TEST(LDA, "[0,Y]",    0xA6, 0xB8, 0x00);
    TEST(LDA, "[127,Y]",  0xA6, 0xB8, 0x7F);
    TEST(LDA, "[-128,Y]", 0xA6, 0xB8, 0x80);
    TEST(LDA, "0,U",    0xA6, 0xC8, 0x00);
    TEST(LDA, "127,U",  0xA6, 0xC8, 0x7F);
    TEST(LDA, "-128,U", 0xA6, 0xC8, 0x80);
    TEST(LDA, "[0,U]",    0xA6, 0xD8, 0x00);
    TEST(LDA, "[127,U]",  0xA6, 0xD8, 0x7F);
    TEST(LDA, "[-128,U]", 0xA6, 0xD8, 0x80);
    TEST(LDA, "0,S",    0xA6, 0xE8, 0x00);
    TEST(LDA, "127,S",  0xA6, 0xE8, 0x7F);
    TEST(LDA, "-128,S", 0xA6, 0xE8, 0x80);
    TEST(LDA, "[0,S]",    0xA6, 0xF8, 0x00);
    TEST(LDA, "[127,S]",  0xA6, 0xF8, 0x7F);
    TEST(LDA, "[-128,S]", 0xA6, 0xF8, 0x80);

    TEST(LDA, "0,X",      0xA6, 0x89, 0x00, 0x00);
    TEST(LDA, "32767,X",  0xA6, 0x89, 0x7F, 0xFF);
    TEST(LDA, "-32768,X", 0xA6, 0x89, 0x80, 0x00);
    TEST(LDA, "[0,X]",      0xA6, 0x99, 0x00, 0x00);
    TEST(LDA, "[32767,X]",  0xA6, 0x99, 0x7F, 0xFF);
    TEST(LDA, "[-32768,X]", 0xA6, 0x99, 0x80, 0x00);
    TEST(LDA, "0,Y",      0xA6, 0xA9, 0x00, 0x00);
    TEST(LDA, "32767,Y",  0xA6, 0xA9, 0x7F, 0xFF);
    TEST(LDA, "-32768,Y", 0xA6, 0xA9, 0x80, 0x00);
    TEST(LDA, "[0,Y]",      0xA6, 0xB9, 0x00, 0x00);
    TEST(LDA, "[32767,Y]",  0xA6, 0xB9, 0x7F, 0xFF);
    TEST(LDA, "[-32768,Y]", 0xA6, 0xB9, 0x80, 0x00);
    TEST(LDA, "0,U",      0xA6, 0xC9, 0x00, 0x00);
    TEST(LDA, "32767,U",  0xA6, 0xC9, 0x7F, 0xFF);
    TEST(LDA, "-32768,U", 0xA6, 0xC9, 0x80, 0x00);
    TEST(LDA, "[0,U]",      0xA6, 0xD9, 0x00, 0x00);
    TEST(LDA, "[32767,U]",  0xA6, 0xD9, 0x7F, 0xFF);
    TEST(LDA, "[-32768,U]", 0xA6, 0xD9, 0x80, 0x00);
    TEST(LDA, "0,S",      0xA6, 0xE9, 0x00, 0x00);
    TEST(LDA, "32767,S",  0xA6, 0xE9, 0x7F, 0xFF);
    TEST(LDA, "-32768,S", 0xA6, 0xE9, 0x80, 0x00);
    TEST(LDA, "[0,S]",      0xA6, 0xF9, 0x00, 0x00);
    TEST(LDA, "[32767,S]",  0xA6, 0xF9, 0x7F, 0xFF);
    TEST(LDA, "[-32768,S]", 0xA6, 0xF9, 0x80, 0x00);

    TEST(LDA, "A,X", 0xA6, 0x86);
    TEST(LDA, "B,X", 0xA6, 0x85);
    TEST(LDA, "D,X", 0xA6, 0x8B);
    TEST(LDA, "[A,X]", 0xA6, 0x96);
    TEST(LDA, "[B,X]", 0xA6, 0x95);
    TEST(LDA, "[D,X]", 0xA6, 0x9B);

    TEST(LDA, "A,Y", 0xA6, 0xA6);
    TEST(LDA, "B,Y", 0xA6, 0xA5);
    TEST(LDA, "D,Y", 0xA6, 0xAB);
    TEST(LDA, "[A,Y]", 0xA6, 0xB6);
    TEST(LDA, "[B,Y]", 0xA6, 0xB5);
    TEST(LDA, "[D,Y]", 0xA6, 0xBB);

    TEST(LDA, "A,U", 0xA6, 0xC6);
    TEST(LDA, "B,U", 0xA6, 0xC5);
    TEST(LDA, "D,U", 0xA6, 0xCB);
    TEST(LDA, "[A,U]", 0xA6, 0xD6);
    TEST(LDA, "[B,U]", 0xA6, 0xD5);
    TEST(LDA, "[D,U]", 0xA6, 0xDB);

    TEST(LDA, "A,S", 0xA6, 0xE6);
    TEST(LDA, "B,S", 0xA6, 0xE5);
    TEST(LDA, "D,S", 0xA6, 0xEB);
    TEST(LDA, "[A,S]", 0xA6, 0xF6);
    TEST(LDA, "[B,S]", 0xA6, 0xF5);
    TEST(LDA, "[D,S]", 0xA6, 0xFB);

    TEST(LDA, ",X+",    0xA6, 0x80);
    TEST(LDA, ",X++",   0xA6, 0x81);
    TEST(LDA, ",-X",    0xA6, 0x82);
    TEST(LDA, ",--X",   0xA6, 0x83);
    TEST(LDA, "[,X++]", 0xA6, 0x91);
    TEST(LDA, "[,--X]", 0xA6, 0x93);

    TEST(LDA, ",Y+",    0xA6, 0xA0);
    TEST(LDA, ",Y++",   0xA6, 0xA1);
    TEST(LDA, ",-Y",    0xA6, 0xA2);
    TEST(LDA, ",--Y",   0xA6, 0xA3);
    TEST(LDA, "[,Y++]", 0xA6, 0xB1);
    TEST(LDA, "[,--Y]", 0xA6, 0xB3);

    TEST(LDA, ",U+",    0xA6, 0xC0);
    TEST(LDA, ",U++",   0xA6, 0xC1);
    TEST(LDA, ",-U",    0xA6, 0xC2);
    TEST(LDA, ",--U",   0xA6, 0xC3);
    TEST(LDA, "[,U++]", 0xA6, 0xD1);
    TEST(LDA, "[,--U]", 0xA6, 0xD3);

    TEST(LDA, ",S+",    0xA6, 0xE0);
    TEST(LDA, ",S++",   0xA6, 0xE1);
    TEST(LDA, ",-S",    0xA6, 0xE2);
    TEST(LDA, ",--S",   0xA6, 0xE3);
    TEST(LDA, "[,S++]", 0xA6, 0xF1);
    TEST(LDA, "[,--S]", 0xA6, 0xF3);

    ATEST(0x1000, LDA, "$1003,PCR",   0xA6, 0x8C, 0x00);
    ATEST(0x1000, LDA, "$1082,PCR",   0xA6, 0x8C, 0x7F);
    ATEST(0x1000, LDA, "$0F83,PCR",   0xA6, 0x8C, 0x80);
    ATEST(0x1000, LDA, "[$1003,PCR]", 0xA6, 0x9C, 0x00);
    ATEST(0x1000, LDA, "[$1082,PCR]", 0xA6, 0x9C, 0x7F);
    ATEST(0x1000, LDA, "[$0F83,PCR]", 0xA6, 0x9C, 0x80);

    ATEST(0x1000, LDA, "$1004,PCR",   0xA6, 0x8D, 0x00, 0x00);
    ATEST(0x1000, LDA, "$9003,PCR",   0xA6, 0x8D, 0x7F, 0xFF);
    ATEST(0x1000, LDA, "$9004,PCR",   0xA6, 0x8D, 0x80, 0x00);
    ATEST(0x1000, LDA, "[$1004,PCR]", 0xA6, 0x9D, 0x00, 0x00);
    ATEST(0x1000, LDA, "[$9003,PCR]", 0xA6, 0x9D, 0x7F, 0xFF);
    ATEST(0x1000, LDA, "[$9004,PCR]", 0xA6, 0x9D, 0x80, 0x00);

    TEST(LDA, "[$0009]", 0xA6, 0x9F, 0x00, 0x09);
    TEST(LDA, "[$1234]", 0xA6, 0x9F, 0x12, 0x34);

    // HD6309
    dis6809.acceptCpu(McuType::HD6309);
    TEST(LDA, "E,X", 0xA6, 0x87);
    TEST(LDA, "F,X", 0xA6, 0x8A);
    TEST(LDA, "W,X", 0xA6, 0x8E);
    TEST(LDA, "[E,X]", 0xA6, 0x97);
    TEST(LDA, "[F,X]", 0xA6, 0x9A);
    TEST(LDA, "[W,X]", 0xA6, 0x9E);

    TEST(LDA, "E,Y", 0xA6, 0xA7);
    TEST(LDA, "F,Y", 0xA6, 0xAA);
    TEST(LDA, "W,Y", 0xA6, 0xAE);
    TEST(LDA, "[E,Y]", 0xA6, 0xB7);
    TEST(LDA, "[F,Y]", 0xA6, 0xBA);
    TEST(LDA, "[W,Y]", 0xA6, 0xBE);

    TEST(LDA, "E,U", 0xA6, 0xC7);
    TEST(LDA, "F,U", 0xA6, 0xCA);
    TEST(LDA, "W,U", 0xA6, 0xCE);
    TEST(LDA, "[E,U]", 0xA6, 0xD7);
    TEST(LDA, "[F,U]", 0xA6, 0xDA);
    TEST(LDA, "[W,U]", 0xA6, 0xDE);

    TEST(LDA, "E,S", 0xA6, 0xE7);
    TEST(LDA, "F,S", 0xA6, 0xEA);
    TEST(LDA, "W,S", 0xA6, 0xEE);
    TEST(LDA, "[E,S]", 0xA6, 0xF7);
    TEST(LDA, "[F,S]", 0xA6, 0xFA);
    TEST(LDA, "[W,S]", 0xA6, 0xFE);

    TEST(LDA, ",W",  0xA6, 0x8F);
    TEST(LDA, "0,W", 0xA6, 0xAF, 0x00, 0x00);
    TEST(LDA, "32767,W",  0xA6, 0xAF, 0x7F, 0xFF);
    TEST(LDA, "-32768,W", 0xA6, 0xAF, 0x80, 0x00);
    TEST(LDA, ",W++", 0xA6, 0xCF);
    TEST(LDA, ",--W", 0xA6, 0xEF);
    TEST(LDA, "[,W]",  0xA6, 0x90);
    TEST(LDA, "[0,W]", 0xA6, 0xB0, 0x00, 0x00);
    TEST(LDA, "[32767,W]",  0xA6, 0xB0, 0x7F, 0xFF);
    TEST(LDA, "[-32768,W]", 0xA6, 0xB0, 0x80, 0x00);
    TEST(LDA, "[,W++]", 0xA6, 0xD0);
    TEST(LDA, "[,--W]", 0xA6, 0xF0);

    symtab.intern(0x0F83, "label0F83");
    symtab.intern(0x1003, "label1003");
    symtab.intern(0x1004, "label1004");
    symtab.intern(0x1082, "label1082");
    symtab.intern(0x1234, "label1234");
    symtab.intern(0x9003, "label9003");
    symtab.intern(0x9004, "label9004");

    dis6809.acceptCpu(McuType::MC6809);
    ATEST(0x1000, LDA, "label1003,PCR",   0xA6, 0x8C, 0x00);
    ATEST(0x1000, LDA, "label1082,PCR",   0xA6, 0x8C, 0x7F);
    ATEST(0x1000, LDA, "label0F83,PCR",   0xA6, 0x8C, 0x80);
    ATEST(0x1000, LDA, "[label1003,PCR]", 0xA6, 0x9C, 0x00);
    ATEST(0x1000, LDA, "[label1082,PCR]", 0xA6, 0x9C, 0x7F);
    ATEST(0x1000, LDA, "[label0F83,PCR]", 0xA6, 0x9C, 0x80);

    ATEST(0x1000, LDA, "label1004,PCR",   0xA6, 0x8D, 0x00, 0x00);
    ATEST(0x1000, LDA, "label9003,PCR",   0xA6, 0x8D, 0x7F, 0xFF);
    ATEST(0x1000, LDA, "label9004,PCR",   0xA6, 0x8D, 0x80, 0x00);
    ATEST(0x1000, LDA, "[label1004,PCR]", 0xA6, 0x9D, 0x00, 0x00);
    ATEST(0x1000, LDA, "[label9003,PCR]", 0xA6, 0x9D, 0x7F, 0xFF);
    ATEST(0x1000, LDA, "[label9004,PCR]", 0xA6, 0x9D, 0x80, 0x00);

    ATEST(0x1000, LDA, "[label1234]",    0xA6, 0x9F, 0x12, 0x34);
}

static void test_relative() {
    ATEST(0x1000, BRA, "$1002", 0x20, 0x00);
    ATEST(0x1000, BRN, "$1000", 0x21, 0xFE);
    ATEST(0x1000, BHI, "$1004", 0x22, 0x02);
    ATEST(0x1000, BLS, "$1002", 0x23, 0x00);
    ATEST(0x1000, BHS, "$1002", 0x24, 0x00);
    ATEST(0x1000, BLO, "$1002", 0x25, 0x00);
    ATEST(0x1000, BNE, "$1002", 0x26, 0x00);
    ATEST(0x1000, BEQ, "$1002", 0x27, 0x00);
    ATEST(0x1000, BVC, "$1002", 0x28, 0x00);
    ATEST(0x1000, BVS, "$1002", 0x29, 0x00);
    ATEST(0x1000, BPL, "$1002", 0x2A, 0x00);
    ATEST(0x1000, BMI, "$1002", 0x2B, 0x00);
    ATEST(0x1000, BGE, "$1002", 0x2C, 0x00);
    ATEST(0x1000, BLT, "$1002", 0x2D, 0x00);
    ATEST(0x1000, BGT, "$1002", 0x2E, 0x00);
    ATEST(0x1000, BLE, "$1002", 0x2F, 0x00);

    ATEST(0x1000, LBRA, "$1003", 0x16, 0x00, 0x00);
    ATEST(0x1000, LBRN, "$1000", 0x10, 0x21, 0xFF, 0xFC);
    ATEST(0x1000, LBHI, "$1008", 0x10, 0x22, 0x00, 0x04);
    ATEST(0x1000, LBLS, "$1004", 0x10, 0x23, 0x00, 0x00);
    ATEST(0x1000, LBHS, "$1004", 0x10, 0x24, 0x00, 0x00);
    ATEST(0x1000, LBLO, "$1004", 0x10, 0x25, 0x00, 0x00);
    ATEST(0x1000, LBNE, "$1004", 0x10, 0x26, 0x00, 0x00);
    ATEST(0x1000, LBEQ, "$1004", 0x10, 0x27, 0x00, 0x00);
    ATEST(0x1000, LBVC, "$1004", 0x10, 0x28, 0x00, 0x00);
    ATEST(0x1000, LBVS, "$1004", 0x10, 0x29, 0x00, 0x00);
    ATEST(0x1000, LBPL, "$1004", 0x10, 0x2A, 0x00, 0x00);
    ATEST(0x1000, LBMI, "$1004", 0x10, 0x2B, 0x00, 0x00);
    ATEST(0x1000, LBGE, "$1004", 0x10, 0x2C, 0x00, 0x00);
    ATEST(0x1000, LBLT, "$1004", 0x10, 0x2D, 0x00, 0x00);
    ATEST(0x1000, LBGT, "$1004", 0x10, 0x2E, 0x00, 0x00);
    ATEST(0x1000, LBLE, "$1004", 0x10, 0x2F, 0x00, 0x00);

    ATEST(0x1000, BSR,  "$1042", 0x8D, 0x40);
    ATEST(0x1000, LBSR, "$1043", 0x17, 0x00, 0x40);

    symtab.intern(0x0F82, "sub0F82");
    symtab.intern(0x1081, "sub1081");
    symtab.intern(0x9002, "sub9002");
    symtab.intern(0x9003, "sub9003");

    ATEST(0x1000, BSR,  "sub1081", 0x8D, 0x7F);
    ATEST(0x1000, BSR,  "sub0F82", 0x8D, 0x80);
    ATEST(0x1000, LBSR, "sub9002", 0x17, 0x7F, 0xFF);
    ATEST(0x1000, LBSR, "sub9003", 0x17, 0x80, 0x00);
}

static void test_stack() {
    TEST(PSHS, "A", 0x34, 0x02);
    TEST(PULS, "A", 0x35, 0x02);
    TEST(PSHU, "A", 0x36, 0x02);
    TEST(PULU, "A", 0x37, 0x02);

    TEST(PSHS, "CC", 0x34, 0x01);
    TEST(PSHS, "A",  0x34, 0x02);
    TEST(PSHS, "B",  0x34, 0x04);
    TEST(PSHS, "DP", 0x34, 0x08);
    TEST(PSHS, "X",  0x34, 0x10);
    TEST(PSHS, "Y",  0x34, 0x20);
    TEST(PSHS, "U",  0x34, 0x40);
    TEST(PSHS, "PC", 0x34, 0x80);

    TEST(PULU, "CC", 0x37, 0x01);
    TEST(PULU, "A",  0x37, 0x02);
    TEST(PULU, "B",  0x37, 0x04);
    TEST(PULU, "DP", 0x37, 0x08);
    TEST(PULU, "X",  0x37, 0x10);
    TEST(PULU, "Y",  0x37, 0x20);
    TEST(PULU, "S",  0x37, 0x40);
    TEST(PULU, "PC", 0x37, 0x80);

    TEST(PULS, "CC,A,B,DP,X,Y,U,PC", 0x35, 0xFF);
    TEST(PSHU, "PC,S,Y,X,DP,B,A,CC", 0x36, 0xFF);

    TEST(PSHS, "#$00", 0x34, 0x00);
}

static void test_register() {
    TEST(EXG, "A,B", 0x1E, 0x89);
    TEST(TFR, "X,Y", 0x1F, 0x12);

    TEST(TFR, "A,B",  0x1F, 0x89);
    TEST(TFR, "A,CC", 0x1F, 0x8A);
    TEST(TFR, "A,DP", 0x1F, 0x8B);
    TEST(TFR, "B,A",  0x1F, 0x98);
    TEST(TFR, "CC,A", 0x1F, 0xA8);
    TEST(TFR, "DP,A", 0x1F, 0xB8);

    TEST(TFR, "D,X",  0x1F, 0x01);
    TEST(TFR, "D,Y",  0x1F, 0x02);
    TEST(TFR, "D,U",  0x1F, 0x03);
    TEST(TFR, "D,S",  0x1F, 0x04);
    TEST(TFR, "D,PC", 0x1F, 0x05);
    TEST(TFR, "X,D",  0x1F, 0x10);
    TEST(TFR, "Y,D",  0x1F, 0x20);
    TEST(TFR, "U,D",  0x1F, 0x30);
    TEST(TFR, "S,D",  0x1F, 0x40);
    TEST(TFR, "PC,D", 0x1F, 0x50);

    // HD6309
    dis6809.acceptCpu(McuType::HD6309);
    TEST(ADDR, "A,B", 0x10, 0x30, 0x89);
    TEST(ADCR, "A,B", 0x10, 0x31, 0x89);
    TEST(SUBR, "A,B", 0x10, 0x32, 0x89);
    TEST(SBCR, "A,B", 0x10, 0x33, 0x89);
    TEST(ANDR, "A,B", 0x10, 0x34, 0x89);
    TEST(ORR,  "A,B", 0x10, 0x35, 0x89);
    TEST(EORR, "A,B", 0x10, 0x36, 0x89);
    TEST(CMPR, "A,B", 0x10, 0x37, 0x89);

    TEST(TFR, "A,0", 0x1F, 0x8C);
    TEST(TFR, "A,0", 0x1F, 0x8D);
    TEST(TFR, "A,E", 0x1F, 0x8E);
    TEST(TFR, "A,F", 0x1F, 0x8F);
    TEST(TFR, "0,A", 0x1F, 0xC8);
    TEST(TFR, "0,A", 0x1F, 0xD8);
    TEST(TFR, "E,A", 0x1F, 0xE8);
    TEST(TFR, "F,A", 0x1F, 0xF8);

    TEST(TFR, "D,W", 0x1F, 0x06);
    TEST(TFR, "D,V", 0x1F, 0x07);
    TEST(TFR, "W,D", 0x1F, 0x60);
    TEST(TFR, "V,D", 0x1F, 0x70);
}

static void test_transfer() {
    dis6809.acceptCpu(McuType::HD6309);
    TEST(TFM, "X+,Y+", 0x11, 0x38, 0x12);
    TEST(TFM, "X-,Y-", 0x11, 0x39, 0x12);
    TEST(TFM, "X+,Y",  0x11, 0x3A, 0x12);
    TEST(TFM, "X,Y+",  0x11, 0x3B, 0x12);

    TEST(TFM, "D+,X+", 0x11, 0x38, 0x01);
    TEST(TFM, "Y+,U+", 0x11, 0x38, 0x23);
    TEST(TFM, "U+,S+", 0x11, 0x38, 0x34);
    TEST(TFM, "S+,D+", 0x11, 0x38, 0x40);
}

static void test_bit_position() {
    dis6809.acceptCpu(McuType::HD6309);
    TEST(BAND,  "A.1,$34.2", 0x11, 0x30, 0x51, 0x34);
    TEST(BIAND, "A.1,$34.2", 0x11, 0x31, 0x51, 0x34);
    TEST(BOR,   "A.1,$34.2", 0x11, 0x32, 0x51, 0x34);
    TEST(BIOR,  "A.1,$34.2", 0x11, 0x33, 0x51, 0x34);
    TEST(BEOR,  "A.1,$34.2", 0x11, 0x34, 0x51, 0x34);
    TEST(BIEOR, "A.1,$34.2", 0x11, 0x35, 0x51, 0x34);
    TEST(LDBT,  "A.1,$34.2", 0x11, 0x36, 0x51, 0x34);
    TEST(STBT,  "A.1,$34.2", 0x11, 0x37, 0x51, 0x34);

    TEST(LDBT, "CC.0,$34.7", 0x11, 0x36, 0x38, 0x34);
    TEST(LDBT, "CC.1,$34.7", 0x11, 0x36, 0x39, 0x34);
    TEST(LDBT, "CC.2,$34.7", 0x11, 0x36, 0x3A, 0x34);
    TEST(LDBT, "CC.3,$34.7", 0x11, 0x36, 0x3B, 0x34);
    TEST(LDBT, "CC.4,$34.7", 0x11, 0x36, 0x3C, 0x34);
    TEST(LDBT, "CC.5,$34.7", 0x11, 0x36, 0x3D, 0x34);
    TEST(LDBT, "CC.6,$34.7", 0x11, 0x36, 0x3E, 0x34);
    TEST(LDBT, "CC.7,$34.7", 0x11, 0x36, 0x3F, 0x34);
    TEST(LDBT, "A.7,$34.0",  0x11, 0x36, 0x47, 0x34);
    TEST(LDBT, "B.2,$34.4",  0x11, 0x36, 0xA2, 0x34);

    symtab.intern(0x0034, "dir34");

    TEST(LDBT, "CC.0,<dir34.7", 0x11, 0x36, 0x38, 0x34);
    TEST(LDBT, "B.2,<dir34.4",  0x11, 0x36, 0xA2, 0x34);
}

static void assert_illegal(uint8_t opc, uint8_t prefix = 0) {
    char operands[40];
    Insn insn;
    const uint8_t codes[] = { prefix, opc };
    if (prefix == 0) {
        memory.setBytes(&codes[1], 1);
    } else {
        memory.setBytes(&codes[0], 2);
    }
    disassembler.decode(memory, insn, operands, nullptr);
    char message[40];
    if (prefix == 0) {
        sprintf(message, "%s opecode 0x%02" PRIX8, __FUNCTION__, opc);
    } else {
        sprintf(message, "%s opecode 0x%02x 0x%02" PRIX8, __FUNCTION__, prefix, opc);
    }
    asserter.equals(message, UNKNOWN_INSTRUCTION, disassembler.getError());
}

static void test_illegal_mc6809() {
    dis6809.acceptCpu(McuType::MC6809);

    const uint8_t p00_illegals[] = {
        0x01, 0x02, 0x05, 0x0b,
        0x14, 0x15, 0x18, 0x1b,
        0x38, 0x3e,
        0x41, 0x42, 0x45, 0x4b, 0x4e,
        0x51, 0x52, 0x55, 0x5b, 0x5e,
        0x61, 0x62, 0x65, 0x6b,
        0x71, 0x72, 0x75, 0x7b,
        0x87, 0x8f,
        0xc7, 0xcd, 0xcf,
    };
    for (uint8_t idx = 0; idx < sizeof(p00_illegals); idx++)
        assert_illegal(p00_illegals[idx]);

    const uint8_t p10_legals[] = {
        0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
        0x3f,
        0x83, 0x8c, 0x8e,
        0x93, 0x9c, 0x9e, 0x9f,
        0xa3, 0xac, 0xae, 0xaf,
        0xb3, 0xbc, 0xbe, 0xbf,
        0xce,
        0xde, 0xdf,
        0xee, 0xef,
        0xfe, 0xff,
    };
    uint8_t idx = 0;
    for (uint16_t opc = 0x00; opc < 0x100; opc++) {
        if (idx == sizeof(p10_legals) || opc < p10_legals[idx])
            assert_illegal(opc++, 0x10);
        else idx++;
    }

    const uint8_t p11_legals[] = {
        0x3f,
        0x83, 0x8c,
        0x93, 0x9c,
        0xa3, 0xac,
        0xb3, 0xbc,
    };
    idx = 0;
    for (uint16_t opc = 0x00; opc < 0x100; opc++) {
        if (idx == sizeof(p11_legals) || opc < p11_legals[idx])
            assert_illegal(opc++, 0x11);
        else idx++;
    }
}

static void test_illegal_hd6309() {
    dis6809.acceptCpu(McuType::HD6309);

    const uint8_t p00_illegals[] = {
        0x15, 0x1b,
        0x38, 0x3e,
        0x41, 0x42, 0x45, 0x4b, 0x4e,
        0x51, 0x52, 0x55, 0x5b, 0x5e,
        0x87, 0x8f,
        0xc7, 0xcf,
    };
    for (uint8_t idx = 0; idx < sizeof(p00_illegals); idx++)
        assert_illegal(p00_illegals[idx]);

    const uint8_t p10_legals[] = {
        0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3f,
        0x40, 0x43, 0x44, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4c, 0x4d, 0x4f,
        0x53, 0x54, 0x56, 0x59, 0x5a, 0x5c, 0x5d, 0x5f,
        0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8e,
        0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9e, 0x9f,
        0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xae, 0xaf,
        0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbe, 0xbf,
        0xce,
        0xdc, 0xdd, 0xde, 0xdf,
        0xec, 0xed, 0xee, 0xef,
        0xfc, 0xfd, 0xfe, 0xff,
    };
    uint8_t idx = 0;
    for (uint16_t opc = 0x00; opc < 0x100; opc++) {
        if (idx == sizeof(p10_legals) || opc < p10_legals[idx])
            assert_illegal(opc++, 0x10);
        else idx++;
    }

    const uint8_t p11_legals[] = {
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3f,
        0x43, 0x4a, 0x4c, 0x4d, 0x4f,
        0x53, 0x5a, 0x5c, 0x5d, 0x5f,
        0x80, 0x81, 0x83, 0x86, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
        0x90, 0x91, 0x93, 0x96, 0x97, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f,
        0xa0, 0xa1, 0xa3, 0xa6, 0xa7, 0xab, 0xac, 0xad, 0xae, 0xaf,
        0xb0, 0xb1, 0xb3, 0xb6, 0xb7, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf,
        0xc0, 0xc1, 0xc6, 0xcb,
        0xd0, 0xd1, 0xd6, 0xd7, 0xdb,
        0xe0, 0xe1, 0xe6, 0xe7, 0xeb,
        0xf0, 0xf1, 0xf6, 0xf7, 0xfb,
    };
    idx = 0;
    for (uint16_t opc = 0x00; opc < 0x100; opc++) {
        if (idx == sizeof(p11_legals) || opc < p11_legals[idx])
            assert_illegal(opc++, 0x11);
        else idx++;
    }
}

static void run_test(void (*test)(), const char *test_name) {
    asserter.clear(test_name);
    set_up();
    test();
    tear_down();
    asserter.check();
}

int main(int argc, char **argv) {
    RUN_TEST(test_inherent);
    RUN_TEST(test_immediate);
    RUN_TEST(test_direct);
    RUN_TEST(test_extended);
    RUN_TEST(test_indexed);
    RUN_TEST(test_indexed_mode);
    RUN_TEST(test_relative);
    RUN_TEST(test_stack);
    RUN_TEST(test_register);
    RUN_TEST(test_illegal_mc6809);
    // HD6309
    RUN_TEST(test_transfer);
    RUN_TEST(test_bit_position);
    RUN_TEST(test_illegal_hd6309);
    return 0;
}
