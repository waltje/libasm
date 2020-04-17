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

#ifndef __TEST_DIS_HELPER_H__
#define __TEST_DIS_HELPER_H__

#include "test_asserter.h"
#include "test_memory.h"
#include "test_symtab.h"
#include "symbol_table.h"

#include <stdio.h>

#define __ASSERT(file, line, error, addr, mnemonic, expected_opr)       \
    do {                                                                \
        Insn insn;                                                      \
        char actual_opr[40], message[80];                               \
        memory.setAddress(addr);                                        \
        disassembler.decode(memory, insn, actual_opr, &symtab, true);   \
        sprintf(message, "%s:%d: %s: ", file, line, #mnemonic);         \
        memory.dump(message + strlen(message));                         \
        asserter.equals(message, error, disassembler.getError());       \
        if (error == OK) {                                              \
            asserter.equals(message, #mnemonic, insn.name());           \
            asserter.equals(message, expected_opr, actual_opr);         \
            asserter.equals(message,                                    \
                            mnemonic, sizeof(mnemonic),                 \
                            insn.bytes(), insn.length());               \
        }                                                               \
    } while (0)
#define __VASSERT(file, line, error, addr, mnemonic, opr, ...)  \
    do {                                                        \
        const Config::opcode_t mnemonic[] = { __VA_ARGS__ };    \
        memory.setMemory(mnemonic, sizeof(mnemonic));           \
        __ASSERT(file, line, error, addr, mnemonic, opr);       \
    } while (0)
#define EATEST(error, addr, mnemonic, opr, ...)                         \
    __VASSERT(__FILE__, __LINE__, error, addr, mnemonic, opr, __VA_ARGS__)
#define ATEST(addr, mnemonic, opr, ...)                                 \
    __VASSERT(__FILE__, __LINE__, OK, addr, mnemonic, opr, __VA_ARGS__)
#define ETEST(error, mnemonic, opr, ...)                                \
    __VASSERT(__FILE__, __LINE__, error, 0x0000, mnemonic, opr, __VA_ARGS__)
#define TEST(mnemonic, opr, ...) ETEST(OK, mnemonic, opr, __VA_ARGS__)

#define RUN_TEST(test) run_test(test, #test)

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
