#include "test_expr_helper.h"

AsmMotoOperand parser;
TestSymtab symtab;
TestAsserter asserter;

static void set_up() {
}

static void tear_down() {
    symtab.reset();
}

static void test_hex_constant() {
    E8("$0",  0x00, OK);
    E8("$7f", 0x7f, OK);
    E8("$80", 0x80, OK);
    E8("$FF", 0xff, OK);
    E8("$100", 0,    OVERFLOW_RANGE);

    E16("$0",    0x0000, OK);
    E16("$7fff", 0x7fff, OK);
    E16("$8000", 0x8000, OK);
    E16("$ffff", 0xffff, OK);
    E16("$10000", 0,      OVERFLOW_RANGE);

    E32("$0",        0x00000000, OK);
    E32("$7FFFFFFF", 0x7fffffff, OK);
    E32("$80000000", 0x80000000, OK);
    E32("$FFFFffff", 0xffffffff, OK);
    E32("$100000000", 0,          OVERFLOW_RANGE);
}

static void test_oct_constant() {
    E8("@0",   0x00, OK);
    E8("@177", 0x7f, OK);
    E8("@200", 0x80, OK);
    E8("@377", 0xff, OK);
    E8("@400", 0,    OVERFLOW_RANGE);

    E16("@0",      0x0000, OK);
    E16("@077777", 0x7fff, OK);
    E16("@100000", 0x8000, OK);
    E16("@177777", 0xffff, OK);
    E16("@200000", 0,      OVERFLOW_RANGE);

    E32("@0",           0x00000000, OK);
    E32("@17777777777", 0x7fffffff, OK);
    E32("@20000000000", 0x80000000, OK);
    E32("@37777777777", 0xffffffff, OK);
    E32("@40000000000", 0,          OVERFLOW_RANGE);
}

static void test_bin_constant() {
    E8("%0",        0x00, OK);
    E8("%01111111", 0x7f, OK);
    E8("%10000000", 0x80, OK);
    E8("%11111111", 0xff, OK);
    E8("100000000", 0,    OVERFLOW_RANGE);

    E16("%0",                0x0000, OK);
    E16("%0111111111111111", 0x7fff, OK);
    E16("%1000000000000000", 0x8000, OK);
    E16("%1111111111111111", 0xffff, OK);
    E16("%10000000000000000", 0,      OVERFLOW_RANGE);

    E32("%0",                                0x00000000, OK);
    E32("%01111111111111111111111111111111", 0x7fffffff, OK);
    E32("%10000000000000000000000000000000", 0x80000000, OK);
    E32("%11111111111111111111111111111111", 0xffffffff, OK);
    E32("%100000000000000000000000000000000", 0,          OVERFLOW_RANGE);
}

static void test_current_address() {
    symtab.setCurrentAddress(0x1000);
    E16("*",       0x1000, OK);
    E16("*+2",     0x1002, OK);
    E16("*-2",     0x0FFE, OK);
    E16("*+$F000", 0,      OVERFLOW_RANGE);
    E16("*-$1001", 0,      OVERFLOW_RANGE);
    E32("*+$F000", 0x00010000, OK);
    E32("*-$1001", 0xFFFFFFFF, OK);

    symtab.put(0x1000, "table");
    symtab.setCurrentAddress(0x1100);
    E16("*-table",     0x100, OK);
    E16("(*-table)/2", 0x80,  OK);
}

static void test_errors() {
    E32("$xxx",    0, ILLEGAL_CONSTANT);
    E32("@ooo",    0, ILLEGAL_CONSTANT);
    E32("%bbb",    0, ILLEGAL_CONSTANT);
}

static void run_test(void (*test)(), const char *test_name) {
    asserter.clear(test_name);
    set_up();
    test();
    tear_down();
    asserter.check();
}

int main(int argc, char **argv) {
    RUN_TEST(test_hex_constant);
    RUN_TEST(test_oct_constant);
    RUN_TEST(test_bin_constant);
    RUN_TEST(test_current_address);
    RUN_TEST(test_errors);
    return 0;
}
