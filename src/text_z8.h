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

#ifndef __TEXT_Z8__
#define __TEXT_Z8__

#include "config_host.h"

namespace libasm {
namespace z8 {

// Z8
static const char TEXT_ADC[]  PROGMEM = "ADC";
static const char TEXT_ADD[]  PROGMEM = "ADD";
static const char TEXT_AND[]  PROGMEM = "AND";
static const char TEXT_CALL[] PROGMEM = "CALL";
static const char TEXT_CCF[]  PROGMEM = "CCF";
static const char TEXT_CLR[]  PROGMEM = "CLR";
static const char TEXT_COM[]  PROGMEM = "COM";
static const char TEXT_CP[]   PROGMEM = "CP";
static const char TEXT_DA[]   PROGMEM = "DA";
static const char TEXT_DEC[]  PROGMEM = "DEC";
static const char TEXT_DECW[] PROGMEM = "DECW";
static const char TEXT_DI[]   PROGMEM = "DI";
static const char TEXT_DJNZ[] PROGMEM = "DJNZ";
static const char TEXT_EI[]   PROGMEM = "EI";
static const char TEXT_INC[]  PROGMEM = "INC";
static const char TEXT_INCW[] PROGMEM = "INCW";
static const char TEXT_IRET[] PROGMEM = "IRET";
static const char TEXT_JP[]   PROGMEM = "JP";
static const char TEXT_JR[]   PROGMEM = "JR";
static const char TEXT_LD[]   PROGMEM = "LD";
static const char TEXT_LDC[]  PROGMEM = "LDC";
static const char TEXT_LDCI[] PROGMEM = "LDCI";
static const char TEXT_LDE[]  PROGMEM = "LDE";
static const char TEXT_LDEI[] PROGMEM = "LDEI";
static const char TEXT_NOP[]  PROGMEM = "NOP";
static const char TEXT_OR[]   PROGMEM = "OR";
static const char TEXT_POP[]  PROGMEM = "POP";
static const char TEXT_PUSH[] PROGMEM = "PUSH";
static const char TEXT_RCF[]  PROGMEM = "RCF";
static const char TEXT_RET[]  PROGMEM = "RET";
static const char TEXT_RL[]   PROGMEM = "RL";
static const char TEXT_RLC[]  PROGMEM = "RLC";
static const char TEXT_RR[]   PROGMEM = "RR";
static const char TEXT_RRC[]  PROGMEM = "RRC";
static const char TEXT_SBC[]  PROGMEM = "SBC";
static const char TEXT_SCF[]  PROGMEM = "SCF";
static const char TEXT_SRA[]  PROGMEM = "SRA";
static const char TEXT_SRP[]  PROGMEM = "SRP";
static const char TEXT_SUB[]  PROGMEM = "SUB";
static const char TEXT_SWAP[] PROGMEM = "SWAP";
static const char TEXT_TCM[]  PROGMEM = "TCM";
static const char TEXT_TM[]   PROGMEM = "TM";
static const char TEXT_XOR[]  PROGMEM = "XOR";

// Z86C
static const char TEXT_HALT[] PROGMEM = "HALT";
static const char TEXT_STOP[] PROGMEM = "STOP";

} // namespace z8
} // namespace libasm

#endif // __TEXT_Z8__

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
