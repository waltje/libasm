#include <ctype.h>

#include "asm_i8080.h"

static bool isidchar(const char c) {
    return isalnum(c) || c == '_';
}

static Error getInt16(const char *&in, target::uint16_t &val) {
    val = 0;
    const char *p;

    for (p = in; isxdigit(*p); p++)
        ;
    if (p > in && toupper(*p) == 'H') {
        for (p = in; isxdigit(*p); p++) {
            val <<= 4;
            val += isdigit(*p) ? *p - '0' : toupper(*p) - 'A' + 10;
        }
        in = ++p;
        return OK;
    }

    for (p = in; *p >= '0' && *p < '8'; p++)
        ;
    if (p > in && toupper(*p) == 'O') {
        for (p = in; *p >= '0' && *p < '8'; p++) {
            val <<= 3;
            val += *p - '0';
        }
        in = ++p;
        return OK;
    }

    for (p = in; *p == '0' || *p == '1'; p++)
        ;
    if (p > in && toupper(*p) == 'B') {
        for (p = in; *p == '0' || *p == '1'; p++) {
            val <<= 1;
            val += *p - '0';
        }
        in = ++p;
        return OK;
    }

    p = in;
    const char sign = (*p == '+' || *p == '-') ? *p++ : 0;
    if (!isdigit(*p)) return UNKNOWN_OPERAND;
    while (isdigit(*p)) {
        val *= 10;
        val += *p++ - '0';
    }
    in = p;
    if (sign == '-') val = -(target::int16_t)val;
    return OK;
}

Error Assembler::getOperand16(const char *&in, target::uint16_t &val) {
    if (getInt16(in, val) == OK) return setError(OK);
    char symbol_buffer[20];
    host::uint_t idx;
    for (idx = 0; idx < sizeof(symbol_buffer) - 1 && isidchar(in[idx]); idx++) {
        symbol_buffer[idx] = in[idx];
    }
    symbol_buffer[idx] = 0;
    if (hasSymbol(symbol_buffer)) {
        val = lookup(symbol_buffer);
        in += idx;
        return OK;
    }
    return setError(UNKNOWN_OPERAND);
}

static const char *skipSpace(const char *line) {
    while (*line == ' ') line++;
    return line;
}

const char *Assembler::encodePointerReg(const char *line, Insn &insn) {
    const RegName regName = Registers::parsePointerReg(line);
    const host::int_t num = Registers::encodePointerReg(regName);
    if (num < 0) {
        setError(UNKNOWN_REGISTER);
    } else {
        insn.setInsnCode(insn.insnCode() | (num << 4));
        line += Registers::regNameLen(regName);
        setError(OK);
    }
    return line;
}

const char *Assembler::encodeStackReg(const char *line, Insn &insn) {
    const RegName regName = Registers::parseStackReg(line);
    const host::int_t num = Registers::encodeStackReg(regName);
    if (num < 0) {
        setError(UNKNOWN_REGISTER);
    } else {
        insn.setInsnCode(insn.insnCode() | (num << 4));
        line += Registers::regNameLen(regName);
        setError(OK);
    }
    return line;
}

const char *Assembler::encodeIndexReg(const char *line, Insn &insn) {
    const RegName regName = Registers::parseIndexReg(line);
    const host::int_t num = Registers::encodeIndexReg(regName);
    if (num < 0) {
        setError(UNKNOWN_REGISTER);
    } else {
        insn.setInsnCode(insn.insnCode() | (num << 4));
        line += Registers::regNameLen(regName);
        setError(OK);
    }
    return line;
}

const char *Assembler::encodeDataReg(const char *line, Insn &insn) {
    const RegName regName = Registers::parseDataReg(line);
    const host::int_t num = Registers::encodeDataReg(regName);
    if (num < 0) {
        setError(UNKNOWN_REGISTER);
    } else {
        if (insn.insnFormat() == DATA_REG)
            insn.setInsnCode(insn.insnCode() | (num << 3));
        if (insn.insnFormat() == LOW_DATA_REG)
            insn.setInsnCode(insn.insnCode() | num);
        line += Registers::regNameLen(regName);
        setError(OK);
    }
    return line;
}

const char *Assembler::encodeDataDataReg(const char *line, Insn &insn) {
    const RegName dstReg = Registers::parseDataReg(line);
    if (dstReg == REG_UNDEF) {
        setError(UNKNOWN_REGISTER);
        return line;
    }
    line += Registers::regNameLen(dstReg);
    if (*line++ != ',') {
        setError(UNKNOWN_OPERAND);
        return --line;
    }
    const RegName srcReg = Registers::parseDataReg(line);
    if (srcReg == REG_UNDEF) {
        setError(UNKNOWN_REGISTER);
        return line;
    }
    line += Registers::regNameLen(srcReg);

    const host::uint_t dstNum = Registers::encodeDataReg(dstReg);
    const host::uint_t srcNum = Registers::encodeDataReg(srcReg);
    insn.setInsnCode(insn.insnCode() | (dstNum << 3) | srcNum);

    setError(OK);
    return line;
}

const char *Assembler::encodeVectorNo(const char *line, Insn &insn) {
    target::uint16_t vecNo;
    if (getOperand16(line, vecNo) == OK) {
        if (vecNo < 8) {
            insn.setInsnCode(insn.insnCode() | (vecNo << 3));
            setError(OK);
        } else {
            setError(UNKNOWN_OPERAND);
        }
    }
    return line;
}

Error Assembler::encodeImmediate(const char *line, Insn &insn) {
    if (insn.insnFormat() != NO_FORMAT && *line++ != ',')
        return setError(UNKNOWN_OPERAND);
    target::uint16_t val;
    if (getOperand16(line, val)) return getError();
    if (insn.addrMode() == IMM8)
        insn.emitByte(val);
    if (insn.addrMode() == IMM16)
        insn.emitUint16(val);
    return *skipSpace(line) == 0 ? setError(OK) : setError(GARBAGE_AT_END);
}

Error Assembler::encodeDirect(const char *line, Insn &insn) {
    target::uint16_t addr;
    if (getOperand16(line, addr)) return getError();
    insn.emitUint16(addr);
    return *skipSpace(line) == 0 ? setError(OK) : setError(GARBAGE_AT_END);
}

Error Assembler::encodeIoaddr(const char *line, Insn &insn) {
    target::uint16_t addr;
    if (getOperand16(line, addr)) return getError();
    insn.emitByte(addr);
    return *skipSpace(line) == 0 ? setError(OK) : setError(GARBAGE_AT_END);
}

Error Assembler::encode(
    const char *line, Insn &insn, target::uintptr_t addr, SymbolTable *symtab) {
    reset(symtab);
    insn.resetAddress(addr);
    line = skipSpace(line);
    if (!*line) return setError(NO_TEXT);
    const char *endName;
    for (endName = line; isidchar(*endName); endName++)
        ;
    insn.setName(line, endName);
    line = skipSpace(endName);

    if (InsnTable.searchName(insn))
        return setError(UNKNOWN_INSTRUCTION);

    switch (insn.insnFormat()) {
    case NO_FORMAT:
        setError(OK);
        break;
    case POINTER_REG:
        line = encodePointerReg(line, insn);
        break;
    case STACK_REG:
        line = encodeStackReg(line, insn);
        break;
    case INDEX_REG:
        line = encodeIndexReg(line, insn);
        break;
    case DATA_REG:
    case LOW_DATA_REG:
        line = encodeDataReg(line, insn);
        break;
    case DATA_DATA_REG:
        line = encodeDataDataReg(line, insn);
        break;
    case VECTOR_NO:
        line = encodeVectorNo(line, insn);
        break;
    default:
        break;
    }
    if (getError()) return getError();

    insn.emitByte(insn.insnCode());
    switch (insn.addrMode()) {
    case INHR:
        return *skipSpace(line) == 0 ? setError(OK) : setError(GARBAGE_AT_END);
    case IMM8:
    case IMM16:
        return encodeImmediate(line, insn);
    case DIRECT:
        return encodeDirect(line, insn);
    case IOADR:
        return encodeIoaddr(line, insn);
    default:
        return setError(UNKNOWN_OPERAND);
    }
}
