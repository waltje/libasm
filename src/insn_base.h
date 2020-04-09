/*
 * Copyright 2020 Tadashi G. Takaoka
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __INSN_BASE_H__
#define __INSN_BASE_H__

#include <ctype.h>
#include <string.h>

#include "error_reporter.h"
#include "dis_memory.h"

namespace libasm {

class Insn {
public:
    uint32_t address() const { return _address; }
    const uint8_t *bytes() const { return _bytes; }
    host::uint_t length() const { return _length; }
    const char *name() const { return _name; }

    void resetAddress(uint32_t addr) {
        _address = addr;
        _length = 0;
    }

    void setName(const char *name, size_t len) {
        if (len >= NAME_MAX) len = NAME_MAX;
        strncpy(_name, name, len);
        _name[len] = 0;
    }

    void appendName(const char *suffix) {
        strncat(_name, suffix, NAME_MAX - strlen(_name));
    }

    void toLowerName() {
        for (char *p = _name; *p; p++)
            *p = tolower(*p);
    }

    Error emitByte(uint8_t val) {
        return emitByte(val, _length);
    }

    Error emitByte(uint8_t val, host::uint_t pos) {
        if (pos >= CODE_MAX) return NO_MEMORY;
        _bytes[pos++] = val;
        if (_length < pos) _length = pos;
        return OK;
    }

private:
    uint32_t     _address;
    host::uint_t _length;
    static constexpr size_t CODE_MAX = 10;
    uint8_t      _bytes[CODE_MAX];
    static constexpr size_t NAME_MAX = 7;
    char         _name[NAME_MAX+ 1];
};

template<typename Conf>
class InsnBase {
public:
    InsnBase(Insn &insn) : _insn(insn) {}

    typename Conf::uintptr_t address() const { return _insn.address(); }
    const uint8_t *bytes() const { return _insn.bytes(); }
    host::uint_t length() const { return _insn.length(); }
    const char *name() const { return _insn.name(); }

    void resetAddress(typename Conf::uintptr_t addr) {
        _insn.resetAddress(addr);
    }
    void setName(const char *name, const char *end = nullptr) {
        _insn.setName(name, end ? end - name : strlen(name));
    }

    Error readByte(DisMemory<Conf> &memory, uint8_t &val) {
        if (!memory.hasNext()) return NO_MEMORY;
        val = memory.readByte();
        return _insn.emitByte(val);
    }

    Error readUint16(DisMemory<Conf> &memory, uint16_t &val) {
        uint8_t msb, lsb;
        if (Conf::endian == ENDIAN_BIG) {
            if (readByte(memory, msb)) return NO_MEMORY;
            if (readByte(memory, lsb)) return NO_MEMORY;
        } else {
            if (readByte(memory, lsb)) return NO_MEMORY;
            if (readByte(memory, msb)) return NO_MEMORY;
        }
        val = static_cast<uint16_t>(msb) << 8 | lsb;
        return OK;
    }

    Error readUint32(DisMemory<Conf> &memory, uint32_t &val) {
        uint16_t msw, lsw;
        if (Conf::endian == ENDIAN_BIG) {
            if (readUint16(memory, msw)) return NO_MEMORY;
            if (readUint16(memory, lsw)) return NO_MEMORY;
        } else {
            if (readUint16(memory, lsw)) return NO_MEMORY;
            if (readUint16(memory, msw)) return NO_MEMORY;
        }
        val = static_cast<uint32_t>(msw) << 16 | lsw;
        return OK;
    }

    Error emitByte(uint8_t val) {
        return _insn.emitByte(val);
    }

    Error emitUint16(uint16_t val) {
        if (Conf::endian == ENDIAN_BIG) {
            if (emitByte(static_cast<uint8_t>(val >> 8))) return NO_MEMORY;
            if (emitByte(static_cast<uint8_t>(val >> 0))) return NO_MEMORY;
        } else {
            if (emitByte(static_cast<uint8_t>(val >> 0))) return NO_MEMORY;
            if (emitByte(static_cast<uint8_t>(val >> 8))) return NO_MEMORY;
        }
        return OK;
    }

    Error emitUint32(uint32_t val) {
        if (Conf::endian == ENDIAN_BIG) {
            if (emitUint16(static_cast<uint16_t>(val >> 16))) return NO_MEMORY;
            if (emitUint16(static_cast<uint16_t>(val >>  0))) return NO_MEMORY;
        } else {
            if (emitUint16(static_cast<uint16_t>(val >>  0))) return NO_MEMORY;
            if (emitUint16(static_cast<uint16_t>(val >> 16))) return NO_MEMORY;
        }
        return OK;
    }

protected:
    Insn &_insn;
};

} // namespace libasm

#endif // __INSN_BASE_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
