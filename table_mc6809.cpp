#include "config_mc6809.h"
#include "insn.h"
#include "table_mc6809.h"
#include "text_mc6809.h"

static const EntryMc6809 TABLE_P00[] PROGMEM = {
    P00(0x00, NEG,   2, DIRECT_PG)
    P00(0x03, COM,   2, DIRECT_PG)
    P00(0x04, LSR,   2, DIRECT_PG)
    P00(0x06, ROR,   2, DIRECT_PG)
    P00(0x07, ASR,   2, DIRECT_PG)
    P00(0x08, ASL,   2, DIRECT_PG)
    P00(0x08, LSL,   2, DIRECT_PG)
    P00(0x09, ROL,   2, DIRECT_PG)
    P00(0x0A, DEC,   2, DIRECT_PG)
    P00(0x0C, INC,   2, DIRECT_PG)
    P00(0x0D, TST,   2, DIRECT_PG)
    P00(0x0E, JMP,   2, DIRECT_PG)
    P00(0x0F, CLR,   2, DIRECT_PG)
    P00(0x12, NOP,   1, INHERENT)
    P00(0x13, SYNC,  1, INHERENT)
    P00(0x16, LBRA,  3, RELATIVE)
    P00(0x17, LBSR,  3, RELATIVE)
    P00(0x19, DAA,   1, INHERENT)
    P00(0x1A, ORCC,  2, IMMEDIATE)
    P00(0x1C, ANDCC, 2, IMMEDIATE)
    P00(0x1D, SEX,   1, INHERENT)
    P00(0x1E, EXG,   2, REGISTERS)
    P00(0x1F, TFR,   2, REGISTERS)
    P00(0x20, BRA,   2, RELATIVE)
    P00(0x21, BRN,   2, RELATIVE)
    P00(0x22, BHI,   2, RELATIVE)
    P00(0x23, BLS,   2, RELATIVE)
    P00(0x24, BHS,   2, RELATIVE)
    P00(0x24, BCC,   2, RELATIVE)
    P00(0x25, BLO,   2, RELATIVE)
    P00(0x25, BCS,   2, RELATIVE)
    P00(0x26, BNE,   2, RELATIVE)
    P00(0x27, BEQ,   2, RELATIVE)
    P00(0x28, BVC,   2, RELATIVE)
    P00(0x29, BVS,   2, RELATIVE)
    P00(0x2A, BPL,   2, RELATIVE)
    P00(0x2B, BMI,   2, RELATIVE)
    P00(0x2C, BGE,   2, RELATIVE)
    P00(0x2D, BLT,   2, RELATIVE)
    P00(0x2E, BGT,   2, RELATIVE)
    P00(0x2F, BLE,   2, RELATIVE)
    P00(0x30, LEAX,  2, INDEXED)
    P00(0x31, LEAY,  2, INDEXED)
    P00(0x32, LEAS,  2, INDEXED)
    P00(0x33, LEAU,  2, INDEXED)
    P00(0x34, PSHS,  2, STACK_OP)
    P00(0x35, PULS,  2, STACK_OP)
    P00(0x36, PSHU,  2, STACK_OP)
    P00(0x37, PULU,  2, STACK_OP)
    P00(0x39, RTS,   1, INHERENT)
    P00(0x3A, ABX,   1, INHERENT)
    P00(0x3B, RTI,   1, INHERENT)
    P00(0x3C, CWAI,  2, IMMEDIATE)
    P00(0x3D, MUL,   1, INHERENT)
    P00(0x3F, SWI,   1, INHERENT)
    P00(0x40, NEGA,  1, INHERENT)
    P00(0x43, COMA,  1, INHERENT)
    P00(0x44, LSRA,  1, INHERENT)
    P00(0x46, RORA,  1, INHERENT)
    P00(0x47, ASRA,  1, INHERENT)
    P00(0x48, ASLA,  1, INHERENT)
    P00(0x48, LSLA,  1, INHERENT)
    P00(0x49, ROLA,  1, INHERENT)
    P00(0x4A, DECA,  1, INHERENT)
    P00(0x4C, INCA,  1, INHERENT)
    P00(0x4D, TSTA,  1, INHERENT)
    P00(0x4F, CLRA,  1, INHERENT)
    P00(0x50, NEGB,  1, INHERENT)
    P00(0x53, COMB,  1, INHERENT)
    P00(0x54, LSRB,  1, INHERENT)
    P00(0x56, RORB,  1, INHERENT)
    P00(0x57, ASRB,  1, INHERENT)
    P00(0x58, ASLB,  1, INHERENT)
    P00(0x58, LSLB,  1, INHERENT)
    P00(0x59, ROLB,  1, INHERENT)
    P00(0x5A, DECB,  1, INHERENT)
    P00(0x5C, INCB,  1, INHERENT)
    P00(0x5D, TSTB,  1, INHERENT)
    P00(0x5F, CLRB,  1, INHERENT)
    P00(0x60, NEG,   2, INDEXED)
    P00(0x63, COM,   2, INDEXED)
    P00(0x64, LSR,   2, INDEXED)
    P00(0x66, ROR,   2, INDEXED)
    P00(0x67, ASR,   2, INDEXED)
    P00(0x68, ASL,   2, INDEXED)
    P00(0x68, LSL,   2, INDEXED)
    P00(0x69, ROL,   2, INDEXED)
    P00(0x6A, DEC,   2, INDEXED)
    P00(0x6C, INC,   2, INDEXED)
    P00(0x6D, TST,   2, INDEXED)
    P00(0x6E, JMP,   2, INDEXED)
    P00(0x6F, CLR,   2, INDEXED)
    P00(0x70, NEG,   3, EXTENDED)
    P00(0x73, COM,   3, EXTENDED)
    P00(0x74, LSR,   3, EXTENDED)
    P00(0x76, ROR,   3, EXTENDED)
    P00(0x77, ASR,   3, EXTENDED)
    P00(0x78, ASL,   3, EXTENDED)
    P00(0x78, LSL,   3, EXTENDED)
    P00(0x79, ROL,   3, EXTENDED)
    P00(0x7A, DEC,   3, EXTENDED)
    P00(0x7C, INC,   3, EXTENDED)
    P00(0x7D, TST,   3, EXTENDED)
    P00(0x7E, JMP,   3, EXTENDED)
    P00(0x7F, CLR,   3, EXTENDED)
    P00(0x80, SUBA,  2, IMMEDIATE)
    P00(0x81, CMPA,  2, IMMEDIATE)
    P00(0x82, SBCA,  2, IMMEDIATE)
    P00(0x83, SUBD,  3, IMMEDIATE)
    P00(0x84, ANDA,  2, IMMEDIATE)
    P00(0x85, BITA,  2, IMMEDIATE)
    P00(0x86, LDA,   2, IMMEDIATE)
    P00(0x88, EORA,  2, IMMEDIATE)
    P00(0x89, ADCA,  2, IMMEDIATE)
    P00(0x8A, ORA,   2, IMMEDIATE)
    P00(0x8B, ADDA,  2, IMMEDIATE)
    P00(0x8C, CMPX,  3, IMMEDIATE)
    P00(0x8D, BSR,   2, RELATIVE)
    P00(0x8E, LDX,   3, IMMEDIATE)
    P00(0x90, SUBA,  2, DIRECT_PG)
    P00(0x91, CMPA,  2, DIRECT_PG)
    P00(0x92, SBCA,  2, DIRECT_PG)
    P00(0x93, SUBD,  2, DIRECT_PG)
    P00(0x94, ANDA,  2, DIRECT_PG)
    P00(0x95, BITA,  2, DIRECT_PG)
    P00(0x96, LDA,   2, DIRECT_PG)
    P00(0x97, STA,   2, DIRECT_PG)
    P00(0x98, EORA,  2, DIRECT_PG)
    P00(0x99, ADCA,  2, DIRECT_PG)
    P00(0x9A, ORA,   2, DIRECT_PG)
    P00(0x9B, ADDA,  2, DIRECT_PG)
    P00(0x9C, CMPX,  2, DIRECT_PG)
    P00(0x9D, JSR,   2, DIRECT_PG)
    P00(0x9E, LDX,   2, DIRECT_PG)
    P00(0x9F, STX,   2, DIRECT_PG)
    P00(0xA0, SUBA,  2, INDEXED)
    P00(0xA1, CMPA,  2, INDEXED)
    P00(0xA2, SBCA,  2, INDEXED)
    P00(0xA3, SUBD,  2, INDEXED)
    P00(0xA4, ANDA,  2, INDEXED)
    P00(0xA5, BITA,  2, INDEXED)
    P00(0xA6, LDA,   2, INDEXED)
    P00(0xA7, STA,   2, INDEXED)
    P00(0xA8, EORA,  2, INDEXED)
    P00(0xA9, ADCA,  2, INDEXED)
    P00(0xAA, ORA,   2, INDEXED)
    P00(0xAB, ADDA,  2, INDEXED)
    P00(0xAC, CMPX,  2, INDEXED)
    P00(0xAD, JSR,   2, INDEXED)
    P00(0xAE, LDX,   2, INDEXED)
    P00(0xAF, STX,   2, INDEXED)
    P00(0xB0, SUBA,  3, EXTENDED)
    P00(0xB1, CMPA,  3, EXTENDED)
    P00(0xB2, SBCA,  3, EXTENDED)
    P00(0xB3, SUBD,  3, EXTENDED)
    P00(0xB4, ANDA,  3, EXTENDED)
    P00(0xB5, BITA,  3, EXTENDED)
    P00(0xB6, LDA,   3, EXTENDED)
    P00(0xB7, STA,   3, EXTENDED)
    P00(0xB8, EORA,  3, EXTENDED)
    P00(0xB9, ADCA,  3, EXTENDED)
    P00(0xBA, ORA,   3, EXTENDED)
    P00(0xBB, ADDA,  3, EXTENDED)
    P00(0xBC, CMPX,  3, EXTENDED)
    P00(0xBD, JSR,   3, EXTENDED)
    P00(0xBE, LDX,   3, EXTENDED)
    P00(0xBF, STX,   3, EXTENDED)
    P00(0xC0, SUBB,  2, IMMEDIATE)
    P00(0xC1, CMPB,  2, IMMEDIATE)
    P00(0xC2, SBCB,  2, IMMEDIATE)
    P00(0xC3, ADDD,  3, IMMEDIATE)
    P00(0xC4, ANDB,  2, IMMEDIATE)
    P00(0xC5, BITB,  2, IMMEDIATE)
    P00(0xC6, LDB,   2, IMMEDIATE)
    P00(0xC8, EORB,  2, IMMEDIATE)
    P00(0xC9, ADCB,  2, IMMEDIATE)
    P00(0xCA, ORB,   2, IMMEDIATE)
    P00(0xCB, ADDB,  2, IMMEDIATE)
    P00(0xCC, LDD,   3, IMMEDIATE)
    P00(0xCE, LDU,   3, IMMEDIATE)
    P00(0xD0, SUBB,  2, DIRECT_PG)
    P00(0xD1, CMPB,  2, DIRECT_PG)
    P00(0xD2, SBCB,  2, DIRECT_PG)
    P00(0xD3, ADDD,  2, DIRECT_PG)
    P00(0xD4, ANDB,  2, DIRECT_PG)
    P00(0xD5, BITB,  2, DIRECT_PG)
    P00(0xD6, LDB,   2, DIRECT_PG)
    P00(0xD7, STB,   2, DIRECT_PG)
    P00(0xD8, EORB,  2, DIRECT_PG)
    P00(0xD9, ADCB,  2, DIRECT_PG)
    P00(0xDA, ORB,   2, DIRECT_PG)
    P00(0xDB, ADDB,  2, DIRECT_PG)
    P00(0xDC, LDD,   2, DIRECT_PG)
    P00(0xDD, STD,   2, DIRECT_PG)
    P00(0xDE, LDU,   2, DIRECT_PG)
    P00(0xDF, STU,   2, DIRECT_PG)
    P00(0xE0, SUBB,  2, INDEXED)
    P00(0xE1, CMPB,  2, INDEXED)
    P00(0xE2, SBCB,  2, INDEXED)
    P00(0xE3, ADDD,  2, INDEXED)
    P00(0xE4, ANDB,  2, INDEXED)
    P00(0xE5, BITB,  2, INDEXED)
    P00(0xE6, LDB,   2, INDEXED)
    P00(0xE7, STB,   2, INDEXED)
    P00(0xE8, EORB,  2, INDEXED)
    P00(0xE9, ADCB,  2, INDEXED)
    P00(0xEA, ORB,   2, INDEXED)
    P00(0xEB, ADDB,  2, INDEXED)
    P00(0xEC, LDD,   2, INDEXED)
    P00(0xED, STD,   2, INDEXED)
    P00(0xEE, LDU,   2, INDEXED)
    P00(0xEF, STU,   2, INDEXED)
    P00(0xF0, SUBB,  3, EXTENDED)
    P00(0xF1, CMPB,  3, EXTENDED)
    P00(0xF2, SBCB,  3, EXTENDED)
    P00(0xF3, ADDD,  3, EXTENDED)
    P00(0xF4, ANDB,  3, EXTENDED)
    P00(0xF5, BITB,  3, EXTENDED)
    P00(0xF6, LDB,   3, EXTENDED)
    P00(0xF7, STB,   3, EXTENDED)
    P00(0xF8, EORB,  3, EXTENDED)
    P00(0xF9, ADCB,  3, EXTENDED)
    P00(0xFA, ORB,   3, EXTENDED)
    P00(0xFB, ADDB,  3, EXTENDED)
    P00(0xFC, LDD,   3, EXTENDED)
    P00(0xFD, STD,   3, EXTENDED)
    P00(0xFE, LDU,   3, EXTENDED)
    P00(0xFF, STU,   3, EXTENDED)
};

static const EntryMc6809 TABLE_P10[] PROGMEM = {
    P10(0x21, LBRN,  4, RELATIVE)
    P10(0x22, LBHI,  4, RELATIVE)
    P10(0x23, LBLS,  4, RELATIVE)
    P10(0x24, LBHS,  4, RELATIVE)
    P10(0x24, LBCC,  4, RELATIVE)
    P10(0x25, LBLO,  4, RELATIVE)
    P10(0x25, LBCS,  4, RELATIVE)
    P10(0x26, LBNE,  4, RELATIVE)
    P10(0x27, LBEQ,  4, RELATIVE)
    P10(0x28, LBVC,  4, RELATIVE)
    P10(0x29, LBVS,  4, RELATIVE)
    P10(0x2A, LBPL,  4, RELATIVE)
    P10(0x2B, LBMI,  4, RELATIVE)
    P10(0x2C, LBGE,  4, RELATIVE)
    P10(0x2D, LBLT,  4, RELATIVE)
    P10(0x2E, LBGT,  4, RELATIVE)
    P10(0x2F, LBLE,  4, RELATIVE)
    P10(0x3F, SWI2,  2, INHERENT)
    P10(0x83, CMPD,  4, IMMEDIATE)
    P10(0x8C, CMPY,  4, IMMEDIATE)
    P10(0x8E, LDY,   4, IMMEDIATE)
    P10(0x93, CMPD,  3, DIRECT_PG)
    P10(0x9C, CMPY,  3, DIRECT_PG)
    P10(0x9E, LDY,   3, DIRECT_PG)
    P10(0x9F, STY,   3, DIRECT_PG)
    P10(0xA3, CMPD,  3, INDEXED)
    P10(0xAC, CMPY,  3, INDEXED)
    P10(0xAE, LDY,   3, INDEXED)
    P10(0xAF, STY,   3, INDEXED)
    P10(0xB3, CMPD,  4, EXTENDED)
    P10(0xBC, CMPY,  4, EXTENDED)
    P10(0xBE, LDY,   4, EXTENDED)
    P10(0xBF, STY,   4, EXTENDED)
    P10(0xCE, LDS,   4, IMMEDIATE)
    P10(0xDE, LDS,   3, DIRECT_PG)
    P10(0xDF, STS,   3, DIRECT_PG)
    P10(0xEE, LDS,   3, INDEXED)
    P10(0xEF, STS,   3, INDEXED)
    P10(0xFE, LDS,   4, EXTENDED)
    P10(0xFF, STS,   4, EXTENDED)
};

static const EntryMc6809 TABLE_P11[] PROGMEM = {
    P11(0x3F, SWI3,  2, INHERENT)
    P11(0x83, CMPU,  4, IMMEDIATE)
    P11(0x8C, CMPS,  4, IMMEDIATE)
    P11(0x93, CMPU,  3, DIRECT_PG)
    P11(0x9C, CMPS,  3, DIRECT_PG)
    P11(0xA3, CMPU,  3, INDEXED)
    P11(0xAC, CMPS,  3, INDEXED)
    P11(0xB3, CMPU,  4, EXTENDED)
    P11(0xBC, CMPS,  4, EXTENDED)
};

constexpr host::uindex_t LENGTH_P00 = sizeof(TABLE_P00) / sizeof(TABLE_P00[0]);
constexpr host::uindex_t LENGTH_P10 = sizeof(TABLE_P10) / sizeof(TABLE_P10[0]);
constexpr host::uindex_t LENGTH_P11 = sizeof(TABLE_P11) / sizeof(TABLE_P11[0]);

constexpr target::opcode_t PREFIX_P00 = 0x00;
constexpr target::opcode_t PREFIX_P10 = 0x10;
constexpr target::opcode_t PREFIX_P11 = 0x11;

const EntryMc6809 *TableMc6809::searchEntry(
    const char *name,
    const EntryMc6809 *table, const EntryMc6809 *end) {
    for (const EntryMc6809 *entry = table; entry < end; entry++) {
        if (pgm_strcasecmp(name, entry->name) == 0)
            return entry;
    }
    return nullptr;
}

const EntryMc6809 *TableMc6809::searchEntry(
    const char *name, AddrMode mode,
    const EntryMc6809 *table, const EntryMc6809 *end) {
    for (const EntryMc6809 *entry = table; entry < end; entry++) {
        const host::uint_t flags = pgm_read_byte(&entry->flags);
        const AddrMode m = AddrMode((flags >> mode_shift) & mode_mask);
        if (mode == m && pgm_strcasecmp(name, entry->name) == 0)
            return entry;
    }
    return nullptr;
}

const EntryMc6809 *TableMc6809::searchEntry(
    const target::opcode_t opCode,
    const EntryMc6809 *table, const EntryMc6809 *end) {
    for (const EntryMc6809 *entry = table; entry < end; entry++) {
        if (opCode == pgm_read_byte(&entry->opc))
            return entry;
    }
    return nullptr;
}

bool TableMc6809::isPrefixCode(target::opcode_t opCode) {
    return opCode == PREFIX_P10 || opCode == PREFIX_P11;
}

Error TableMc6809::searchPages(
    Insn &insn, const char *name,
    const EntryPage *pages, const EntryPage *end) const {
    for (const EntryPage *page = pages; page < end; page++) {
        const EntryMc6809 *entry;
        if ((entry = searchEntry(name, page->table, page->end)) != nullptr) {
            setInsnCode(insn, insnCode(page->prefix, pgm_read_byte(&entry->opc)));
            const host::uint_t flags = pgm_read_byte(&entry->flags);
            setAddrMode(insn, AddrMode((flags >> mode_shift) & mode_mask));
            setOprLen(insn, flags & oprLen_mask);
            return OK;
        }
    }
    return UNKNOWN_INSTRUCTION;
}

Error TableMc6809::searchPages(
    Insn &insn, AddrMode mode,
    const EntryPage *pages, const EntryPage *end) const {
    const char *name = insn.name();
    for (const EntryPage *page = pages; page < end; page++) {
        const EntryMc6809 *entry;
        if ((entry = searchEntry(name, mode, page->table, page->end)) != nullptr) {
            setInsnCode(insn, insnCode(page->prefix, pgm_read_byte(&entry->opc)));
            const host::uint_t flags = pgm_read_byte(&entry->flags);
            setAddrMode(insn, AddrMode((flags >> mode_shift) & mode_mask));
            setOprLen(insn, flags & oprLen_mask);
            return OK;
        }
    }
    return UNKNOWN_INSTRUCTION;
}

Error TableMc6809::searchPages(
    Insn &insn, target::insn_t insnCode,
    const EntryPage *pages, const EntryPage *end) const {
    for (const EntryPage *page = pages; page < end; page++) {
        if (prefixCode(insnCode) != page->prefix) continue;
        const EntryMc6809 *entry =
            searchEntry(opCode(insnCode), page->table, page->end);
        if (entry) {
            setInsnCode(insn, insnCode);
            const host::uint_t flags = pgm_read_byte(&entry->flags);
            setAddrMode(insn, AddrMode((flags >> mode_shift) & mode_mask));
            setOprLen(insn, flags & oprLen_mask);
            char name[8];
            pgm_strcpy(name, entry->name);
            setName(insn, name);
            return OK;
        }
    }
    return UNKNOWN_INSTRUCTION;
}

static const TableMc6809::EntryPage PAGES[] = {
    { PREFIX_P00, &TABLE_P00[0], &TABLE_P00[LENGTH_P00] },
    { PREFIX_P10, &TABLE_P10[0], &TABLE_P10[LENGTH_P10] },
    { PREFIX_P11, &TABLE_P11[0], &TABLE_P11[LENGTH_P11] },
};
constexpr host::uindex_t PAGES_LENGTH = sizeof(PAGES) / sizeof(PAGES[0]);

Error TableMc6809::search(Insn &insn, const char *name) const {
    return searchPages(insn, name, &PAGES[0], &PAGES[PAGES_LENGTH]);
}

Error TableMc6809::search(Insn &insn, AddrMode mode) const {
    return searchPages(insn, mode, &PAGES[0], &PAGES[PAGES_LENGTH]);
}

Error TableMc6809::search(Insn &insn, target::insn_t insnCode) const {
    return searchPages(insn, insnCode, &PAGES[0], &PAGES[PAGES_LENGTH]);
}

class TableMc6809 TableMc6809;
