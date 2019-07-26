#include "config_hd6309.h"

#include "entry_hd6309.h"
#include "table_hd6309.h"
#include "text_hd6309.h"

const Entry TABLE_P00[] PROGMEM = {
    P00(0x00, NEG,   2, MC6809, DIRECT_PG)
    P00(0x01, OIM,   3, HD6309, IMM_DIRECT)
    P00(0x02, AIM,   3, HD6309, IMM_DIRECT)
    P00(0x03, COM,   2, MC6809, DIRECT_PG)
    P00(0x04, LSR,   2, MC6809, DIRECT_PG)
    P00(0x05, EIM,   3, HD6309, IMM_DIRECT)
    P00(0x06, ROR,   2, MC6809, DIRECT_PG)
    P00(0x07, ASR,   2, MC6809, DIRECT_PG)
    P00(0x08, ASL,   2, MC6809, DIRECT_PG)
    P00(0x08, LSL,   2, MC6809, DIRECT_PG)
    P00(0x09, ROL,   2, MC6809, DIRECT_PG)
    P00(0x0A, DEC,   2, MC6809, DIRECT_PG)
    P00(0x0B, TIM,   3, HD6309, IMM_DIRECT)
    P00(0x0C, INC,   2, MC6809, DIRECT_PG)
    P00(0x0D, TST,   2, MC6809, DIRECT_PG)
    P00(0x0E, JMP,   2, MC6809, DIRECT_PG)
    P00(0x0F, CLR,   2, MC6809, DIRECT_PG)
    P00(0x12, NOP,   1, MC6809, INHERENT)
    P00(0x13, SYNC,  1, MC6809, INHERENT)
    P00(0x14, SEXW,  1, HD6309, INHERENT)
    P00(0x16, LBRA,  3, MC6809, RELATIVE)
    P00(0x17, LBSR,  3, MC6809, RELATIVE)
    P00(0x19, DAA,   1, MC6809, INHERENT)
    P00(0x1A, ORCC,  2, MC6809, IMMEDIATE)
    P00(0x1C, ANDCC, 2, MC6809, IMMEDIATE)
    P00(0x1D, SEX,   1, MC6809, INHERENT)
    P00(0x1E, EXG,   2, MC6809, REGISTERS)
    P00(0x1F, TFR,   2, MC6809, REGISTERS)
    P00(0x20, BRA,   2, MC6809, RELATIVE)
    P00(0x21, BRN,   2, MC6809, RELATIVE)
    P00(0x22, BHI,   2, MC6809, RELATIVE)
    P00(0x23, BLS,   2, MC6809, RELATIVE)
    P00(0x24, BHS,   2, MC6809, RELATIVE)
    P00(0x24, BCC,   2, MC6809, RELATIVE)
    P00(0x25, BLO,   2, MC6809, RELATIVE)
    P00(0x25, BCS,   2, MC6809, RELATIVE)
    P00(0x26, BNE,   2, MC6809, RELATIVE)
    P00(0x27, BEQ,   2, MC6809, RELATIVE)
    P00(0x28, BVC,   2, MC6809, RELATIVE)
    P00(0x29, BVS,   2, MC6809, RELATIVE)
    P00(0x2A, BPL,   2, MC6809, RELATIVE)
    P00(0x2B, BMI,   2, MC6809, RELATIVE)
    P00(0x2C, BGE,   2, MC6809, RELATIVE)
    P00(0x2D, BLT,   2, MC6809, RELATIVE)
    P00(0x2E, BGT,   2, MC6809, RELATIVE)
    P00(0x2F, BLE,   2, MC6809, RELATIVE)
    P00(0x30, LEAX,  2, MC6809, INDEXED)
    P00(0x31, LEAY,  2, MC6809, INDEXED)
    P00(0x32, LEAS,  2, MC6809, INDEXED)
    P00(0x33, LEAU,  2, MC6809, INDEXED)
    P00(0x34, PSHS,  2, MC6809, STACK_OP)
    P00(0x35, PULS,  2, MC6809, STACK_OP)
    P00(0x36, PSHU,  2, MC6809, STACK_OP)
    P00(0x37, PULU,  2, MC6809, STACK_OP)
    P00(0x39, RTS,   1, MC6809, INHERENT)
    P00(0x3A, ABX,   1, MC6809, INHERENT)
    P00(0x3B, RTI,   1, MC6809, INHERENT)
    P00(0x3C, CWAI,  2, MC6809, IMMEDIATE)
    P00(0x3D, MUL,   1, MC6809, INHERENT)
    P00(0x3F, SWI,   1, MC6809, INHERENT)
    P00(0x40, NEGA,  1, MC6809, INHERENT)
    P00(0x43, COMA,  1, MC6809, INHERENT)
    P00(0x44, LSRA,  1, MC6809, INHERENT)
    P00(0x46, RORA,  1, MC6809, INHERENT)
    P00(0x47, ASRA,  1, MC6809, INHERENT)
    P00(0x48, ASLA,  1, MC6809, INHERENT)
    P00(0x48, LSLA,  1, MC6809, INHERENT)
    P00(0x49, ROLA,  1, MC6809, INHERENT)
    P00(0x4A, DECA,  1, MC6809, INHERENT)
    P00(0x4C, INCA,  1, MC6809, INHERENT)
    P00(0x4D, TSTA,  1, MC6809, INHERENT)
    P00(0x4F, CLRA,  1, MC6809, INHERENT)
    P00(0x50, NEGB,  1, MC6809, INHERENT)
    P00(0x53, COMB,  1, MC6809, INHERENT)
    P00(0x54, LSRB,  1, MC6809, INHERENT)
    P00(0x56, RORB,  1, MC6809, INHERENT)
    P00(0x57, ASRB,  1, MC6809, INHERENT)
    P00(0x58, ASLB,  1, MC6809, INHERENT)
    P00(0x58, LSLB,  1, MC6809, INHERENT)
    P00(0x59, ROLB,  1, MC6809, INHERENT)
    P00(0x5A, DECB,  1, MC6809, INHERENT)
    P00(0x5C, INCB,  1, MC6809, INHERENT)
    P00(0x5D, TSTB,  1, MC6809, INHERENT)
    P00(0x5F, CLRB,  1, MC6809, INHERENT)
    P00(0x60, NEG,   2, MC6809, INDEXED)
    P00(0x61, OIM,   3, HD6309, IMM_INDEXED)
    P00(0x62, AIM,   3, HD6309, IMM_INDEXED)
    P00(0x63, COM,   2, MC6809, INDEXED)
    P00(0x64, LSR,   2, MC6809, INDEXED)
    P00(0x65, EIM,   3, HD6309, IMM_INDEXED)
    P00(0x66, ROR,   2, MC6809, INDEXED)
    P00(0x67, ASR,   2, MC6809, INDEXED)
    P00(0x68, ASL,   2, MC6809, INDEXED)
    P00(0x68, LSL,   2, MC6809, INDEXED)
    P00(0x69, ROL,   2, MC6809, INDEXED)
    P00(0x6A, DEC,   2, MC6809, INDEXED)
    P00(0x6B, TIM,   3, HD6309, IMM_INDEXED)
    P00(0x6C, INC,   2, MC6809, INDEXED)
    P00(0x6D, TST,   2, MC6809, INDEXED)
    P00(0x6E, JMP,   2, MC6809, INDEXED)
    P00(0x6F, CLR,   2, MC6809, INDEXED)
    P00(0x70, NEG,   3, MC6809, EXTENDED)
    P00(0x71, OIM,   4, HD6309, IMM_EXTENDED)
    P00(0x72, AIM,   4, HD6309, IMM_EXTENDED)
    P00(0x73, COM,   3, MC6809, EXTENDED)
    P00(0x74, LSR,   3, MC6809, EXTENDED)
    P00(0x75, EIM,   4, HD6309, IMM_EXTENDED)
    P00(0x76, ROR,   3, MC6809, EXTENDED)
    P00(0x77, ASR,   3, MC6809, EXTENDED)
    P00(0x78, ASL,   3, MC6809, EXTENDED)
    P00(0x78, LSL,   3, MC6809, EXTENDED)
    P00(0x79, ROL,   3, MC6809, EXTENDED)
    P00(0x7A, DEC,   3, MC6809, EXTENDED)
    P00(0x7B, TIM,   4, HD6309, IMM_EXTENDED)
    P00(0x7C, INC,   3, MC6809, EXTENDED)
    P00(0x7D, TST,   3, MC6809, EXTENDED)
    P00(0x7E, JMP,   3, MC6809, EXTENDED)
    P00(0x7F, CLR,   3, MC6809, EXTENDED)
    P00(0x80, SUBA,  2, MC6809, IMMEDIATE)
    P00(0x81, CMPA,  2, MC6809, IMMEDIATE)
    P00(0x82, SBCA,  2, MC6809, IMMEDIATE)
    P00(0x83, SUBD,  3, MC6809, IMMEDIATE)
    P00(0x84, ANDA,  2, MC6809, IMMEDIATE)
    P00(0x85, BITA,  2, MC6809, IMMEDIATE)
    P00(0x86, LDA,   2, MC6809, IMMEDIATE)
    P00(0x88, EORA,  2, MC6809, IMMEDIATE)
    P00(0x89, ADCA,  2, MC6809, IMMEDIATE)
    P00(0x8A, ORA,   2, MC6809, IMMEDIATE)
    P00(0x8B, ADDA,  2, MC6809, IMMEDIATE)
    P00(0x8C, CMPX,  3, MC6809, IMMEDIATE)
    P00(0x8D, BSR,   2, MC6809, RELATIVE)
    P00(0x8E, LDX,   3, MC6809, IMMEDIATE)
    P00(0x90, SUBA,  2, MC6809, DIRECT_PG)
    P00(0x91, CMPA,  2, MC6809, DIRECT_PG)
    P00(0x92, SBCA,  2, MC6809, DIRECT_PG)
    P00(0x93, SUBD,  2, MC6809, DIRECT_PG)
    P00(0x94, ANDA,  2, MC6809, DIRECT_PG)
    P00(0x95, BITA,  2, MC6809, DIRECT_PG)
    P00(0x96, LDA,   2, MC6809, DIRECT_PG)
    P00(0x97, STA,   2, MC6809, DIRECT_PG)
    P00(0x98, EORA,  2, MC6809, DIRECT_PG)
    P00(0x99, ADCA,  2, MC6809, DIRECT_PG)
    P00(0x9A, ORA,   2, MC6809, DIRECT_PG)
    P00(0x9B, ADDA,  2, MC6809, DIRECT_PG)
    P00(0x9C, CMPX,  2, MC6809, DIRECT_PG)
    P00(0x9D, JSR,   2, MC6809, DIRECT_PG)
    P00(0x9E, LDX,   2, MC6809, DIRECT_PG)
    P00(0x9F, STX,   2, MC6809, DIRECT_PG)
    P00(0xA0, SUBA,  2, MC6809, INDEXED)
    P00(0xA1, CMPA,  2, MC6809, INDEXED)
    P00(0xA2, SBCA,  2, MC6809, INDEXED)
    P00(0xA3, SUBD,  2, MC6809, INDEXED)
    P00(0xA4, ANDA,  2, MC6809, INDEXED)
    P00(0xA5, BITA,  2, MC6809, INDEXED)
    P00(0xA6, LDA,   2, MC6809, INDEXED)
    P00(0xA7, STA,   2, MC6809, INDEXED)
    P00(0xA8, EORA,  2, MC6809, INDEXED)
    P00(0xA9, ADCA,  2, MC6809, INDEXED)
    P00(0xAA, ORA,   2, MC6809, INDEXED)
    P00(0xAB, ADDA,  2, MC6809, INDEXED)
    P00(0xAC, CMPX,  2, MC6809, INDEXED)
    P00(0xAD, JSR,   2, MC6809, INDEXED)
    P00(0xAE, LDX,   2, MC6809, INDEXED)
    P00(0xAF, STX,   2, MC6809, INDEXED)
    P00(0xB0, SUBA,  3, MC6809, EXTENDED)
    P00(0xB1, CMPA,  3, MC6809, EXTENDED)
    P00(0xB2, SBCA,  3, MC6809, EXTENDED)
    P00(0xB3, SUBD,  3, MC6809, EXTENDED)
    P00(0xB4, ANDA,  3, MC6809, EXTENDED)
    P00(0xB5, BITA,  3, MC6809, EXTENDED)
    P00(0xB6, LDA,   3, MC6809, EXTENDED)
    P00(0xB7, STA,   3, MC6809, EXTENDED)
    P00(0xB8, EORA,  3, MC6809, EXTENDED)
    P00(0xB9, ADCA,  3, MC6809, EXTENDED)
    P00(0xBA, ORA,   3, MC6809, EXTENDED)
    P00(0xBB, ADDA,  3, MC6809, EXTENDED)
    P00(0xBC, CMPX,  3, MC6809, EXTENDED)
    P00(0xBD, JSR,   3, MC6809, EXTENDED)
    P00(0xBE, LDX,   3, MC6809, EXTENDED)
    P00(0xBF, STX,   3, MC6809, EXTENDED)
    P00(0xC0, SUBB,  2, MC6809, IMMEDIATE)
    P00(0xC1, CMPB,  2, MC6809, IMMEDIATE)
    P00(0xC2, SBCB,  2, MC6809, IMMEDIATE)
    P00(0xC3, ADDD,  3, MC6809, IMMEDIATE)
    P00(0xC4, ANDB,  2, MC6809, IMMEDIATE)
    P00(0xC5, BITB,  2, MC6809, IMMEDIATE)
    P00(0xC6, LDB,   2, MC6809, IMMEDIATE)
    P00(0xC8, EORB,  2, MC6809, IMMEDIATE)
    P00(0xC9, ADCB,  2, MC6809, IMMEDIATE)
    P00(0xCA, ORB,   2, MC6809, IMMEDIATE)
    P00(0xCB, ADDB,  2, MC6809, IMMEDIATE)
    P00(0xCC, LDD,   3, MC6809, IMMEDIATE)
    P00(0xCD, LDQ,   5, HD6309, IMMEDIATE)
    P00(0xCE, LDU,   3, MC6809, IMMEDIATE)
    P00(0xD0, SUBB,  2, MC6809, DIRECT_PG)
    P00(0xD1, CMPB,  2, MC6809, DIRECT_PG)
    P00(0xD2, SBCB,  2, MC6809, DIRECT_PG)
    P00(0xD3, ADDD,  2, MC6809, DIRECT_PG)
    P00(0xD4, ANDB,  2, MC6809, DIRECT_PG)
    P00(0xD5, BITB,  2, MC6809, DIRECT_PG)
    P00(0xD6, LDB,   2, MC6809, DIRECT_PG)
    P00(0xD7, STB,   2, MC6809, DIRECT_PG)
    P00(0xD8, EORB,  2, MC6809, DIRECT_PG)
    P00(0xD9, ADCB,  2, MC6809, DIRECT_PG)
    P00(0xDA, ORB,   2, MC6809, DIRECT_PG)
    P00(0xDB, ADDB,  2, MC6809, DIRECT_PG)
    P00(0xDC, LDD,   2, MC6809, DIRECT_PG)
    P00(0xDD, STD,   2, MC6809, DIRECT_PG)
    P00(0xDE, LDU,   2, MC6809, DIRECT_PG)
    P00(0xDF, STU,   2, MC6809, DIRECT_PG)
    P00(0xE0, SUBB,  2, MC6809, INDEXED)
    P00(0xE1, CMPB,  2, MC6809, INDEXED)
    P00(0xE2, SBCB,  2, MC6809, INDEXED)
    P00(0xE3, ADDD,  2, MC6809, INDEXED)
    P00(0xE4, ANDB,  2, MC6809, INDEXED)
    P00(0xE5, BITB,  2, MC6809, INDEXED)
    P00(0xE6, LDB,   2, MC6809, INDEXED)
    P00(0xE7, STB,   2, MC6809, INDEXED)
    P00(0xE8, EORB,  2, MC6809, INDEXED)
    P00(0xE9, ADCB,  2, MC6809, INDEXED)
    P00(0xEA, ORB,   2, MC6809, INDEXED)
    P00(0xEB, ADDB,  2, MC6809, INDEXED)
    P00(0xEC, LDD,   2, MC6809, INDEXED)
    P00(0xED, STD,   2, MC6809, INDEXED)
    P00(0xEE, LDU,   2, MC6809, INDEXED)
    P00(0xEF, STU,   2, MC6809, INDEXED)
    P00(0xF0, SUBB,  3, MC6809, EXTENDED)
    P00(0xF1, CMPB,  3, MC6809, EXTENDED)
    P00(0xF2, SBCB,  3, MC6809, EXTENDED)
    P00(0xF3, ADDD,  3, MC6809, EXTENDED)
    P00(0xF4, ANDB,  3, MC6809, EXTENDED)
    P00(0xF5, BITB,  3, MC6809, EXTENDED)
    P00(0xF6, LDB,   3, MC6809, EXTENDED)
    P00(0xF7, STB,   3, MC6809, EXTENDED)
    P00(0xF8, EORB,  3, MC6809, EXTENDED)
    P00(0xF9, ADCB,  3, MC6809, EXTENDED)
    P00(0xFA, ORB,   3, MC6809, EXTENDED)
    P00(0xFB, ADDB,  3, MC6809, EXTENDED)
    P00(0xFC, LDD,   3, MC6809, EXTENDED)
    P00(0xFD, STD,   3, MC6809, EXTENDED)
    P00(0xFE, LDU,   3, MC6809, EXTENDED)
    P00(0xFF, STU,   3, MC6809, EXTENDED)
};

const Entry TABLE_P10[] PROGMEM = {
    P10(0x21, LBRN,  4, MC6809, RELATIVE)
    P10(0x22, LBHI,  4, MC6809, RELATIVE)
    P10(0x23, LBLS,  4, MC6809, RELATIVE)
    P10(0x24, LBHS,  4, MC6809, RELATIVE)
    P10(0x24, LBCC,  4, MC6809, RELATIVE)
    P10(0x25, LBLO,  4, MC6809, RELATIVE)
    P10(0x25, LBCS,  4, MC6809, RELATIVE)
    P10(0x26, LBNE,  4, MC6809, RELATIVE)
    P10(0x27, LBEQ,  4, MC6809, RELATIVE)
    P10(0x28, LBVC,  4, MC6809, RELATIVE)
    P10(0x29, LBVS,  4, MC6809, RELATIVE)
    P10(0x2A, LBPL,  4, MC6809, RELATIVE)
    P10(0x2B, LBMI,  4, MC6809, RELATIVE)
    P10(0x2C, LBGE,  4, MC6809, RELATIVE)
    P10(0x2D, LBLT,  4, MC6809, RELATIVE)
    P10(0x2E, LBGT,  4, MC6809, RELATIVE)
    P10(0x2F, LBLE,  4, MC6809, RELATIVE)
    P10(0x30, ADDR,  3, HD6309, REGISTERS)
    P10(0x31, ADCR,  3, HD6309, REGISTERS)
    P10(0x32, SUBR,  3, HD6309, REGISTERS)
    P10(0x33, SBCR,  3, HD6309, REGISTERS)
    P10(0x34, ANDR,  3, HD6309, REGISTERS)
    P10(0x35, ORR,   3, HD6309, REGISTERS)
    P10(0x36, EORR,  3, HD6309, REGISTERS)
    P10(0x37, CMPR,  3, HD6309, REGISTERS)
    P10(0x38, PSHSW, 2, HD6309, INHERENT)
    P10(0x39, PULSW, 2, HD6309, INHERENT)
    P10(0x3A, PSHUW, 2, HD6309, INHERENT)
    P10(0x3B, PULUW, 2, HD6309, INHERENT)
    P10(0x3F, SWI2,  2, MC6809, INHERENT)
    P10(0x40, NEGD,  2, HD6309, INHERENT)
    P10(0x43, COMD,  2, HD6309, INHERENT)
    P10(0x44, LSRD,  2, HD6309, INHERENT)
    P10(0x46, RORD,  2, HD6309, INHERENT)
    P10(0x47, ASRD,  2, HD6309, INHERENT)
    P10(0x48, ASLD,  2, HD6309, INHERENT)
    P10(0x48, LSLD,  2, HD6309, INHERENT)
    P10(0x49, ROLD,  2, HD6309, INHERENT)
    P10(0x4A, DECD,  2, HD6309, INHERENT)
    P10(0x4C, INCD,  2, HD6309, INHERENT)
    P10(0x4D, TSTD,  2, HD6309, INHERENT)
    P10(0x4F, CLRD,  2, HD6309, INHERENT)
    P10(0x53, COMW,  2, HD6309, INHERENT)
    P10(0x54, LSRW,  2, HD6309, INHERENT)
    P10(0x56, RORW,  2, HD6309, INHERENT)
    P10(0x59, ROLW,  2, HD6309, INHERENT)
    P10(0x5A, DECW,  2, HD6309, INHERENT)
    P10(0x5C, INCW,  2, HD6309, INHERENT)
    P10(0x5D, TSTW,  2, HD6309, INHERENT)
    P10(0x5F, CLRW,  2, HD6309, INHERENT)
    P10(0x80, SUBW,  4, HD6309, IMMEDIATE)
    P10(0x81, CMPW,  4, HD6309, IMMEDIATE)
    P10(0x82, SBCD,  4, HD6309, IMMEDIATE)
    P10(0x83, CMPD,  4, MC6809, IMMEDIATE)
    P10(0x84, ANDD,  4, HD6309, IMMEDIATE)
    P10(0x85, BITD,  4, HD6309, IMMEDIATE)
    P10(0x86, LDW,   4, HD6309, IMMEDIATE)
    P10(0x88, EORD,  4, HD6309, IMMEDIATE)
    P10(0x89, ADCD,  4, HD6309, IMMEDIATE)
    P10(0x8A, ORD,   4, HD6309, IMMEDIATE)
    P10(0x8B, ADDW,  4, HD6309, IMMEDIATE)
    P10(0x8C, CMPY,  4, MC6809, IMMEDIATE)
    P10(0x8E, LDY,   4, MC6809, IMMEDIATE)
    P10(0x90, SUBW,  3, HD6309, DIRECT_PG)
    P10(0x91, CMPW,  3, HD6309, DIRECT_PG)
    P10(0x92, SBCD,  3, HD6309, DIRECT_PG)
    P10(0x93, CMPD,  3, MC6809, DIRECT_PG)
    P10(0x94, ANDD,  3, HD6309, DIRECT_PG)
    P10(0x95, BITD,  3, HD6309, DIRECT_PG)
    P10(0x96, LDW,   3, HD6309, DIRECT_PG)
    P10(0x97, STW,   3, HD6309, DIRECT_PG)
    P10(0x98, EORD,  3, HD6309, DIRECT_PG)
    P10(0x99, ADCD,  3, HD6309, DIRECT_PG)
    P10(0x9A, ORD,   3, HD6309, DIRECT_PG)
    P10(0x9B, ADDW,  3, HD6309, DIRECT_PG)
    P10(0x9C, CMPY,  3, MC6809, DIRECT_PG)
    P10(0x9E, LDY,   3, MC6809, DIRECT_PG)
    P10(0x9F, STY,   3, MC6809, DIRECT_PG)
    P10(0xA0, SUBW,  3, HD6309, INDEXED)
    P10(0xA1, CMPW,  3, HD6309, INDEXED)
    P10(0xA2, SBCD,  3, HD6309, INDEXED)
    P10(0xA3, CMPD,  3, MC6809, INDEXED)
    P10(0xA4, ANDD,  3, HD6309, INDEXED)
    P10(0xA5, BITD,  3, HD6309, INDEXED)
    P10(0xA6, LDW,   3, HD6309, INDEXED)
    P10(0xA7, STW,   3, HD6309, INDEXED)
    P10(0xA8, EORD,  3, HD6309, INDEXED)
    P10(0xA9, ADCD,  3, HD6309, INDEXED)
    P10(0xAA, ORD,   3, HD6309, INDEXED)
    P10(0xAB, ADDW,  3, HD6309, INDEXED)
    P10(0xAC, CMPY,  3, MC6809, INDEXED)
    P10(0xAE, LDY,   3, MC6809, INDEXED)
    P10(0xAF, STY,   3, MC6809, INDEXED)
    P10(0xB0, SUBW,  4, HD6309, EXTENDED)
    P10(0xB1, CMPW,  4, HD6309, EXTENDED)
    P10(0xB2, SBCD,  4, HD6309, EXTENDED)
    P10(0xB3, CMPD,  4, MC6809, EXTENDED)
    P10(0xB4, ANDD,  4, HD6309, EXTENDED)
    P10(0xB5, BITD,  4, HD6309, EXTENDED)
    P10(0xB6, LDW,   4, HD6309, EXTENDED)
    P10(0xB7, STW,   4, HD6309, EXTENDED)
    P10(0xB8, EORD,  4, HD6309, EXTENDED)
    P10(0xB9, ADCD,  4, HD6309, EXTENDED)
    P10(0xBA, ORD,   4, HD6309, EXTENDED)
    P10(0xBB, ADDW,  4, HD6309, EXTENDED)
    P10(0xBC, CMPY,  4, MC6809, EXTENDED)
    P10(0xBE, LDY,   4, MC6809, EXTENDED)
    P10(0xBF, STY,   4, MC6809, EXTENDED)
    P10(0xCE, LDS,   4, MC6809, IMMEDIATE)
    P10(0xDC, LDQ,   3, HD6309, DIRECT_PG)
    P10(0xDD, STQ,   3, HD6309, DIRECT_PG)
    P10(0xDE, LDS,   3, MC6809, DIRECT_PG)
    P10(0xDF, STS,   3, MC6809, DIRECT_PG)
    P10(0xEC, LDQ,   3, HD6309, INDEXED)
    P10(0xED, STQ,   3, HD6309, INDEXED)
    P10(0xEE, LDS,   3, MC6809, INDEXED)
    P10(0xEF, STS,   3, MC6809, INDEXED)
    P10(0xFC, LDQ,   4, HD6309, EXTENDED)
    P10(0xFD, STQ,   4, HD6309, EXTENDED)
    P10(0xFE, LDS,   4, MC6809, EXTENDED)
    P10(0xFF, STS,   4, MC6809, EXTENDED)
};

const Entry TABLE_P11[] PROGMEM = {
    P11(0x30, BAND,  4, HD6309, BIT_OPERATION)
    P11(0x31, BIAND, 4, HD6309, BIT_OPERATION)
    P11(0x32, BOR,   4, HD6309, BIT_OPERATION)
    P11(0x33, BIOR,  4, HD6309, BIT_OPERATION)
    P11(0x34, BEOR,  4, HD6309, BIT_OPERATION)
    P11(0x35, BIEOR, 4, HD6309, BIT_OPERATION)
    P11(0x36, LDBT,  4, HD6309, BIT_OPERATION)
    P11(0x37, STBT,  4, HD6309, BIT_OPERATION)
    P11(0x38, TFM,   3, HD6309, TRANSFER_MEM) /* R+,R+ */
    P11(0x39, TFM,   3, HD6309, TRANSFER_MEM) /* R-,R- */
    P11(0x3A, TFM,   3, HD6309, TRANSFER_MEM) /* R+,R */
    P11(0x3B, TFM,   3, HD6309, TRANSFER_MEM) /* R,R+ */
    P11(0x3C, BITMD, 3, HD6309, IMMEDIATE)
    P11(0x3D, LDMD,  3, HD6309, IMMEDIATE)
    P11(0x3F, SWI3,  2, MC6809, INHERENT)
    P11(0x43, COME,  2, HD6309, INHERENT)
    P11(0x4A, DECE,  2, HD6309, INHERENT)
    P11(0x4C, INCE,  2, HD6309, INHERENT)
    P11(0x4D, TSTE,  2, HD6309, INHERENT)
    P11(0x4F, CLRE,  2, HD6309, INHERENT)
    P11(0x53, COMF,  2, HD6309, INHERENT)
    P11(0x5A, DECF,  2, HD6309, INHERENT)
    P11(0x5C, INCF,  2, HD6309, INHERENT)
    P11(0x5D, TSTF,  2, HD6309, INHERENT)
    P11(0x5F, CLRF,  2, HD6309, INHERENT)
    P11(0x80, SUBE,  3, HD6309, IMMEDIATE)
    P11(0x81, CMPE,  3, HD6309, IMMEDIATE)
    P11(0x83, CMPU,  4, MC6809, IMMEDIATE)
    P11(0x86, LDE,   3, HD6309, IMMEDIATE)
    P11(0x8B, ADDE,  3, HD6309, IMMEDIATE)
    P11(0x8C, CMPS,  4, MC6809, IMMEDIATE)
    P11(0x8D, DIVD,  4, HD6309, IMMEDIATE)
    P11(0x8E, DIVQ,  4, HD6309, IMMEDIATE)
    P11(0x8F, MULD,  4, HD6309, IMMEDIATE)
    P11(0x90, SUBE,  3, HD6309, DIRECT_PG)
    P11(0x91, CMPE,  3, HD6309, DIRECT_PG)
    P11(0x93, CMPU,  3, MC6809, DIRECT_PG)
    P11(0x96, LDE,   3, HD6309, DIRECT_PG)
    P11(0x97, STE,   3, HD6309, DIRECT_PG)
    P11(0x9B, ADDE,  3, HD6309, DIRECT_PG)
    P11(0x9C, CMPS,  3, MC6809, DIRECT_PG)
    P11(0x9D, DIVD,  3, HD6309, DIRECT_PG)
    P11(0x9E, DIVQ,  3, HD6309, DIRECT_PG)
    P11(0x9F, MULD,  3, HD6309, DIRECT_PG)
    P11(0xA0, SUBE,  3, HD6309, INDEXED)
    P11(0xA1, CMPE,  3, HD6309, INDEXED)
    P11(0xA3, CMPU,  3, MC6809, INDEXED)
    P11(0xA6, LDE,   3, HD6309, INDEXED)
    P11(0xA7, STE,   3, HD6309, INDEXED)
    P11(0xAB, ADDE,  3, HD6309, INDEXED)
    P11(0xAC, CMPS,  3, MC6809, INDEXED)
    P11(0xAD, DIVD,  3, HD6309, INDEXED)
    P11(0xAE, DIVQ,  3, HD6309, INDEXED)
    P11(0xAF, MULD,  3, HD6309, INDEXED)
    P11(0xB0, SUBE,  4, HD6309, EXTENDED)
    P11(0xB1, CMPE,  4, HD6309, EXTENDED)
    P11(0xB3, CMPU,  4, MC6809, EXTENDED)
    P11(0xB6, LDE,   4, HD6309, EXTENDED)
    P11(0xB7, STE,   4, HD6309, EXTENDED)
    P11(0xBB, ADDE,  4, HD6309, EXTENDED)
    P11(0xBC, CMPS,  4, MC6809, EXTENDED)
    P11(0xBD, DIVD,  4, HD6309, EXTENDED)
    P11(0xBE, DIVQ,  4, HD6309, EXTENDED)
    P11(0xBF, MULD,  4, HD6309, EXTENDED)
    P11(0xC0, SUBF,  3, HD6309, IMMEDIATE)
    P11(0xC1, CMPF,  3, HD6309, IMMEDIATE)
    P11(0xC6, LDF,   3, HD6309, IMMEDIATE)
    P11(0xCB, ADDF,  3, HD6309, IMMEDIATE)
    P11(0xD0, SUBF,  3, HD6309, DIRECT_PG)
    P11(0xD1, CMPF,  3, HD6309, DIRECT_PG)
    P11(0xD6, LDF,   3, HD6309, DIRECT_PG)
    P11(0xD7, STF,   3, HD6309, DIRECT_PG)
    P11(0xDB, ADDF,  3, HD6309, DIRECT_PG)
    P11(0xE0, SUBF,  3, HD6309, INDEXED)
    P11(0xE1, CMPF,  3, HD6309, INDEXED)
    P11(0xE6, LDF,   3, HD6309, INDEXED)
    P11(0xE7, STF,   3, HD6309, INDEXED)
    P11(0xEB, ADDF,  3, HD6309, INDEXED)
    P11(0xF0, SUBF,  4, HD6309, EXTENDED)
    P11(0xF1, CMPF,  4, HD6309, EXTENDED)
    P11(0xF6, LDF,   4, HD6309, EXTENDED)
    P11(0xF7, STF,   4, HD6309, EXTENDED)
    P11(0xFB, ADDF,  4, HD6309, EXTENDED)
};

constexpr target::opcode_t PREFIX_P00 = 0x00;
constexpr target::opcode_t PREFIX_P10 = 0x10;
constexpr target::opcode_t PREFIX_P11 = 0x11;

static const Entry *searchEntry(
    const char *name,
    const Entry *table, const Entry *end) {
    for (const Entry *entry = table; entry < end; entry++) {
        if (pgm_strcasecmp(name, entry->name) == 0)
            return entry;
    }
    return nullptr;
}

static const Entry *searchEntry(
    const target::opcode_t opCode,
    const Entry *table, const Entry *end) {
    for (const Entry *entry = table; entry < end; entry++) {
        if (opCode == pgm_read_byte(&entry->opc))
            return entry;
    }
    return nullptr;
}

struct EntryPage {
    const target::opcode_t prefix;
    const Entry *const table;
    const Entry *const end;
};

static Error searchPages(
    Insn &insn, const char *name, const EntryPage *pages, const EntryPage *end) {
    for (const EntryPage *page = pages; page < end; page++) {
        const Entry *entry;
        if ((entry = searchEntry(name, page->table, page->end)) != nullptr) {
            insn.setInsnCode(InsnTable::insnCode(page->prefix, pgm_read_byte(&entry->opc)));
            insn.setFlags(pgm_read_byte(&entry->flags));
            return OK;
        }
    }
    return UNKNOWN_INSTRUCTION;
}

static Error searchPages(
    Insn &insn, const char *name, AddrMode addrMode, const EntryPage *pages, const EntryPage *end) {
    for (const EntryPage *page = pages; page < end; page++) {
        for (const Entry *entry = page->table; entry < page->end
                 && (entry = searchEntry(name, entry, page->end)) != nullptr; entry++) {
            const host::uint_t flags = pgm_read_byte(&entry->flags);
            if (_addrMode(flags) == addrMode) {
                insn.setInsnCode(InsnTable::insnCode(page->prefix, pgm_read_byte(&entry->opc)));
                insn.setFlags(flags);
                return OK;
            }
        }
    }
    return UNKNOWN_INSTRUCTION;
}

static Error searchPages(
    Insn &insn, target::insn_t insnCode, const EntryPage *pages, const EntryPage *end) {
    for (const EntryPage *page = pages; page < end; page++) {
        if (InsnTable::prefixCode(insnCode) != page->prefix) continue;
        const Entry *entry =
            searchEntry(InsnTable::opCode(insnCode), page->table, page->end);
        if (entry) {
            insn.setFlags(pgm_read_byte(&entry->flags));
            char name[8];
            pgm_strcpy(name, entry->name);
            insn.setName(name);
            return OK;
        }
    }
    return UNKNOWN_INSTRUCTION;
}

#define ARRAY_END(array) &(array)[(sizeof(array) / sizeof(array[0]))]
#define ARRAY_RANGE(array) &array[0], ARRAY_END(array)

static const EntryPage PAGES[] = {
    { PREFIX_P00, &TABLE_P00[0], ARRAY_END(TABLE_P00) },
    { PREFIX_P10, &TABLE_P10[0], ARRAY_END(TABLE_P10) },
    { PREFIX_P11, &TABLE_P11[0], ARRAY_END(TABLE_P11) },
};

bool InsnTable::isPrefixCode(target::opcode_t opCode) {
    return opCode == PREFIX_P10 || opCode == PREFIX_P11;
}

Error InsnTable::searchName(Insn &insn) const {
    if (searchPages(insn, insn.name(), ARRAY_RANGE(PAGES)) == OK)
        return OK;
    return UNKNOWN_INSTRUCTION;
}

Error InsnTable::searchNameAndAddrMode(Insn &insn) const {
    if (searchPages(insn, insn.name(), insn.addrMode(), ARRAY_RANGE(PAGES)) == OK)
        return OK;
    return UNKNOWN_INSTRUCTION;
}

Error InsnTable::searchInsnCode(Insn &insn) const {
    if (searchPages(insn, insn.insnCode(), ARRAY_RANGE(PAGES)) == OK)
        return OK;
    return UNKNOWN_INSTRUCTION;
}

class InsnTable InsnTable;
