/* -*- mode: c++; -*- */
#ifndef __ASM_Z80_H__
#define __ASM_Z80_H__

#include "config_z80.h"

#include "symbol_table.h"
#include "table_z80.h"
#include "asm_interface.h"

class AsmZ80 : public AsmCommon<target::uintptr_t> {
public:
    Error encode(
        const char *line,
        Insn &insn,
        target::uintptr_t addr,
        SymbolTable<target::uintptr_t> *symtab) override;
    bool isRegister(const char *text) const override {
        return RegZ80::parseRegister(text) != REG_UNDEF;
    }

private:
    void emitInsnCode(Insn &insn) const {
        const target::opcode_t prefix = TableZ80::prefixCode(insn.insnCode());
        if (TableZ80::isPrefixCode(prefix))
            insn.emitByte(prefix);
        insn.emitByte(TableZ80::opCode(insn.insnCode()));
    }

    Error parseOperand(
        OprFormat &oprFormat, RegName &regName, uint16_t &operand);
    Error getOperand16(uint16_t &val);

    Error encodeImmediate(Insn &insn, RegName leftReg, uint16_t rightOpr);
    Error encodeInherent(
        Insn &insn, RegName leftReg, RegName rightReg, uint16_t leftOpr);
    Error encodeDirect(
        Insn &insn, RegName leftReg, RegName rightReg,
        target::uintptr_t leftOpr, target::uintptr_t rightOpr);
    Error encodeIoaddr(
        Insn &insn, uint16_t leftOpr, uint16_t rightOpr);
    Error encodeRelative(
        Insn &insn, target::uintptr_t leftOpr, target::uintptr_t rightOpr);
    Error encodeIndexed(
        Insn &insn, RegName leftReg, RegName rightReg,
        uint16_t leftOpr, uint16_t rightOpr);
    Error encodeIndexedImmediate8(
        Insn &insn, RegName leftReg, RegName rightReg,
        uint16_t leftOpr, uint16_t rightOpr);
};

#endif // __ASM_Z80_H__
