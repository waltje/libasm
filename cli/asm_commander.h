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

#ifndef __ASM_COMMANDER_H__
#define __ASM_COMMANDER_H__

#include "asm_directive.h"
#include "asm_driver.h"
#include "bin_memory.h"
#include "file_reader.h"
#include "text_reader.h"

#include <list>

namespace libasm {
namespace cli {

class AsmCommander {
public:
    AsmCommander(driver::AsmDirective **begin, driver::AsmDirective **end);

    int parseArgs(int argc, const char **argv);
    int usage();
    int assemble();

private:
    class FileFactory : public driver::AsmSourceFactory {
    public:
        Error open(const StrScanner &name) override;
        const driver::TextReader *current() const override;
        Error closeCurrent() override;
        size_t size() const override { return _sources.size(); }
        StrScanner *readLine() override;
        driver::TextPrinter &errors() override;

    private:
        static constexpr int max_includes = 4;
        std::list<FileReader> _sources;
    };

    FileFactory _sources;
    driver::AsmDriver _driver;
    // command line arguments
    const char *_prog_name;
    const char *_input_name;
    const char *_output_name;
    const char *_list_name;
    const char *_cpu;
    char _encoder;
    size_t _record_bytes;
    bool _uppercase;
    bool _line_number;
    bool _verbose;
    std::map<std::string, std::string> _options;

    static constexpr const char *PROG_PREFIX = "asm";
    driver::AsmDirective *defaultDirective();
    int assemble(driver::BinMemory &memory, driver::TextPrinter &out, bool reportError = false);
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
