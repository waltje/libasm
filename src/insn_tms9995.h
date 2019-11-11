/* -*- mode: c++; -*- */
#ifndef __INSN_TMS9995_H__
#define __INSN_TMS9995_H__

#include <string.h>

#include "entry_tms9995.h"

class Insn {
public:
    target::uintptr_t address() const { return _address; }
    const target::byte_t *bytes() const { return _bytes; }
    host::uint_t insnLen() const { return _insnLen; }
    target::insn_t insnCode() const { return _insnCode; }
    const char *name() const { return _name; }
    AddrMode addrMode() const { return _addrMode(_flags); }

    void resetAddress(target::uintptr_t addr) {
        _address = addr;
        _insnLen = 0;
    }
    void emitUint16(target::uint16_t val) {
        emitByte(target::byte_t(val >> 8));
        emitByte(target::byte_t(val & 0xff));
    }
    void setInsnCode(target::insn_t insnCode) { _insnCode = insnCode; }
    void setName(const char *name, const char *end = nullptr) {
        if (!end) end = name + strlen(name);
        char *p = _name;
        while (name < end && p < _name + sizeof(_name) - 1)
            *p++ = *name++;
        *p = 0;
    }
    void setFlags(host::uint_t flags) { _flags = flags; }

private:
    target::uintptr_t _address;
    target::insn_t    _insnCode;
    host::uint_t      _insnLen;
    host::uint_t      _flags;
    char              _name[5];
    target::byte_t    _bytes[6];

    void emitByte(target::byte_t val) {
        _bytes[_insnLen++] = val;
    }
};

#endif // __INSN_TMS9995_H__
