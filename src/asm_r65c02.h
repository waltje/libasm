/* -*- mode: c++; -*- */
#ifndef __ASM_R65C02_H__
#define __ASM_R65C02_H__

#include "config_r65c02.h"

#include "symbol_table.h"
#include "table_r65c02.h"
#include "asm_interface.h"

template<McuType mcuType>
class Asm6502 : public Assembler<target::uintptr_t> {
public:
    Error encode(
        const char *line,
        Insn &insn,
        target::uintptr_t addr,
        SymbolTable *symtab) override;
    AsmOperand &getParser() override { return _parser; }
    bool acceptCpu(const char *cpu) const override;

private:
    AsmMotoOperand _parser;

    void emitInsnCode(Insn &insn) const { insn.emitByte(insn.insnCode()); }

    Error parseOperand(Insn &insn, uint16_t &val);

    Error encodeRelative(Insn &insn, bool emitInsn);
    Error encodeZeroPageRelative(Insn &insn);
};

#include "asm_r65c02_impl.h"

typedef Asm6502<R6502> AsmR6502;
typedef Asm6502<R65C02> AsmR65c02;

#endif // __ASM_R65C02_H__
