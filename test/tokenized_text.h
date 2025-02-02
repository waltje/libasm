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

#ifndef __TOKENIZED_TEXT_H__
#define __TOKENIZED_TEXT_H__

#include <cstdint>
#include <string>
#include <vector>

namespace libasm {
namespace gen {

class TokenizedText {
public:
    TokenizedText(const char *text);

    std::size_t length() const { return _tokens.size(); }
    int count() const { return ++_count; }

    struct hash {
        std::size_t operator()(const TokenizedText &it) const;
    };
    struct eq {
        bool operator()(const TokenizedText &a, const TokenizedText &b) const {
            return a._tokens == b._tokens;
        }
    };

private:
    const std::vector<uint8_t> _tokens;
    mutable int _count;

    static std::vector<uint8_t> tokenize(const char *text);
};

}  // namespace gen
}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
