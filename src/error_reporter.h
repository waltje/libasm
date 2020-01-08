/* -*- mode: c++; -*- */
#ifndef __ERROR_REPORTER_H__
#define __ERROR_REPORTER_H__

#include "config_host.h"

enum Error : host::uint_t {
    OK = 0,
    INVALID_STATE = 1,

    // Disassembler
    NO_MEMORY = 2,
    INTERNAL_ERROR = 3,
    UNKNOWN_POSTBYTE = 4,       // HD6309
    ILLEGAL_REGISTER = 5,
    ILLEGAL_OPERAND = 6,        // Z80

    // Assembler
    NO_INSTRUCTION = 20,
    UNKNOWN_INSTRUCTION = 21,
    UNKNOWN_REGISTER = 22,
    UNKNOWN_OPERAND = 23,
    OPERAND_NOT_ZP = 24,        // R6502
    OPERAND_TOO_FAR = 25,
    ILLEGAL_BIT_NUMBER = 16,    // HD6309
    GARBAGE_AT_END = 27,

    // AsmOperand
    ILLEGAL_CONSTANT = 100,
    OVERFLOW_RANGE = 101,
    MISSING_CLOSING_PAREN = 102,
    MISSING_CLOSING_QUOTE = 103,
    UNKNOWN_ESCAPE_SEQUENCE = 104,
    UNKNOWN_EXPR_OPERATOR = 105,
    DIVIDE_BY_ZERO = 106,
    UNDEFINED_SYMBOL = 107,
};

class ErrorReporter {
public:
    Error getError() const { return _error; }

protected:
    Error _error;

    Error setError(Error error) {
        _error = error;
        return error;
    }

    Error setError(const ErrorReporter *other) {
        return setError(other->getError());
    }

    void resetError() {
        _error = INVALID_STATE;
    }
};

#endif // __ERROR_HD6309_H__
