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

#include "test_expr_helper.h"

using namespace libasm;
using namespace libasm::test;

const MotorolaNumberParser number;
const AsteriskCommentParser comment;
const SimpleSymbolParser symbol{SymbolParser::DOLLAR_DOT_UNDER};
const MotorolaLetterParser letter;
const AsteriskLocationParser location;
const Mc68xxOperatorParser operators;
ValueParser parser{number, comment, symbol, letter, location, operators};
MotorolaValueFormatter formatter;

static void set_up() {
    formatter.setCStyle(false);
    formatter.setUpperHex(false);
}

static void tear_down() {
    symtab.reset();
}

// clang-format off
static void test_char_constant() {
    E8("'a'",   0x61);
    E8("'a'+5", 0x66);
    E8("5+'a'", 0x66);
    E8("'a",    0x61);
    E8("'a+5",  0x66);
    E8("5+'a",  0x66);
    E8("' ",    0x20);
    E8("''",    0x27);
    E8("'''",   0x27);
    X8("''''",  GARBAGE_AT_END, "'");

    E16("'a'", 0x61);
    E32("'a'", 0x61);
}

static void test_char_closing() {
    const MotorolaLetterParser letter{true};
    ValueParser parser{number, comment, symbol, letter, location};

    X8("'a",   MISSING_CLOSING_QUOTE, "'a");
    X8("'a+5", MISSING_CLOSING_QUOTE, "'a+5");
    X8("5+'a", MISSING_CLOSING_QUOTE, "'a");
    X8("' ",   MISSING_CLOSING_QUOTE, "' ");
    X8("''",   MISSING_CLOSING_QUOTE, "''");
}

static void test_dec_constant() {
    E8("0",   0x00);
    E8("127", 0x7f);
    E8("128", 0x80);
    E8("255", 0xff);
    X8("256", OVERFLOW_RANGE, "256");

    E16("&0",     0x0000);
    E16("&32767", 0x7fff);
    E16("&32768", 0x8000);
    E16("&65535", 0xffff);
    X16("&65536", OVERFLOW_RANGE, "&65536");

    E32("&0",          0x00000000);
    E32("&2147483647", 0x7fffffff);
    E32("2147483648",  0x80000000);
    E32("&4294967295", 0xffffffff);
    X32("4294967296",  OVERFLOW_RANGE, "4294967296");
    X32("&9999999999", OVERFLOW_RANGE, "&9999999999");
}

static void test_hex_constant() {
    E8("$0",   0x00);
    E8("$7f",  0x7f);
    E8("$80",  0x80);
    E8("$FF",  0xff);
    X8("$100", OVERFLOW_RANGE, "$100");

    E16("$0",     0x0000);
    E16("$7fff",  0x7fff);
    E16("$8000",  0x8000);
    E16("$ffff",  0xffff);
    X16("$10000", OVERFLOW_RANGE, "$10000");

    E32("$0",         0x00000000);
    E32("$7FFFFFFF",  0x7fffffff);
    E32("$80000000",  0x80000000);
    E32("$FFFFffff",  0xffffffff);
    X32("$100000000", OVERFLOW_RANGE, "$100000000");

    E32("0x0",         0x00000000);
    E32("0x7FFFFFFF",  0x7fffffff);
    E32("0x80000000",  0x80000000);
    E32("0xFFFFffff",  0xffffffff);
    X32("0x100000000", OVERFLOW_RANGE, "0x100000000");
}

static void test_oct_constant() {
    E8("@0",   0x00);
    E8("@177", 0x7f);
    E8("@200", 0x80);
    E8("@377", 0xff);
    X8("@400", OVERFLOW_RANGE, "@400");

    E16("@0",      0x0000);
    E16("@077777", 0x7fff);
    E16("@100000", 0x8000);
    E16("@177777", 0xffff);
    X16("@200000", OVERFLOW_RANGE, "@200000");

    E32("@0",           0x00000000);
    E32("@17777777777", 0x7fffffff);
    E32("@20000000000", 0x80000000);
    E32("@37777777777", 0xffffffff);
    X32("@40000000000", OVERFLOW_RANGE, "@40000000000");

    E32("00",           0x00000000);
    E32("017777777777", 0x7fffffff);
    E32("020000000000", 0x80000000);
    E32("037777777777", 0xffffffff);
    X32("040000000000", OVERFLOW_RANGE, "040000000000");
}

static void test_bin_constant() {
    E8("%0",         0x00);
    E8("%01111111",  0x7f);
    E8("%10000000",  0x80);
    E8("%11111111",  0xff);
    X8("%100000000", OVERFLOW_RANGE, "%100000000");

    E16("%0",                 0x0000);
    E16("%0111111111111111",  0x7fff);
    E16("%1000000000000000",  0x8000);
    E16("%1111111111111111",  0xffff);
    X16("%10000000000000000", OVERFLOW_RANGE, "%10000000000000000");

    E32("%0",                                 0x00000000);
    E32("%01111111111111111111111111111111",  0x7fffffff);
    E32("%10000000000000000000000000000000",  0x80000000);
    E32("%11111111111111111111111111111111",  0xffffffff);
    X32("%100000000000000000000000000000000", OVERFLOW_RANGE, "%100000000000000000000000000000000");

    E32("0b0",                                 0x00000000);
    E32("0b01111111111111111111111111111111",  0x7fffffff);
    E32("0b10000000000000000000000000000000",  0x80000000);
    E32("0b11111111111111111111111111111111",  0xffffffff);
    X32("0b100000000000000000000000000000000", OVERFLOW_RANGE, "0b100000000000000000000000000000000");
}

static void test_unary_operator() {
    E8("-1",   0xff);
    E8("-128", 0x80);
    X8("-129", OVERFLOW_RANGE, "-129");

    E16("-1",     0xffff);
    E16("-32768", 0x8000);
    X16("-32769", OVERFLOW_RANGE, "-32769");

    E32("-1",          0xffffffff);
    E32("-2147483648", 0x80000000);
    X32("-2147483649", OVERFLOW_RANGE, "-2147483649");
    X32("-4294967295", OVERFLOW_RANGE, "-4294967295");

    E8("+128", 0x80);
    E8("+129", 0x81);
    X8("+256", OVERFLOW_RANGE, "+256");

    E16("+32768", 0x8000);
    E16("+32769", 0x8001);
    X16("+65536", OVERFLOW_RANGE, "+65536");

    E32("+2147483648", 0x80000000);
    E32("+2147483649", 0x80000001);
    X32("+4294967296", OVERFLOW_RANGE, "4294967296");

    E8("~+0",    0xFF);
    E8("~(1|8)", 0xF6);
    E8("~-1",    0x00);

    E16("~0x0",     0xFFFF);
    E16("~(1|010)", 0xFFF6);
    E16("~-0b1",    0x0000);

    E32("~+0",    0xFFFFFFFF);
    E32("~(1|8)", 0xFFFFFFF6);
    E32("~-1",    0x00000000);

    E32("!0", 1);
    E32("!5", 0);
    E32("!-1", 0);
    E32("~!-1", 0xFFFFFFFF);
    E32("!~!-1", 0);
}

static void test_binary_operator() {
    E8("( 2)+( 3)",  5);
    E8("( 2)+(-3)", -1);
    E8("(-2)+( 3)",  1);
    E8("(-2)+(-3)", -5);
    E8("-128+383", 255);
    E8("255-383", -128);
    E8("( 2)-( 3)", -1);
    E8("( 2)-(-3)",  5);
    E8("(-2)-( 3)", -5);
    E8("(-2)-(-3)",  1);
    E8("( 2)*( 3)",  6);
    E8("( 2)*(-3)",  -6);
    E8("(-2)*( 3)",  -6);
    E8("(-2)*(-3)",   6);
    E8("( 20)/( 3)",  6);
    E8("( 22)/(-5)", -4);
    E8("(-22)/( 5)", -4);
    E8("(-22)/(-5)",  4);
    E8("( 20)%( 3)",  2);
    E8("( 20)%(-3)",  2);
    E8("(-20)%( 3)", -2);
    E8("(-20)%(-3)", -2);

    E32("  2 !^ 3",   8);
    E32("(-2)!^ 3",  -8);
    E32("  2 !^ 0",   1);
    E32("  2 !^ 31",  0x80000000);
    E32("  2 !^ 32",  0);
    E32("$100 !^ 3",  0x01000000);
    E32("$8000 !^ 2", 0x40000000);

    E8("1<<0", 1);
    E8("1<<7", 128);
    X8("1<<8", OVERFLOW_RANGE, "1<<8");
    E8("0<<8", 0);
    E8("-1>>8", -1);
    X8("-1<<8", OVERFLOW_RANGE, "-1<<8");
    E8("-128>>1", -64);
    E8("-128>>7", -1);
    E8("-128>>8", -1);
    E8("0x80>>7", 0x01);
    E8("0x80>>8", 0);
    E8("0xFF>>4", 0x0F);
    X8("0xFF<<4", OVERFLOW_RANGE, "0xFF<<4");
    E8("(0xFF<<4)&0xFF", 0xF0);

    E8("0b0001|0b0100", 0x05);
    E8("0b1011&0b0110", 0x02);
    E8("0b0110^0b0011", 0x05);

    E8("0b0001 !+ 0b0100", 0x05);
    E8("0b1011 !. 0b0110", 0x02);
    E8("0b0110 !X 0b0011", 0x05);

    E32("( 2)+( 3)",  5);
    E32("( 2)+(-3)", -1);
    E32("(-2)+( 3)",  1);
    E32("(-2)+(-3)", -5);
    E32("( 2)-( 3)", -1);
    E32("( 2)-(-3)",  5);
    E32("(-2)-( 3)", -5);
    E32("(-2)-(-3)",  1);
    E32("( 2)*( 3)",  6);
    E32("( 2)*(-3)",  -6);
    E32("(-2)*( 3)",  -6);
    E32("(-2)*(-3)",   6);
    E32("( 20)/( 3)",  6);
    E32("( 22)/(-5)", -4);
    E32("(-22)/( 5)", -4);
    E32("(-22)/(-5)",  4);
    E32("( 20)%( 3)",  2);
    E32("( 20)%(-3)",  2);
    E32("(-20)%( 3)", -2);
    E32("(-20)%(-3)", -2);

    // Shift and rotate operators work on unsigned 16-bit operands.
    E16("1 !< 0",    0x0001);
    E16("1 !< 7",    0x0080);
    E16("1 !< 8",    0x0100);
    E16("0 !< 8",    0x0000);
    E16("-1 !> 8",   0x00FF);
    E16("-1 !< 8",   0xFF00);
    E16("-128 !> 1", 0x7FC0);
    E16("-128 !> 7", 0x01FF);
    E16("-128 !> 8", 0x00FF);
    E16("0x80 !> 7", 0x0001);
    E16("0x80 !> 8", 0x0000);
    E16("0xFF !> 4", 0x000F);
    E16("0xFF !< 4", 0x0FF0);
    E16("     1 !L 0",  0x0001);
    E16("     1 !L 16", 0x0001);
    E16("0x1234 !L 4",  0x2341);
    E16("0x8000 !L 1",  0x0001);
    E16("0x8000 !L 7",  0x0040);
    E16("     0 !L 8",  0x0000);
    E16("     1 !R 0",  0x0001);
    E16("     1 !R 16", 0x0001);
    E16("0x1234 !R 4",  0x4123);
    E16("     1 !R 1",  0x8000);
    E16("     1 !R 7",  0x0200);
    E16("     0 !R 8",  0x0000);

    E32("0x00000001<<31", 0x80000000);
    E32("0x00000001<<32", 0);
    E32("0x80000000>>31", 1);
    E32("0x80000000>>32", 0);
    E32("1>>(-1)", 0);
    E32("1<<(-1)", 0);
    E32("-1<<31", -0x80000000);
    E32("-1<<32", 0);
    E32("-1>>31", -1);
    E32("-1>>32", -1);

    E32("0b0001|0b0100", 0x05);
    E32("0b1011&0b0110", 0x02);
    E32("0b0110^-1",    0xfffffff9);

    E32("0b0001 !+ 0b0100", 0x05);
    E32("0b1011 !. 0b0110", 0x02);
    E32("0b0110 !X -1",     0xfffffff9);

    E32(" 10< 10", 0);
    E32(" 10<=10", 1);
    E32(" 10>=10", 1);
    E32(" 10> 10", 0);

    E32(" 10< 20", 1);
    E32("-10<-20", 0);
    E32(" 10> 20", 0);
    E32("-10>-20", 1);

    E32(" 0 <  0xFFFFFFFD", 1);
    E32(" 2 <  0xFFFFFFFD", 1); // unsigned comparison
    E32("-2 <  0xFFFFFFFD", 0); // signed comparison
    E32("-3 == 0xFFFFFFFD", 1);

    E32("10 == 10", 1);
    E32("10 == 20", 0);
    E32("10 != 10", 0);
    E32("10 != 20", 1);

    E32("1 && !0 && 2", 1);
    E32("0 || !2 || 0", 0);
}

static void test_precedence() {
    E16("1+2-3+4",    4);
    E16("1+2*3+4",   11);
    E16("1+2-7/3",    1); // 1-2-(7/3)
    E16("1-8%3*3",   -5); // 1-(8%3)*3
    E16("(1+2)*3+4", 13);
    E16("1+(2-7)/3",  0);
    E16("(1-8)%3*3", -3); // ((1-8)%3)*3

    E8(" 1<<2 +3", 0x20); // 1 << (2+3)
    E8("(1<<2)+3" ,   7);
    E8(" 1<<2 *3", 0x40); // 1 << (2*3)
    E8("(1<<2)*3",   12);
    E8(" 0x20>>2 +3",  1); // 0x20 >> (2+3)
    E8("(0x20>>2)+3", 11);
    E8(" 0x40>>2 *3",  1); // 0x40 >> (2*6)
    E8("(0x40>>2)*3", 48);

    E8("1<< 4>>1",  8); // (1<<4) >> 1
    E8("1<<(4>>1)", 4);
    E8("4>> 1<<1",  4); // (4>>1) << 1
    E8("4>>(1<<1)", 1);

    // Shift and rotate operator has the same precedence of multipulication
    E16("1!< 2 +3",     7); // (1!<2)+3
    E16("1!<(2 +3)", 0x20);
    E16("1!< 2 *3",    12); // (1!<2)*3
    E16("1!<(2*3)",  0x40);
    E16("0x20!> 2+3",  11); // (0x20!>2)+3
    E16("0x20!>(2+3)",  1);
    E16("0x40!> 2*3",  48); // (0x40!>2)*6
    E16("0x40!>(2*3)",  1);

    E8("1!< 4!>1",  8); // (1!<4) !> 1
    E8("1!<(4!>1)", 4);
    E8("4!> 1!<1",  4); // (4!>1) !< 1
    E8("4!>(1!<1)", 1);

    // Bitwise, shift and rotate operators have the same precedence
    E8(" 1 |  2 &  6 ^  8", 0b1011); // 1 | ((2 & 6) ^ 8)
    E8("(1 |  2)&  6 ^  8", 0b1010); // ((1 | 2) & 6) ^ 8
    E8(" 1 |  2 & (6 ^  8)",0b0011); // 1 | (2 & (6 ^ 8))
    E8(" 1 !+ 2 !. 6 !X 8", 0b1010); // ((1 | 2) & 6) ^ 8

    E8(" 1 ^  2 &  6 |  8", 0b1011); // (1 ^ (2 & 6)) | 8
    E8(" 1 ^  2 & (6 |  8)",0b0011); // 1 ^ (2 & (6 | 8))
    E8("(1 ^  2)&  6 |  8", 0b1010); // ((1 ^ 2) & 6) | 8
    E8(" 1 !X 2 !. 6 !+ 8", 0b1010); // ((1 ^ 2) & 6) | 8

    E8(" 1 |  2 ^  4 &  12", 0b0111); // 1 | (2 ^ (4 & 12))
    E8("(1 |  2)^  4 &  12", 0b0111); // (1 | 2) ^ (4 & 12)
    E8(" 1 | (2 ^  4)&  12", 0b0101); // 1 | ((2 ^ 4) & 12)
    E8(" 1 !+ 2 !X 4 !. 12", 0b0100); // ((1 | 2) ^ 4) & 12

    E8(" 1 ^  2 |  4 &  12", 0b0111); // (1 ^ 2) | (4 & 12)
    E8(" 1 ^ (2 |  4)&  12", 0b0101); // 1 ^ ((2 | 4) & 12)
    E8("(1 ^  2)|  4 &  12", 0b0111); // (1 ^ 2) | (4 & 12)
    E8(" 1 !X 2 !+ 4 !. 12", 0b0100); // ((1 ^ 2) | 4) & 12

    E8("1 &  3 ^  4 |  8",  0b1101); // ((1 & 3) ^ 4) | 8
    E8("1 &  3 ^ (4 |  8)", 0b1101); // (1 & 3) ^ (4 | 8)
    E8("1 & (3 ^  4)|  8",  0b1001); // (1 & (3 ^ 4)) | 8
    E8("1 !. 3 !X 4 !+ 8",  0b1101); // ((1 & 3) ^ 4) | 8

    E8("1 &  3 |  4 ^  8",  0b1101); // (1 & 3) | (4 ^ 8)
    E8("1 &  3 | (4 ^  8)", 0b1101); // (1 & 3) | (4 ^ 8)
    E8("1 & (3 |  4)^  8",  0b1001); // (1 & (3 | 4)) ^ 8
    E8("1 !. 3 !+ 4 !X 8",  0b1101); // ((1 & 3) | 4) ^8

    // Bitwise, shift and rotate operators have the same precedence

    E16(" 1 |  2 << 3", 17); // 1 | (2 << 3)
    E16("(1 |  2)<< 3", 24); // (1 | 2) << 3
    E16(" 1 !+ 2 !< 3", 24); // (1 | 2) << 3
    E16(" 3 &  7 << 1",  2); // 3 & (7 << 1)
    E16("(3 &  7)<< 1",  6); // (3 & 7) << 1)
    E16(" 3 !. 7 !< 1",  6); // (3 & 7) << 1)
    E16(" 1 ^  3 << 3", 25); // 1 ^ (3 << 3)
    E16("(1 ^  3)<< 3", 16); // (1 ^ 3) << 3
    E16(" 1 !X 3 !< 3", 16); // (1 ^ 3) << 3

    E32("1 << 3 <  8",  0); // (1<<3) <  8
    E32("1 <<(3 <  8)", 2);
    E32("1 << 3 == 0",  0); // (1<<3) == 3
    E32("1 <<(3 == 3)", 2);

    E32("0 + 2 < 3 + 0", 1);
    E32("0 > 2 != 1",  1); // (0 > 2) != 1
    E32("0 >(2 != 1)", 0);
    E32(" 0 == 0 <= 1", 0); // 0 == (0 <= 1)
    E32("(0 == 0)<= 1", 1);
    X32(" 0 < 2 <= 3",  OPERATOR_NOT_ASSOCIATIVE, "<= 3");
    E32("(0 < 2)<= 3",  1);
    E32(" 0 <(2 <= 3)", 1);
    X32(" 0 == 2 != 3",  OPERATOR_NOT_ASSOCIATIVE, "!= 3");
    E32(" 0 ==(2 != 3)", 0);
    E32("(0 == 2)!= 3",  1);
    X32(" 0 ==(2 != 3)!= 0", OPERATOR_NOT_ASSOCIATIVE, "!= 0");
    X32("(0 == 2)!= 3 == 1", OPERATOR_NOT_ASSOCIATIVE, "== 1");

    E32("10==10 && 20==20", 1);
    E32("10==20 && 10!=20", 0);
    E32("10>=10 && 20<=10", 0);
    E32("10> 20 && 20< 10", 0);
    E32("10==10 || 20==20", 1);
    E32("10==20 || 10!=20", 1);
    E32("10>=10 || 20<=10", 1);
    E32("10> 20 || 20< 10", 0);

    E32(" 0 && 1 || 1",  1);
    E32(" 0 &&(1 || 1)", 0);
    E32(" 1 || 1 && 0",  1);
    E32("(1 || 1)&& 0",  0);
}

static void test_current_address() {
    parser.setCurrentOrigin(0x1000);
    E16("*",       0x1000);
    E16("*+2",     0x1002);
    E16("*-2",     0x0FFE);
    X16("*+$F000", OVERFLOW_RANGE, "*+$F000");
    E16("*-$1001", 0xFFFF);
    E32("*+$F000", 0x00010000);
    E32("*-$1001", 0xFFFFFFFF);

    symtab.intern(0x1000, "table");
    parser.setCurrentOrigin(0x1100);
    E16("*-table",     0x100);
    E16("(*-table)/2", 0x080);
}

static void test_scan() {
    SCAN('|', "|a+b|c+d",     "");
    SCAN('|', "a+b|c+d",      "a+b");
    SCAN('|', "a+(b|c)+d|e",  "a+(b|c)+d");
    SCAN('|', "a+'|'+d|e",    "a+'|'+d");
    SCAN(',', "|a+b|c+d",     "");
    SCAN(',', "','+'\'',abc", "','+'\''");
    SCAN('x', "0x1230xG",     "0x1230");
    SCAN('b', "0b1010b0",     "0b1010");
    SCAN('$', "$1230$X230",   "$1230");
    SCAN('@', "@1230@8230",   "@1230");
    SCAN('%', "%1010%2010",   "%1010");
}

static void test_errors() {
    X32("$bcdefg", GARBAGE_AT_END, "g");
    X32("@345678", GARBAGE_AT_END, "8");
    X32("%101012", GARBAGE_AT_END, "2");
    X32("456789a", GARBAGE_AT_END, "a");
}

static void test_formatter_8bit() {
    DEC(0,     8, "0");
    DEC(32,    8, "32");
    DEC(128,   8, "128");
    DEC(255,   8, "255");
    DEC(256,   8, "0");
    DEC(-32,   8, "224");
    DEC(-128,  8, "128");
    DEC(-255,  8, "1");
    DEC(-256,  8, "0");

    DEC(0,    -8, "0");
    DEC(32,   -8, "32");
    DEC(128,  -8, "-128");
    DEC(255,  -8, "-1");
    DEC(256,  -8, "0");
    DEC(-32,  -8, "-32");
    DEC(-128, -8, "-128");
    DEC(-255, -8, "1");
    DEC(-256, -8, "0");

    DEC(0,     8, "0");
    DEC(32,    8, "32");
    DEC(128,   8, "128");
    DEC(255,   8, "255");
    DEC(256,   8, "0");
    DEC(-32,   8, "224");
    DEC(-128,  8, "128");
    DEC(-255,  8, "1");
    DEC(-256,  8, "0");

    DEC(0,    -8, "0");
    DEC(32,   -8, "32");
    DEC(128,  -8, "-128");
    DEC(255,  -8, "-1");
    DEC(256,  -8, "0");
    DEC(-32,  -8, "-32");
    DEC(-128, -8, "-128");
    DEC(-255, -8, "1");
    DEC(-256, -8, "0");

    HEX(0,    8, true, "0");
    HEX(32,   8, true, "32");
    HEX(33,   8, true, "$21");
    HEX(128,  8, true, "$80");
    HEX(255,  8, true, "$ff");
    HEX(256,  8, true, "0");
    HEX(-32,  8, true, "$e0");
    HEX(-33,  8, true, "$df");
    HEX(-128, 8, true, "$80");
    HEX(-255, 8, true, "1");
    HEX(-256, 8, true, "0");

    HEX(0,   -8, true, "0");
    HEX(32,  -8, true, "32");
    HEX(33,  -8, true, "$21");
    HEX(128, -8, true, "-$80");
    HEX(255, -8, true, "-1");
    HEX(256, -8, true, "0");
    HEX(-32, -8, true, "-32");
    HEX(-33, -8, true, "-$21");
    HEX(-128,-8, true, "-$80");
    HEX(-255,-8, true, "1");
    HEX(-256,-8, true, "0");

    HEX(0,    8, false, "$00");
    HEX(32,   8, false, "$20");
    HEX(128,  8, false, "$80");
    HEX(255,  8, false, "$ff");
    HEX(256,  8, false, "$00");
    HEX(-32,  8, false, "$e0");
    HEX(-128, 8, false, "$80");
    HEX(-255, 8, false, "$01");
    HEX(-256, 8, false, "$00");

    HEX(0,   -8, false, "$00");
    HEX(32,  -8, false, "$20");
    HEX(128, -8, false, "-$80");
    HEX(255, -8, false, "-$01");
    HEX(256, -8, false, "$00");
    HEX(-32, -8, false, "-$20");
    HEX(-128,-8, false, "-$80");
    HEX(-255,-8, false, "$01");
    HEX(-256,-8, false, "$00");
}

static void test_formatter_16bit() {
    DEC(0,         16, "0");
    DEC(32,        16, "32");
    DEC(0x8000,    16, "32768");
    DEC(0xffff,    16, "65535");
    DEC(0x10000,   16, "0");
    DEC(-32,       16, "65504");
    DEC(-0x8000,   16, "32768");
    DEC(-0xffff,   16, "1");
    DEC(-0x10000,  16, "0");

    DEC(0,        -16, "0");
    DEC(32,       -16, "32");
    DEC(0x8000,   -16, "-32768");
    DEC(0xffff,   -16, "-1");
    DEC(0x10000,  -16, "0");
    DEC(-32,      -16, "-32");
    DEC(-0x8000,  -16, "-32768");
    DEC(-0xffff,  -16, "1");
    DEC(-0x10000, -16, "0");

    DEC(0,         16, "0");
    DEC(32,        16, "32");
    DEC(0x8000,    16, "32768");
    DEC(0xffff,    16, "65535");
    DEC(0x10000,   16, "0");
    DEC(-32,       16, "65504");
    DEC(-0x8000,   16, "32768");
    DEC(-0xffff,   16, "1");
    DEC(-0x10000,  16, "0");

    DEC(0,        -16, "0");
    DEC(32,       -16, "32");
    DEC(0x8000,   -16, "-32768");
    DEC(0xffff,   -16, "-1");
    DEC(0x10000,  -16, "0");
    DEC(-32,      -16, "-32");
    DEC(-0x8000,  -16, "-32768");
    DEC(-0xffff,  -16, "1");
    DEC(-0x10000, -16, "0");

    HEX(0,         16, true, "0");
    HEX(32,        16, true, "32");
    HEX(33,        16, true, "$0021");
    HEX(0x8000,    16, true, "$8000");
    HEX(0xffff,    16, true, "$ffff");
    HEX(0x10000,   16, true, "0");
    HEX(-32,       16, true, "$ffe0");
    HEX(-33,       16, true, "$ffdf");
    HEX(-0x8000,   16, true, "$8000");
    HEX(-0xffff,   16, true, "1");
    HEX(-0x10000,  16, true, "0");

    HEX(0,        -16, true, "0");
    HEX(32,       -16, true, "32");
    HEX(33,       -16, true, "$0021");
    HEX(0x8000,   -16, true, "-$8000");
    HEX(0xffff,   -16, true, "-1");
    HEX(0x10000,  -16, true, "0");
    HEX(-32,      -16, true, "-32");
    HEX(-33,      -16, true, "-$0021");
    HEX(-0x8000,  -16, true, "-$8000");
    HEX(-0xffff,  -16, true, "1");
    HEX(-0x10000, -16, true, "0");

    HEX(0,         16, false, "$0000");
    HEX(32,        16, false, "$0020");
    HEX(0x8000,    16, false, "$8000");
    HEX(0xffff,    16, false, "$ffff");
    HEX(0x10000,   16, false, "$0000");
    HEX(-32,       16, false, "$ffe0");
    HEX(-0x8000,   16, false, "$8000");
    HEX(-0xffff,   16, false, "$0001");
    HEX(-0x10000,  16, false, "$0000");

    HEX(0,        -16, false, "$0000");
    HEX(32,       -16, false, "$0020");
    HEX(0x8000,   -16, false, "-$8000");
    HEX(0xffff,   -16, false, "-$0001");
    HEX(0x10000,  -16, false, "$0000");
    HEX(-32,      -16, false, "-$0020");
    HEX(-0x8000,  -16, false, "-$8000");
    HEX(-0xffff,  -16, false, "$0001");
    HEX(-0x10000, -16, false, "$0000");
}

static void test_formatter_24bit() {
    DEC(0,           24, "0");
    DEC(32,          24, "32");
    DEC(0x800000,    24, "8388608");
    DEC(0xffffff,    24, "16777215");
    DEC(0x1000000,   24, "0");
    DEC(-32,         24, "16777184");
    DEC(-0x800000,   24, "8388608");
    DEC(-0xffffff,   24, "1");
    DEC(-0x1000000,  24, "0");

    DEC(0,          -24, "0");
    DEC(32,         -24, "32");
    DEC(0x800000,   -24, "-8388608");
    DEC(0xffffff,   -24, "-1");
    DEC(0x1000000,  -24, "0");
    DEC(-32,        -24, "-32");
    DEC(-0x800000,  -24, "-8388608");
    DEC(-0xffffff,  -24, "1");
    DEC(-0x1000000, -24, "0");

    DEC(0,           24, "0");
    DEC(32,          24, "32");
    DEC(0x800000,    24, "8388608");
    DEC(0xffffff,    24, "16777215");
    DEC(0x1000000,   24, "0");
    DEC(-32,         24, "16777184");
    DEC(-0x800000,   24, "8388608");
    DEC(-0xffffff,   24, "1");
    DEC(-0x1000000,  24, "0");

    DEC(0,          -24, "0");
    DEC(32,         -24, "32");
    DEC(0x800000,   -24, "-8388608");
    DEC(0xffffff,   -24, "-1");
    DEC(0x1000000,  -24, "0");
    DEC(-32,        -24, "-32");
    DEC(-0x800000,  -24, "-8388608");
    DEC(-0xffffff,  -24, "1");
    DEC(-0x1000000, -24, "0");

    HEX(0,           24, true, "0");
    HEX(32,          24, true, "32");
    HEX(33,          24, true, "$000021");
    HEX(0x800000,    24, true, "$800000");
    HEX(0xffffff,    24, true, "$ffffff");
    HEX(0x1000000,   24, true, "0");
    HEX(-32,         24, true, "$ffffe0");
    HEX(-33,         24, true, "$ffffdf");
    HEX(-0x800000,   24, true, "$800000");
    HEX(-0xffffff,   24, true, "1");
    HEX(-0x1000000,  24, true, "0");

    HEX(0,          -24, true, "0");
    HEX(32,         -24, true, "32");
    HEX(33,         -24, true, "$000021");
    HEX(0x800000,   -24, true, "-$800000");
    HEX(0xffffff,   -24, true, "-1");
    HEX(0x1000000,  -24, true, "0");
    HEX(-32,        -24, true, "-32");
    HEX(-33,        -24, true, "-$000021");
    HEX(-0x800000,  -24, true, "-$800000");
    HEX(-0xffffff,  -24, true, "1");
    HEX(-0x1000000, -24, true, "0");

    HEX(0,           24, false, "$000000");
    HEX(32,          24, false, "$000020");
    HEX(0x800000,    24, false, "$800000");
    HEX(0xffffff,    24, false, "$ffffff");
    HEX(0x1000000,   24, false, "$000000");
    HEX(-32,         24, false, "$ffffe0");
    HEX(-0x800000,   24, false, "$800000");
    HEX(-0xffffff,   24, false, "$000001");
    HEX(-0x1000000,  24, false, "$000000");

    HEX(0,          -24, false, "$000000");
    HEX(32,         -24, false, "$000020");
    HEX(0x800000,   -24, false, "-$800000");
    HEX(0xffffff,   -24, false, "-$000001");
    HEX(0x1000000,  -24, false, "$000000");
    HEX(-32,        -24, false, "-$000020");
    HEX(-0x800000,  -24, false, "-$800000");
    HEX(-0xffffff,  -24, false, "$000001");
    HEX(-0x1000000, -24, false, "$000000");
}

static void test_formatter_32bit() {
    DEC(0,            32, "0");
    DEC(32,           32, "32");
    DEC(0x80000000,   32, "2147483648");
    DEC(0xffffffff,   32, "4294967295");
    DEC(-32,          32, "4294967264");
    DEC(-0x80000000,  32, "2147483648");
    DEC(-0xffffffff,  32, "1");

    DEC(0,           -32, "0");
    DEC(32,          -32, "32");
    DEC(0x80000000,  -32, "-2147483648");
    DEC(0xffffffff,  -32, "-1");
    DEC(-32,         -32, "-32");
    DEC(-0x80000000, -32, "-2147483648");
    DEC(-0xffffffff, -32, "1");

    DEC(0,            32, "0");
    DEC(32,           32, "32");
    DEC(0x80000000,   32, "2147483648");
    DEC(0xffffffff,   32, "4294967295");
    DEC(-32,          32, "4294967264");
    DEC(-0x80000000,  32, "2147483648");
    DEC(-0xffffffff,  32, "1");

    DEC(0,           -32, "0");
    DEC(32,          -32, "32");
    DEC(0x80000000,  -32, "-2147483648");
    DEC(0xffffffff,  -32, "-1");
    DEC(-32,         -32, "-32");
    DEC(-0x80000000, -32, "-2147483648");
    DEC(-0xffffffff, -32, "1");

    HEX(0,           32, true, "0");
    HEX(32,          32, true, "32");
    HEX(33,          32, true, "$00000021");
    HEX(0x80000000,  32, true, "$80000000");
    HEX(0xffffffff,  32, true, "$ffffffff");
    HEX(-32,         32, true, "$ffffffe0");
    HEX(-33,         32, true, "$ffffffdf");
    HEX(-0x80000000, 32, true, "$80000000");
    HEX(-0xffffffff, 32, true, "1");

    HEX(0,           -32, true, "0");
    HEX(32,          -32, true, "32");
    HEX(33,          -32, true, "$00000021");
    HEX(0x80000000,  -32, true, "-$80000000");
    HEX(0xffffffff,  -32, true, "-1");
    HEX(-32,         -32, true, "-32");
    HEX(-33,         -32, true, "-$00000021");
    HEX(-0x80000000, -32, true, "-$80000000");
    HEX(-0xffffffff, -32, true, "1");

    HEX(0,           32, false, "$00000000");
    HEX(32,          32, false, "$00000020");
    HEX(0x80000000,  32, false, "$80000000");
    HEX(0xffffffff,  32, false, "$ffffffff");
    HEX(-32,         32, false, "$ffffffe0");
    HEX(-0x80000000, 32, false, "$80000000");
    HEX(-0xffffffff, 32, false, "$00000001");

    HEX(0,           -32, false, "$00000000");
    HEX(32,          -32, false, "$00000020");
    HEX(0x80000000,  -32, false, "-$80000000");
    HEX(0xffffffff,  -32, false, "-$00000001");
    HEX(-32,         -32, false, "-$00000020");
    HEX(-0x80000000, -32, false, "-$80000000");
    HEX(-0xffffffff, -32, false, "$00000001");
}

static void test_formatter_cstyle() {
    formatter.setCStyle(true);

    HEX(0,    8, false, "0x00");
    HEX(32,   8, false, "0x20");
    HEX(128,  8, false, "0x80");
    HEX(255,  8, false, "0xff");
    HEX(256,  8, false, "0x00");
    HEX(-32,  8, false, "0xe0");
    HEX(-128, 8, false, "0x80");
    HEX(-255, 8, false, "0x01");
    HEX(-256, 8, false, "0x00");

    HEX(0,         16, false, "0x0000");
    HEX(32,        16, false, "0x0020");
    HEX(0x8000,    16, false, "0x8000");
    HEX(0xffff,    16, false, "0xffff");
    HEX(0x10000,   16, false, "0x0000");
    HEX(-32,       16, false, "0xffe0");
    HEX(-0x8000,   16, false, "0x8000");
    HEX(-0xffff,   16, false, "0x0001");
    HEX(-0x10000,  16, false, "0x0000");

    HEX(0,           24, false, "0x000000");
    HEX(32,          24, false, "0x000020");
    HEX(0x800000,    24, false, "0x800000");
    HEX(0xffffff,    24, false, "0xffffff");
    HEX(0x1000000,   24, false, "0x000000");
    HEX(-32,         24, false, "0xffffe0");
    HEX(-0x800000,   24, false, "0x800000");
    HEX(-0xffffff,   24, false, "0x000001");
    HEX(-0x1000000,  24, false, "0x000000");

    HEX(0,           32, false, "0x00000000");
    HEX(32,          32, false, "0x00000020");
    HEX(0x80000000,  32, false, "0x80000000");
    HEX(0xffffffff,  32, false, "0xffffffff");
    HEX(-32,         32, false, "0xffffffe0");
    HEX(-0x80000000, 32, false, "0x80000000");
    HEX(-0xffffffff, 32, false, "0x00000001");

    HEX(-128 *2, 9, false, "0x100");
    HEX(   0 *2, 9, false, "0x000");
    HEX(+127 *2, 9, false, "0x0fe");

    HEX(-2048 *2, 13, false, "0x1000");
    HEX(    0 *2, 13, false, "0x0000");
    HEX(+2047 *2, 13, false, "0x0ffe");
}

// clang-format on

void run_tests() {
    RUN_TEST(test_char_constant);
    RUN_TEST(test_char_closing);
    RUN_TEST(test_dec_constant);
    RUN_TEST(test_hex_constant);
    RUN_TEST(test_oct_constant);
    RUN_TEST(test_bin_constant);
    RUN_TEST(test_unary_operator);
    RUN_TEST(test_binary_operator);
    RUN_TEST(test_precedence);
    RUN_TEST(test_current_address);
    RUN_TEST(test_scan);
    RUN_TEST(test_errors);
    RUN_TEST(test_formatter_8bit);
    RUN_TEST(test_formatter_16bit);
    RUN_TEST(test_formatter_24bit);
    RUN_TEST(test_formatter_32bit);
    RUN_TEST(test_formatter_cstyle);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
