/* -*- mode: c++; -*- */
#ifndef __ASM_HD6309_H__
#define __ASM_HD6309_H__

#include "config_hd6309.h"

#include "insn_hd6309.h"
#include "reg_hd6309.h"
#include "table_hd6309.h"
#include "asm_interface.h"

template<McuType mcuType>
class Asm09 : public Assembler<target::uintptr_t> {
public:
    Error encode(
        const char *line,
        Insn &insn,
        target::uintptr_t addr,
        SymbolTable *symtab) override;
    AsmOperand &getParser() override { return _parser; }
    bool acceptCpu(const char *cpu) const override;

private:
    RegHd6309<mcuType> _regs;
    AsmMotoOperand _parser;
    uint8_t _direct_page = 0;

    void emitInsnCode(Insn &insn) const {
        const target::opcode_t prefix = TableHd6309Base::prefixCode(insn.insnCode());
        if (TableHd6309Base::isPrefixCode(prefix))
            insn.emitByte(prefix);
        insn.emitByte(TableHd6309Base::opCode(insn.insnCode()));
    }

    Error determineAddrMode(const char *line, Insn &insn);

    // MC6809
    Error encodeStackOp(Insn &insn);
    Error encodeRegisters(Insn &insn);
    Error encodeRelative(Insn &insn);
    Error encodeImmediate(Insn &insn);
    Error encodeDirect(Insn &insn, bool emitInsn = true);
    Error encodeExtended(Insn &insn, bool emitInsn = true);
    Error encodeIndexed(Insn &insn, bool emitInsn = true);
    // HD6309
    Error encodeBitOperation(Insn &insn);
    Error encodeImmediatePlus(Insn &insn);
    Error encodeTransferMemory(Insn &insn);
    // Pseudo instruction
    Error processPseudo(Insn &insn);
};

#include "asm_hd6309_impl.h"

typedef Asm09<MC6809> AsmMc6809;
typedef Asm09<HD6309> AsmHd6309;

#endif // __ASM_HD6309_H__
