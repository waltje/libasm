/* -*- mode: c++; -*- */
#ifndef __DIS_I8080_H__
#define __DIS_I8080_H__

#include "config_i8080.h"

#include "insn_i8080.h"
#include "dis_memory.h"
#include "symbol_table.h"
#include "dis_interface.h"

class DisI8080 : public DisCommon<target::uintptr_t> {
public:
    Error decode(
        DisMemory<target::uintptr_t> &memory,
        Insn& insn,
        char *operands,
        SymbolTable<target::uintptr_t> *symtab) override;

private:
    void outOpr8Hex(uint8_t val);
    void outOpr16Hex(uint16_t val);
    void outOpr16Int(uint16_t val);
    void outRegister(RegName regName);

    Error decodeImmediate8(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeImmediate16(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeDirect(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeIoaddr(DisMemory<target::uintptr_t> &memory, Insn &insn);
};

#endif // __DIS_I8080_H__
