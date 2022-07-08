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

#ifndef __ASM_DIRECTIVE_H__
#define __ASM_DIRECTIVE_H__

#include "asm_base.h"
#include "asm_formatter.h"
#include "bin_memory.h"
#include "error_reporter.h"
#include "function_store.h"
#include "intel_hex.h"
#include "list_formatter.h"
#include "moto_srec.h"
#include "str_scanner.h"
#include "text_reader.h"

#include <functional>
#include <list>
#include <map>

namespace libasm {
namespace cli {

class AsmDirective;

enum SymbolMode {
    REPORT_UNDEFINED = 0,
    REPORT_DUPLICATE = 1,
};

class AsmSourceFactory {
public:
    virtual Error open(const StrScanner &name) = 0;
    virtual const TextReader *current() const = 0;
    virtual Error closeCurrent() = 0;
    virtual size_t size() const = 0;
    virtual StrScanner *readLine() = 0;
};

class ICommon : public SymbolTable {
public:
    virtual uint32_t origin() const = 0;
    virtual uint32_t setOrigin(uint32_t origin) = 0;
    virtual AsmDirective *setCpu(const char *cpu) = 0;
    virtual SymbolMode symbolMode() const = 0;
    virtual Error internSymbol(uint32_t value, const StrScanner &symbol) = 0;
    virtual Error internFunction(
            const StrScanner &name, std::list<StrScanner> &params, const StrScanner &body) = 0;
    virtual Error openSource(const StrScanner &filename) = 0;
};

class AsmCommonDirective : public ErrorAt, public ICommon {
public:
    AsmCommonDirective(AsmDirective **begin, AsmDirective **end, AsmSourceFactory &sources);

    AsmDirective *restrictCpu(const char *cpu);
    AsmDirective *setCpu(const char *cpu) override;
    std::list<std::string> listCpu() const;
    AsmDirective *current() const { return _current; }

    Error assembleLine(const StrScanner &line, AsmFormatter &list);

    void reset();
    void setSymbolMode(SymbolMode mode) { _symbolMode = mode; }

    // ICommon
    uint32_t origin() const override { return _origin; }
    uint32_t setOrigin(uint32_t origin) override { return _origin = origin; }
    SymbolMode symbolMode() const override { return _symbolMode; }
    Error internFunction(const StrScanner &name, std::list<StrScanner> &params,
            const StrScanner &body) override {
        return _functions.internFunction(name, params, body);
    }
    Error openSource(const StrScanner &filename) override { return _sources.open(filename); }

private:
    std::list<AsmDirective *> _directives;
    AsmDirective *_current;
    AsmSourceFactory &_sources;
    FunctionStore *_functionStore;
    ValueParser::FuncParser *_savedFuncParser;

    uint32_t _origin;
    SymbolMode _symbolMode;

    FunctionStore _functions;
    void setFunctionStore(FunctionStore *functionStore);
    AsmDirective *switchDirective(AsmDirective *dir);

    // SymbolTable
    const char *lookupValue(uint32_t address) const override;
    bool hasSymbol(const StrScanner &symbol) const override;
    uint32_t lookupSymbol(const StrScanner &symbol) const override;
    Error internSymbol(uint32_t value, const StrScanner &symbol) override;

private:
    std::map<std::string, uint32_t, std::less<>> _symbols;
    bool symbolExists(const std::string &key) const;
    uint32_t symbolLookup(const std::string &key) const;
    Error symbolIntern(uint32_t value, const std::string &key);
};

class AsmDirective : public ErrorAt {
public:
    typedef Error (AsmDirective::*PseudoHandler)(
            StrScanner &scan, AsmFormatter &list, ICommon &common);

    Error assembleLine(const StrScanner &line, AsmFormatter &list, ICommon &common);
    Assembler &assembler() const { return _assembler; }
    virtual BinEncoder &defaultEncoder() = 0;
    Error processPseudo(
            const StrScanner &name, StrScanner &scan, AsmFormatter &list, ICommon &common);

    // PseudoHandler
    Error defineOrigin(StrScanner &scan, AsmFormatter &list, ICommon &common);
    Error alignOrigin(StrScanner &scan, AsmFormatter &list, ICommon &common);
    Error defineLabel(StrScanner &scan, AsmFormatter &list, ICommon &common);
    Error includeFile(StrScanner &scan, AsmFormatter &list, ICommon &common);
    Error defineUint8s(StrScanner &scan, AsmFormatter &list, ICommon &common);
    Error defineString(StrScanner &scan, AsmFormatter &list, ICommon &common);
    Error defineUint16s(StrScanner &scan, AsmFormatter &list, ICommon &common);
    Error defineUint32s(StrScanner &scan, AsmFormatter &list, ICommon &common);
    Error allocateUint8s(StrScanner &scan, AsmFormatter &list, ICommon &common);
    Error allocateUint16s(StrScanner &scan, AsmFormatter &list, ICommon &common);
    Error allocateUint32s(StrScanner &scan, AsmFormatter &list, ICommon &common);
    Error switchCpu(StrScanner &scan, AsmFormatter &list, ICommon &common);
    Error switchIntelZilog(StrScanner &scan, AsmFormatter &list, ICommon &common);
    Error endAssemble(StrScanner &scan, AsmFormatter &list, ICommon &common);
    Error defineFunction(StrScanner &scan, AsmFormatter &list, ICommon &common);

    static bool is8080(const /* PROGMEM */ char *cpu_P);

protected:
    Assembler &_assembler;
    std::map<std::string, PseudoHandler, icasecmp> _pseudos;

    AsmDirective(Assembler &assembler);

    void registerPseudo(const char *name, PseudoHandler handler);

    // PseudoHanlder helper
    Error defineBytes(StrScanner &scan, AsmFormatter &list, ICommon &common, bool delimitor);
    Error allocateSpaces(StrScanner &scan, AsmFormatter &list, ICommon &common, size_t unit);
};

class MotorolaDirective : public AsmDirective {
public:
    MotorolaDirective(Assembler &assembler);
    BinEncoder &defaultEncoder() override { return MotoSrec::encoder(); }
};

class IntelDirective : public AsmDirective {
public:
    IntelDirective(Assembler &assembler);
    BinEncoder &defaultEncoder() override { return IntelHex::encoder(); }
};

class NationalDirective : public IntelDirective {
public:
    NationalDirective(Assembler &assembler);
};

}  // namespace cli
}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
