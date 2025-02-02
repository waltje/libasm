/*
 * Copyright 2022 Tadashi G. Takaoka
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

#include <cctype>
#include <cstring>

#include "function_store.h"

namespace libasm {
namespace driver {

void FunctionStore::reset() {
    _functions.clear();
}

bool FunctionStore::hasFunction(const StrScanner &_name) const {
    return lookupFunction(_name) != nullptr;
}

const void *FunctionStore::lookupFunction(const StrScanner &name) const {
    auto it = _functions.find(std::string(name.str(), name.size()));
    if (it == _functions.end())
        return nullptr;
    const Function *fn = &it->second;
    return reinterpret_cast<const void *>(fn);
}

Error FunctionStore::internFunction(const StrScanner &name_, const Parameters &params,
        const StrScanner &body, const ValueParser &parser, const SymbolTable *symtab) {
    const std::string name(name_.str(), name_.size());
    if (_functions.find(name) != _functions.end())
        return DUPLICATE_FUNCTION;
    auto argc = params.size();
    ParametersAt paramsAt;
    for (const auto &param : params)
        paramsAt.emplace(std::string(param.str(), param.size()), --argc);
    _functions.emplace(
            name, Function{std::string(body.str(), body.size()), paramsAt, parser, symtab});
    return OK;
}

bool FunctionStore::Binding::hasSymbol(const StrScanner &symbol) const {
    return paramsAt.find(std::string(symbol.str(), symbol.size())) != paramsAt.end() ||
           parent->hasSymbol(symbol);
}

uint32_t FunctionStore::Binding::lookupSymbol(const StrScanner &symbol) const {
    const auto it = paramsAt.find(std::string(symbol.str(), symbol.size()));
    if (it == paramsAt.end())
        return parent->lookupSymbol(symbol);
    return stack.at(it->second).getUnsigned();
}

const void *FunctionStore::Binding::lookupFunction(const StrScanner &symbol) const {
    return parent->lookupFunction(symbol);
}

Error FunctionStore::Function::eval(ValueStack &stack, uint8_t argc) const {
    Binding binding{paramsAt, stack, symtab};
    StrScanner body_scan(body.c_str());
    ErrorAt error;
    const auto val = parser.eval(body_scan, error, &binding);
    while (argc) {
        stack.pop();
        argc--;
    }
    stack.push(val);
    return error.getError();
}

}  // namespace driver
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
