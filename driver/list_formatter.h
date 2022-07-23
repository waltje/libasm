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

#ifndef __CLI_LISTING_H__
#define __CLI_LISTING_H__

#include <stdint.h>

#include <string>

#include "config_base.h"
#include "config_host.h"

namespace libasm {
namespace driver {

class ListLine {
public:
    virtual uint32_t startAddress() const = 0;
    virtual int generatedSize() const = 0;
    virtual uint8_t getByte(int offset) const = 0;
    virtual bool hasInstruction() const = 0;
    virtual std::string getInstruction() const = 0;
    virtual bool hasOperand() const = 0;
    virtual std::string getOperand() const = 0;

    virtual std::string inputName() const { return ""; }
    virtual bool isError() const { return false; }
    virtual std::string errorText() const { return ""; }

    // assemble listing only
    virtual uint32_t lineNumber() const { return 0; }
    virtual int16_t columnNumber() const { return -1; }
    virtual uint16_t includeNest() const { return 0; }
    virtual bool hasValue() const { return false; }
    virtual uint32_t value() const { return 0; }
    virtual bool hasLabel() const { return false; }
    virtual std::string getLabel() const { return ""; }
    virtual bool hasComment() const { return false; }
    virtual std::string getComment() const { return ""; }

    // configuration
    virtual const ConfigBase &config() const = 0;
    virtual int labelWidth() const = 0;
    virtual int nameWidth() const = 0;
    virtual int codeBytes() const = 0;
    virtual int operandWidth() const = 0;
};

class ListFormatter {
public:
    void reset(ListLine &line);
    void setUppercase(bool uppercase) { _uppercase = uppercase; }
    void enableLineNumber(bool enable) { _lineNumber = enable; }
    bool hasNextContent() const;
    bool hasNextLine() const;
    const char *getContent();
    const char *getLine();

protected:
    const ListLine *_line;
    bool _uppercase = false;
    bool _lineNumber = false;
    int _nextContent;
    int _nextLine;
    std::string _out;
    bool _errorContent;
    bool _errorLine;

    void formatHex(uint8_t val);
    void formatUint8(uint8_t val, bool fixedWidth = true, bool zeroSuppress = false);
    void formatUint12(uint16_t val, bool fixedWidth = true, bool zeroSuppress = false);
    void formatUint16(uint16_t val, bool fixedWidth = true, bool zeroSuppress = false);
    void formatUint20(uint32_t val, bool fixedWidth = true, bool zeroSuppress = false);
    void formatUint24(uint32_t val, bool fixedWidth = true, bool zeroSuppress = false);
    void formatUint32(uint32_t val, bool fixedWidth = true, bool zeroSuppress = false);
    void formatAddress(uint32_t addr, bool fixedWidth = true, bool zeroSuppress = false);
    int formatBytes(int base);
    void formatTab(size_t pos, int delta = 4);
    void formatContent(int pos);
    void formatLine();
};

}  // namespace driver
}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
