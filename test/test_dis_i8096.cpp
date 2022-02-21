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

#include "dis_i8096.h"
#include "test_dis_helper.h"

using namespace libasm;
using namespace libasm::i8096;
using namespace libasm::test;

DisI8096 dis8096;
Disassembler &disassembler(dis8096);

static void set_up() {
    disassembler.reset();
}

static void tear_down() {
    symtab.reset();
}

// clang-format off
static void test_cpu() {
    EQUALS("cpu 8096", true,   disassembler.setCpu("8096"));
    EQUALS("cpu 8096", "8096", disassembler.getCpu());

    EQUALS("cpu i8096", true,   disassembler.setCpu("i8096"));
    EQUALS("cpu i8096", "8096", disassembler.getCpu());
}

static void test_2_operands() {
    TEST(ADD,   "52, 18",          0x64, 0x12, 0x34);
    ERRR(ADD,   "53, 18",          0x64, 0x12, 0x35);
    ERRR(ADD,   "52, 19",          0x64, 0x13, 0x34);
    TEST(ADD,   "86, #3412H",      0x65, 0x12, 0x34, 0x56);
    ERRR(ADD,   "87, #3412H",      0x65, 0x12, 0x34, 0x57);
    TEST(ADD,   "52, [18]",        0x66, 0x12, 0x34);
    ERRR(ADD,   "53, [18]",        0x66, 0x12, 0x35);
    ERRR(ADD,   "52, [19]",        0x66, 0x13, 0x34);
    TEST(ADD,   "86, 52[18]",      0x67, 0x12, 0x34, 0x56);
    ERRR(ADD,   "87, 52[18]",      0x67, 0x12, 0x34, 0x57);
    TEST(ADD,   "120, 5634H[18]",  0x67, 0x13, 0x34, 0x56, 0x78);
    ERRR(ADD,   "121, 5634H[18]",  0x67, 0x13, 0x34, 0x56, 0x79);
    TEST(ADDB,  "35, 18",          0x74, 0x12, 0x23);
    TEST(ADDB,  "34, 19",          0x74, 0x13, 0x22);
    TEST(ADDB,  "35, #18",         0x75, 0x12, 0x23);
    TEST(ADDB,  "35, [18]",        0x76, 0x12, 0x23);
    ERRR(ADDB,  "35, [19]",        0x76, 0x13, 0x23);
    TEST(ADDB,  "52, 35[18]",      0x77, 0x12, 0x23, 0x34);
    TEST(ADDB,  "53, 35[18]",      0x77, 0x12, 0x23, 0x35);
    TEST(ADDB,  "69, 3423H[18]",   0x77, 0x13, 0x23, 0x34, 0x45);
    TEST(ADDC,  "52, 18",          0xA4, 0x12, 0x34);
    TEST(ADDC,  "86, #3412H",      0xA5, 0x12, 0x34, 0x56);
    TEST(ADDC,  "52, [18]",        0xA6, 0x12, 0x34);
    TEST(ADDC,  "86, 52[18]",      0xA7, 0x12, 0x34, 0x56);
    TEST(ADDC,  "120, 5634H[18]",  0xA7, 0x13, 0x34, 0x56, 0x78);
    TEST(ADDCB, "35, 18",          0xB4, 0x12, 0x23);
    TEST(ADDCB, "35, #18",         0xB5, 0x12, 0x23);
    TEST(ADDCB, "35, [18]",        0xB6, 0x12, 0x23);
    TEST(ADDCB, "52, 35[18]",      0xB7, 0x12, 0x23, 0x34);
    TEST(ADDCB, "69, 3423H[18]",   0xB7, 0x13, 0x23, 0x34, 0x45);
    TEST(SUB,   "52, 18",          0x68, 0x12, 0x34);
    TEST(SUB,   "86, #3412H",      0x69, 0x12, 0x34, 0x56);
    TEST(SUB,   "52, [18]",        0x6A, 0x12, 0x34);
    TEST(SUB,   "86, 52[18]",      0x6B, 0x12, 0x34, 0x56);
    TEST(SUB,   "120, 5634H[18]",  0x6B, 0x13, 0x34, 0x56, 0x78);
    TEST(SUBB,  "35, 18",          0x78, 0x12, 0x23);
    TEST(SUBB,  "35, #18",         0x79, 0x12, 0x23);
    TEST(SUBB,  "35, [18]",        0x7A, 0x12, 0x23);
    TEST(SUBB,  "52, 35[18]",      0x7B, 0x12, 0x23, 0x34);
    TEST(SUBB,  "69, 3423H[18]",   0x7B, 0x13, 0x23, 0x34, 0x45);
    TEST(SUBC,  "52, 18",          0xA8, 0x12, 0x34);
    TEST(SUBC,  "86, #3412H",      0xA9, 0x12, 0x34, 0x56);
    TEST(SUBC,  "52, [18]",        0xAA, 0x12, 0x34);
    TEST(SUBC,  "86, 52[18]",      0xAB, 0x12, 0x34, 0x56);
    TEST(SUBC,  "120, 5634H[18]",  0xAB, 0x13, 0x34, 0x56, 0x78);
    TEST(SUBCB, "35, 18",          0xB8, 0x12, 0x23);
    TEST(SUBCB, "35, #18",         0xB9, 0x12, 0x23);
    TEST(SUBCB, "35, [18]",        0xBA, 0x12, 0x23);
    TEST(SUBCB, "52, 35[18]",      0xBB, 0x12, 0x23, 0x34);
    TEST(SUBCB, "69, 3423H[18]",   0xBB, 0x13, 0x23, 0x34, 0x45);
    TEST(CMP,   "52, 18",          0x88, 0x12, 0x34);
    TEST(CMP,   "86, #3412H",      0x89, 0x12, 0x34, 0x56);
    TEST(CMP,   "52, [18]",        0x8A, 0x12, 0x34);
    TEST(CMP,   "86, 52[18]",      0x8B, 0x12, 0x34, 0x56);
    TEST(CMP,   "120, 5634H[18]",  0x8B, 0x13, 0x34, 0x56, 0x78);
    TEST(CMPB,  "35, 18",          0x98, 0x12, 0x23);
    TEST(CMPB,  "35, #18",         0x99, 0x12, 0x23);
    TEST(CMPB,  "35, [18]",        0x9A, 0x12, 0x23);
    TEST(CMPB,  "52, 35[18]",      0x9B, 0x12, 0x23, 0x34);
    TEST(CMPB,  "69, 3423H[18]",   0x9B, 0x13, 0x23, 0x34, 0x45);
    TEST(MUL,   "52, 16",          0xFE, 0x6C, 0x10, 0x34);
    ERRR(MUL,   "52, 17",          0xFE, 0x6C, 0x11, 0x34);
    ERRR(MUL,   "53, 16",          0xFE, 0x6C, 0x10, 0x35);
    ERRR(MUL,   "54, 16",          0xFE, 0x6C, 0x10, 0x36);
    ERRR(MUL,   "55, 16",          0xFE, 0x6C, 0x10, 0x37);
    TEST(MUL,   "84, #3412H",      0xFE, 0x6D, 0x12, 0x34, 0x54);
    ERRR(MUL,   "85, #3412H",      0xFE, 0x6D, 0x12, 0x34, 0x55);
    ERRR(MUL,   "86, #3412H",      0xFE, 0x6D, 0x12, 0x34, 0x56);
    ERRR(MUL,   "87, #3412H",      0xFE, 0x6D, 0x12, 0x34, 0x57);
    TEST(MUL,   "52, [18]",        0xFE, 0x6E, 0x12, 0x34);
    ERRR(MUL,   "52, [19]",        0xFE, 0x6E, 0x13, 0x34);
    ERRR(MUL,   "53, [18]",        0xFE, 0x6E, 0x12, 0x35);
    ERRR(MUL,   "54, [18]",        0xFE, 0x6E, 0x12, 0x36);
    ERRR(MUL,   "55, [18]",        0xFE, 0x6E, 0x12, 0x37);
    TEST(MUL,   "84, 52[18]",      0xFE, 0x6F, 0x12, 0x34, 0x54);
    ERRR(MUL,   "85, 52[18]",      0xFE, 0x6F, 0x12, 0x34, 0x55);
    ERRR(MUL,   "86, 52[18]",      0xFE, 0x6F, 0x12, 0x34, 0x56);
    ERRR(MUL,   "87, 52[18]",      0xFE, 0x6F, 0x12, 0x34, 0x57);
    TEST(MUL,   "120, 5634H[18]",  0xFE, 0x6F, 0x13, 0x34, 0x56, 0x78);
    ERRR(MUL,   "121, 5634H[18]",  0xFE, 0x6F, 0x13, 0x34, 0x56, 0x79);
    ERRR(MUL,   "122, 5634H[18]",  0xFE, 0x6F, 0x13, 0x34, 0x56, 0x7A);
    ERRR(MUL,   "123, 5634H[18]",  0xFE, 0x6F, 0x13, 0x34, 0x56, 0x7B);
    TEST(MULB,  "36, 19",          0xFE, 0x7C, 0x13, 0x24);
    ERRR(MULB,  "37, 18",          0xFE, 0x7C, 0x12, 0x25);
    TEST(MULB,  "36, #18",         0xFE, 0x7D, 0x12, 0x24);
    ERRR(MULB,  "37, #18",         0xFE, 0x7D, 0x12, 0x25);
    TEST(MULB,  "36, [18]",        0xFE, 0x7E, 0x12, 0x24);
    ERRR(MULB,  "36, [19]",        0xFE, 0x7E, 0x13, 0x24);
    ERRR(MULB,  "37, [18]",        0xFE, 0x7E, 0x12, 0x25);
    TEST(MULB,  "52, 35[18]",      0xFE, 0x7F, 0x12, 0x23, 0x34);
    ERRR(MULB,  "53, 35[18]",      0xFE, 0x7F, 0x12, 0x23, 0x35);
    TEST(MULB,  "70, 3423H[18]",   0xFE, 0x7F, 0x13, 0x23, 0x34, 0x46);
    ERRR(MULB,  "71, 3423H[18]",   0xFE, 0x7F, 0x13, 0x23, 0x34, 0x47);
    TEST(MULU,  "52, 18",          0x6C, 0x12, 0x34);
    TEST(MULU,  "84, #3412H",      0x6D, 0x12, 0x34, 0x54);
    TEST(MULU,  "52, [18]",        0x6E, 0x12, 0x34);
    TEST(MULU,  "84, 52[18]",      0x6F, 0x12, 0x34, 0x54);
    TEST(MULU,  "120, 5634H[18]",  0x6F, 0x13, 0x34, 0x56, 0x78);
    TEST(MULUB, "36, 18",          0x7C, 0x12, 0x24);
    TEST(MULUB, "36, #18",         0x7D, 0x12, 0x24);
    TEST(MULUB, "36, [18]",        0x7E, 0x12, 0x24);
    TEST(MULUB, "52, 35[18]",      0x7F, 0x12, 0x23, 0x34);
    TEST(MULUB, "70, 3423H[18]",   0x7F, 0x13, 0x23, 0x34, 0x46);
    TEST(DIV,   "52, 18",          0xFE, 0x8C, 0x12, 0x34);
    TEST(DIV,   "84, #3412H",      0xFE, 0x8D, 0x12, 0x34, 0x54);
    TEST(DIV,   "52, [18]",        0xFE, 0x8E, 0x12, 0x34);
    TEST(DIV,   "84, 52[18]",      0xFE, 0x8F, 0x12, 0x34, 0x54);
    TEST(DIV,   "120, 5634H[18]",  0xFE, 0x8F, 0x13, 0x34, 0x56, 0x78);
    ERRR(DIV,   "121, 5634H[18]",  0xFE, 0x8F, 0x13, 0x34, 0x56, 0x79);
    ERRR(DIV,   "122, 5634H[18]",  0xFE, 0x8F, 0x13, 0x34, 0x56, 0x7A);
    ERRR(DIV,   "123, 5634H[18]",  0xFE, 0x8F, 0x13, 0x34, 0x56, 0x7B);
    TEST(DIVB,  "36, 19",          0xFE, 0x9C, 0x13, 0x24);
    TEST(DIVB,  "36, #18",         0xFE, 0x9D, 0x12, 0x24);
    TEST(DIVB,  "36, [18]",        0xFE, 0x9E, 0x12, 0x24);
    TEST(DIVB,  "52, 35[18]",      0xFE, 0x9F, 0x12, 0x23, 0x34);
    TEST(DIVB,  "70, 3423H[18]",   0xFE, 0x9F, 0x13, 0x23, 0x34, 0x46);
    TEST(DIVU,  "52, 18",          0x8C, 0x12, 0x34);
    ERRR(DIVU,  "52, 19",          0x8C, 0x13, 0x34);
    ERRR(DIVU,  "53, 18",          0x8C, 0x12, 0x35);
    ERRR(DIVU,  "54, 18",          0x8C, 0x12, 0x36);
    ERRR(DIVU,  "55, 18",          0x8C, 0x12, 0x37);
    TEST(DIVU,  "84, #3412H",      0x8D, 0x12, 0x34, 0x54);
    ERRR(DIVU,  "85, #3412H",      0x8D, 0x12, 0x34, 0x55);
    ERRR(DIVU,  "86, #3412H",      0x8D, 0x12, 0x34, 0x56);
    ERRR(DIVU,  "87, #3412H",      0x8D, 0x12, 0x34, 0x57);
    TEST(DIVU,  "52, [18]",        0x8E, 0x12, 0x34);
    ERRR(DIVU,  "52, [19]",        0x8E, 0x13, 0x34);
    ERRR(DIVU,  "53, [18]",        0x8E, 0x12, 0x35);
    ERRR(DIVU,  "54, [18]",        0x8E, 0x12, 0x36);
    ERRR(DIVU,  "55, [18]",        0x8E, 0x12, 0x37);
    TEST(DIVU,  "84, 52[18]",      0x8F, 0x12, 0x34, 0x54);
    ERRR(DIVU,  "85, 52[18]",      0x8F, 0x12, 0x34, 0x55);
    ERRR(DIVU,  "86, 52[18]",      0x8F, 0x12, 0x34, 0x56);
    ERRR(DIVU,  "87, 52[18]",      0x8F, 0x12, 0x34, 0x57);
    TEST(DIVU,  "120, 5634H[18]",  0x8F, 0x13, 0x34, 0x56, 0x78);
    ERRR(DIVU,  "121, 5634H[18]",  0x8F, 0x13, 0x34, 0x56, 0x79);
    ERRR(DIVU,  "122, 5634H[18]",  0x8F, 0x13, 0x34, 0x56, 0x7A);
    ERRR(DIVU,  "123, 5634H[18]",  0x8F, 0x13, 0x34, 0x56, 0x7B);
    TEST(DIVUB, "36, 19",          0x9C, 0x13, 0x24);
    ERRR(DIVUB, "37, 18",          0x9C, 0x12, 0x25);
    TEST(DIVUB, "36, #18",         0x9D, 0x12, 0x24);
    ERRR(DIVUB, "37, #18",         0x9D, 0x12, 0x25);
    TEST(DIVUB, "36, [18]",        0x9E, 0x12, 0x24);
    ERRR(DIVUB, "36, [19]",        0x9E, 0x13, 0x24);
    ERRR(DIVUB, "37, [18]",        0x9E, 0x12, 0x25);
    TEST(DIVUB, "52, 35[18]",      0x9F, 0x12, 0x23, 0x34);
    ERRR(DIVUB, "53, 35[18]",      0x9F, 0x12, 0x23, 0x35);
    TEST(DIVUB, "70, 3423H[18]",   0x9F, 0x13, 0x23, 0x34, 0x46);
    ERRR(DIVUB, "71, 3423H[18]",   0x9F, 0x13, 0x23, 0x34, 0x47);
    TEST(AND,   "52, 18",          0x60, 0x12, 0x34);
    TEST(AND,   "86, #3412H",      0x61, 0x12, 0x34, 0x56);
    TEST(AND,   "52, [18]",        0x62, 0x12, 0x34);
    TEST(AND,   "86, 52[18]",      0x63, 0x12, 0x34, 0x56);
    TEST(AND,   "120, 5634H[18]",  0x63, 0x13, 0x34, 0x56, 0x78);
    TEST(ANDB,  "35, 18",          0x70, 0x12, 0x23);
    TEST(ANDB,  "35, #18",         0x71, 0x12, 0x23);
    TEST(ANDB,  "35, [18]",        0x72, 0x12, 0x23);
    TEST(ANDB,  "52, 35[18]",      0x73, 0x12, 0x23, 0x34);
    TEST(ANDB,  "69, 3423H[18]",   0x73, 0x13, 0x23, 0x34, 0x45);
    TEST(OR,    "52, 18",          0x80, 0x12, 0x34);
    TEST(OR,    "86, #3412H",      0x81, 0x12, 0x34, 0x56);
    TEST(OR,    "52, [18]",        0x82, 0x12, 0x34);
    TEST(OR,    "86, 52[18]",      0x83, 0x12, 0x34, 0x56);
    TEST(OR,    "120, 5634H[18]",  0x83, 0x13, 0x34, 0x56, 0x78);
    TEST(ORB,   "35, 18",          0x90, 0x12, 0x23);
    TEST(ORB,   "35, #18",         0x91, 0x12, 0x23);
    TEST(ORB,   "35, [18]",        0x92, 0x12, 0x23);
    TEST(ORB,   "52, 35[18]",      0x93, 0x12, 0x23, 0x34);
    TEST(ORB,   "69, 3423H[18]",   0x93, 0x13, 0x23, 0x34, 0x45);
    TEST(XOR,   "52, 18",          0x84, 0x12, 0x34);
    TEST(XOR,   "86, #3412H",      0x85, 0x12, 0x34, 0x56);
    TEST(XOR,   "52, [18]",        0x86, 0x12, 0x34);
    TEST(XOR,   "86, 52[18]",      0x87, 0x12, 0x34, 0x56);
    TEST(XOR,   "120, 5634H[18]",  0x87, 0x13, 0x34, 0x56, 0x78);
    TEST(XORB,  "35, 18",          0x94, 0x12, 0x23);
    TEST(XORB,  "35, #18",         0x95, 0x12, 0x23);
    TEST(XORB,  "35, [18]",        0x96, 0x12, 0x23);
    TEST(XORB,  "52, 35[18]",      0x97, 0x12, 0x23, 0x34);
    TEST(XORB,  "69, 3423H[18]",   0x97, 0x13, 0x23, 0x34, 0x45);
}

static void test_3_operands() {
    TEST(ADD,   "86, 52, 18",          0x44, 0x12, 0x34, 0x56);
    TEST(ADD,   "120, 86, #3412H",     0x45, 0x12, 0x34, 0x56, 0x78);
    TEST(ADD,   "86, 52, [18]",        0x46, 0x12, 0x34, 0x56);
    TEST(ADD,   "120, 86, 52[18]",     0x47, 0x12, 0x34, 0x56, 0x78);
    TEST(ADD,   "154, 120, 5634H[18]", 0x47, 0x13, 0x34, 0x56, 0x78, 0x9A);
    TEST(ADDB,  "52, 35, 18",          0x54, 0x12, 0x23, 0x34);
    TEST(ADDB,  "52, 35, #18",         0x55, 0x12, 0x23, 0x34);
    TEST(ADDB,  "52, 35, [18]",        0x56, 0x12, 0x23, 0x34);
    TEST(ADDB,  "69, 52, 35[18]",      0x57, 0x12, 0x23, 0x34, 0x45);
    TEST(ADDB,  "86, 69, 3423H[18]",   0x57, 0x13, 0x23, 0x34, 0x45, 0x56);
    TEST(SUB,   "86, 52, 18",          0x48, 0x12, 0x34, 0x56);
    TEST(SUB,   "120, 86, #3412H",     0x49, 0x12, 0x34, 0x56, 0x78);
    TEST(SUB,   "86, 52, [18]",        0x4A, 0x12, 0x34, 0x56);
    TEST(SUB,   "120, 86, 52[18]",     0x4B, 0x12, 0x34, 0x56, 0x78);
    TEST(SUB,   "154, 120, 5634H[18]", 0x4B, 0x13, 0x34, 0x56, 0x78, 0x9A);
    TEST(SUBB,  "52, 35, 18",          0x58, 0x12, 0x23, 0x34);
    TEST(SUBB,  "52, 35, #18",         0x59, 0x12, 0x23, 0x34);
    TEST(SUBB,  "52, 35, [18]",        0x5A, 0x12, 0x23, 0x34);
    TEST(SUBB,  "69, 52, 35[18]",      0x5B, 0x12, 0x23, 0x34, 0x45);
    TEST(SUBB,  "86, 69, 3423H[18]",   0x5B, 0x13, 0x23, 0x34, 0x45, 0x56);
    TEST(MUL,   "84, 52, 18",          0xFE, 0x4C, 0x12, 0x34, 0x54);
    TEST(MUL,   "120, 86, #3412H",     0xFE, 0x4D, 0x12, 0x34, 0x56, 0x78);
    TEST(MUL,   "84, 52, [18]",        0xFE, 0x4E, 0x12, 0x34, 0x54);
    TEST(MUL,   "120, 86, 52[18]",     0xFE, 0x4F, 0x12, 0x34, 0x56, 0x78);
    TEST(MUL,   "152, 120, 5634H[18]", 0xFE, 0x4F, 0x13, 0x34, 0x56, 0x78, 0x98);
    TEST(MULB,  "52, 35, 18",          0xFE, 0x5C, 0x12, 0x23, 0x34);
    TEST(MULB,  "52, 35, #18",         0xFE, 0x5D, 0x12, 0x23, 0x34);
    TEST(MULB,  "52, 35, [18]",        0xFE, 0x5E, 0x12, 0x23, 0x34);
    TEST(MULB,  "70, 52, 35[18]",      0xFE, 0x5F, 0x12, 0x23, 0x34, 0x46);
    TEST(MULB,  "86, 69, 3423H[18]",   0xFE, 0x5F, 0x13, 0x23, 0x34, 0x45, 0x56);
    TEST(MULU,  "84, 52, 18",          0x4C, 0x12, 0x34, 0x54);
    ERRR(MULU,  "84, 52, 19",          0x4C, 0x13, 0x34, 0x54);
    ERRR(MULU,  "84, 53, 18",          0x4C, 0x12, 0x35, 0x54);
    ERRR(MULU,  "85, 52, 18",          0x4C, 0x12, 0x34, 0x55);
    ERRR(MULU,  "86, 52, 18",          0x4C, 0x12, 0x34, 0x56);
    ERRR(MULU,  "87, 52, 18",          0x4C, 0x12, 0x34, 0x57);
    TEST(MULU,  "120, 86, #3412H",     0x4D, 0x12, 0x34, 0x56, 0x78);
    ERRR(MULU,  "120, 87, #3412H",     0x4D, 0x12, 0x34, 0x57, 0x78);
    ERRR(MULU,  "121, 86, #3412H",     0x4D, 0x12, 0x34, 0x56, 0x79);
    ERRR(MULU,  "122, 86, #3412H",     0x4D, 0x12, 0x34, 0x56, 0x7A);
    ERRR(MULU,  "123, 86, #3412H",     0x4D, 0x12, 0x34, 0x56, 0x7B);
    TEST(MULU,  "84, 52, [18]",        0x4E, 0x12, 0x34, 0x54);
    ERRR(MULU,  "84, 52, [19]",        0x4E, 0x13, 0x34, 0x54);
    ERRR(MULU,  "84, 53, [18]",        0x4E, 0x12, 0x35, 0x54);
    ERRR(MULU,  "85, 52, [18]",        0x4E, 0x12, 0x34, 0x55);
    ERRR(MULU,  "86, 52, [18]",        0x4E, 0x12, 0x34, 0x56);
    ERRR(MULU,  "87, 52, [18]",        0x4E, 0x12, 0x34, 0x57);
    TEST(MULU,  "120, 86, 52[18]",     0x4F, 0x12, 0x34, 0x56, 0x78);
    ERRR(MULU,  "120, 87, 52[18]",     0x4F, 0x12, 0x34, 0x57, 0x78);
    ERRR(MULU,  "121, 86, 52[18]",     0x4F, 0x12, 0x34, 0x56, 0x79);
    ERRR(MULU,  "122, 86, 52[18]",     0x4F, 0x12, 0x34, 0x56, 0x7A);
    ERRR(MULU,  "123, 86, 52[18]",     0x4F, 0x12, 0x34, 0x56, 0x7B);
    TEST(MULU,  "152, 120, 5634H[18]", 0x4F, 0x13, 0x34, 0x56, 0x78, 0x98);
    ERRR(MULU,  "152, 121, 5634H[18]", 0x4F, 0x13, 0x34, 0x56, 0x79, 0x98);
    ERRR(MULU,  "153, 120, 5634H[18]", 0x4F, 0x13, 0x34, 0x56, 0x78, 0x99);
    ERRR(MULU,  "154, 120, 5634H[18]", 0x4F, 0x13, 0x34, 0x56, 0x78, 0x9A);
    ERRR(MULU,  "155, 120, 5634H[18]", 0x4F, 0x13, 0x34, 0x56, 0x78, 0x9B);
    TEST(MULUB, "52, 36, 19",          0x5C, 0x13, 0x24, 0x34);
    TEST(MULUB, "52, 35, 18",          0x5C, 0x12, 0x23, 0x34);
    ERRR(MULUB, "53, 36, 18",          0x5C, 0x12, 0x24, 0x35);
    TEST(MULUB, "52, 35, #18",         0x5D, 0x12, 0x23, 0x34);
    ERRR(MULUB, "53, 35, #18",         0x5D, 0x12, 0x23, 0x35);
    TEST(MULUB, "52, 35, [18]",        0x5E, 0x12, 0x23, 0x34);
    ERRR(MULUB, "52, 35, [19]",        0x5E, 0x13, 0x23, 0x34);
    ERRR(MULUB, "53, 35, [18]",        0x5E, 0x12, 0x23, 0x35);
    TEST(MULUB, "70, 53, 35[18]",      0x5F, 0x12, 0x23, 0x35, 0x46);
    ERRR(MULUB, "71, 53, 35[18]",      0x5F, 0x12, 0x23, 0x35, 0x47);
    TEST(MULUB, "86, 69, 3423H[18]",   0x5F, 0x13, 0x23, 0x34, 0x45, 0x56);
    ERRR(MULUB, "87, 69, 3423H[18]",   0x5F, 0x13, 0x23, 0x34, 0x45, 0x57);
    TEST(AND,   "86, 52, 18",          0x40, 0x12, 0x34, 0x56);
    TEST(AND,   "120, 86, #3412H",     0x41, 0x12, 0x34, 0x56, 0x78);
    TEST(AND,   "86, 52, [18]",        0x42, 0x12, 0x34, 0x56);
    TEST(AND,   "120, 86, 52[18]",     0x43, 0x12, 0x34, 0x56, 0x78);
    TEST(AND,   "154, 120, 5634H[18]", 0x43, 0x13, 0x34, 0x56, 0x78, 0x9A);
    TEST(ANDB,  "52, 35, 18",          0x50, 0x12, 0x23, 0x34);
    TEST(ANDB,  "52, 35, #18",         0x51, 0x12, 0x23, 0x34);
    TEST(ANDB,  "52, 35, [18]",        0x52, 0x12, 0x23, 0x34);
    TEST(ANDB,  "69, 52, 35[18]",      0x53, 0x12, 0x23, 0x34, 0x45);
    TEST(ANDB,  "86, 69, 3423H[18]",   0x53, 0x13, 0x23, 0x34, 0x45, 0x56);
}

static void test_move() {
    TEST(LD,    "52, 18",           0xA0, 0x12, 0x34);
    TEST(LD,    "86, #3412H",       0xA1, 0x12, 0x34, 0x56);
    TEST(LD,    "52, [18]",         0xA2, 0x12, 0x34);
    TEST(LD,    "86, 52[18]",       0xA3, 0x12, 0x34, 0x56);
    TEST(LD,    "120, 5634H[18]",   0xA3, 0x13, 0x34, 0x56, 0x78);
    TEST(LDB,   "35, 18",           0xB0, 0x12, 0x23);
    TEST(LDB,   "35, #18",          0xB1, 0x12, 0x23);
    TEST(LDB,   "35, [18]",         0xB2, 0x12, 0x23);
    TEST(LDB,   "236, -19[254]",    0xB3, 0xFE, 0xED, 0xEC);
    TEST(LDB,   "235, 0ECEDH[254]", 0xB3, 0xFF, 0xED, 0xEC, 0xEB);
    TEST(ST,    "52, 18",           0xC0, 0x12, 0x34);
    ERRI(                           0xC1, 0x12, 0x34, 0x56);
    TEST(ST,    "52, [18]",         0xC2, 0x12, 0x34);
    TEST(ST,    "86, 52[18]",       0xC3, 0x12, 0x34, 0x56);
    TEST(ST,    "120, 5634H[18]",   0xC3, 0x13, 0x34, 0x56, 0x78);
    TEST(STB,   "35, 18",           0xC4, 0x12, 0x23);
    ERRI(                           0xC5, 0x12, 0x23);
    TEST(STB,   "35, [18]",         0xC6, 0x12, 0x23);
    TEST(STB,   "236, -19[254]",    0xC7, 0xFE, 0xED, 0xEC);
    TEST(STB,   "235, 0ECEDH[254]", 0xC7, 0xFF, 0xED, 0xEC, 0xEB);
    TEST(LDBSE, "52, 19",           0xBC, 0x13, 0x34);
    ERRR(LDBSE, "53, 19",           0xBC, 0x13, 0x35);
    TEST(LDBSE, "52, #18",          0xBD, 0x12, 0x34);
    ERRR(LDBSE, "53, #18",          0xBD, 0x12, 0x35);
    TEST(LDBSE, "52, [18]",         0xBE, 0x12, 0x34);
    ERRR(LDBSE, "52, [19]",         0xBE, 0x13, 0x34);
    ERRR(LDBSE, "53, [18]",         0xBE, 0x12, 0x35);
    TEST(LDBSE, "86, 52[18]",       0xBF, 0x12, 0x34, 0x56);
    ERRR(LDBSE, "87, 52[18]",       0xBF, 0x12, 0x34, 0x57);
    TEST(LDBSE, "120, 5634H[18]",   0xBF, 0x13, 0x34, 0x56, 0x78);
    ERRR(LDBSE, "121, 5634H[18]",   0xBF, 0x13, 0x34, 0x56, 0x79);
    TEST(LDBZE, "52, 19",           0xAC, 0x13, 0x34);
    TEST(LDBZE, "52, #18",          0xAD, 0x12, 0x34);
    TEST(LDBZE, "52, [18]",         0xAE, 0x12, 0x34);
    TEST(LDBZE, "86, 52[18]",       0xAF, 0x12, 0x34, 0x56);
    TEST(LDBZE, "120, 5634H[18]",   0xAF, 0x13, 0x34, 0x56, 0x78);
    TEST(PUSH,  "18",           0xC8, 0x12);
    ERRR(PUSH,  "19",           0xC8, 0x13);
    TEST(PUSH,  "#3412H",       0xC9, 0x12, 0x34);
    TEST(PUSH,  "[18]",         0xCA, 0x12);
    ERRR(PUSH,  "[19]",         0xCA, 0x13);
    TEST(PUSH,  "52[18]",       0xCB, 0x12, 0x34);
    TEST(PUSH,  "5634H[18]",    0xCB, 0x13, 0x34, 0x56);
    TEST(POP,   "18",           0xCC, 0x12);
    ERRR(POP,   "19",           0xCC, 0x13);
    ERRI(                       0xCD, 0x12, 0x34);
    TEST(POP,   "[18]",         0xCE, 0x12);
    ERRR(POP,   "[19]",         0xCE, 0x13);
    TEST(POP,   "52[18]",       0xCF, 0x12, 0x34);
    TEST(POP,   "5634H[18]",    0xCF, 0x13, 0x34, 0x56);
    TEST(PUSHF, "",             0xF2);
    TEST(POPF,  "",             0xF3);
}

static void test_jump() {
    ATEST(0x2000, SJMP, "2002H",   0x20, 0x00);
    ATEST(0x2000, SJMP, "2200H",   0x21, 0xFE);
    ATEST(0x2000, SJMP, "2202H",   0x22, 0x00);
    ATEST(0x2000, SJMP, "2401H",   0x23, 0xFF);
    ATEST(0x2000, SJMP, "1C02H",   0x24, 0x00);
    ATEST(0x2000, SJMP, "1E00H",   0x25, 0xFE);
    ATEST(0x2000, SJMP, "1E02H",   0x26, 0x00);
    ATEST(0x2000, SJMP, "2000H",   0x27, 0xFE);
    ATEST(0x2000, LJMP, "2003H",   0xE7, 0x00, 0x00);
    ATEST(0x2000, LJMP, "0A002H",  0xE7, 0xFF, 0x7F);
    ATEST(0x2000, LJMP, "2103H",   0xE7, 0x00, 0x01);
    ATEST(0x2000, LJMP, "2400H",   0xE7, 0xFD, 0x03);
    ATEST(0x2000, LJMP, "2003H",   0xE7, 0x00, 0x00);
    ATEST(0x2000, LJMP, "2003H",   0xE7, 0x00, 0x00);
    ATEST(0xA000, LJMP, "2003H",   0xE7, 0x00, 0x80);
    ATEST(0x2000, LJMP, "2000H",   0xE7, 0xFD, 0xFF);
    ATEST(0x2000, SCALL, "2002H",  0x28, 0x00);
    ATEST(0x2000, SCALL, "2200H",  0x29, 0xFE);
    ATEST(0x2000, SCALL, "2202H",  0x2A, 0x00);
    ATEST(0x2000, SCALL, "2401H",  0x2B, 0xFF);
    ATEST(0x2000, SCALL, "1C02H",  0x2C, 0x00);
    ATEST(0x2000, SCALL, "1E00H",  0x2D, 0xFE);
    ATEST(0x2000, SCALL, "1E02H",  0x2E, 0x00);
    ATEST(0x2000, SCALL, "2000H",  0x2F, 0xFE);
    ATEST(0x2000, LCALL, "2003H",  0xEF, 0x00, 0x00);
    ATEST(0x2000, LCALL, "0A002H", 0xEF, 0xFF, 0x7F);
    ATEST(0x2000, LCALL, "2103H",  0xEF, 0x00, 0x01);
    ATEST(0x2000, LCALL, "2400H",  0xEF, 0xFD, 0x03);
    ATEST(0x2000, LCALL, "2003H",  0xEF, 0x00, 0x00);
    ATEST(0x2000, LCALL, "2003H",  0xEF, 0x00, 0x00);
    ATEST(0xA000, LCALL, "2003H",  0xEF, 0x00, 0x80);
    ATEST(0x2000, LCALL, "2000H",  0xEF, 0xFD, 0xFF);

    TEST(BR,  "[18]", 0xE3, 0x12);
    ERRR(BR,  "[19]", 0xE3, 0x13);
    TEST(RET, "",     0xF0);

    ATEST(0x2000, JNST, "2002H",   0xD0, 0x00);
    ATEST(0x2000, JNH,  "2000H",   0xD1, 0xFE);
    ATEST(0x2000, JGT,  "2002H",   0xD2, 0x00);
    ATEST(0x2000, JNC,  "2001H",   0xD3, 0xFF);
    ATEST(0x2000, JNVT, "2002H",   0xD4, 0x00);
    ATEST(0x2000, JNV,  "2000H",   0xD5, 0xFE);
    ATEST(0x2000, JGE,  "2002H",   0xD6, 0x00);
    ATEST(0x2000, JNE,  "2000H",   0xD7, 0xFE);
    ATEST(0x2000, JNST, "2002H",   0xD0, 0x00);
    ATEST(0x2000, JNH,  "2000H",   0xD1, 0xFE);
    ATEST(0x2000, JGT,  "2002H",   0xD2, 0x00);
    ATEST(0x2000, JNC,  "2001H",   0xD3, 0xFF);
    ATEST(0x2000, JNVT, "2002H",   0xD4, 0x00);
    ATEST(0x2000, JNV,  "2000H",   0xD5, 0xFE);
    ATEST(0x2000, JGE,  "2002H",   0xD6, 0x00);
    ATEST(0x2000, JNE,  "2000H",   0xD7, 0xFE);

    ATEST(0x2000, DJNZ, "18, 2000H", 0xE0, 0x12, 0xFD);
    ATEST(0x2000, DJNZ, "18, 2082H", 0xE0, 0x12, 0x7F);
    ATEST(0x2000, DJNZ, "18, 1F83H", 0xE0, 0x12, 0x80);
    ATEST(0x2000, DJNZ, "18, 2003H", 0xE0, 0x12, 0x00);

    ATEST(0x2000, JBC, "18, 0, 2000H", 0x30, 0x12, 0xFD);
    ATEST(0x2000, JBC, "18, 1, 2082H", 0x31, 0x12, 0x7F);
    ATEST(0x2000, JBC, "18, 2, 1F83H", 0x32, 0x12, 0x80);
    ATEST(0x2000, JBC, "18, 3, 2003H", 0x33, 0x12, 0x00);
    ATEST(0x2000, JBC, "18, 4, 2000H", 0x34, 0x12, 0xFD);
    ATEST(0x2000, JBC, "18, 5, 2082H", 0x35, 0x12, 0x7F);
    ATEST(0x2000, JBC, "18, 6, 1F83H", 0x36, 0x12, 0x80);
    ATEST(0x2000, JBC, "18, 7, 2003H", 0x37, 0x12, 0x00);
    ATEST(0x2000, JBS, "18, 0, 2000H", 0x38, 0x12, 0xFD);
    ATEST(0x2000, JBS, "18, 1, 2082H", 0x39, 0x12, 0x7F);
    ATEST(0x2000, JBS, "18, 2, 1F83H", 0x3A, 0x12, 0x80);
    ATEST(0x2000, JBS, "18, 3, 2003H", 0x3B, 0x12, 0x00);
    ATEST(0x2000, JBS, "18, 4, 2000H", 0x3C, 0x12, 0xFD);
    ATEST(0x2000, JBS, "18, 5, 2082H", 0x3D, 0x12, 0x7F);
    ATEST(0x2000, JBS, "18, 6, 1F83H", 0x3E, 0x12, 0x80);
    ATEST(0x2000, JBS, "18, 7, 2003H", 0x3F, 0x12, 0x00);
}

static void test_jump_relative() {
    disassembler.setRelativeTarget(true);

    ATEST(0x2000, SJMP, "$+2",     0x20, 0x00);
    ATEST(0x2000, SJMP, "$+512",   0x21, 0xFE);
    ATEST(0x2000, SJMP, "$+514",   0x22, 0x00);
    ATEST(0x2000, SJMP, "$+1025",  0x23, 0xFF);
    ATEST(0x2000, SJMP, "$-1022",  0x24, 0x00);
    ATEST(0x2000, SJMP, "$-512",   0x25, 0xFE);
    ATEST(0x2000, SJMP, "$-510",   0x26, 0x00);
    ATEST(0x2000, SJMP, "$",       0x27, 0xFE);
    ATEST(0x2000, LJMP, "$+3",     0xE7, 0x00, 0x00);
    ATEST(0x2000, LJMP, "$-7FFEH", 0xE7, 0xFF, 0x7F);
    //ATEST(0x2000, LJMP, "$+8002H", 0xE7, 0xFF, 0x7F);
    ATEST(0x2000, LJMP, "$+0103H", 0xE7, 0x00, 0x01);
    ATEST(0x2000, LJMP, "$+0400H", 0xE7, 0xFD, 0x03);
    ATEST(0x2000, LJMP, "$+3",     0xE7, 0x00, 0x00);
    ATEST(0xA000, LJMP, "$-7FFDH", 0xE7, 0x00, 0x80);
    ATEST(0x2000, LJMP, "$",       0xE7, 0xFD, 0xFF);
    ATEST(0x2000, SCALL, "$+2",    0x28, 0x00);
    ATEST(0x2000, SCALL, "$+512",  0x29, 0xFE);
    ATEST(0x2000, SCALL, "$+514",  0x2A, 0x00);
    ATEST(0x2000, SCALL, "$+1025", 0x2B, 0xFF);
    ATEST(0x2000, SCALL, "$-1022", 0x2C, 0x00);
    ATEST(0x2000, SCALL, "$-512",  0x2D, 0xFE);
    ATEST(0x2000, SCALL, "$-510",  0x2E, 0x00);
    ATEST(0x2000, SCALL, "$",      0x2F, 0xFE);
    ATEST(0x2000, LCALL, "$+3",     0xEF, 0x00, 0x00);
    ATEST(0x2000, LCALL, "$-7FFEH", 0xEF, 0xFF, 0x7F);
    //ATEST(0x2000, LCALL, "$+8002H", 0xEF, 0xFF, 0x7F);
    ATEST(0x2000, LCALL, "$+0103H", 0xEF, 0x00, 0x01);
    ATEST(0x2000, LCALL, "$+0400H", 0xEF, 0xFD, 0x03);
    ATEST(0x2000, LCALL, "$+3",     0xEF, 0x00, 0x00);
    ATEST(0xA000, LCALL, "$-7FFDH", 0xEF, 0x00, 0x80);
    ATEST(0x2000, LCALL, "$",       0xEF, 0xFD, 0xFF);

    ATEST(0x2000, JNST, "$+2",  0xD0, 0x00);
    ATEST(0x2000, JNH,  "$",    0xD1, 0xFE);
    ATEST(0x2000, JGT,  "$+2",  0xD2, 0x00);
    ATEST(0x2000, JNC,  "$+1",  0xD3, 0xFF);
    ATEST(0x2000, JNVT, "$+2",  0xD4, 0x00);
    ATEST(0x2000, JNV,  "$",    0xD5, 0xFE);
    ATEST(0x2000, JGE,  "$+2",  0xD6, 0x00);
    ATEST(0x2000, JNE,  "$",    0xD7, 0xFE);
    ATEST(0x2000, JNST, "$+2",  0xD0, 0x00);
    ATEST(0x2000, JNH,  "$",    0xD1, 0xFE);
    ATEST(0x2000, JGT,  "$+2",  0xD2, 0x00);
    ATEST(0x2000, JNC,  "$+1",  0xD3, 0xFF);
    ATEST(0x2000, JNVT, "$+2",  0xD4, 0x00);
    ATEST(0x2000, JNV,  "$",    0xD5, 0xFE);
    ATEST(0x2000, JGE,  "$+2",  0xD6, 0x00);
    ATEST(0x2000, JNE,  "$",    0xD7, 0xFE);

    ATEST(0x2000, DJNZ, "18, $",     0xE0, 0x12, 0xFD);
    ATEST(0x2000, DJNZ, "18, $+130", 0xE0, 0x12, 0x7F);
    ATEST(0x2000, DJNZ, "18, $-125", 0xE0, 0x12, 0x80);
    ATEST(0x2000, DJNZ, "18, $+3",   0xE0, 0x12, 0x00);

    ATEST(0x2000, JBC, "18, 0, $",     0x30, 0x12, 0xFD);
    ATEST(0x2000, JBC, "18, 1, $+130", 0x31, 0x12, 0x7F);
    ATEST(0x2000, JBC, "18, 2, $-125", 0x32, 0x12, 0x80);
    ATEST(0x2000, JBC, "18, 3, $+3",   0x33, 0x12, 0x00);
    ATEST(0x2000, JBC, "18, 4, $",     0x34, 0x12, 0xFD);
    ATEST(0x2000, JBC, "18, 5, $+130", 0x35, 0x12, 0x7F);
    ATEST(0x2000, JBC, "18, 6, $-125", 0x36, 0x12, 0x80);
    ATEST(0x2000, JBC, "18, 7, $+3",   0x37, 0x12, 0x00);
    ATEST(0x2000, JBS, "18, 0, $",     0x38, 0x12, 0xFD);
    ATEST(0x2000, JBS, "18, 1, $+130", 0x39, 0x12, 0x7F);
    ATEST(0x2000, JBS, "18, 2, $-125", 0x3A, 0x12, 0x80);
    ATEST(0x2000, JBS, "18, 3, $+3",   0x3B, 0x12, 0x00);
    ATEST(0x2000, JBS, "18, 4, $",     0x3C, 0x12, 0xFD);
    ATEST(0x2000, JBS, "18, 5, $+130", 0x3D, 0x12, 0x7F);
    ATEST(0x2000, JBS, "18, 6, $-125", 0x3E, 0x12, 0x80);
    ATEST(0x2000, JBS, "18, 7, $+3",   0x3F, 0x12, 0x00);
}

static void test_modify() {
    TEST(DEC,  "6", 0x05, 0x06);
    TEST(DECB, "7", 0x15, 0x07);
    TEST(INC,  "8", 0x07, 0x08);
    TEST(INCB, "9", 0x17, 0x09);
    TEST(EXT,  "8", 0x06, 0x08);
    TEST(EXTB, "6", 0x16, 0x06);
    TEST(NOT,  "8", 0x02, 0x08);
    TEST(NOTB, "9", 0x12, 0x09);
    TEST(CLR,  "6", 0x01, 0x06);
    TEST(CLRB, "7", 0x11, 0x07);

    TEST(SHL,  "10, #11", 0x09, 0x0B, 0x0A);
    TEST(SHL,  "10, 17",  0x09, 0x11, 0x0A);
    ERRR(SHL,  "11, 17",  0x09, 0x11, 0x0B);
    TEST(SHLB, "23, #11", 0x19, 0x0B, 0x17);
    TEST(SHLB, "23, 17",  0x19, 0x11, 0x17);
    TEST(SHLL, "24, #11", 0x0D, 0x0B, 0x18);
    TEST(SHLL, "24, 17",  0x0D, 0x11, 0x18);
    ERRR(SHLL, "25, 17",  0x0D, 0x11, 0x19);
    ERRR(SHLL, "26, 17",  0x0D, 0x11, 0x1A);
    ERRR(SHLL, "27, 17",  0x0D, 0x11, 0x1B);    

    TEST(SHR,  "10, #11", 0x08, 0x0B, 0x0A);
    TEST(SHR,  "10, 16",  0x08, 0x10, 0x0A);
    TEST(SHRB, "23, #11", 0x18, 0x0B, 0x17);
    TEST(SHRB, "23, 16",  0x18, 0x10, 0x17);
    TEST(SHRL, "24, #11", 0x0C, 0x0B, 0x18);
    TEST(SHRL, "24, 16",  0x0C, 0x10, 0x18);

    TEST(SHRA,  "10, #11", 0x0A, 0x0B, 0x0A);
    TEST(SHRA,  "10, 16",  0x0A, 0x10, 0x0A);
    TEST(SHRAB, "23, #11", 0x1A, 0x0B, 0x17);
    TEST(SHRAB, "23, 16",  0x1A, 0x10, 0x17);
    TEST(SHRAL, "24, #11", 0x0E, 0x0B, 0x18);
    TEST(SHRAL, "24, 16",  0x0E, 0x10, 0x18);

    TEST(NORML, "20, 19",  0x0F, 0x13, 0x14);
}

static void test_control() {
    TEST(SETC,  "",  0xF9);
    TEST(CLRC,  "",  0xF8);
    TEST(CLRVT, "",  0xFC);
    TEST(RST,   "",  0xFF);
    TEST(DI,    "",  0xFA);
    TEST(EI,    "",  0xFB);
    TEST(NOP,   "",  0xFD);
    TEST(SKIP,  "1", 0x00, 0x01);
    TEST(TRAP,  "",  0xF7);
}

static void test_illegal() {
    ERRI(0x04);
    ERRI(0x0B);
    ERRI(0x10);
    ERRI(0x14);
    ERRI(0x1B);
    ERRI(0x1C);
    ERRI(0x1D);
    ERRI(0x1E);
    ERRI(0x1F);
    ERRI(0x1B);
    ERRI(0xC1);
    ERRI(0xC5);
    ERRI(0xCD);
    ERRI(0xE1);
    ERRI(0xE2);
    ERRI(0xE4);
    ERRI(0xE5);
    ERRI(0xE6);
    ERRI(0xE8);
    ERRI(0xE9);
    ERRI(0xEA);
    ERRI(0xEB);
    ERRI(0xEC);
    ERRI(0xED);
    ERRI(0xEE);
    ERRI(0xF1);
    ERRI(0xF4);
    ERRI(0xF5);
    ERRI(0xF6);
}
// clang-format on

const char *run_cpu_test() {
    RUN_TEST(test_cpu);
    RUN_TEST(test_2_operands);
    RUN_TEST(test_3_operands);
    RUN_TEST(test_move);
    RUN_TEST(test_jump);
    RUN_TEST(test_jump_relative);
    RUN_TEST(test_modify);
    RUN_TEST(test_control);
    RUN_TEST(test_illegal);
    return disassembler.listCpu();
}

void run_tests(const char *cpu) {
    disassembler.setCpu(cpu);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
