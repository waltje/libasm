/* -*- mode: c++; -*- */
#ifndef __TABLE_Z80_H__
#define __TABLE_Z80_H__

#include "config_z80.h"
#include "insn_z80.h"

class TableZ80 {
public:
    Error searchName(Insn &insn) const;
    Error searchNameAndOprFormats(
        Insn &insn, OprFormat leftOpr, OprFormat rightOpr) const;
    Error searchInsnCode(Insn &insn) const;

    void setMcuType(McuType mcuType) { _mcuType = mcuType; }
    bool isZ80() const { return _mcuType == Z80; }

    static bool isPrefixCode(target::opcode_t opCode);

    static constexpr target::opcode_t PREFIX_IX = 0xDD;
    static constexpr target::opcode_t PREFIX_IY = 0xFD;

private:
    McuType _mcuType;
};

extern TableZ80 TableZ80;

#endif // __TABLE_Z80_H__
