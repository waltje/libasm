#include <ctype.h>

#include "asm_i8080.h"

static bool isidchar(const char c) {
    return isalnum(c) || c == '_';
}

Error AsmI8080::getInt16(uint16_t &val) {
    uint16_t v = 0;
    const char *p;

    for (p = _scan; isxdigit(*p); p++)
        ;
    if (p > _scan && toupper(*p) == 'H') {
        for (p = _scan; isxdigit(*p); p++) {
            v <<= 4;
            v += isdigit(*p) ? *p - '0' : toupper(*p) - 'A' + 10;
        }
        val = v;
        _scan = ++p;
        return OK;
    }

    for (p = _scan; *p >= '0' && *p < '8'; p++)
        ;
    if (p > _scan && toupper(*p) == 'O') {
        for (p = _scan; *p >= '0' && *p < '8'; p++) {
            v <<= 3;
            v += *p - '0';
        }
        val = v;
        _scan = ++p;
        return OK;
    }

    for (p = _scan; *p == '0' || *p == '1'; p++)
        ;
    if (p > _scan && toupper(*p) == 'B') {
        for (p = _scan; *p == '0' || *p == '1'; p++) {
            v <<= 1;
            v += *p - '0';
        }
        val = v;
        _scan = ++p;
        return OK;
    }

    p = _scan;
    const char sign = (*p == '+' || *p == '-') ? *p++ : 0;
    if (!isdigit(*p)) return UNKNOWN_OPERAND;
    while (isdigit(*p)) {
        v *= 10;
        v += *p++ - '0';
    }
    if (sign == '-') v = -(int16_t)v;
    val = v;
    _scan = p;
    return OK;
}

Error AsmI8080::getOperand16(uint16_t &val) {
    if (getInt16(val) == OK) return setError(OK);
    char symbol_buffer[20];
    host::uint_t idx;
    for (idx = 0; idx < sizeof(symbol_buffer) - 1 && isidchar(_scan[idx]); idx++) {
        symbol_buffer[idx] = _scan[idx];
    }
    symbol_buffer[idx] = 0;
    if (hasSymbol(symbol_buffer)) {
        val = lookup(symbol_buffer);
        _scan += idx;
        return OK;
    }
    return setError(UNKNOWN_OPERAND);
}

static const char *skipSpace(const char *line) {
    while (*line == ' ') line++;
    return line;
}

Error AsmI8080::checkLineEnd() {
    if (*skipSpace(_scan) == 0) return setError(OK);
    return setError(GARBAGE_AT_END);
}

Error AsmI8080::encodePointerReg(Insn &insn) {
    const RegName regName = RegI8080::parsePointerReg(_scan);
    const host::int_t num = RegI8080::encodePointerReg(regName);
    if (num < 0) return setError(UNKNOWN_REGISTER);
    insn.setInsnCode(insn.insnCode() | (num << 4));
    _scan += RegI8080::regNameLen(regName);
    return setError(OK);
}

Error AsmI8080::encodeStackReg(Insn &insn) {
    const RegName regName = RegI8080::parseStackReg(_scan);
    const host::int_t num = RegI8080::encodeStackReg(regName);
    if (num < 0) return setError(UNKNOWN_REGISTER);
    insn.setInsnCode(insn.insnCode() | (num << 4));
    _scan += RegI8080::regNameLen(regName);
    return setError(OK);
}

Error AsmI8080::encodeIndexReg(Insn &insn) {
    const RegName regName = RegI8080::parseIndexReg(_scan);
    const host::int_t num = RegI8080::encodeIndexReg(regName);
    if (num < 0) return setError(UNKNOWN_REGISTER);
    insn.setInsnCode(insn.insnCode() | (num << 4));
    _scan += RegI8080::regNameLen(regName);
    return setError(OK);
}

Error AsmI8080::encodeDataReg(Insn &insn) {
    const RegName regName = RegI8080::parseDataReg(_scan);
    const host::int_t num = RegI8080::encodeDataReg(regName);
    if (num < 0) return setError(UNKNOWN_REGISTER);
    if (insn.insnFormat() == DATA_REG)
        insn.setInsnCode(insn.insnCode() | (num << 3));
    if (insn.insnFormat() == LOW_DATA_REG)
        insn.setInsnCode(insn.insnCode() | num);
    _scan += RegI8080::regNameLen(regName);
    return setError(OK);
}

Error AsmI8080::encodeDataDataReg(Insn &insn) {
    const RegName dstReg = RegI8080::parseDataReg(_scan);
    if (dstReg == REG_UNDEF)
        return setError(UNKNOWN_REGISTER);
    _scan += RegI8080::regNameLen(dstReg);
    if (*_scan != ',') return setError(UNKNOWN_OPERAND);
    const RegName srcReg = RegI8080::parseDataReg(++_scan);
    if (srcReg == REG_UNDEF) return setError(UNKNOWN_REGISTER);
    _scan += RegI8080::regNameLen(srcReg);

    const host::uint_t dstNum = RegI8080::encodeDataReg(dstReg);
    const host::uint_t srcNum = RegI8080::encodeDataReg(srcReg);
    insn.setInsnCode(insn.insnCode() | (dstNum << 3) | srcNum);
    return setError(OK);
}

Error AsmI8080::encodeVectorNo(Insn &insn) {
    uint16_t vecNo;
    if (getOperand16(vecNo) == OK && vecNo < 8) {
        insn.setInsnCode(insn.insnCode() | (vecNo << 3));
        return setError(OK);
    }
    return setError(UNKNOWN_OPERAND);
}

Error AsmI8080::encodeImmediate(Insn &insn) {
    if (insn.insnFormat() != NO_FORMAT && *_scan++ != ',')
        return setError(UNKNOWN_OPERAND);
    uint16_t val;
    if (getOperand16(val)) return getError();
    if (insn.addrMode() == IMM8)
        insn.emitByte(val);
    if (insn.addrMode() == IMM16)
        insn.emitUint16(val);
    return checkLineEnd();
}

Error AsmI8080::encodeDirect(Insn &insn) {
    uint16_t addr;
    if (getOperand16(addr)) return getError();
    insn.emitUint16(addr);
    return checkLineEnd();
}

Error AsmI8080::encodeIoaddr(Insn &insn) {
    uint16_t addr;
    if (getOperand16(addr)) return getError();
    insn.emitByte(addr);
    return checkLineEnd();
}

Error AsmI8080::encode(
    const char *line, Insn &insn, target::uintptr_t addr,
    SymbolTable<target::uintptr_t> *symtab) {
    reset(line, symtab);
    insn.resetAddress(addr);
    _scan = skipSpace(_scan);
    if (!*_scan) return setError(NO_TEXT);
    const char *endName;
    for (endName = _scan; isidchar(*endName); endName++)
        ;
    insn.setName(_scan, endName);

    if (TableI8080.searchName(insn))
        return setError(UNKNOWN_INSTRUCTION);
    _scan = skipSpace(endName);

    switch (insn.insnFormat()) {
    case NO_FORMAT:     setError(OK); break;
    case POINTER_REG:   encodePointerReg(insn); break;
    case STACK_REG:     encodeStackReg(insn); break;
    case INDEX_REG:     encodeIndexReg(insn); break;
    case DATA_REG:
    case LOW_DATA_REG:  encodeDataReg(insn); break;
    case DATA_DATA_REG: encodeDataDataReg(insn); break;
    case VECTOR_NO:     encodeVectorNo(insn); break;
    default: break;
    }
    if (getError()) return getError();

    insn.emitByte(insn.insnCode());
    switch (insn.addrMode()) {
    case INHR:   return checkLineEnd();
    case IMM8:
    case IMM16:  return encodeImmediate(insn);
    case DIRECT: return encodeDirect(insn);
    case IOADR:  return encodeIoaddr(insn);
    default:     return setError(UNKNOWN_OPERAND);
    }
}
