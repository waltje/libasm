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

#include "dis_tms32010.h"
#include "test_dis_helper.h"
#include "array_memory.h"

using namespace libasm;
using namespace libasm::tms32010;
using namespace libasm::test;

DisTms32010 dis32010;
Disassembler &disassembler(dis32010);

static bool is32010() {
    return strcmp_P("32010", disassembler.cpu_P()) == 0;
}

static void set_up() {
    disassembler.reset();
    disassembler.setOption("relative", "no");
}

static void tear_down() {
    symtab.reset();
}

// clang-format off
void test_cpu() {
    EQUALS("cpu 32010", true,   disassembler.setCpu("32010"));
    EQUALS_P("cpu 32010", "32010", disassembler.cpu_P());

    EQUALS("cpu 32015", true,   disassembler.setCpu("32015"));
    EQUALS_P("cpu 32015", "32015", disassembler.cpu_P());

    EQUALS("cpu TMS32010", true,   disassembler.setCpu("TMS32010"));
    EQUALS_P("cpu TMS32010", "32010", disassembler.cpu_P());

    EQUALS("cpu TMS32015", true,   disassembler.setCpu("TMS32015"));
    EQUALS_P("cpu TMS32015", "32015", disassembler.cpu_P());
}

static void test_accumrator() {
    TEST(ABS, "",    0x7F88);

    TEST(ADD, "70H",         0x0070);
    TEST(ADD, "70H, 15",     0x0F70);
    TEST(ADD, "*, 0, AR0",   0x0080);
    TEST(ADD, "*, 0, AR1",   0x0081);
    ERRI(                    0x0082);
    ERRI(                    0x0083);
    ERRI(                    0x0084);
    ERRI(                    0x0085);
    ERRI(                    0x0086);
    ERRI(                    0x0087);
    TEST(ADD, "*",           0x0088);
    ERRI(                    0x0089);
    ERRI(                    0x008A);
    ERRI(                    0x008B);
    ERRI(                    0x008C);
    ERRI(                    0x008D);
    ERRI(                    0x008E);
    ERRI(                    0x008F);
    TEST(ADD, "*-",          0x0098);
    TEST(ADD, "*+",          0x00A8);
    ERRI(                    0x00B8);
    ERRI(                    0x00C8);
    TEST(ADD, "*, 1",        0x0188);
    TEST(ADD, "*-, 2",       0x0298);
    TEST(ADD, "*+, 3",       0x03A8);
    TEST(ADD, "*, 0, AR0",   0x0080);
    TEST(ADD, "*, 4, AR1",   0x0481);
    TEST(ADD, "*-, 4, AR0",  0x0490);
    TEST(ADD, "*-, 0, AR1",  0x0091);
    TEST(ADD, "*+, 15, AR0", 0x0FA0);
    TEST(ADD, "*+, 15, AR1", 0x0FA1);

    TEST(ADDH, "70H",     0x6070);
    TEST(ADDH, "*",       0x6088);
    TEST(ADDH, "*-",      0x6098);
    TEST(ADDH, "*+",      0x60A8);
    TEST(ADDH, "*, AR0",  0x6080);
    TEST(ADDH, "*-, AR1", 0x6091);
    TEST(ADDH, "*+, AR0", 0x60A0);

    TEST(ADDS, "70H",     0x6170);
    TEST(ADDS, "*",       0x6188);
    TEST(ADDS, "*-",      0x6198);
    TEST(ADDS, "*+",      0x61A8);
    TEST(ADDS, "*, AR0",  0x6180);
    TEST(ADDS, "*-, AR1", 0x6191);
    TEST(ADDS, "*+, AR0", 0x61A0);

    TEST(AND, "70H",     0x7970);
    TEST(AND, "*",       0x7988);
    TEST(AND, "*-",      0x7998);
    TEST(AND, "*+",      0x79A8);
    TEST(AND, "*, AR0",  0x7980);
    TEST(AND, "*-, AR1", 0x7991);
    TEST(AND, "*+, AR0", 0x79A0);

    TEST(LAC, "70H",         0x2070);
    TEST(LAC, "70H, 15",     0x2F70);
    TEST(LAC, "*, 0, AR0",   0x2080);
    TEST(LAC, "*, 0, AR1",   0x2081);
    ERRI(                    0x2082);
    ERRI(                    0x2083);
    ERRI(                    0x2084);
    ERRI(                    0x2085);
    ERRI(                    0x2086);
    ERRI(                    0x2087);
    TEST(LAC, "*",           0x2088);
    TEST(LAC, "*-",          0x2098);
    TEST(LAC, "*+",          0x20A8);
    ERRI(                    0x20B8);
    ERRI(                    0x20C8);
    TEST(LAC, "*, 1",        0x2188);
    TEST(LAC, "*-, 2",       0x2298);
    TEST(LAC, "*+, 3",       0x23A8);
    TEST(LAC, "*, 0, AR0",   0x2080);
    TEST(LAC, "*, 4, AR1",   0x2481);
    TEST(LAC, "*-, 4, AR0",  0x2490);
    TEST(LAC, "*-, 0, AR1",  0x2091);
    TEST(LAC, "*+, 15, AR0", 0x2FA0);
    TEST(LAC, "*+, 15, AR1", 0x2FA1);

    TEST(LACK, "0",   0x7E00);
    TEST(LACK, "255", 0x7EFF);

    TEST(OR, "70H",     0x7A70);
    TEST(OR, "*",       0x7A88);
    TEST(OR, "*-",      0x7A98);
    TEST(OR, "*+",      0x7AA8);
    TEST(OR, "*, AR0",  0x7A80);
    TEST(OR, "*-, AR1", 0x7A91);
    TEST(OR, "*+, AR0", 0x7AA0);

    TEST(SACH, "70H",        0x5870);
    TEST(SACH, "70H, 1",     0x5970);
    ERRI(                    0x5A70);
    ERRI(                    0x5B70);
    TEST(SACH, "70H, 4",     0x5C70);
    ERRI(                    0x5D70);
    ERRI(                    0x5E70);
    ERRI(                    0x5F70);
    TEST(SACH, "*, 0, AR0",  0x5880);
    TEST(SACH, "*, 0, AR1",  0x5881);
    ERRI(                    0x5882);
    ERRI(                    0x5883);
    ERRI(                    0x5884);
    ERRI(                    0x5885);
    ERRI(                    0x5886);
    ERRI(                    0x5887);
    TEST(SACH, "*",          0x5888);
    TEST(SACH, "*-",         0x5898);
    TEST(SACH, "*+",         0x58A8);
    ERRI(                    0x58B8);
    ERRI(                    0x58C8);
    TEST(SACH, "*, 1",       0x5988);
    TEST(SACH, "*-, 4",      0x5C98);
    TEST(SACH, "*+, 1",      0x59A8);
    TEST(SACH, "*, 0, AR0",  0x5880);
    TEST(SACH, "*, 4, AR1",  0x5C81);
    TEST(SACH, "*-, 4, AR0", 0x5C90);
    TEST(SACH, "*-, 0, AR1", 0x5891);
    TEST(SACH, "*+, 0, AR0", 0x58A0);
    TEST(SACH, "*+, 4, AR1", 0x5CA1);

    TEST(SACL, "70H",        0x5070);
    ERRI(                    0x5170);
    ERRI(                    0x5270);
    ERRI(                    0x5370);
    ERRI(                    0x5470);
    ERRI(                    0x5570);
    ERRI(                    0x5670);
    ERRI(                    0x5770);
    TEST(SACL, "*",          0x5088);
    TEST(SACL, "*-",         0x5098);
    TEST(SACL, "*+",         0x50A8);
    TEST(SACL, "*, 0, AR0",  0x5080);
    TEST(SACL, "*-, 0, AR1", 0x5091);
    TEST(SACL, "*+, 0, AR0", 0x50A0);

    TEST(SUB, "70H",         0x1070);
    TEST(SUB, "70H, 15",     0x1F70);
    TEST(SUB, "*, 0, AR0",   0x1080);
    TEST(SUB, "*, 0, AR1",   0x1081);
    ERRI(                    0x1082);
    ERRI(                    0x1083);
    ERRI(                    0x1084);
    ERRI(                    0x1085);
    ERRI(                    0x1086);
    ERRI(                    0x1087);
    TEST(SUB, "*",           0x1088);
    TEST(SUB, "*-",          0x1098);
    TEST(SUB, "*+",          0x10A8);
    ERRI(                    0x10B8);
    ERRI(                    0x10C8);
    TEST(SUB, "*, 1",        0x1188);
    TEST(SUB, "*-, 2",       0x1298);
    TEST(SUB, "*+, 3",       0x13A8);
    TEST(SUB, "*, 0, AR0",   0x1080);
    TEST(SUB, "*, 4, AR1",   0x1481);
    TEST(SUB, "*-, 4, AR0",  0x1490);
    TEST(SUB, "*-, 0, AR1",  0x1091);
    TEST(SUB, "*+, 15, AR0", 0x1FA0);
    TEST(SUB, "*+, 15, AR1", 0x1FA1);

    TEST(SUBC, "70H",     0x6470);
    TEST(SUBC, "*",       0x6488);
    TEST(SUBC, "*-",      0x6498);
    TEST(SUBC, "*+",      0x64A8);
    TEST(SUBC, "*, AR0",  0x6480);
    TEST(SUBC, "*-, AR1", 0x6491);
    TEST(SUBC, "*+, AR0", 0x64A0);

    TEST(SUBH, "70H",     0x6270);
    TEST(SUBH, "*",       0x6288);
    TEST(SUBH, "*-",      0x6298);
    TEST(SUBH, "*+",      0x62A8);
    TEST(SUBH, "*, AR0",  0x6280);
    TEST(SUBH, "*-, AR1", 0x6291);
    TEST(SUBH, "*+, AR0", 0x62A0);

    TEST(SUBS, "70H",     0x6370);
    TEST(SUBS, "*",       0x6388);
    TEST(SUBS, "*-",      0x6398);
    TEST(SUBS, "*+",      0x63A8);
    TEST(SUBS, "*, AR0",  0x6380);
    TEST(SUBS, "*-, AR1", 0x6391);
    TEST(SUBS, "*+, AR0", 0x63A0);

    TEST(XOR, "70H",     0x7870);
    TEST(XOR, "*",       0x7888);
    TEST(XOR, "*-",      0x7898);
    TEST(XOR, "*+",      0x78A8);
    TEST(XOR, "*, AR0",  0x7880);
    TEST(XOR, "*-, AR1", 0x7891);
    TEST(XOR, "*+, AR0", 0x78A0);

    TEST(ZAC, "", 0x7F89);

    TEST(ZALH, "70H",     0x6570);
    TEST(ZALH, "*",       0x6588);
    TEST(ZALH, "*-",      0x6598);
    TEST(ZALH, "*+",      0x65A8);
    TEST(ZALH, "*, AR0",  0x6580);
    TEST(ZALH, "*-, AR1", 0x6591);
    TEST(ZALH, "*+, AR0", 0x65A0);

    TEST(ZALS, "70H",     0x6670);
    TEST(ZALS, "*",       0x6688);
    TEST(ZALS, "*-",      0x6698);
    TEST(ZALS, "*+",      0x66A8);
    TEST(ZALS, "*, AR0",  0x6680);
    TEST(ZALS, "*-, AR1", 0x6691);
    TEST(ZALS, "*+, AR0", 0x66A0);
}

static void test_auxiliary() {
    TEST(LAR, "AR0, 70H",     0x3870);
    TEST(LAR, "AR0, *",       0x3888);
    TEST(LAR, "AR0, *-",      0x3898);
    TEST(LAR, "AR0, *+",      0x38A8);
    TEST(LAR, "AR0, *, AR0",  0x3880);
    TEST(LAR, "AR0, *-, AR1", 0x3891);
    TEST(LAR, "AR0, *+, AR0", 0x38A0);
    TEST(LAR, "AR1, 70H",     0x3970);
    TEST(LAR, "AR1, *",       0x3988);
    TEST(LAR, "AR1, *-",      0x3998);
    TEST(LAR, "AR1, *+",      0x39A8);
    TEST(LAR, "AR1, *, AR0",  0x3980);
    TEST(LAR, "AR1, *-, AR1", 0x3991);
    TEST(LAR, "AR1, *+, AR0", 0x39A0);

    TEST(LARK, "AR0, 255", 0x70FF);
    TEST(LARK, "AR1, 128", 0x7180);

    TEST(LARP, "AR0", 0x6880);
    TEST(LARP, "AR1", 0x6881);

    TEST(LDP, "70H",     0x6F70);
    TEST(LDP, "*",       0x6F88);
    TEST(LDP, "*-",      0x6F98);
    TEST(LDP, "*+",      0x6FA8);
    TEST(LDP, "*, AR0",  0x6F80);
    TEST(LDP, "*-, AR1", 0x6F91);
    TEST(LDP, "*+, AR0", 0x6FA0);

    TEST(LDPK, "0", 0x6E00);
    TEST(LDPK, "1", 0x6E01);

    TEST(MAR,  "70H",     0x6870);
    TEST(MAR,  "*",       0x6888);
    TEST(MAR,  "*-",      0x6898);
    TEST(MAR,  "*+",      0x68A8);
    TEST(LARP, "AR0",     0x6880);
    TEST(MAR,  "*-, AR1", 0x6891);
    TEST(MAR,  "*+, AR0", 0x68A0);

    TEST(SAR, "AR0, 70H",     0x3070);
    TEST(SAR, "AR0, *",       0x3088);
    TEST(SAR, "AR0, *-",      0x3098);
    TEST(SAR, "AR0, *+",      0x30A8);
    TEST(SAR, "AR0, *, AR0",  0x3080);
    TEST(SAR, "AR0, *-, AR1", 0x3091);
    TEST(SAR, "AR0, *+, AR0", 0x30A0);
    TEST(SAR, "AR1, 70H",     0x3170);
    TEST(SAR, "AR1, *",       0x3188);
    TEST(SAR, "AR1, *-",      0x3198);
    TEST(SAR, "AR1, *+",      0x31A8);
    TEST(SAR, "AR1, *, AR0",  0x3180);
    TEST(SAR, "AR1, *-, AR1", 0x3191);
    TEST(SAR, "AR1, *+, AR0", 0x31A0);
}

static void test_multiply() {
    TEST(APAC, "", 0x7F8F);

    TEST(LT, "70H",     0x6A70);
    TEST(LT, "*",       0x6A88);
    TEST(LT, "*-",      0x6A98);
    TEST(LT, "*+",      0x6AA8);
    TEST(LT, "*, AR0",  0x6A80);
    TEST(LT, "*-, AR1", 0x6A91);
    TEST(LT, "*+, AR0", 0x6AA0);

    TEST(LTA, "70H",     0x6C70);
    TEST(LTA, "*",       0x6C88);
    TEST(LTA, "*-",      0x6C98);
    TEST(LTA, "*+",      0x6CA8);
    TEST(LTA, "*, AR0",  0x6C80);
    TEST(LTA, "*-, AR1", 0x6C91);
    TEST(LTA, "*+, AR0", 0x6CA0);

    TEST(LTD, "70H",     0x6B70);
    TEST(LTD, "*",       0x6B88);
    TEST(LTD, "*-",      0x6B98);
    TEST(LTD, "*+",      0x6BA8);
    TEST(LTD, "*, AR0",  0x6B80);
    TEST(LTD, "*-, AR1", 0x6B91);
    TEST(LTD, "*+, AR0", 0x6BA0);

    TEST(MPY, "70H",     0x6D70);
    TEST(MPY, "*",       0x6D88);
    TEST(MPY, "*-",      0x6D98);
    TEST(MPY, "*+",      0x6DA8);
    TEST(MPY, "*, AR0",  0x6D80);
    TEST(MPY, "*-, AR1", 0x6D91);
    TEST(MPY, "*+, AR0", 0x6DA0);

    TEST(MPYK, "0",     0x8000);
    TEST(MPYK, "2",     0x8002);
    TEST(MPYK, "4095",  0x8FFF);
    TEST(MPYK, "-1",    0x9FFF);
    TEST(MPYK, "-2",    0x9FFE);
    TEST(MPYK, "-4096", 0x9000);

    TEST(PAC,  "", 0x7F8E);
    TEST(SPAC, "", 0x7F90);
}

static void test_branch() {
    TEST(B, "000H",  0xF900, 0x0000);
    TEST(B, "001H",  0xF900, 0x0001);
    TEST(B, "002H",  0xF900, 0x0002);
    TEST(B, "004H",  0xF900, 0x0004);
    TEST(B, "008H",  0xF900, 0x0008);
    TEST(B, "010H",  0xF900, 0x0010);
    TEST(B, "020H",  0xF900, 0x0020);
    TEST(B, "040H",  0xF900, 0x0040);
    TEST(B, "080H",  0xF900, 0x0080);
    TEST(B, "100H",  0xF900, 0x0100);
    TEST(B, "200H",  0xF900, 0x0200);
    TEST(B, "400H",  0xF900, 0x0400);
    TEST(B, "800H",  0xF900, 0x0800);
    TEST(B, "0FFFH", 0xF900, 0x0FFF);
    ERRT(B, "1000H", OVERFLOW_RANGE, 0xF900, 0x1000);
    ERRT(B, "2000H", OVERFLOW_RANGE, 0xF900, 0x2000);
    ERRT(B, "4000H", OVERFLOW_RANGE, 0xF900, 0x4000);
    ERRT(B, "8000H", OVERFLOW_RANGE, 0xF900, 0x8000);

    TEST(BANZ, "900H",  0xF400, 0x0900);
    TEST(BGEZ, "900H",  0xFD00, 0x0900);
    TEST(BGZ,  "900H",  0xFC00, 0x0900);
    TEST(BIOZ, "900H",  0xF600, 0x0900);
    TEST(BLEZ, "900H",  0xFB00, 0x0900);
    TEST(BLZ,  "900H",  0xFA00, 0x0900);
    TEST(BNZ,  "900H",  0xFE00, 0x0900);
    TEST(BV,   "900H",  0xF500, 0x0900);
    TEST(BZ,   "900H",  0xFF00, 0x0900);
    TEST(CALL, "900H",  0xF800, 0x0900);

    TEST(CALA, "", 0x7F8C);
    TEST(RET,  "", 0x7F8D);
}

static void test_control() {
    TEST(DINT, "", 0x7F81);
    TEST(EINT, "", 0x7F82);
    TEST(NOP,  "", 0x7F80);
    TEST(POP,  "", 0x7F9D);
    TEST(PUSH, "", 0x7F9C);
    TEST(ROVM, "", 0x7F8A);
    TEST(SOVM, "", 0x7F8B);

    TEST(LST, "00H",     0x7B00);
    TEST(LST, "70H",     0x7B70);
    TEST(LST, "*",       0x7B88);
    TEST(LST, "*-",      0x7B98);
    TEST(LST, "*+",      0x7BA8);
    TEST(LST, "*, AR0",  0x7B80);
    TEST(LST, "*-, AR1", 0x7B91);
    TEST(LST, "*+, AR0", 0x7BA0);

    TEST(SST, "80H",     0x7C00);
    TEST(SST, "8FH",     0x7C0F);
    if (is32010()) {
        ERRT(SST, "90H", OVERFLOW_RANGE, 0x7C10);
    } else {
        TEST(SST, "90H",                  0x7C10);
    }
    TEST(SST, "*",       0x7C88);
    TEST(SST, "*-",      0x7C98);
    TEST(SST, "*+",      0x7CA8);
    TEST(SST, "*, AR0",  0x7C80);
    TEST(SST, "*-, AR1", 0x7C91);
    TEST(SST, "*+, AR0", 0x7CA0);
}

static void test_dataio() {
    TEST(DMOV, "00H",     0x6900);
    TEST(DMOV, "70H",     0x6970);
    TEST(DMOV, "*",       0x6988);
    TEST(DMOV, "*-",      0x6998);
    TEST(DMOV, "*+",      0x69A8);
    TEST(DMOV, "*, AR0",  0x6980);
    TEST(DMOV, "*-, AR1", 0x6991);
    TEST(DMOV, "*+, AR0", 0x69A0);

    TEST(TBLR, "00H",     0x6700);
    TEST(TBLR, "70H",     0x6770);
    TEST(TBLR, "*",       0x6788);
    TEST(TBLR, "*-",      0x6798);
    TEST(TBLR, "*+",      0x67A8);
    TEST(TBLR, "*, AR0",  0x6780);
    TEST(TBLR, "*-, AR1", 0x6791);
    TEST(TBLR, "*+, AR0", 0x67A0);

    TEST(TBLW, "00H",     0x7D00);
    TEST(TBLW, "70H",     0x7D70);
    TEST(TBLW, "*",       0x7D88);
    TEST(TBLW, "*-",      0x7D98);
    TEST(TBLW, "*+",      0x7DA8);
    TEST(TBLW, "*, AR0",  0x7D80);
    TEST(TBLW, "*-, AR1", 0x7D91);
    TEST(TBLW, "*+, AR0", 0x7DA0);

    TEST(IN, "70H, PA0",     0x4070);
    TEST(IN, "70H, PA7",     0x4770);
    TEST(IN, "*, PA0, AR0",  0x4080);
    TEST(IN, "*, PA7, AR1",  0x4781);
    ERRI(                    0x4082);
    ERRI(                    0x4083);
    ERRI(                    0x4084);
    ERRI(                    0x4085);
    ERRI(                    0x4086);
    ERRI(                    0x4087);
    TEST(IN, "*, PA0",       0x4088);
    TEST(IN, "*-, PA1",      0x4198);
    TEST(IN, "*+, PA2",      0x42A8);
    ERRI(                    0x40B8);
    ERRI(                    0x40C8);
    TEST(IN, "*, PA1",       0x4188);
    TEST(IN, "*-, PA2",      0x4298);
    TEST(IN, "*+, PA3",      0x43A8);
    TEST(IN, "*, PA0, AR0",  0x4080);
    TEST(IN, "*, PA4, AR1",  0x4481);
    TEST(IN, "*-, PA4, AR0", 0x4490);
    TEST(IN, "*-, PA0, AR1", 0x4091);
    TEST(IN, "*+, PA7, AR0", 0x47A0);
    TEST(IN, "*+, PA7, AR1", 0x47A1);

    TEST(OUT, "70H, PA0",     0x4870);
    TEST(OUT, "70H, PA7",     0x4F70);
    TEST(OUT, "*, PA0, AR0",  0x4880);
    TEST(OUT, "*, PA7, AR1",  0x4F81);
    ERRI(                     0x4882);
    ERRI(                     0x4883);
    ERRI(                     0x4884);
    ERRI(                     0x4885);
    ERRI(                     0x4886);
    ERRI(                     0x4887);
    TEST(OUT, "*, PA0",       0x4888);
    TEST(OUT, "*-, PA1",      0x4998);
    TEST(OUT, "*+, PA2",      0x4AA8);
    ERRI(                     0x48B8);
    ERRI(                     0x48C8);
    TEST(OUT, "*, PA1",       0x4988);
    TEST(OUT, "*-, PA2",      0x4A98);
    TEST(OUT, "*+, PA3",      0x4BA8);
    TEST(OUT, "*, PA0, AR0",  0x4880);
    TEST(OUT, "*, PA4, AR1",  0x4C81);
    TEST(OUT, "*-, PA4, AR0", 0x4C90);
    TEST(OUT, "*-, PA0, AR1", 0x4891);
    TEST(OUT, "*+, PA7, AR0", 0x4FA0);
    TEST(OUT, "*+, PA7, AR1", 0x4FA1);
}

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
static bool in_array(uint8_t v, const uint8_t *begin, const uint8_t *end) {
    for (const uint8_t *entry = begin; entry < end; entry++) {
        if (v == *entry)
            return true;
    }
    return false;
}

static void assert_ok(Config::opcode_t opc) {
    uint16_t words[2] = { opc, 0 };
    Insn insn(0x100);
    char operands[40], message[40];
    const ArrayMemory memory(0x100, words, sizeof(words), disassembler.config().endian());
    auto it = memory.iterator();

    disassembler.setOption("uppercase", "yes");
    disassembler.decode(it, insn, operands, sizeof(operands));
    sprintf(message, "%04X must be LEGAL", opc);
    EQUALS(message, OK, disassembler.getError());
}

static void assert_illegal(Config::opcode_t opc, Error err, const char *message) {
    uint16_t words[2] = { opc, 0 };
    Insn insn(0x100);
    char operands[40];
    const ArrayMemory memory(0x100, words, sizeof(words), disassembler.config().endian());
    auto it = memory.iterator();

    disassembler.setOption("uppercase", "enable");
    disassembler.decode(it, insn, operands, sizeof(operands));
    EQUALS(message, err, disassembler.getError());
    if (disassembler.getError() == OK)
        printf("actual: %s %s\n", insn.name(), operands);
}

static void assert_unknown(Config::opcode_t opc) {
    char message[80];
    sprintf(message, "%04X must be UNKNOWN_INSTRUCTION", opc);
    assert_illegal(opc, UNKNOWN_INSTRUCTION, message);
}

static void assert_low8(Config::opcode_t base) {

    for (Config::opcode_t low8 = 0x00; low8 < 0x100; low8++) {
        assert_unknown(base | low8);
    }
}

static void assert_indirect(Config::opcode_t base) {
    static const uint8_t valids[] = {
        0x80, // *, AR0
        0x81, // *, AR1
        0x88, // *
        0x90, // *-, AR0
        0x91, // *-, AR1
        0x98, // *-
        0xA0, // *+, AR0
        0xA1, // *+, AR1
        0xA8, // *+
    };

    for (Config::opcode_t indir = 0x80; indir < 0x100; indir++) {
        const Config::opcode_t opc = base | indir;
        if (in_array(indir, valids, std::end(valids))) {
            assert_ok(opc);
        } else {
            assert_unknown(opc);
        }
    }
}

static void test_illegal() {
    // ADD, SUB, LAC
    static const Config::opcode_t mam_ls4[] = { 0x0000, 0x1000, 0x2000, };
    for (size_t i = 0; i < ARRAY_SIZE(mam_ls4); i++) {
        const Config::opcode_t base = mam_ls4[i];
        for (Config::opcode_t ls4 = 0; ls4 < 16; ls4++) {
            assert_indirect(base | (ls4 << 8));
        }
    }

    // SAR, LAC
    static const Config::opcode_t ar_mam[] = { 0x3000, 0x3800 };
    for (size_t i = 0; i < ARRAY_SIZE(ar_mam); i++) {
        const Config::opcode_t base = ar_mam[i];
        for (Config::opcode_t ar = 0; ar < 2; ar++) {
            assert_indirect(base | (ar << 8));
        }
        for (Config::opcode_t ar = 2; ar < 8; ar++) {
            assert_low8(base | (ar << 8));
        }
    }

    // IN, OUT
    for (Config::opcode_t pa = 0; pa < 8; pa++) {
        assert_indirect(0x4000 | (pa << 8));
        assert_indirect(0x4800 | (pa << 8));
    }

    // SACL
    for (Config::opcode_t ls0 = 0; ls0 < 8; ls0++) {
        if (ls0 == 0) {
            assert_indirect(0x5000 | (ls0 << 8));
        } else {
            assert_low8(0x5000 | (ls0 << 8));
        }
    }
    // SACH
    for (Config::opcode_t ls3 = 0; ls3 < 8; ls3++) {
        if (ls3 == 0 || ls3 == 1 || ls3 == 4) {
            assert_indirect(0x5800 | (ls3 << 8));
        } else {
            assert_low8(0x5800 | (ls3 << 8));
        }
    }

    // SAR, LAC
    static const uint8_t mam_hi8[] = {
        0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
        0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6F,
        0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D,
    };
    for (size_t i = 0; i < ARRAY_SIZE(mam_hi8); i++) {
        assert_indirect(static_cast<Config::opcode_t>(mam_hi8[i]) << 8);
    }

    // LDPK
    for (Config::opcode_t dpk = 0; dpk < 0x100; dpk++) {
        if (dpk == 0 || dpk == 1) {
            assert_ok(0x6E00 | dpk);
        } else {
            assert_unknown(0x6E00 | dpk);
        }
    }

    // LARK
    for (Config::opcode_t ar = 0; ar < 8; ar++) {
        if (ar == 0 || ar == 1) {
            assert_ok(0x7000 | (ar << 8));
        } else {
            assert_unknown(0x7000 | (ar << 8));
        }
    }

    // controls etc.
    static const uint8_t low8[] = {
        0x80, 0x81, 0x82, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
        0x90, 0x9C, 0x9D,
    };
    for (Config::opcode_t opc = 0; opc < 0x100; opc++) {
        if (in_array(opc, low8, std::end(low8))) {
            assert_ok(0x7F00 | opc);
        } else {
            assert_unknown(0x7F00 | opc);
        }
    }

    for (Config::opcode_t opc = 0xA0; opc < 0xF4; opc++) {
        assert_low8(opc << 8);
    }

    // branches
    for (Config::opcode_t br = 0xF4; br < 0x100; br++) {
        if (br == 0xF7) {
            assert_low8(br << 8);
        } else {
            for (Config::opcode_t low8 = 0; low8 < 0x100; low8++) {
                if (low8 == 0) {
                    assert_ok((br << 8) | low8);
                } else {
                    assert_unknown((br << 8) | low8);
                }
            }
        }
    }
}

// clang-format on

void run_tests(const char *cpu) {
    disassembler.setCpu(cpu);
    RUN_TEST(test_accumrator);
    RUN_TEST(test_auxiliary);
    RUN_TEST(test_multiply);
    RUN_TEST(test_branch);
    RUN_TEST(test_control);
    RUN_TEST(test_dataio);
    RUN_TEST(test_illegal);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
