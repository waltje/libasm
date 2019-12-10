/* -*- mode: c++; -*- */
#ifndef __INSN_I8080_H__
#define __INSN_I8080_H__

#include <string.h>

#include "entry_i8080.h"
#include "dis_memory.h"

class Insn {
public:
    target::uintptr_t address() const { return _address; }
    const uint8_t *bytes() const { return _bytes; }
    host::uint_t insnLen() const { return _insnLen; }
    target::insn_t insnCode() const { return _insnCode; }
    const char *name() const { return _name; }
    AddrMode addrMode() const { return _addrMode(_flags); }
    InsnFormat insnFormat() const { return _insnFormat(_flags); }

    void resetAddress(target::uintptr_t addr) {
        _address = addr;
        _insnLen = 0;
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

    Error readByte(DisMemory<target::uintptr_t> &memory, uint8_t &val) {
        if (!memory.hasNext()) return NO_MEMORY;
        val = memory.readByte();
        emitByte(val);
        return OK;
    }
    Error readUint16(DisMemory<target::uintptr_t> &memory, uint16_t &val) {
        if (!memory.hasNext()) return NO_MEMORY;
        val = memory.readByte();
        if (!memory.hasNext()) return NO_MEMORY;
        val |= (uint16_t)memory.readByte() << 8;
        emitUint16(val);
        return OK;
    }

    void emitByte(uint8_t val) {
        _bytes[_insnLen++] = val;
    }
    void emitUint16(uint16_t val) {
        emitByte(uint8_t(val & 0xff));
        emitByte(uint8_t(val >> 8));
    }

private:
    target::uintptr_t _address;
    target::insn_t    _insnCode;
    host::uint_t      _insnLen;
    host::uint_t      _flags;
    char              _name[5];
    uint8_t    _bytes[3];
};

#endif // __INSN_I8080_H__
