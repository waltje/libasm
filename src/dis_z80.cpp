#include "dis_z80.h"
#include "table_z80.h"

#include <string.h>

bool DisZ80::acceptCpu(const char *cpu) {
    return strcasecmp(cpu, "Z80") == 0;
}

template<typename T>
void DisZ80::outAddress(T addr, bool indir) {
    if (indir) *_operands++ = '(';
    const char *label = lookup(addr);
    if (label) {
        outText(label);
    } else {
        outConstant(addr, 16, false);
    }
    if (indir) *_operands++ = ')';
    *_operands = 0;
}

void DisZ80::outIndexOffset(
    target::insn_t insnCode, int8_t offset) {
    *_operands++ = '(';
    outRegister(TableZ80::decodeIndexReg(insnCode));
    if (offset >= 0) *_operands++ = '+';
    outConstant(offset, 10);
    *_operands++ = ')';
    *_operands = 0;
}

void DisZ80::outRegister(RegName regName) {
    _operands = _regs.outRegName(_operands, regName);
}

void DisZ80::outPointer(RegName regName) {
    char *out = _operands;
    *out++ = '(';
    out = _regs.outRegName(out, regName);
    *out++ = ')';
    *(_operands = out) = 0;
}

void DisZ80::outDataRegister(RegName regName) {
    if (regName == REG_UNDEF) {
        outPointer(REG_HL);
    } else {
        _operands = _regs.outRegName(_operands, regName);
    }
}

void DisZ80::outConditionName(target::opcode_t cc, bool cc8) {
    if (cc8) {
        _operands = _regs.outCc8Name(_operands, cc);
    } else {
        _operands = _regs.outCc4Name(_operands, cc);
    }
}

Error DisZ80::decodeInherent(Insn& insn) {
    const target::opcode_t opc = TableZ80::opCode(insn.insnCode());
    switch (insn.leftFormat()) {
    case A_REG:
        outRegister(REG_A);
        break;
    case REG_8:
        if (insn.insnFormat() == DST_FMT || insn.insnFormat() == DST_SRC_FMT) {
            RegName regName = RegZ80::decodeDataReg(opc >> 3);
            if (regName == REG_UNDEF && insn.rightFormat() == C_PTR)
                return setError(ILLEGAL_OPERAND);
            outDataRegister(regName);
        } else if (insn.insnFormat() == SRC_FMT) {
            outDataRegister(RegZ80::decodeDataReg(opc));
        } else return setError(INTERNAL_ERROR);
        break;
    case IR_REG:
        outRegister(RegZ80::decodeIrReg(opc >> 3));
        break;
    case BC_REG:
        outRegister(REG_BC);
        break;
    case DE_REG:
        outRegister(REG_DE);
        break;
    case HL_REG:
        outRegister(REG_HL);
        break;
    case AF_REG:
        outRegister(REG_AF);
        break;
    case SP_REG:
        outRegister(REG_SP);
        break;
    case REG_16:
        outRegister(RegZ80::decodePointerReg(opc >> 4));
        break;
    case STK_16:
        outRegister(RegZ80::decodeStackReg(opc >> 4));
        break;
    case IX_REG:
        outRegister(TableZ80::decodeIndexReg(insn.insnCode()));
        break;
    case COND_8:
        outConditionName((opc >> 3) & 7);
        break;
    case C_PTR:
        if (insn.rightFormat() == REG_8 && insn.insnFormat() == DST_FMT
            && RegZ80::decodeDataReg(opc >> 3) == REG_UNDEF)
            return setError(ILLEGAL_OPERAND);
        outPointer(REG_C);
        break;
    case BC_PTR:
        outPointer(RegZ80::decodeIndexReg(opc >> 4));
        break;
    case HL_PTR:
        outPointer(REG_HL);
        break;
    case SP_PTR:
        outPointer(REG_SP);
        break;
    case IX_PTR:
        outPointer(TableZ80::decodeIndexReg(insn.insnCode()));
        break;
    case VEC_NO:
        outConstant(uint8_t(opc & 0x38), 16, false);
        break;
    case BIT_NO:
        outConstant(uint8_t((opc >> 3) & 7));
        break;
    case IMM_NO:
        switch (opc) {
        case 0x46: *_operands++ = '0'; break;
        case 0x56: *_operands++ = '1'; break;
        case 0x5E: *_operands++ = '2'; break;
        default: return setError(ILLEGAL_OPERAND);
        }
        *_operands = 0;
        break;
    default:
        break;
    }

    if (insn.leftFormat() != NO_OPR && insn.rightFormat() != NO_OPR)
        *_operands++ = ',';

    switch (insn.rightFormat()) {
    case A_REG:
        outRegister(REG_A);
        break;
    case REG_8:
        if (insn.insnFormat() == SRC_FMT || insn.insnFormat() == DST_SRC_FMT) {
            outDataRegister(RegZ80::decodeDataReg(opc));
        } else if (insn.insnFormat() == DST_FMT) {
            outDataRegister(RegZ80::decodeDataReg(opc >> 3));
        } else return setError(INTERNAL_ERROR);
        break;
    case IR_REG:
        outRegister(RegZ80::decodeIrReg(opc >> 3));
        break;
    case AFPREG:
        outRegister(REG_AFP);
        break;
    case HL_REG:
        outRegister(REG_HL);
        break;
    case REG_16:
        outRegister(RegZ80::decodePointerReg(opc >> 4));
        break;
    case IX_REG:
        outRegister(TableZ80::decodeIndexReg(insn.insnCode()));
        break;
    case REG_16X:
        outRegister(RegZ80::decodePointerReg(opc >> 4, insn.insnCode()));
        break;
    case C_PTR:
        outPointer(REG_C);
        break;
    case BC_PTR:
        outPointer(RegZ80::decodeIndexReg(opc >> 4));
        break;
    default:
        break;
    }

    return setError(OK);
}

Error DisZ80::decodeImmediate8(Insn &insn, uint8_t val) {
    const target::opcode_t opc = TableZ80::opCode(insn.insnCode());
    switch (insn.leftFormat()) {
    case A_REG:
        outRegister(REG_A);
        break;
    case IMM_8:
        outConstant(val);
        break;
    case REG_8:
        outDataRegister(RegZ80::decodeDataReg(opc >> 3));
        break;
    default:
        break;
    }
    if (insn.rightFormat() == IMM_8) {
        *_operands++ = ',';
        outConstant(val);
    }
    return setError(OK);
}

Error DisZ80::decodeImmediate16(Insn &insn, uint16_t val) {
    const target::opcode_t opc = TableZ80::opCode(insn.insnCode());
    switch (insn.leftFormat()) {
    case REG_16:
        outRegister(RegZ80::decodePointerReg((opc >> 4) & 3));
        break;
    case IX_REG:
        outRegister(TableZ80::decodeIndexReg(insn.insnCode()));
        break;
    default:
        break;
    }
    *_operands++ = ',';
    outConstant(val);
    return setError(OK);
}

Error DisZ80::decodeDirect(Insn &insn, target::uintptr_t addr) {
    const target::opcode_t opc = TableZ80::opCode(insn.insnCode());
    switch (insn.leftFormat()) {
    case ADDR_16:
        outAddress(addr);
        break;
    case HL_REG:
        outRegister(REG_HL);
        break;
    case A_REG:
        outRegister(REG_A);
        break;
    case COND_8:
        outConditionName((opc >> 3) & 7);
        break;
    case IMM_16:
        outAddress(addr, false);
        break;
    case REG_16:
        outRegister(RegZ80::decodePointerReg((opc >> 4) & 3));
        break;
    case IX_REG:
        outRegister(TableZ80::decodeIndexReg(insn.insnCode()));
        break;
    default:
        break;
    }
    if (insn.rightFormat() != NO_OPR) *_operands++ = ',';
    switch (insn.rightFormat()) {
    case HL_REG:
        outRegister(REG_HL);
        break;
    case ADDR_16:
        outAddress(addr);
        break;
    case A_REG:
        outRegister(REG_A);
        break;
    case IMM_16:
        outAddress(addr, false);
        break;
    case REG_16:
        outRegister(RegZ80::decodePointerReg((opc >> 4) & 3));
        break;
    case IX_REG:
        outRegister(TableZ80::decodeIndexReg(insn.insnCode()));
        break;
    default:
        break;
    }
    return setError(OK);
}

Error DisZ80::decodeIoaddr(Insn &insn, uint8_t ioaddr) {
    switch (insn.leftFormat()) {
    case ADDR_8:
        outAddress(ioaddr);
        break;
    case A_REG:
        outRegister(REG_A);
        break;
    default:
        break;
    }
    *_operands++ = ',';
    switch (insn.rightFormat()) {
    case ADDR_8:
        outAddress(ioaddr);
        break;
    case A_REG:
        outRegister(REG_A);
        break;
    default:
        break;
    }
    return setError(OK);
}

Error DisZ80::decodeRelative(Insn &insn, int8_t delta) {
    if (insn.leftFormat() == COND_4) {
        const target::opcode_t opc = TableZ80::opCode(insn.insnCode());
        outConditionName((opc >> 3) & 3, false);
        *_operands++ = ',';
    }
    const target::uintptr_t addr = insn.address() + 2 + delta;
    outAddress(addr, false);
    return setError(OK);
}

Error DisZ80::decodeIndexed(Insn &insn, int8_t offset) {
    const target::opcode_t opc = TableZ80::opCode(insn.insnCode());
    RegName regName;
    switch (insn.leftFormat()) {
    case IX_OFF:
        if (insn.rightFormat() == REG_8 && RegZ80::decodeDataReg(opc) == REG_UNDEF)
            return setError(ILLEGAL_OPERAND);
        outIndexOffset(insn.insnCode(), offset);
        break;
    case REG_8:
        regName = RegZ80::decodeDataReg(opc >> 3);
        if (insn.rightFormat() == IX_OFF && regName == REG_UNDEF)
            return setError(ILLEGAL_OPERAND);
        outDataRegister(regName);
        break;
    case A_REG:
        outRegister(REG_A);
        break;
    default:
        break;
    }
    if (insn.rightFormat() != NO_OPR) *_operands++ = ',';
    switch (insn.rightFormat()) {
    case IX_OFF:
        outIndexOffset(insn.insnCode(), offset);
        break;
    case REG_8:
        outDataRegister(RegZ80::decodeDataReg(opc));
        break;
    default:
        break;
    }
    return setError(OK);
}

Error DisZ80::decodeIndexedImmediate8(
    Insn &insn, int8_t offset, uint8_t val) {
    outIndexOffset(insn.insnCode(), offset);
    *_operands++ = ',';
    outConstant(val);
    return setError(OK);
}

Error DisZ80::decodeIndexedBitOp(
    Insn &insn, int8_t offset, target::opcode_t opCode) {
    const target::opcode_t opc = TableZ80::opCode(insn.insnCode());
    Insn ixBit;
    ixBit.resetAddress(insn.address());
    ixBit.setInsnCode(TableZ80::insnCode(opc, opCode));
    if (TableZ80.searchInsnCode(ixBit)) return getError();
    insn.setName(ixBit.name());
    insn.setFlags(ixBit);

    const RegName regName = RegZ80::decodeDataReg(opCode);
    switch (insn.leftFormat()) {
    case HL_PTR:
        outIndexOffset(insn.insnCode(), offset);
        break;
    case REG_8:
        if (regName == REG_UNDEF) {
            outIndexOffset(insn.insnCode(), offset);
        } else {
            outDataRegister(regName);
        }
        break;
    case BIT_NO:
        outConstant(uint8_t((opCode >> 3) & 7));
        break;
    default:
        break;
    }
    if (insn.rightFormat() != NO_OPR) *_operands++ = ',';

    if (insn.rightFormat() == HL_PTR) {
        outIndexOffset(insn.insnCode(), offset);
    } else if (insn.rightFormat() == REG_8) {
        if (regName == REG_UNDEF) {
            outIndexOffset(insn.insnCode(), offset);
        } else {
            outDataRegister(regName);
        }
    }
    return setError(OK);
}

Error DisZ80::decode(
    DisMemory<target::uintptr_t> &memory, Insn &insn) {
    target::opcode_t opCode;
    if (insn.readByte(memory, opCode)) return setError(NO_MEMORY);
    target::insn_t insnCode = opCode;
    if (TableZ80::isPrefixCode(opCode)) {
        const target::opcode_t prefix = opCode;
        if (insn.readByte(memory, opCode)) return setError(NO_MEMORY);
        insnCode = TableZ80::insnCode(prefix, opCode);
    }
    insn.setInsnCode(insnCode);

    if (TableZ80.searchInsnCode(insn))
        return setError(UNKNOWN_INSTRUCTION);

    uint8_t u8;
    uint16_t u16;
    uint8_t offset;

    switch (insn.addrMode()) {
    case INHR:
        return decodeInherent(insn);
    case IMM8:
        if (insn.readByte(memory, u8)) return setError(NO_MEMORY);
        return decodeImmediate8(insn, u8);
    case IMM16:
        if (insn.readUint16(memory, u16)) return setError(NO_MEMORY);
        return decodeImmediate16(insn, u16);
    case DIRECT:
        if (insn.readUint16(memory, u16)) return setError(NO_MEMORY);
        return decodeDirect(insn, u16);
    case IOADR:
        if (insn.readByte(memory, u8)) return setError(NO_MEMORY);
        return decodeIoaddr(insn, u8);
    case REL8:
        if (insn.readByte(memory, u8)) return setError(NO_MEMORY);
        return decodeRelative(insn, u8);
    case INDX:
        if (insn.readByte(memory, u8)) return setError(NO_MEMORY);
        return decodeIndexed(insn, u8);
    case INDX_IMM8:
        if (insn.readByte(memory, offset)) return setError(NO_MEMORY);
        if (insn.readByte(memory, u8)) return setError(NO_MEMORY);
        if (insn.leftFormat() == IX_BIT)
            return decodeIndexedBitOp(insn, offset, u8);
        return decodeIndexedImmediate8(insn, offset, u8);
    default:
        return setError(INTERNAL_ERROR);
    }
}
