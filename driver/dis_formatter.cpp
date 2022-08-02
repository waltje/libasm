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

#include "dis_formatter.h"

#include "str_buffer.h"

namespace libasm {
namespace driver {

DisFormatter::DisFormatter(Disassembler &disassembler, const char *input_name)
    : ListFormatter(),
      _disassembler(disassembler),
      _input_name(input_name),
      _upper_hex(true),
      _uppercase(false),
      _insn(0),
      _insnBase(_insn) {
    _disassembler.formatter().setUppercase(true);
}

void DisFormatter::setUpperHex(bool enable) {
    _upper_hex = enable;
    ListFormatter::setUpperHex(enable);
    _disassembler.formatter().setUppercase(enable);
}

void DisFormatter::setUppercase(bool enable) {
    _uppercase = enable;
    _disassembler.setOption("uppercase", _uppercase ? "on" : "off");
    _disassembler.formatter().setUppercase(_upper_hex);
}

void DisFormatter::reset() {
    _disassembler.resetError();
    /* There is at least one line even when generated byte is zero. */
    _nextContent = _nextLine = -1;
    _errorContent = _errorLine = false;
}

Error DisFormatter::disassemble(DisMemory &memory, uint32_t addr) {
    reset();
    _insnBase.reset(addr);
    return _disassembler.decode(memory, _insn, _operands, sizeof(_operands));
}

Error DisFormatter::setCpu(const char *cpu) {
    reset();
    StrBuffer buf(_operands, sizeof(_operands));
    _insnBase.reset(_insn.address() + _insn.length());
    _insnBase.nameBuffer().text_P(PSTR("CPU"), _uppercase);
    buf.text(cpu, _uppercase);
    return _disassembler.setCpu(cpu) ? UNSUPPORTED_CPU : OK;
}

Error DisFormatter::setOrigin(uint32_t origin) {
    reset();
    StrBuffer buf(_operands, sizeof(_operands));
    if (_disassembler.checkAddress(origin))
        return _disassembler.getError();
    _insnBase.reset(origin);
    _insnBase.nameBuffer().text_P(PSTR("ORG"), _uppercase);
    _disassembler.formatter().formatHex(buf, origin, config().addressWidth(), false);
    return OK;
}

bool DisFormatter::hasNextContent() const {
    return _nextContent < generatedSize();
}

static int max(int a, int b) {
    return a < b ? b : a;
}

const char *DisFormatter::getContent() {
    resetOut();
    if (isError()) {
        if (!_errorContent) {
            _errorContent = true;
            _out.text("; ").text(_input_name).text(": ");
            _disassembler.formatter().formatHex(
                    _out, startAddress(), config().addressWidth(), false);
            _out.text(": error: ").text_P(_disassembler.errorText_P());
            _nextContent = 0;
        } else {
            _out.text("; ");
            formatAddress(startAddress() + _nextContent);
            _nextContent += formatBytes(_nextContent);
        }
    } else {
        _out.text("      ");
        const auto pos = outLength();
        _out.text(_insn.name());
        if (*_operands) {
            const auto nameWidth = max(config().nameMax(), min_nameWidth) + 1;
            formatTab(pos + nameWidth);
            _out.text(_operands);
        }
        _nextContent = generatedSize();
    }
    return _out_buffer;
}

bool DisFormatter::hasNextLine() const {
    return _nextLine < generatedSize();
}

const char *DisFormatter::getLine() {
    resetOut();
    if (isError() && !_errorLine) {
        _errorLine = true;
        _out.text(_input_name).text(": ");
        _disassembler.formatter().formatHex(_out, startAddress(), config().addressWidth(), false);
        _out.text(": error: ").text_P(_disassembler.errorText_P());
        _nextLine = 0;
    } else {
        if (_nextLine < 0)
            _nextLine = 0;
        formatAddress(startAddress() + _nextLine);
        auto pos = outLength();
        const auto formatted = formatBytes(_nextLine);
        if (_nextLine == 0 && *_insn.name()) {
            formatTab(pos + bytesColumnWidth() + 1);
            _out.text("        ");
            pos = outLength();
            _out.text(_insn.name());
            if (*_operands) {
                const auto nameWidth = max(config().nameMax(), min_nameWidth) + 1;
                formatTab(pos + nameWidth);
                _out.text(_operands);
            }
        }
        _nextLine += formatted;
    }
    return _out_buffer;
}

}  // namespace driver
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
