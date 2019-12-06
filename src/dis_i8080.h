/* -*- mode: c++; -*- */
#ifndef __DIS_I8080_H__
#define __DIS_I8080_H__

#include "config_i8080.h"

#include "error_reporter.h"
#include "insn_i8080.h"
#include "memory.h"
#include "symbol_table.h"

class Disassembler : public ErrorReporter {
public:
    Error decode(
        Memory &memory, Insn& insn, char *operands, SymbolTable *symtab);

private:
    char *_operands;
    SymbolTable *_symtab;

    void reset(char *operands, SymbolTable *symtab) {
        *(_operands = operands) = 0;
        _symtab = symtab;
        resetError();
    }

    const char *lookup(target::uintptr_t addr) const {
        return _symtab ? _symtab->lookup(addr) : nullptr;
    }

    void outChar(char c) { *_operands++ = c; *_operands = 0; }
    void outText(const char *text);
    void outOpr8Hex(target::byte_t val);
    void outOpr16Hex(target::uint16_t val);
    void outOpr16Int(target::uint16_t val);
    void outRegister(RegName regName);

    Error decodeImmediate8(Memory &memory, Insn &insn);
    Error decodeImmediate16(Memory &memory, Insn &insn);
    Error decodeDirect(Memory &memory, Insn &insn);
    Error decodeIoaddr(Memory &memory, Insn &insn);
};

#endif // __DIS_I8080_H__
