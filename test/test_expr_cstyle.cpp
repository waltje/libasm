#include "test_expr_helper.h"

AsmOperand parser;
TestSymtab symtab;
TestAsserter asserter;

static void set_up() {
}

static void tear_down() {
    symtab.reset();
}

static void test_char_constant() {
    E8("'a'",    0x61, OK);
    E8("'\\''",  0x27, OK);
    E8("'\\\"'", 0x22, OK);
    E8("'\\?'",  0x3F, OK);
    E8("'\\\\'", 0x5C, OK);
    E8("'\\b'",  0x08, OK);
    E8("'\\t'",  0x09, OK);
    E8("'\\n'",  0x0A, OK);
    E8("'\\r'",  0x0D, OK);
    E8("'\\X0'", 0x00, OK);
    E8("'\\xfF'", 0xFF, OK);
    E8("'\\0'",  0x00, OK);
    E8("'\\377'", 0xFF, OK);

    E8("'\\x100'", 0,  OVERFLOW_RANGE);
    E8("'\\400'", 0,   OVERFLOW_RANGE);
    E8("''",     0,    MISSING_CLOSING_QUOTE);
    E8("'a",     0,    MISSING_CLOSING_QUOTE);
    E8("'\\z'",  0,    UNKNOWN_ESCAPE_SEQUENCE);

    E16("'a'", 0x61, OK);
    E32("'a'", 0x61, OK);
}

static void test_dec_constant() {
    E8("0",   0x00, OK);
    E8("127", 0x7f, OK);
    E8("128", 0x80, OK);
    E8("255", 0xff, OK);
    E8("256", 0,    OVERFLOW_RANGE);

    E16("0",     0x0000, OK);
    E16("32767", 0x7fff, OK);
    E16("32768", 0x8000, OK);
    E16("65535", 0xffff, OK);
    E16("65536", 0,      OVERFLOW_RANGE);

    E32("0",          0x00000000, OK);
    E32("2147483647", 0x7fffffff, OK);
    E32("2147483648", 0x80000000, OK);
    E32("4294967295", 0xffffffff, OK);
    E32("4294967296", 0,          OVERFLOW_RANGE);
}

static void test_hex_constant() {
    E8("0x0",   0x00, OK);
    E8("0x7f",  0x7f, OK);
    E8("0x80",  0x80, OK);
    E8("0xFF",  0xff, OK);
    E8("0x100", 0,    OVERFLOW_RANGE);

    E16("0x0",     0x0000, OK);
    E16("0x7fff",  0x7fff, OK);
    E16("0x8000",  0x8000, OK);
    E16("0xffff",  0xffff, OK);
    E16("0x10000", 0,      OVERFLOW_RANGE);

    E32("0x0",         0x00000000, OK);
    E32("0x7FFFFFFF",  0x7fffffff, OK);
    E32("0x80000000",  0x80000000, OK);
    E32("0xFFFFffff",  0xffffffff, OK);
    E32("0x100000000", 0,          OVERFLOW_RANGE);
}

static void test_oct_constant() {
    E8("0177", 0x7f, OK);
    E8("0200", 0x80, OK);
    E8("0377", 0xff, OK);
    E8("0400", 0,    OVERFLOW_RANGE);

    E16("00",      0x0000, OK);
    E16("0077777", 0x7fff, OK);
    E16("0100000", 0x8000, OK);
    E16("0177777", 0xffff, OK);
    E16("0200000", 0,      OVERFLOW_RANGE);

    E32("00",           0x00000000, OK);
    E32("017777777777", 0x7fffffff, OK);
    E32("020000000000", 0x80000000, OK);
    E32("037777777777", 0xffffffff, OK);
    E32("040000000000", 0,          OVERFLOW_RANGE);
}

static void test_bin_constant() {
    E8("0b0",         0x00, OK);
    E8("0b01111111",  0x7f, OK);
    E8("0b10000000",  0x80, OK);
    E8("0b11111111",  0xff, OK);
    E8("0b100000000", 0,    OVERFLOW_RANGE);

    E16("0b0",                 0x0000, OK);
    E16("0b0111111111111111",  0x7fff, OK);
    E16("0b1000000000000000",  0x8000, OK);
    E16("0b1111111111111111",  0xffff, OK);
    E16("0b10000000000000000", 0,      OVERFLOW_RANGE);

    E32("0b0",                                 0x00000000, OK);
    E32("0b01111111111111111111111111111111",  0x7fffffff, OK);
    E32("0b10000000000000000000000000000000",  0x80000000, OK);
    E32("0b11111111111111111111111111111111",  0xffffffff, OK);
    E32("0b100000000000000000000000000000000", 0,          OVERFLOW_RANGE);
}

static void test_unary_operator() {
    E8("-1",   0xff, OK);
    E8("-128", 0x80, OK);
    E8("-129", 0,    OVERFLOW_RANGE);

    E16("-1",     0xffff, OK);
    E16("-32768", 0x8000, OK);
    E16("-32769", 0,      OVERFLOW_RANGE);

    E32("-1",          0xffffffff, OK);
    E32("-2147483648", 0x80000000, OK);
    E32("-2147483649", 0,          OVERFLOW_RANGE);

    E8("+128", 0x80, OK);
    E8("+129", 0x81, OK);
    E8("+256", 0,    OVERFLOW_RANGE);

    E16("+32768", 0x8000, OK);
    E16("+32769", 0x8001, OK);
    E16("+65536", 0,      OVERFLOW_RANGE);

    E32("+2147483648", 0x80000000, OK);
    E32("+2147483649", 0x80000001, OK);
    E32("+4294967296", 0,          OVERFLOW_RANGE);

    E8("~+0",    0xFF, OK);
    E8("~(1|8)", 0xF6, OK);
    E8("~-1",    0x00, OK);

    E16("~0x0",     0xFFFF, OK);
    E16("~(1|010)", 0xFFF6, OK);
    E16("~-0b1",    0x0000, OK);

    E32("~+0",    0xFFFFFFFF, OK);
    E32("~(1|8)", 0xFFFFFFF6, OK);
    E32("~-1",    0x00000000, OK);
}

static void test_binary_operator() {
    E8("2+3",   5, OK);
    E8("-128+383", 255, OK);
    E8("2-3",  -1, OK);
    E8("255-383", -128, OK);
    E8("2*3",   6, OK);
    E8("20/3",  6, OK);
    E8("22/(-5)", -4, OK);
    E8("-22/5", -4, OK);
    E8("-22/(-5)", 4, OK);
    E8("20%3",  2, OK);
    E8("22%(-5)", 2, OK);
    E8("-22%5", -2, OK);
    E8("-22%(-5)", -2, OK);

    E8("1<<0", 1, OK);
    E8("1<<7", 128, OK);
    E8("1<<8", 0, OVERFLOW_RANGE);
    E8("0<<8", 0, OK);
    E8("-1>>8", -1, OK);
    E8("-1<<8", 0, OVERFLOW_RANGE);
    E8("-128>>1", -64, OK);
    E8("-128>>7", -1, OK);
    E8("-128>>8", -1, OK);
    E8("0x80>>7", 0x01, OK);
    E8("0x80>>8", 0, OK);
    E8("0xFF>>4", 0x0F, OK);
    E8("(0xFF<<4)&0xFF", 0xF0, OK);

    E8("0b0001|0b0100", 0x05, OK);
    E8("0b1011&0b0110", 0x02, OK);
    E8("0b0110^0b0011", 0x05, OK);

    E32("2+3",   5, OK);
    E32("2-3",  -1, OK);
    E32("2*3",   6, OK);
    E32("20/3",  6, OK);
    E32("20%3",  2, OK);

    E32("0x00000001<<31", 0x80000000, OK);
    E32("0x00000001<<32", 0, OK);
    E32("0x80000000>>31", 1, OK);
    E32("0x80000000>>32", 0, OK);
    E32("1>>(-1)", 0, OK);
    E32("1<<(-1)", 0, OK);
    E32("-1<<31", -0x80000000, OK);
    E32("-1<<32", 0, OK);
    E32("-1>>31", -1, OK);
    E32("-1>>32", -1, OK);

    E32("0b0001|0b0100", 0x05, OK);
    E32("0b1011&0b0110", 0x02, OK);
    E32("0b0110^-1",    0xfffffff9, OK);
}

static void test_precedence() {
    E16("1+2-3+4",    4, OK);
    E16("1+2*3+4",   11, OK);
    E16("1+2-7/3",    1, OK);
    E16("1-8%3*3",   -5, OK);
    E16("(1+2)*3+4", 13, OK);
    E16("1+(2-7)/3",  0, OK);
    E16("(1-8)%3*3", -3, OK);

    E8("1<<2+3", 0x20, OK);
    E8("(1<<2)+3" , 7, OK);
    E8("1<<2*3", 0x40, OK);
    E8("(1<<2)*3", 12, OK);
    E8("0x20>>2+3",    1, OK);
    E8("(0x20>>2)+3", 11, OK);
    E8("0x40>>2*3",    1, OK);
    E8("(0x40>>2)*3", 48, OK);

    E8("1<<4>>1",   8, OK);
    E8("1<<(4>>1)", 4, OK);
    E8("4>>1<<1",   4, OK);
    E8("4>>(1<<1)", 1, OK);

    E8("1|2&6^8", 11, OK);
    E8("1|2^4&12", 7, OK);
    E8("1&3|4^8", 13, OK);
    E8("1^2|4&12", 7, OK);
    E8("1&3^4|8", 13, OK);
    E8("1^2&6|8", 11, OK);

    E8("(1|2)&6^8", 10, OK);
    E8("(1|2)^4&12", 7, OK);
    E8("1&(3|4)^8",  9, OK);
    E8("1^(2|4)&12", 5, OK);
    E8("1&3^(4|8)", 13, OK);
    E8("1^2&(6|8)",  3, OK);

    E8("1|2&(6^8)",  3, OK);
    E8("1|(2^4)&12", 5, OK);
    E8("1&3|(4^8)", 13, OK);
    E8("(1^2)|4&12", 7, OK);
    E8("1&(3^4)|8",  9, OK);
    E8("(1^2)&6|8", 10, OK);

    E8("1|2<<3",   17, OK);
    E8("(1|2)<<3", 24, OK);
    E8("3&7<<1",    2, OK);
    E8("(3&7)<<1",  6, OK);
    E8("1^3<<3",   25, OK);
    E8("(1^3)<<3", 16, OK);
}

static void test_errors() {
    E32("undef",   0, UNDEFINED_SYMBOL);
    E32("0xxx",    0, ILLEGAL_CONSTANT);
    E32("0ooo",    0, ILLEGAL_CONSTANT);
    E32("0bbb",    0, ILLEGAL_CONSTANT);
    E32("2*(1+3",  0, MISSING_CLOSING_PAREN);
    E32("'a",      0, MISSING_CLOSING_QUOTE);
    E32("'\\'",    0, MISSING_CLOSING_QUOTE);
    E32("'\\x20",  0, MISSING_CLOSING_QUOTE);
    E32("'\\a'",   0, UNKNOWN_ESCAPE_SEQUENCE);
    E32("--1",     0, UNKNOWN_EXPR_OPERATOR);
    E32("-+1",     0, UNKNOWN_EXPR_OPERATOR);
    E32("+-1",     0, UNKNOWN_EXPR_OPERATOR);
    E32("++1",     0, UNKNOWN_EXPR_OPERATOR);
    E32("1<2",     0, UNKNOWN_EXPR_OPERATOR);
    E32("1>2",     0, UNKNOWN_EXPR_OPERATOR);
    E32("100/0",   0, DIVIDE_BY_ZERO);
    E32("100%0",   0, DIVIDE_BY_ZERO);
}

static void run_test(void (*test)(), const char *test_name) {
    asserter.clear(test_name);
    set_up();
    test();
    tear_down();
    asserter.check();
}

int main(int argc, char **argv) {
    RUN_TEST(test_char_constant);
    RUN_TEST(test_dec_constant);
    RUN_TEST(test_hex_constant);
    RUN_TEST(test_oct_constant);
    RUN_TEST(test_bin_constant);
    RUN_TEST(test_unary_operator);
    RUN_TEST(test_binary_operator);
    RUN_TEST(test_precedence);
    RUN_TEST(test_errors);
    return 0;
}
