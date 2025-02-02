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

#include "dis_commander.h"

#include "bin_decoder.h"
#include "dis_formatter.h"
#include "file_printer.h"
#include "file_reader.h"

#include <algorithm>
#include <cstring>
#include <string>

namespace libasm {
namespace cli {

using namespace libasm::driver;

DisCommander::DisCommander(Disassembler **begin, Disassembler **end) : _driver(begin, end) {}

int DisCommander::disassemble() {
    if (_verbose)
        fprintf(stderr, "libasm disassembler (version " LIBASM_VERSION_STRING ")\n");
    if (defaultDisassembler() == nullptr && _cpu == nullptr) {
        fprintf(stderr, "No target CPU specified\n");
        return 1;
    }
    if (_driver.setCpu(_cpu) == nullptr) {
        fprintf(stderr, "Unknown target CPU %s\n", _cpu);
        return 1;
    }

    BinMemory memory;
    FileReader input(_input_name);
    if (!input.open()) {
        fprintf(stderr, "Can't open input file %s\n", _input_name);
        return 1;
    }
    if (readBinary(input, memory) < 0)
        return 1;
    input.close();

    const auto addrUnit = static_cast<uint8_t>(_driver.current()->config().addressUnit());
    const auto mem_start = memory.startAddress() / addrUnit;
    const auto mem_end = memory.endAddress() / addrUnit;
    if (_dis_start > mem_end || _dis_end < mem_start) {
        fprintf(stderr, "Input file has address range: 0x%04X,0x%04X\n", mem_start, mem_end);
        fprintf(stderr, "-A range has no intersection: 0x%04X,0x%04X\n", _dis_start, _dis_end);
        return 1;
    }

    auto &disassembler = *_driver.current();
    DisFormatter listing(disassembler, _input_name);
    listing.setUpperHex(_upper_hex);
    listing.setUppercase(_uppercase);
    for (auto &opt : _options) {
        disassembler.setOption(opt.first.c_str(), opt.second.c_str());
    }

    FilePrinter output;
    if (_output_name) {
        if (!output.open(_output_name)) {
            fprintf(stderr, "Can't open output file %s\n", _output_name);
            return 1;
        }
        if (_verbose)
            fprintf(stderr, "%s: Opened for output\n", output.name().c_str());
        output.println(listing.getContent());
    }
    FilePrinter listout;
    if (_list_name) {
        if (!listout.open(_list_name)) {
            fprintf(stderr, "Can't open list file %s\n", _list_name);
            return 1;
        }
        if (_verbose)
            fprintf(stderr, "%s: Opened for listing\n", listout.name().c_str());
        listout.println(listing.getLine());
    }

    listing.setCpu(_cpu);
    _driver.disassemble(
            memory, _dis_start, _dis_end, listing, output, listout, FilePrinter::STDERR);

    return 0;
}

int DisCommander::readBinary(FileReader &input, BinMemory &memory) {
    const auto filename = input.name().c_str();
    const auto addrUnit = static_cast<uint8_t>(_driver.current()->config().addressUnit());
    const auto size = BinDecoder::decode(input, memory);
    if (size < 0) {
        fprintf(stderr, "%s:%d: Unrecognizable binary format\n", filename, input.lineno());
        return size;
    }
    if (_verbose) {
        for (const auto &it : memory) {
            const uint32_t start = it.base / addrUnit;
            const size_t size = it.data.size();
            const uint32_t end = (it.base + size - 1) / addrUnit;
            fprintf(stderr, "%s: Read %4zu bytes 0x%04X-0x%04X\n", filename, size, start, end);
        }
    }
    return size;
}

Disassembler *DisCommander::defaultDisassembler() {
    Disassembler *dis = nullptr;
    const auto *prefix = strstr(_prog_name, PROG_PREFIX);
    if (prefix) {
        const char *cpu = prefix + strlen(PROG_PREFIX);
        dis = _driver.restrictCpu(cpu);
    }
    return dis;
}

int DisCommander::usage() {
    defaultDisassembler();
    const auto cpuList = _driver.listCpu();
    const auto cpuOption = cpuList.size() == 1 ? "" : " -C <CPU>";
    std::string list;
    const auto cpuSep = "\n                ";
    std::string buf;
    for (auto &cpu : cpuList) {
        if (buf.size())
            buf += " ";
        if (buf.size() + cpu.size() < 60) {
            buf += cpu;
        } else {
            list += cpuSep;
            list += buf;
            buf = cpu;
        }
    }
    if (list.size() + buf.size() < 60) {
        list += ": " + buf;
    } else {
        list += cpuSep + buf;
    }
    fprintf(stderr,
            "libasm disassembler (version " LIBASM_VERSION_STRING
            ")\n"
            "usage: %s%s [-o <output>] [-l <list>] <input>\n"
            "  -C <CPU>    : target CPU%s\n"
            "  -o <output> : output file\n"
            "  -l <list>   : list file\n"
            "  <input>     : file can be Motorola S-Record or Intel HEX format\n"
            "  -A start[,end]\n"
            "              : disassemble start address and optional end address\n"
            "  -r          : use program counter relative notation\n"
            "  -h          : use lower case letter for hexadecimal\n"
            "  -u          : use upper case letter for output\n"
            "  -v          : print progress verbosely\n",
            _prog_name, cpuOption, list.c_str());
    bool longOptions = false;
    for (const auto *dis : _driver)
        longOptions |= (dis->commonOptions().head() || dis->options().head());
    if (longOptions) {
        fprintf(stderr,
                "  --<name>=<vale>\n"
                "              : extra options (<type> [, <CPU>])\n");
        const auto dis = *_driver.begin();
        for (const auto *opt = dis->commonOptions().head(); opt; opt = opt->next()) {
            fprintf(stderr, "  %-16s: %s  (%s)\n", opt->name_P(), opt->description_P(),
                    Options::nameof(opt->spec()));
        }
        for (const auto *dis : _driver) {
            for (const auto *opt = dis->options().head(); opt; opt = opt->next()) {
                fprintf(stderr, "  %-16s: %s  (%s, %s)\n", opt->name_P(), opt->description_P(),
                        Options::nameof(opt->spec()), dis->cpu_P());
            }
        }
    }
    return 2;
}

static const char *basename(const char *str, char sep_char = '/') {
    const auto sep = strrchr(str, sep_char);
    return sep ? sep + 1 : str;
}

int DisCommander::parseOptionValue(const char *option) {
    const auto equ = strchr(option, '=');
    if (equ == nullptr)
        return 1;
    _options.emplace(std::string(option, equ), std::string(equ + 1));
    return 0;
}

int DisCommander::parseArgs(int argc, const char **argv) {
    _prog_name = basename(argv[0]);
    _input_name = nullptr;
    _output_name = nullptr;
    _list_name = nullptr;
    _cpu = nullptr;
    _upper_hex = true;
    _uppercase = false;
    _verbose = false;
    _dis_start = 0;
    _dis_end = UINT32_MAX;
    for (auto i = 1; i < argc; i++) {
        const auto *opt = argv[i];
        if (*opt == '-') {
            switch (*++opt) {
            case 'o':
                if (++i >= argc) {
                    fprintf(stderr, "-o requires output file name\n");
                    return 1;
                }
                _output_name = argv[i];
                break;
            case 'l':
                if (++i >= argc) {
                    fprintf(stderr, "-l requires listing file name\n");
                    return 1;
                }
                _list_name = argv[i];
                break;
            case 'C': {
                if (++i >= argc) {
                    fprintf(stderr, "-C requires CPU name\n");
                    return 1;
                }
                _cpu = argv[i];
                break;
            }
            case 'r':
                _options.emplace("relative", "on");
                break;
            case 'h':
                _upper_hex = false;
                break;
            case 'u':
                _uppercase = true;
                break;
            case 'v':
                _verbose = true;
                break;
            case 'A':
                if (++i >= argc) {
                    fprintf(stderr, "-A requires start[,end] address\n");
                    return 1;
                } else {
                    char *end_start;
                    _dis_start = strtoul(argv[i], &end_start, 0);
                    if (end_start == argv[i]) {
                    format_error:
                        fprintf(stderr, "invalid address format for -A: %s\n", argv[i]);
                        return 1;
                    }
                    if (*end_start == 0)
                        break;
                    if (*end_start++ != ',') {
                    form_error:
                        fprintf(stderr, "address must be start[,end] form: %s\n", argv[i]);
                        return 1;
                    }
                    char *end_end;
                    _dis_end = strtoul(end_start, &end_end, 0);
                    if (end_end == end_start)
                        goto format_error;
                    if (*end_end)
                        goto form_error;
                    if (_dis_end < _dis_start) {
                        fprintf(stderr, "end address must be less than start: %s\n", argv[i]);
                        return 1;
                    }
                }
                break;
            case '-':
                if (parseOptionValue(++opt) == 0)
                    break;
                fprintf(stderr, "long option requires option=value\n");
                return 1;
            default:
                fprintf(stderr, "unknown option: %s\n", opt);
                return 1;
            }
        } else {
            if (_input_name) {
                fprintf(stderr, "multiple input files specified: %s and %s\n", _input_name, opt);
                return 1;
            }
            _input_name = opt;
        }
    }
    if (_input_name == nullptr) {
        fprintf(stderr, "no input file\n");
        return 1;
    }
    if (_output_name && strcmp(_output_name, _input_name) == 0) {
        fprintf(stderr, "output file overwrite input file\n");
        return 2;
    }
    if (_list_name && strcmp(_list_name, _input_name) == 0) {
        fprintf(stderr, "listing file overwrite input file\n");
        return 2;
    }
    for (auto &option : _options) {
        auto valid = false;
        for (const auto *dis : _driver) {
            for (const auto *opt = dis->options().head(); opt; opt = opt->next()) {
                if (strcmp_P(option.first.c_str(), opt->name_P()) == 0) {
                    valid = true;
                    break;
                }
            }
            if (valid)
                continue;
            for (const auto *opt = dis->commonOptions().head(); opt; opt = opt->next()) {
                if (strcmp_P(option.first.c_str(), opt->name_P()) == 0) {
                    valid = true;
                    break;
                }
            }
        }
        if (!valid) {
            fprintf(stderr, "unknown option --%s=%s\n", option.first.c_str(),
                    option.second.c_str());
            return 1;
        }
    }
    return 0;
}

}  // namespace cli
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
