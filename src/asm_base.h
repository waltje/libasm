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

#ifndef __ASM_BASE_H__
#define __ASM_BASE_H__

#include <stdint.h>

#include "config_base.h"
#include "entry_table.h"
#include "error_reporter.h"
#include "insn_base.h"
#include "option_base.h"
#include "parsers.h"
#include "symbol_table.h"
#include "value_parser.h"

namespace libasm {

struct Assembler : ErrorAt {
    Error encode(const char *line, Insn &insn, SymbolTable *symtab = nullptr);
    virtual const ConfigBase &config() const = 0;
    virtual void reset() {}

    ValueParser &parser() { return _parser; }
    bool endOfLine(const StrScanner &scan) const { return _parser.endOfLine(scan); }

    const /*PROGMEM*/ char *listCpu_P() const { return config().listCpu_P(); }
    const /*PROGMEM*/ char *cpu_P() const { return config().cpu_P(); }
    bool setCpu(const char *name) { return configSetter().setCpuName(name); }
    Error setCpu(StrScanner &scan) { return configSetter().setCpuName(scan); }

    Error setOption(const char *name, const char *text) {
        if (_commonOptions.setOption(name, text) == OK)
            return getError();
        return options().setOption(name, text);
    }
    const Options &commonOptions() const { return _commonOptions; }
    const Options &options() const { return _options; }

    /** Whether this CPU has "SET" instruction which conflict with "SET" directive */
    virtual bool hasSetInstruction() const { return false; }

protected:
    const Options _options;
    const Options _commonOptions{nullptr};
    ValueParser _parser;

    SymbolTable *_symtab;

    Assembler(const OptionBase *option, const NumberParser &number, const CommentParser &comment,
            const SymbolParser &symbol, const LetterParser &letter, const LocationParser &location,
            const OperatorParser *operators = nullptr, const FunctionParser *function = nullptr);

    int32_t branchDelta(uint32_t base, uint32_t target, const ErrorAt &at);

    /** Parse |expr| text and get value as unsigned 16 bit. */
    uint16_t parseExpr16(StrScanner &expr, ErrorAt &error, char delim = 0) const;
    /** Parse |expr| text and get value as unsigned 32 bit. */
    uint32_t parseExpr32(StrScanner &expr, ErrorAt &error, char delim = 0) const;
    /** Parse |expr| text and get value. */
    Value parseExpr(StrScanner &expr, ErrorAt &error, char delim = 0) const;

private:
    virtual ConfigSetter &configSetter() = 0;
    virtual Error processPseudo(StrScanner &scan, Insn &insn);
    virtual Error encodeImpl(StrScanner &scan, Insn &insn) = 0;
};

}  // namespace libasm

#endif  // __ASM_BASE_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
