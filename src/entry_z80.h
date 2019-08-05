/* -*- mode: c++; -*- */
#ifndef __ENTRY_Z80_H__
#define __ENTRY_Z80_H__

#include "config_z80.h"

struct Entry {
    const target::opcode_t opc;
    const host::uint_t flags1;
    const host::uint_t flags2;
    const char *name;

    static constexpr host::uint_t insnFormat_mask = 0x7;
    static constexpr host::uint_t oprFormat_shift = 3;
    static constexpr host::uint_t oprFormat_mask = 0x1f;
    static constexpr host::uint_t addrMode_mask = 0x7;
};

static inline InsnFormat _insnFormat(host::uint_t flags1) {
    return InsnFormat(flags1 & Entry::insnFormat_mask);
}
static inline OprFormat _oprFormat(host::uint_t flags) {
    return OprFormat((flags >> Entry::oprFormat_shift) & Entry::oprFormat_mask);
}
static inline AddrMode _addrMode(host::uint_t flags2) {
    return AddrMode(flags2 & Entry::addrMode_mask);
}
static constexpr host::uint_t _flags1(OprFormat leftOpr, InsnFormat iformat) {
    return (host::uint_t(leftOpr) << Entry::oprFormat_shift) | host::uint_t(iformat);
}
static constexpr host::uint_t _flags2(OprFormat rightOpr, AddrMode addrMode) {
    return (host::uint_t(rightOpr) << Entry::oprFormat_shift) | host::uint_t(addrMode);
}

#define E(_opc, _name, _iformat, _leftOpr, _rightOpr, _amode)           \
    { _opc, _flags1(_leftOpr, _iformat), _flags2(_rightOpr, _amode), TEXT_##_name },

#endif // __ENTRY_Z80_H__
