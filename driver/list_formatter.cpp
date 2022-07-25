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

#include "list_formatter.h"

#include <string>

namespace libasm {
namespace driver {

void ListFormatter::reset() {
    _nextContent = _nextLine = 0;
}

void ListFormatter::setUppercase(bool uppercase) {
    _formatter.setUppercase(_uppercase = uppercase);
}

bool ListFormatter::hasNextLine() const {
    return _nextLine < generatedSize();
}

const char *ListFormatter::getLine() {
    _out.reset(_outBuffer, sizeof(_outBuffer));
    formatLine();
    return _outBuffer;
}

void ListFormatter::formatDec(uint32_t val, int8_t width) {
    if (width > 0) {
        auto len = 1;
        for (auto v = val; v >= 10; v /= 10)
            len++;
        for (; len < width; len++)
            _out.letter(' ');
    }
    auto start = _out.mark();
    _formatter.formatDec(_out, val, 32);
    if (width < 0) {
        width = -width;
        for (auto len = _out.mark() - start; len < width; len++)
            _out.letter(' ');
    }
}

void ListFormatter::formatHex(uint32_t val, uint8_t bits, bool zeroSuppress) {
    if (zeroSuppress && bits == 0) {
        bits = 32;
        uint32_t mask = 0x80000000;
        while (mask >= 2 && (val & mask) == 0) {
            mask >>= 1;
            bits--;
        }
    }
    auto start = _out.mark();
    _formatter.formatHex(_out, val, bits, false);
    if (zeroSuppress) {
        while (*start == '0' && start[1])
            *start++ = ' ';
    }
}

void ListFormatter::formatAddress(uint32_t addr, bool fixedWidth, bool zeroSuppress) {
    formatHex(addr, fixedWidth ? uint8_t(config().addressWidth()) : 0, zeroSuppress);
    _out.letter(':');
}

int ListFormatter::formatBytes(int base) {
    const auto generated = generatedSize();
    const auto bytes = codeBytes();
    int i = 0;
    if (config().opCodeWidth() == OPCODE_8BIT) {
        while (base + i < generated && i < bytes) {
            const uint8_t val8 = getByte(base + i);
            _out.letter(' ');
            formatHex(val8, 8);
            i++;
        }
    } else {  // OPCODE_16BIT
        const int hi = int(config().endian());
        const int lo = 1 - hi;
        while (base + i < generated && i < bytes) {
            const uint8_t val8hi = getByte(base + i + hi);
            const uint8_t val8lo = getByte(base + i + lo);
            const uint16_t val16 = (static_cast<uint16_t>(val8hi) << 8) | val8lo;
            _out.letter(' ');
            formatHex(val16, 16);
            i += 2;
        }
    }
    return i;
}

void ListFormatter::formatTab(size_t pos, int delta) {
    size_t size = _out.mark() - _outBuffer;
    while (pos < size + 1)
        pos += delta;
    while (size < pos) {
        _out.letter(' ');
        size++;
    }
}

void ListFormatter::formatContent(int pos) {
    if (!hasLabel() && !hasInstruction() && hasComment()) {
        formatTab(pos);
        _out.text(getComment());
        return;
    }
    if (hasLabel()) {
        formatTab(pos);
        _out.text(getLabel());
    }
    pos += labelWidth();
    if (hasInstruction()) {
        formatTab(pos, 8);
        _out.text(getInstruction(), _uppercase);
    }
    pos += nameWidth();
    if (hasOperand()) {
        formatTab(pos);
        _out.text(getOperand());
    }
    pos += operandWidth();
    if (hasComment()) {
        formatTab(pos);
        _out.text(getComment());
    }
}

void ListFormatter::formatLine() {
    formatAddress(startAddress() + _nextLine, true, true);
    const int pos = _out.mark() - _outBuffer;
    const int formattedBytes = formatBytes(_nextLine);
    const int bytes = codeBytes();
    const int dataTextLen = config().opCodeWidth() == OPCODE_8BIT ? (bytes * 3) : (bytes / 2) * 5;
    if (_nextLine == 0)
        formatContent(pos + dataTextLen + 1);
    _nextLine += formattedBytes;
}

}  // namespace driver
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
