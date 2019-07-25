/* -*- mode: c++; -*- */
#ifndef __INSN_Z80_H__
#define __INSN_Z80_H__

#include <string.h>

#include "entry_z80.h"

class Insn {
public:
    target::uintptr_t address() const { return _address; }
    const target::byte_t *bytes() const { return _bytes; }
    host::uint_t insnLen() const { return _insnLen; }
    target::insn_t insnCode() const { return _insnCode; }
    const char *name() const { return _name; }
    AddrMode addrMode() const { return _addrMode(_flags2); }
    InsnFormat insnFormat() const { return _insnFormat(_flags1); }
    OprFormat leftFormat() const { return _oprFormat(_flags1); }
    OprFormat rightFormat() const { return _oprFormat(_flags2); }

    void resetAddress(target::uintptr_t addr) {
        _address = addr;
        _insnLen = 0;
    }
    void emitByte(target::byte_t val) {
        _bytes[_insnLen++] = val;
    }
    void emitUint16(target::uint16_t val) {
        emitByte(target::byte_t(val & 0xff));
        emitByte(target::byte_t(val >> 8));
    }
    void setInsnCode(target::insn_t insnCode) { _insnCode = insnCode; }
    void setName(const char *name, const char *end = nullptr) {
        if (!end) end = name + strlen(name);
        char *p = _name;
        while (name < end && p < _name + sizeof(_name) - 1)
            *p++ = *name++;
        *p = 0;
    }
    void setFlags(host::uint_t flags1, host::uint_t flags2) {
        _flags1 = flags1;
        _flags2 = flags2;
    }
    void setFlags(const Insn &other) {
        _flags1 = other._flags1;
        _flags2 = other._flags2;
    }

private:
    target::uintptr_t _address;
    target::insn_t    _insnCode;
    host::uint_t      _insnLen;
    host::uint_t      _flags1;
    host::uint_t      _flags2;
    char              _name[5];
    target::byte_t    _bytes[4];
};

#endif // __INSN_Z80_H__
