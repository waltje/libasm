#include "config_hd6309.h"

#include "table_hd6309.h"
#include "text_hd6309.h"

const EntryMc6809 TABLE_P00[] PROGMEM = {
    P00(0x01, OIM,   3, IMM_DIRECT)
    P00(0x02, AIM,   3, IMM_DIRECT)
    P00(0x05, EIM,   3, IMM_DIRECT)
    P00(0x0B, TIM,   3, IMM_DIRECT)
    P00(0x14, SEXW,  1, INHERENT)
    P00(0x61, OIM,   3, IMM_INDEXED)
    P00(0x62, AIM,   3, IMM_INDEXED)
    P00(0x65, EIM,   3, IMM_INDEXED)
    P00(0x6B, TIM,   3, IMM_INDEXED)
    P00(0x71, OIM,   4, IMM_EXTENDED)
    P00(0x72, AIM,   4, IMM_EXTENDED)
    P00(0x75, EIM,   4, IMM_EXTENDED)
    P00(0x7B, TIM,   4, IMM_EXTENDED)
    P00(0xCD, LDQ,   5, IMMEDIATE)
};

const EntryMc6809 TABLE_P10[] PROGMEM = {
    P10(0x30, ADDR,  3, REGISTERS)
    P10(0x31, ADCR,  3, REGISTERS)
    P10(0x32, SUBR,  3, REGISTERS)
    P10(0x33, SBCR,  3, REGISTERS)
    P10(0x34, ANDR,  3, REGISTERS)
    P10(0x35, ORR,   3, REGISTERS)
    P10(0x36, EORR,  3, REGISTERS)
    P10(0x37, CMPR,  3, REGISTERS)
    P10(0x38, PSHSW, 2, INHERENT)
    P10(0x39, PULSW, 2, INHERENT)
    P10(0x3A, PSHUW, 2, INHERENT)
    P10(0x3B, PULUW, 2, INHERENT)
    P10(0x40, NEGD,  2, INHERENT)
    P10(0x43, COMD,  2, INHERENT)
    P10(0x44, LSRD,  2, INHERENT)
    P10(0x46, RORD,  2, INHERENT)
    P10(0x47, ASRD,  2, INHERENT)
    P10(0x48, ASLD,  2, INHERENT)
    P10(0x48, LSLD,  2, INHERENT)
    P10(0x49, ROLD,  2, INHERENT)
    P10(0x4A, DECD,  2, INHERENT)
    P10(0x4C, INCD,  2, INHERENT)
    P10(0x4D, TSTD,  2, INHERENT)
    P10(0x4F, CLRD,  2, INHERENT)
    P10(0x53, COMW,  2, INHERENT)
    P10(0x54, LSRW,  2, INHERENT)
    P10(0x56, RORW,  2, INHERENT)
    P10(0x59, ROLW,  2, INHERENT)
    P10(0x5A, DECW,  2, INHERENT)
    P10(0x5C, INCW,  2, INHERENT)
    P10(0x5D, TSTW,  2, INHERENT)
    P10(0x5F, CLRW,  2, INHERENT)
    P10(0x80, SUBW,  4, IMMEDIATE)
    P10(0x81, CMPW,  4, IMMEDIATE)
    P10(0x82, SBCD,  4, IMMEDIATE)
    P10(0x84, ANDD,  4, IMMEDIATE)
    P10(0x85, BITD,  4, IMMEDIATE)
    P10(0x86, LDW,   4, IMMEDIATE)
    P10(0x88, EORD,  4, IMMEDIATE)
    P10(0x89, ADCD,  4, IMMEDIATE)
    P10(0x8A, ORD,   4, IMMEDIATE)
    P10(0x8B, ADDW,  4, IMMEDIATE)
    P10(0x90, SUBW,  3, DIRECT_PG)
    P10(0x91, CMPW,  3, DIRECT_PG)
    P10(0x92, SBCD,  3, DIRECT_PG)
    P10(0x94, ANDD,  3, DIRECT_PG)
    P10(0x95, BITD,  3, DIRECT_PG)
    P10(0x96, LDW,   3, DIRECT_PG)
    P10(0x97, STW,   3, DIRECT_PG)
    P10(0x98, EORD,  3, DIRECT_PG)
    P10(0x99, ADCD,  3, DIRECT_PG)
    P10(0x9A, ORD,   3, DIRECT_PG)
    P10(0x9B, ADDW,  3, DIRECT_PG)
    P10(0xA0, SUBW,  3, INDEXED)
    P10(0xA1, CMPW,  3, INDEXED)
    P10(0xA2, SBCD,  3, INDEXED)
    P10(0xA4, ANDD,  3, INDEXED)
    P10(0xA5, BITD,  3, INDEXED)
    P10(0xA6, LDW,   3, INDEXED)
    P10(0xA7, STW,   3, INDEXED)
    P10(0xA8, EORD,  3, INDEXED)
    P10(0xA9, ADCD,  3, INDEXED)
    P10(0xAA, ORD,   3, INDEXED)
    P10(0xAB, ADDW,  3, INDEXED)
    P10(0xB0, SUBW,  4, EXTENDED)
    P10(0xB1, CMPW,  4, EXTENDED)
    P10(0xB2, SBCD,  4, EXTENDED)
    P10(0xB4, ANDD,  4, EXTENDED)
    P10(0xB5, BITD,  4, EXTENDED)
    P10(0xB6, LDW,   4, EXTENDED)
    P10(0xB7, STW,   4, EXTENDED)
    P10(0xB8, EORD,  4, EXTENDED)
    P10(0xB9, ADCD,  4, EXTENDED)
    P10(0xBA, ORD,   4, EXTENDED)
    P10(0xBB, ADDW,  4, EXTENDED)
    P10(0xDC, LDQ,   3, DIRECT_PG)
    P10(0xDD, STQ,   3, DIRECT_PG)
    P10(0xEC, LDQ,   3, INDEXED)
    P10(0xED, STQ,   3, INDEXED)
    P10(0xFC, LDQ,   4, EXTENDED)
    P10(0xFD, STQ,   4, EXTENDED)
};

const EntryMc6809 TABLE_P11[] PROGMEM = {
    P11(0x30, BAND,  4, BIT_OPERATION)
    P11(0x31, BIAND, 4, BIT_OPERATION)
    P11(0x32, BOR,   4, BIT_OPERATION)
    P11(0x33, BIOR,  4, BIT_OPERATION)
    P11(0x34, BEOR,  4, BIT_OPERATION)
    P11(0x35, BIEOR, 4, BIT_OPERATION)
    P11(0x36, LDBT,  4, BIT_OPERATION)
    P11(0x37, STBT,  4, BIT_OPERATION)
    P11(0x38, TFM,   3, TRANSFER_MEM) /* R+,R+ */
    P11(0x39, TFM,   3, TRANSFER_MEM) /* R-,R- */
    P11(0x3A, TFM,   3, TRANSFER_MEM) /* R+,R */
    P11(0x3B, TFM,   3, TRANSFER_MEM) /* R,R+ */
    P11(0x3C, BITMD, 3, IMMEDIATE)
    P11(0x3D, LDMD,  3, IMMEDIATE)
    P11(0x43, COME,  2, INHERENT)
    P11(0x4A, DECE,  2, INHERENT)
    P11(0x4C, INCE,  2, INHERENT)
    P11(0x4D, TSTE,  2, INHERENT)
    P11(0x4F, CLRE,  2, INHERENT)
    P11(0x53, COMF,  2, INHERENT)
    P11(0x5A, DECF,  2, INHERENT)
    P11(0x5C, INCF,  2, INHERENT)
    P11(0x5D, TSTF,  2, INHERENT)
    P11(0x5F, CLRF,  2, INHERENT)
    P11(0x80, SUBE,  3, IMMEDIATE)
    P11(0x81, CMPE,  3, IMMEDIATE)
    P11(0x86, LDE,   3, IMMEDIATE)
    P11(0x8B, ADDE,  3, IMMEDIATE)
    P11(0x8D, DIVD,  4, IMMEDIATE)
    P11(0x8E, DIVQ,  4, IMMEDIATE)
    P11(0x8F, MULD,  4, IMMEDIATE)
    P11(0x90, SUBE,  3, DIRECT_PG)
    P11(0x91, CMPE,  3, DIRECT_PG)
    P11(0x96, LDE,   3, DIRECT_PG)
    P11(0x97, STE,   3, DIRECT_PG)
    P11(0x9B, ADDE,  3, DIRECT_PG)
    P11(0x9D, DIVD,  3, DIRECT_PG)
    P11(0x9E, DIVQ,  3, DIRECT_PG)
    P11(0x9F, MULD,  3, DIRECT_PG)
    P11(0xA0, SUBE,  3, INDEXED)
    P11(0xA1, CMPE,  3, INDEXED)
    P11(0xA6, LDE,   3, INDEXED)
    P11(0xA7, STE,   3, INDEXED)
    P11(0xAB, ADDE,  3, INDEXED)
    P11(0xAD, DIVD,  3, INDEXED)
    P11(0xAE, DIVQ,  3, INDEXED)
    P11(0xAF, MULD,  3, INDEXED)
    P11(0xB0, SUBE,  4, EXTENDED)
    P11(0xB1, CMPE,  4, EXTENDED)
    P11(0xB6, LDE,   4, EXTENDED)
    P11(0xB7, STE,   4, EXTENDED)
    P11(0xBB, ADDE,  4, EXTENDED)
    P11(0xBD, DIVD,  4, EXTENDED)
    P11(0xBE, DIVQ,  4, EXTENDED)
    P11(0xBF, MULD,  4, EXTENDED)
    P11(0xC0, SUBF,  3, IMMEDIATE)
    P11(0xC1, CMPF,  3, IMMEDIATE)
    P11(0xC6, LDF,   3, IMMEDIATE)
    P11(0xCB, ADDF,  3, IMMEDIATE)
    P11(0xD0, SUBF,  3, DIRECT_PG)
    P11(0xD1, CMPF,  3, DIRECT_PG)
    P11(0xD6, LDF,   3, DIRECT_PG)
    P11(0xD7, STF,   3, DIRECT_PG)
    P11(0xDB, ADDF,  3, DIRECT_PG)
    P11(0xE0, SUBF,  3, INDEXED)
    P11(0xE1, CMPF,  3, INDEXED)
    P11(0xE6, LDF,   3, INDEXED)
    P11(0xE7, STF,   3, INDEXED)
    P11(0xEB, ADDF,  3, INDEXED)
    P11(0xF0, SUBF,  4, EXTENDED)
    P11(0xF1, CMPF,  4, EXTENDED)
    P11(0xF6, LDF,   4, EXTENDED)
    P11(0xF7, STF,   4, EXTENDED)
    P11(0xFB, ADDF,  4, EXTENDED)
};

constexpr host::uindex_t LENGTH_P00 = sizeof(TABLE_P00) / sizeof(TABLE_P00[0]);
constexpr host::uindex_t LENGTH_P10 = sizeof(TABLE_P10) / sizeof(TABLE_P10[0]);
constexpr host::uindex_t LENGTH_P11 = sizeof(TABLE_P11) / sizeof(TABLE_P11[0]);

constexpr target::opcode_t PREFIX_P00 = 0x00;
constexpr target::opcode_t PREFIX_P10 = 0x10;
constexpr target::opcode_t PREFIX_P11 = 0x11;

static const TableMc6809::EntryPage PAGES[] = {
    { PREFIX_P00, &TABLE_P00[0], &TABLE_P00[LENGTH_P00] },
    { PREFIX_P10, &TABLE_P10[0], &TABLE_P10[LENGTH_P10] },
    { PREFIX_P11, &TABLE_P11[0], &TABLE_P11[LENGTH_P11] },
};
constexpr host::uindex_t PAGES_LENGTH = sizeof(PAGES) / sizeof(PAGES[0]);

Error TableHd6309::search(Insn &insn, const char *name) const {
    if (TableMc6809::search(insn, name) == OK)
        return OK;
    return searchPages(insn, name, &PAGES[0], &PAGES[PAGES_LENGTH]);
}

Error TableHd6309::search(Insn &insn, AddrMode mode) const {
    if (TableMc6809::search(insn, mode) == OK)
        return OK;
    return searchPages(insn, mode, &PAGES[0], &PAGES[PAGES_LENGTH]);
}

Error TableHd6309::search(Insn &insn, target::insn_t insnCode) const {
    if (TableMc6809::search(insn, insnCode) == OK)
        return OK;
    return searchPages(insn, insnCode, &PAGES[0], &PAGES[PAGES_LENGTH]);
}

class TableHd6309 TableHd6309;
