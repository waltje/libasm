/* -*- mode: c++; -*- */
#ifndef __ASM_R65C02_IMPL_H__
#define __ASM_R65C02_IMPL_H__

#include <ctype.h>
#include <string.h>

template<>
bool Asm6502<R6502>::acceptCpu(const char *cpu) {
    return strcasecmp(cpu, "6502") == 0;
}

template<>
bool Asm6502<R65C02>::acceptCpu(const char *cpu) {
    return strcasecmp(cpu, "65c02") == 0
        || strcasecmp(cpu, "6502") == 0;
}

template<McuType mcuType>
Error Asm6502<mcuType>::encodeRelative(Insn &insn, bool emitInsn) {
    target::uintptr_t addr;
    if (getOperand16(addr)) return getError();
    const target::uintptr_t base = insn.address() + (emitInsn ? 2 : 3);
    const target::ptrdiff_t delta = addr - base;
    if (emitInsn) emitInsnCode(insn);
    if (delta >= 128 || delta < -128) return setError(OPERAND_TOO_FAR);
    insn.emitByte(uint8_t(delta));
    return checkLineEnd();
}

#ifdef R65C02_ENABLE_BITOPS
template<McuType mcuType>
Error Asm6502<mcuType>::encodeZeroPageRelative(Insn &insn) {
    if (*_scan == '<') _scan++;
    uint16_t zp;
    if (getOperand(zp)) return getError();
    if (*_scan != ',') return setError(UNKNOWN_OPERAND);
    _scan++;
    emitInsnCode(insn);
    insn.emitByte(zp);
    return encodeRelative(insn, /* emitInsn */ false);
}
#endif

template<McuType mcuType>
Error Asm6502<mcuType>::parseOperand(Insn &insn, uint16_t &val16) {
    char c = toupper(*_scan);
    if (c == '#') {
        _scan++;
        uint8_t val8;
        if (getOperand8(val8)) return getError();
        val16 = val8;
        if (checkLineEnd()) return getError();
        insn.setAddrMode(IMMEDIATE);
        return OK;
    }
    if (c == 'A' && checkLineEnd(_scan + 1) == OK) {
        insn.setAddrMode(ACCUMULATOR);
        return OK;
    }
    const bool indirect = (c == '(');
    if (indirect) _scan++;
    const char mode = *_scan;
    if (mode == '<' || mode == '>') _scan++;
    if (getOperand16(val16)) return getError();
    if (!indirect && checkLineEnd() == OK) {
        if (mode == '>' || val16 >= 0x0100) {
            insn.setAddrMode(ABSOLUTE);
            return OK;
        }
        if (mode == '<' || val16 < 0x0100) {
            insn.setAddrMode(ZEROPAGE);
            return OK;
        }
        return setError(OPERAND_TOO_FAR);
    }

    c = *_scan++;
    if (c == ')' && indirect) {
        if (checkLineEnd() == OK) {
            if (mode == '>' || val16 >= 0x0100) {
                insn.setAddrMode(ABS_INDIRECT);
                return OK;
            }
            if (mode == '<' || val16 < 0x0100) {
                insn.setAddrMode(ZP_INDIRECT);
                return OK;
            }
            return setError(OPERAND_NOT_ZP);
        }
        if (*_scan++ != ',')
            return setError(UNKNOWN_OPERAND);
        if (toupper(*_scan) == 'Y' && checkLineEnd(_scan + 1) == OK) {
            if (mode == '<' || val16 < 0x0100) {
                insn.setAddrMode(INDIRECT_IDX);
                return OK;
            }
            return setError(OPERAND_NOT_ZP);
        }
        return setError(UNKNOWN_OPERAND);
    }

    if (c != ',') return setError(UNKNOWN_OPERAND);
    const char index = toupper(*_scan++);
    if (index != 'X' && index != 'Y') return setError(UNKNOWN_OPERAND);

    if (!indirect && checkLineEnd() == OK) {
        if (mode == '>' || val16 >= 0x0100)  {
            insn.setAddrMode(index == 'X' ? ABS_IDX_X : ABS_IDX_Y);
            return OK;
        }
        if (mode == '<' || val16 < 0x100) {
            insn.setAddrMode(index == 'X' ? ZP_IDX_X  : ZP_IDX_Y);
            return OK;
        }
        return setError(OPERAND_NOT_ZP);
    }
    if (indirect && index == 'X' && *_scan == ')'
        && checkLineEnd(_scan + 1) == OK) {
        insn.setAddrMode(INDX_IND);
        return OK;
    }
    return setError(UNKNOWN_OPERAND);
}

template<McuType mcuType>
Error Asm6502<mcuType>::encode(
    const char *line, Insn &insn, target::uintptr_t addr,
    SymbolTable *symtab) {
    reset(skipSpaces(line), symtab);
    insn.resetAddress(addr);

    if (checkLineEnd() == OK) return setError(NO_INSTRUCTION);
    const char *endName = _parser.scanSymbol(_scan);
    insn.setName(_scan, endName);

    if (TableR65c02<mcuType>::table()->searchName(insn))
        return setError(UNKNOWN_INSTRUCTION);
    if (insn.mcuType() == R65C02 && mcuType != R65C02)
        return setError(UNKNOWN_INSTRUCTION);
    _scan = skipSpaces(endName);

    switch (insn.addrMode()) {
    case IMPLIED:
        emitInsnCode(insn);
        return checkLineEnd();
    case REL8:
        return encodeRelative(insn, /* emitInsn */ true);
    default:
#ifdef R65C02_ENABLE_BITOPS
        if (mcuType == R65C02 && insn.addrMode() == ZP_REL8)
            return encodeZeroPageRelative(insn);
#endif
        break;
    }

    uint16_t val16;
    if (parseOperand(insn, val16)) return getError();
    if (TableR65c02<mcuType>::table()->searchNameAndAddrMode(insn))
        return setError(UNKNOWN_INSTRUCTION);
    switch (insn.addrMode()) {
    case ACCUMULATOR:
        emitInsnCode(insn);
        return setError(OK);
    case IMMEDIATE:
    case ZEROPAGE:
    case ZP_IDX_X:
    case ZP_IDX_Y:
    case INDX_IND:
    case INDIRECT_IDX:
    case ZP_INDIRECT:
        emitInsnCode(insn);
        insn.emitByte(uint8_t(val16));
        return setError(OK);
    case ABSOLUTE:
    case ABS_IDX_X:
    case ABS_IDX_Y:
    case ABS_INDIRECT:
    case IDX_ABS_IND:
        emitInsnCode(insn);
        insn.emitUint16(val16);
        return setError(OK);
    default:
        return setError(INTERNAL_ERROR);
    }
}

#endif // __ASM_R65C02_IMPL_H__
