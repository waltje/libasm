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

#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__

#include <stdint.h>

#include "str_scanner.h"

namespace libasm {

struct SymbolTable {
    virtual const char *lookupValue(uint32_t) const { return nullptr; }
    virtual bool hasSymbol(const StrScanner &symbol) const = 0;
    virtual uint32_t lookupSymbol(const StrScanner &symbol) const = 0;
    virtual const void *lookupFunction(const StrScanner &symbol) const = 0;
};

}  // namespace libasm

#endif  // __SYMBOL_TABLE_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
