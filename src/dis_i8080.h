/* -*- mode: c++; -*- */
#ifndef __DIS_I8080_H__
#define __DIS_I8080_H__

#include "config_i8080.h"

#include "insn_i8080.h"
#include "dis_memory.h"
#include "symbol_table.h"
#include "dis_interface.h"

class DisI8080 : public Disassembler<target::uintptr_t> {
public:
    DisOperand &getFormatter() override { return _formatter; }

private:
    DisIntelOperand _formatter;

    void outRegister(RegName regName);

    Error decodeImmediate8(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeImmediate16(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeDirect(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decodeIoaddr(DisMemory<target::uintptr_t> &memory, Insn &insn);
    Error decode(
        DisMemory<target::uintptr_t> &memory, Insn& insn) override;
};

#endif // __DIS_I8080_H__
