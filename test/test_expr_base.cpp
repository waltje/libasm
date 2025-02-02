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

const ValueParser::Plugins plugins{};
struct : ValueParser::Locator {
    uint32_t location = 0;
    uint32_t currentLocation() const { return location; }
} locator;
const ValueParser parser{plugins, locator};

const CStyleHexFormatter hexFormatter;
const ValueFormatter formatter{hexFormatter};

static void set_up() {}

static void tear_down() {
    symtab.reset();
}

// clang-format off
static void test_char_constant() {
    E8("'a'",   0x61);
    E8("'a'+5", 0x66);
    E8("5+'a'", 0x66);
    X8("'a",    MISSING_CLOSING_QUOTE, "'a");
    X8("'a+5",  MISSING_CLOSING_QUOTE, "'a+5");
    X8("5+'a",  MISSING_CLOSING_QUOTE, "'a");
    X8("' ",    MISSING_CLOSING_QUOTE, "' ");
    X8("''",    MISSING_CLOSING_QUOTE, "''");
    X8("'''",   MISSING_CLOSING_QUOTE, "'''");
    E8("''''",  0x27);

    E16("'a'", 0x61);
    E32("'a'", 0x61);
}

static void test_dec_constant() {
    E8("0",   0x00);
    E8("127", 0x7f);
    E8("128", 0x80);
    E8("255", 0xff);
    X8("256", OVERFLOW_RANGE, "256");

    E16("0",     0x0000);
    E16("32767", 0x7fff);
    E16("32768", 0x8000);
    E16("65535", 0xffff);
    X16("65536", OVERFLOW_RANGE, "65536");

    E32("0",          0x00000000);
    E32("2147483647", 0x7fffffff);
    E32("2147483648", 0x80000000);
    E32("4294967295", 0xffffffff);
    X32("4294967296", OVERFLOW_RANGE, "4294967296");
    X32("9999999999", OVERFLOW_RANGE, "9999999999");
}

static void test_hex_constant() {
    E8("0x0",   0x00);
    E8("0x7f",  0x7f);
    E8("0x80",  0x80);
    E8("0xFF",  0xff);
    X8("0x100", OVERFLOW_RANGE, "0x100");

    E16("0x0",     0x0000);
    E16("0x7fff",  0x7fff);
    E16("0x8000",  0x8000);
    E16("0xffff",  0xffff);
    X16("0x10000", OVERFLOW_RANGE, "0x10000");

    E32("0x0",         0x00000000);
    E32("0x7FFFFFFF",  0x7fffffff);
    E32("0x80000000",  0x80000000);
    E32("0xFFFFffff",  0xffffffff);
    X32("0x100000000", OVERFLOW_RANGE, "0x100000000");
    X32("0x7FFFFFFFF", OVERFLOW_RANGE, "0x7FFFFFFFF");
}

static void test_oct_constant() {
    E8("0177", 0x7f);
    E8("0200", 0x80);
    E8("0377", 0xff);
    X8("0400", OVERFLOW_RANGE, "0400");

    E16("00",      0x0000);
    E16("0077777", 0x7fff);
    E16("0100000", 0x8000);
    E16("0177777", 0xffff);
    X16("0200000", OVERFLOW_RANGE, "0200000");

    E32("00",           0x00000000);
    E32("017777777777", 0x7fffffff);
    E32("020000000000", 0x80000000);
    E32("037777777777", 0xffffffff);
    X32("040000000000", OVERFLOW_RANGE, "040000000000");
    X32("047777777777", OVERFLOW_RANGE, "047777777777");
}

static void test_bin_constant() {
    E8("0b0",         0x00);
    E8("0b01111111",  0x7f);
    E8("0b10000000",  0x80);
    E8("0b11111111",  0xff);
    X8("0b100000000", OVERFLOW_RANGE, "0b100000000");

    E16("0b0",                 0x0000);
    E16("0b0111111111111111",  0x7fff);
    E16("0b1000000000000000",  0x8000);
    E16("0b1111111111111111",  0xffff);
    X16("0b10000000000000000", OVERFLOW_RANGE, "0b10000000000000000");

    E32("0b0",                                 0x00000000);
    E32("0b01111111111111111111111111111111",  0x7fffffff);
    E32("0b10000000000000000000000000000000",  0x80000000);
    E32("0b11111111111111111111111111111111",  0xffffffff);
    X32("0b100000000000000000000000000000000", OVERFLOW_RANGE, "0b100000000000000000000000000000000");
    X32("0b111111111111111111111111111111111", OVERFLOW_RANGE, "0b111111111111111111111111111111111");
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

    E8(" 1|2 & 6^8", 11); // 1 | ((2 & 6) ^8)
    E8("(1|2)& 6^8", 10);
    E8(" 1|2 &(6^8)", 3);

    E8(" 1^2 & 6|8", 11); // (1 ^ (2 & 6)) | 8
    E8(" 1^2 &(6|8)", 3);
    E8("(1^2)& 6|8", 10);

    E8(" 1| 2 ^4 &12", 7); // 1 | (2 ^ (4 & 12))
    E8("(1| 2)^4 &12", 7);
    E8(" 1|(2 ^4)&12", 5);

    E8(" 1^(2 |4)&12", 5);
    E8(" 1^ 2 |4 &12", 7); // (1 ^ 2) | (4 & 12)
    E8("(1^ 2)|4 &12", 7);

    E8("1& 3^ 4 |8",  13); // ((1 & 3) ^ 4) | 8
    E8("1& 3^(4 |8)", 13);
    E8("1&(3^ 4)|8",   9);

    E8("1& 3| 4 ^8",  13); // (1 & 3) | (4 ^ 8)
    E8("1& 3|(4 ^8)", 13);
    E8("1&(3| 4)^8",   9);

    E8(" 1|2 <<3", 17); // 1 | (2<<3)
    E8("(1|2)<<3", 24);
    E8(" 3&7 <<1",  2); // 3 & (7<<1)
    E8("(3&7)<<1",  6);
    E8(" 1^3 <<3", 25); // 1 ^ (3<<3)
    E8("(1^3)<<3", 16);

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

static void test_overflow() {
    E8("0x1000-0x1080", -128);
    X8("0x1000-0x1081", OVERFLOW_RANGE, "0x1000-0x1081");
    E8("0x10ff-0x1000", 255);
    X8("0x1100-0x1000", OVERFLOW_RANGE, "0x1100-0x1000");
    E8("0xE000-0xE080", -128);
    X8("0xE000-0xE081", OVERFLOW_RANGE, "0xE000-0xE081");
    E8("0xE0ff-0xE000", 255);
    X8("0xE100-0xE000", OVERFLOW_RANGE, "0xE100-0xE000");

    E16("0xE000-0x6000", 0x8000);
    E16("0xE000-0x5fff", 0x8001);
    E16("0xE000-0xE001", 0xFFFF);
    E16("0x2000-0xA000", -32768);
    X16("0x2000-0xA001", OVERFLOW_RANGE, "0x2000-0xA001");
    E16("0x3000-0x3001", -1);

    E16("0xE000+(-0x2000)", 0xC000);
    E16("(-0x2000)+0xE000", 0xC000);
}

static void test_current_address() {
    locator.location = 0x1000;
    E16("$",       0x1000);
    E16("$+2",     0x1002);
    E16("$-2",     0x0FFE);
    X16("$+0xF000", OVERFLOW_RANGE, "$+0xF000");
    E16("$-0x1001", 0xFFFF);
    E32("$+0xF000", 0x00010000);
    E32("$-0x1001", 0xFFFFFFFF);

    symtab.intern(0x1000, "table");
    locator.location = 0x1100;
    E16("$-table",     0x100);
    E16("($-table)/2", 0x080);
}

static void test_function() {
    // clang-format on
    static const struct : Functor {
        int8_t nargs() const override { return 0; }
        Error eval(ValueStack &stack, uint8_t argc) const override {
            stack.pushUnsigned(0x31415926);
            return OK;
        }
    } FN_PI;
    static const struct : Functor {
        int8_t nargs() const override { return 2; }
        Error eval(ValueStack &stack, uint8_t argc) const override {
            const auto rhs = stack.pop().getSigned();
            const auto lhs = stack.pop().getSigned();
            stack.pushSigned(lhs - rhs);
            return OK;
        }
    } FN_SUB;
    static const struct : Functor {
        Error eval(ValueStack &stack, uint8_t argc) const override {
            int32_t sum = 0;
            while (argc--)
                sum += stack.pop().getSigned();
            stack.pushSigned(sum);
            return OK;
        }
    } FN_SUM;
    symtab.internFunction(&FN_PI, "pi");
    symtab.internFunction(&FN_SUB, "sub");
    symtab.internFunction(&FN_SUM, "sum");
    // clang-format off

    X16("hi (0x1234)", UNDEFINED_SYMBOL, "hi (0x1234)");
    E32("pi()",        0x31415926);
    E32("pi ( ) + 1",  0x31415927);
    X32("pi + 1",      MISSING_FUNC_ARGUMENT,  "pi + 1");
    X32("pi(1) + 1",   TOO_MANY_FUNC_ARGUMENT, "pi(1) + 1");
    E16("1 + sub(2, 10)",     -7);
    X16("1 + sub( 1 )",       TOO_FEW_FUNC_ARGUMENT,  "sub( 1 )");
    X16("1 + sub (1, 2, 3 )", TOO_MANY_FUNC_ARGUMENT, "sub (1, 2, 3 )");
    X16("1 + sub( 1, 2, 3",   MISSING_CLOSING_PAREN,  "( 1, 2, 3");
    E16("sum()",                  0);
    E16("sum(1)",                 1);
    E16("sum(1, -2)",            -1);
    E16("sum(1, -2, 3)",          2);
    E16("sum(1, -2, 3, -4)",     -2);
    E16("sum(1, -2, 3, -4, 5)",   3);
    E16("sum(1,2,3,4,5,6,7,8)",   36);
    X16("sum(1,2,3,4,5,6,7,8,9)", TOO_COMPLEX_EXPRESSION, "9)");
    E32("1+sum(1+sub(1,2)+2,3)",  6);
    SCAN(',', "1+sum(1+sub(1,2)+2,3),x", "1+sum(1+sub(1,2)+2,3)");

    X32("2*", MISSING_OPERAND, "*");
    X32("-",  MISSING_OPERAND, "-");
}

static void test_scan() {
    SCAN('|', "|a+b|c+d",     "");
    SCAN('|', "a+b|c+d",      "a+b");
    SCAN('|', "a+(b|c)+d|e",  "a+(b|c)+d");
    SCAN('|', "a+'|'+d|e",    "a+'|'+d");
    SCAN(',', "|a+b|c+d",     "");
    SCAN(',', "','+'''',abc", "','+''''");
    SCAN('x', "0x1230xG",     "0x1230");
    SCAN('b', "0b1010b0",     "0b1010");
}

static void test_errors() {
    X32("undef",   UNDEFINED_SYMBOL, "undef");
    X32("+undef",  UNDEFINED_SYMBOL, "undef");
    X32("-undef",  UNDEFINED_SYMBOL, "undef");
    X32("~undef",  UNDEFINED_SYMBOL, "undef");
    X32("(undef)", UNDEFINED_SYMBOL, "undef)");
    X32("1-undef", UNDEFINED_SYMBOL, "undef");
    X32("undef+1", UNDEFINED_SYMBOL, "undef+1");
    X32("0xcdefg", GARBAGE_AT_END, "g");
    X32("0345678", GARBAGE_AT_END, "8");
    X32("0b10102", GARBAGE_AT_END, "2");
    X32("456789A", GARBAGE_AT_END, "A");
    X32("2*(1+3",  MISSING_CLOSING_PAREN, "(1+3");
    X32("2*(1+3]", MISSING_CLOSING_PAREN, "(1+3]");
    E32(" ((((((((0))))))))",  0);
    X32("(((((((((0)))))))))", TOO_COMPLEX_EXPRESSION, "(0)))))))))");
    E32("   1+(2+(3+(4+5)))",  15);
    X32("0+(1+(2+(3+(4+5))))", TOO_COMPLEX_EXPRESSION, "+5))))");
    E32("8 | 7 ^ 3 & 4 >> 1",         13);
    E32("8 | 7 ^ 3 & 4 >> 1 + 1",     14);
    E32("8 | 7 ^ 3 & 4 >> 1 + 1 * 2", 15);
    X32("'a",  MISSING_CLOSING_QUOTE, "'a");
    E32("--1", 1);
    E32("-+1", -1);
    E32("+-1", 0xFFFFFFFF);
    E32("++1", 1);
    X32("100/0", DIVIDE_BY_ZERO, "/0");
    X32("100%0", DIVIDE_BY_ZERO, "%0");
}

static void test_spaces() {
    E32(" - 1 ", -1);
    E32(" + 1 ",  1);
    E32(" ~ 0 ", -1);
    E32(" 1 + 2 ; + 3 ", 3);
    E32(" ( 1 + 2 ) * 3 ", 9);
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

    HEX(0,    8, "0x00");
    HEX(32,   8, "0x20");
    HEX(128,  8, "0x80");
    HEX(255,  8, "0xff");
    HEX(256,  8, "0x00");
    HEX(-32,  8, "0xe0");
    HEX(-128, 8, "0x80");
    HEX(-255, 8, "0x01");
    HEX(-256, 8, "0x00");

    HEX(0,    -8, "0x00");
    HEX(32,   -8, "0x20");
    HEX(128,  -8, "-0x80");
    HEX(255,  -8, "-0x01");
    HEX(256,  -8, "0x00");
    HEX(-32,  -8, "-0x20");
    HEX(-128, -8, "-0x80");
    HEX(-255, -8, "0x01");
    HEX(-256, -8, "0x00");

    // Relax
    RHEX(0,    8, "0");
    RHEX(32,   8, "32");
    RHEX(33,   8, "0x21");
    RHEX(128,  8, "0x80");
    RHEX(255,  8, "0xff");
    RHEX(256,  8, "0");
    RHEX(-32,  8, "0xe0");
    RHEX(-33,  8, "0xdf");
    RHEX(-128, 8, "0x80");
    RHEX(-255, 8, "1");
    RHEX(-256, 8, "0");

    RHEX(0,    -8, "0");
    RHEX(32,   -8, "32");
    RHEX(33,   -8, "0x21");
    RHEX(128,  -8, "-0x80");
    RHEX(255,  -8, "-1");
    RHEX(256,  -8, "0");
    RHEX(-32,  -8, "-32");
    RHEX(-33,  -8, "-0x21");
    RHEX(-128, -8, "-0x80");
    RHEX(-255, -8, "1");
    RHEX(-256, -8, "0");
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

    HEX(0,         16, "0x0000");
    HEX(32,        16, "0x0020");
    HEX(0x8000,    16, "0x8000");
    HEX(0xffff,    16, "0xffff");
    HEX(0x10000,   16, "0x0000");
    HEX(-32,       16, "0xffe0");
    HEX(-0x8000,   16, "0x8000");
    HEX(-0xffff,   16, "0x0001");
    HEX(-0x10000,  16, "0x0000");

    HEX(0,        -16, "0x0000");
    HEX(32,       -16, "0x0020");
    HEX(0x8000,   -16, "-0x8000");
    HEX(0xffff,   -16, "-0x0001");
    HEX(0x10000,  -16, "0x0000");
    HEX(-32,      -16, "-0x0020");
    HEX(-0x8000,  -16, "-0x8000");
    HEX(-0xffff,  -16, "0x0001");
    HEX(-0x10000, -16, "0x0000");

    // Relax

    RHEX(0,         16, "0");
    RHEX(32,        16, "32");
    RHEX(33,        16, "0x0021");
    RHEX(0x8000,    16, "0x8000");
    RHEX(0xffff,    16, "0xffff");
    RHEX(0x10000,   16, "0");
    RHEX(-32,       16, "0xffe0");
    RHEX(-33,       16, "0xffdf");
    RHEX(-0x8000,   16, "0x8000");
    RHEX(-0xffff,   16, "1");
    RHEX(-0x10000,  16, "0");

    RHEX(0,        -16, "0");
    RHEX(32,       -16, "32");
    RHEX(33,       -16, "0x0021");
    RHEX(0x8000,   -16, "-0x8000");
    RHEX(0xffff,   -16, "-1");
    RHEX(0x10000,  -16, "0");
    RHEX(-32,      -16, "-32");
    RHEX(-33,      -16, "-0x0021");
    RHEX(-0x8000,  -16, "-0x8000");
    RHEX(-0xffff,  -16, "1");
    RHEX(-0x10000, -16, "0");
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

    HEX(0,           24, "0x000000");
    HEX(32,          24, "0x000020");
    HEX(0x800000,    24, "0x800000");
    HEX(0xffffff,    24, "0xffffff");
    HEX(0x1000000,   24, "0x000000");
    HEX(-32,         24, "0xffffe0");
    HEX(-0x800000,   24, "0x800000");
    HEX(-0xffffff,   24, "0x000001");
    HEX(-0x1000000,  24, "0x000000");

    HEX(0,          -24, "0x000000");
    HEX(32,         -24, "0x000020");
    HEX(0x800000,   -24, "-0x800000");
    HEX(0xffffff,   -24, "-0x000001");
    HEX(0x1000000,  -24, "0x000000");
    HEX(-32,        -24, "-0x000020");
    HEX(-0x800000,  -24, "-0x800000");
    HEX(-0xffffff,  -24, "0x000001");
    HEX(-0x1000000, -24, "0x000000");

    // Relax

    RHEX(0,           24, "0");
    RHEX(32,          24, "32");
    RHEX(33,          24, "0x000021");
    RHEX(0x800000,    24, "0x800000");
    RHEX(0xffffff,    24, "0xffffff");
    RHEX(0x1000000,   24, "0");
    RHEX(-32,         24, "0xffffe0");
    RHEX(-33,         24, "0xffffdf");
    RHEX(-0x800000,   24, "0x800000");
    RHEX(-0xffffff,   24, "1");
    RHEX(-0x1000000,  24, "0");

    RHEX(0,          -24, "0");
    RHEX(32,         -24, "32");
    RHEX(33,         -24, "0x000021");
    RHEX(0x800000,   -24, "-0x800000");
    RHEX(0xffffff,   -24, "-1");
    RHEX(0x1000000,  -24, "0");
    RHEX(-32,        -24, "-32");
    RHEX(-33,        -24, "-0x000021");
    RHEX(-0x800000,  -24, "-0x800000");
    RHEX(-0xffffff,  -24, "1");
    RHEX(-0x1000000, -24, "0");
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

    HEX(0,           32, "0x00000000");
    HEX(32,          32, "0x00000020");
    HEX(0x80000000,  32, "0x80000000");
    HEX(0xffffffff,  32, "0xffffffff");
    HEX(-32,         32, "0xffffffe0");
    HEX(-0x80000000, 32, "0x80000000");
    HEX(-0xffffffff, 32, "0x00000001");

    HEX(0,           -32, "0x00000000");
    HEX(32,          -32, "0x00000020");
    HEX(0x80000000,  -32, "-0x80000000");
    HEX(0xffffffff,  -32, "-0x00000001");
    HEX(-32,         -32, "-0x00000020");
    HEX(-0x80000000, -32, "-0x80000000");
    HEX(-0xffffffff, -32, "0x00000001");

    // Relax

    RHEX(0,           32, "0");
    RHEX(32,          32, "32");
    RHEX(0x80000000,  32, "0x80000000");
    RHEX(0xffffffff,  32, "0xffffffff");
    RHEX(-32,         32, "0xffffffe0");
    RHEX(-0x80000000, 32, "0x80000000");
    RHEX(-0xffffffff, 32, "1");

    RHEX(0,           -32, "0");
    RHEX(32,          -32, "32");
    RHEX(0x80000000,  -32, "-0x80000000");
    RHEX(0xffffffff,  -32, "-1");
    RHEX(-32,         -32, "-32");
    RHEX(-0x80000000, -32, "-0x80000000");
    RHEX(-0xffffffff, -32, "1");

    // Uppercase

    UHEX(0,           32, "0x00000000");
    UHEX(32,          32, "0x00000020");
    UHEX(0x80000000,  32, "0x80000000");
    UHEX(0xffffffff,  32, "0xFFFFFFFF");
    UHEX(-32,         32, "0xFFFFFFE0");
    UHEX(-0x80000000, 32, "0x80000000");
    UHEX(-0xffffffff, 32, "0x00000001");

    UHEX(0,           -32, "0x00000000");
    UHEX(32,          -32, "0x00000020");
    UHEX(0x80000000,  -32, "-0x80000000");
    UHEX(0xffffffff,  -32, "-0x00000001");
    UHEX(-32,         -32, "-0x00000020");
    UHEX(-0x80000000, -32, "-0x80000000");
    UHEX(-0xffffffff, -32, "0x00000001");

    // Uppercase and Relax

    URHEX(0,           32, "0");
    URHEX(32,          32, "32");
    URHEX(33,          32, "0x00000021");
    URHEX(0x80000000,  32, "0x80000000");
    URHEX(0xffffffff,  32, "0xFFFFFFFF");
    URHEX(-32,         32, "0xFFFFFFE0");
    URHEX(-33,         32, "0xFFFFFFDF");
    URHEX(-0x80000000, 32, "0x80000000");
    URHEX(-0xffffffff, 32, "1");
}

static void test_formatter_9bit() {
    HEX(-128 *2, -9, "-0x100");
    HEX(   0 *2, -9, "0x000");
    HEX(+127 *2, -9, "0x0fe");
}

static void test_formatter_13bit() {
    HEX(-2048 *2, -13, "-0x1000");
    HEX(    0 *2, -13, "0x0000");
    HEX(+2047 *2, -13, "0x0ffe");
}

static void test_formatter_bare_hex() {
    const ValueFormatter formatter;

    HEX(0,    8, "00");
    HEX(32,   8, "20");
    HEX(128,  8, "80");
    HEX(255,  8, "ff");
    HEX(256,  8, "00");
    HEX(-32,  8, "e0");
    HEX(-128, 8, "80");
    HEX(-255, 8, "01");
    HEX(-256, 8, "00");

    HEX(0,         16, "0000");
    HEX(32,        16, "0020");
    HEX(0x8000,    16, "8000");
    HEX(0xffff,    16, "ffff");
    HEX(0x10000,   16, "0000");
    HEX(-32,       16, "ffe0");
    HEX(-0x8000,   16, "8000");
    HEX(-0xffff,   16, "0001");
    HEX(-0x10000,  16, "0000");

    HEX(0,           24, "000000");
    HEX(32,          24, "000020");
    HEX(0x800000,    24, "800000");
    HEX(0xffffff,    24, "ffffff");
    HEX(0x1000000,   24, "000000");
    HEX(-32,         24, "ffffe0");
    HEX(-0x800000,   24, "800000");
    HEX(-0xffffff,   24, "000001");
    HEX(-0x1000000,  24, "000000");

    HEX(0,           32, "00000000");
    HEX(32,          32, "00000020");
    HEX(0x80000000,  32, "80000000");
    HEX(0xffffffff,  32, "ffffffff");
    HEX(-32,         32, "ffffffe0");
    HEX(-0x80000000, 32, "80000000");
    HEX(-0xffffffff, 32, "00000001");

    // Uppercase

    UHEX(-128 *2, 9, "100");
    UHEX(   0 *2, 9, "000");
    UHEX(+127 *2, 9, "0FE");

    UHEX(-2048 *2, 13, "1000");
    UHEX(    0 *2, 13, "0000");
    UHEX(+2047 *2, 13, "0FFE");
}

// clang-format on

void run_tests() {
    RUN_TEST(test_char_constant);
    RUN_TEST(test_dec_constant);
    RUN_TEST(test_hex_constant);
    RUN_TEST(test_oct_constant);
    RUN_TEST(test_bin_constant);
    RUN_TEST(test_unary_operator);
    RUN_TEST(test_binary_operator);
    RUN_TEST(test_precedence);
    RUN_TEST(test_overflow);
    RUN_TEST(test_current_address);
    RUN_TEST(test_function);
    RUN_TEST(test_scan);
    RUN_TEST(test_errors);
    RUN_TEST(test_spaces);
    RUN_TEST(test_formatter_8bit);
    RUN_TEST(test_formatter_16bit);
    RUN_TEST(test_formatter_24bit);
    RUN_TEST(test_formatter_32bit);
    RUN_TEST(test_formatter_9bit);
    RUN_TEST(test_formatter_13bit);
    RUN_TEST(test_formatter_bare_hex);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
