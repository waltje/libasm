/*
 * Copyright 2023 Tadashi G. Takaoka
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

#ifndef __TEST_FORMATTER_HELPER_H__
#define __TEST_FORMATTER_HELPER_H__

#include "array_memory.h"
#include "asm_directive.h"
#include "asm_driver.h"
#include "asm_formatter.h"
#include "bin_memory.h"
#include "dis_formatter.h"
#include "test_driver_helper.h"
#include "test_formatter_helper.h"
#include "test_reader.h"
#include "test_sources.h"

#define PREP_ASM_SYMBOL(typeof_asm, typeof_directive, symbolMode) \
    typeof_asm assembler;                                         \
    typeof_directive directive(assembler);                        \
    AsmDirective *dir = &directive;                               \
    TestSources sources;                                          \
    AsmDriver driver(&dir, &dir + 1, sources, symbolMode);        \
    BinMemory memory;                                             \
    AsmFormatter listing(driver, sources, memory)

#define PREP_ASM(typeof_asm, typeof_directive) \
    PREP_ASM_SYMBOL(typeof_asm, typeof_directive, REPORT_UNDEFINED)

#define ASM(_cpu, _source, _expected)                               \
    do {                                                            \
        TestReader expected("expected");                            \
        expected.add(_expected);                                    \
        TestReader source(_cpu);                                    \
        source.add(_source);                                        \
        sources.add(source);                                        \
        sources.open(source.name().c_str());                        \
        StrScanner *line;                                           \
        while ((line = sources.readLine()) != nullptr) {            \
            listing.assemble(*line, /* reportError */ true);        \
            while (listing.hasNextLine())                           \
                EQ("line", expected.readLine(), listing.getLine()); \
        }                                                           \
        EQ(_cpu, nullptr, expected.readLine());                     \
    } while (0)

#define PREP_DIS(typeof_disassembler) \
    typeof_disassembler disassembler; \
    DisFormatter listing(disassembler, "test.bin")

#define DIS(_cpu, _org, _contents, _lines, _memory)                       \
    do {                                                                  \
        TestReader contents(_cpu);                                        \
        contents.add(_contents);                                          \
        TestReader lines(_cpu);                                           \
        lines.add(_lines);                                                \
        TRUE("setcpu" _cpu, listing.setCpu(_cpu));                        \
        EQ("content", contents.readLine(), listing.getContent());         \
        EQ("line", lines.readLine(), listing.getLine());                  \
        EQ("origin", OK, listing.setOrigin(_org));                        \
        EQ("content", contents.readLine(), listing.getContent());         \
        EQ("line", lines.readLine(), listing.getLine());                  \
        const auto unit = disassembler.addressUnit();                     \
        auto reader = _memory.iterator();                                 \
        while (reader.hasNext()) {                                        \
            const auto addr = reader.address() / unit;                    \
            listing.disassemble(reader, addr);                            \
            while (listing.hasNextContent())                              \
                EQ("content", contents.readLine(), listing.getContent()); \
            while (listing.hasNextLine())                                 \
                EQ("line", lines.readLine(), listing.getLine());          \
            FALSE("line eor", listing.hasNextLine());                     \
        }                                                                 \
        EQ("expected content eor", nullptr, contents.readLine());         \
        EQ("expected line eor", nullptr, lines.readLine());               \
    } while (0)

#define DIS16(_cpu, _org, _contents, _expected, ...)                            \
    do {                                                                        \
        const auto unit = disassembler.addressUnit();                           \
        const auto endian = disassembler.endian();                              \
        const uint16_t _memory[] = {__VA_ARGS__};                               \
        const ArrayMemory memory(_org *unit, _memory, sizeof(_memory), endian); \
        DIS(_cpu, _org, _contents, _expected, memory);                          \
    } while (0)

#define DIS8(_cpu, _org, _contents, _expected, ...)               \
    do {                                                          \
        const uint8_t _memory[] = {__VA_ARGS__};                  \
        const ArrayMemory memory(_org, _memory, sizeof(_memory)); \
        DIS(_cpu, _org, _contents, _expected, memory);            \
    } while (0)

#endif  // __TEST_FORMATTER_HELPER_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
