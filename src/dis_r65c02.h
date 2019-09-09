/* -*- mode: c++; -*- */
#ifndef __DIS_R65C02_H__
#define __DIS_R65C02_H__

#include "config_r65c02.h"

#include "error_reporter.h"
#include "insn_r65c02.h"
#include "memory.h"
#include "symbol_table.h"

class Disassembler : public ErrorReporter {
public:
    Disassembler(McuType mcuType = R65C02)
        : _mcuType(mcuType), _symtab(nullptr) {}

    Error decode(
        Memory &memory, Insn& insn,
        char *operands, char *comments, SymbolTable *symtab);

    void setMcuType(McuType mcuType) { _mcuType = mcuType; }

private:
    McuType _mcuType;
    SymbolTable *_symtab;

    void reset(SymbolTable *symtab) {
        _symtab = symtab;
        resetError();
    }

    const char *lookup(uint16_t addr) const {
        return _symtab ? _symtab->lookup(addr) : nullptr;
    }

    Error readByte(Memory &memory, Insn &insn, target::byte_t &val);
    Error readUint16(Memory &memory, Insn &insn, target::uint16_t &val);

    Error decodeImmediate(Memory &memory, Insn &insn, char *operands, char *comments);
    Error decodeAbsolute(Memory &memory, Insn &insn, char *operands, char *comments);
    Error decodeZeroPage(Memory &memory, Insn &insn, char *operands, char *comments);
    Error decodeRelative(Memory &memory, Insn &insn, char *operands, char *comments);
};

#endif // __DIS_R65C02_H__
