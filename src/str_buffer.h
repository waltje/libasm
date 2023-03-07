/*
 * Copyright 2021 Tadashi G. Takaoka
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

#ifndef __STR_BUFFER_H__
#define __STR_BUFFER_H__

#include "error_reporter.h"
#include "str_scanner.h"

#include <ctype.h>

namespace libasm {

class StrBuffer : public ErrorReporter {
public:
    StrBuffer(char *buffer, size_t size) : ErrorReporter() { reset(buffer, size); }

    StrBuffer &letter(char c);
    StrBuffer &letter(char c, bool uppercase) {
        return uppercase ? letter(toupper(c)) : letter(tolower(c));
    }
    StrBuffer &text(const char *text);
    StrBuffer &text(const char *text, bool uppercase);
    StrBuffer &text(const StrScanner &scan);
    StrBuffer &text(const StrScanner &scan, bool uppercase);
    StrBuffer &text_P(const /*PROGMEM*/ char *text_P);
    StrBuffer &text_P(const /*PROGMEM*/ char *text, bool uppercase);
    StrBuffer &format_P(const /*PROGMEM*/ char *fmt, ...);
    StrBuffer &comma();

    StrBuffer &reset(char *buffer, size_t size);
    char *mark() const { return _out; }
    StrBuffer &reverse(char *start);
    StrBuffer &lowercase(char *start);

private:
    char *_out;
    const char *_end;
};

}  // namespace libasm

#endif  // __STR_BUFFER_H__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
