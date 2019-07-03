/*
01_four_op_test.c

Copyright (c) 6/20/2019 Manwoo Yi

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */

#include "TestMain.h"

#include "planck.h"

REGISTER_SUITE_AUTO(Four_OP_Test, "01 Four Operation Test")

TESTCASE(FOT01, "simple add")
{
    char* codeline = "3 + 4";

    object_t ret;
    bool st = Planck_do(codeline, &ret);

    ASSERT_EQ_NUM(true, st);
    ASSERT_EQ_NUM(7.0, ret.value.number);
}

TESTCASE(FOT02, "complex add")
{
    char* codeline = "3 + 4 + (7 + 4) + 382+(392+4+498)+2023+993";

    object_t ret;
    bool st = Planck_do(codeline, &ret);

    ASSERT_EQ_NUM(true, st);
    ASSERT_EQ_NUM((3 + 4 + (7 + 4) + 382+(392+4+498)+2023+993), ret.value.number);
}

TESTCASE(FOT03, "simple sub")
{
    char* codeline = "3 - 4";

    object_t ret;
    bool st = Planck_do(codeline, &ret);

    ASSERT_EQ_NUM(true, st);
    ASSERT_EQ_NUM((3 - 4), ret.value.number);
}

TESTCASE(FOT04, "mixed add, sub")
{
    char* codeline = "3 - 4 + 3290 - (39-4+48) + 3902+332";

    object_t ret;
    bool st = Planck_do(codeline, &ret);

    ASSERT_EQ_NUM(true, st);
    ASSERT_EQ_NUM((3 - 4 + 3290 - (39-4+48) + 3902+332), ret.value.number);
}

TESTCASE(FOT05, "simple mul")
{
    char* codeline = "23*455";

    object_t ret;
    bool st = Planck_do(codeline, &ret);

    ASSERT_EQ_NUM(true, st);
    ASSERT_EQ_NUM((23*455), ret.value.number);
}

TESTCASE(FOT06, "mixed add, sub, mul")
{
    char* codeline = "23*455 + 32 + 323 - (325-33) * 43 - 32 +883";

    object_t ret;
    bool st = Planck_do(codeline, &ret);

    ASSERT_EQ_NUM(true, st);
    ASSERT_EQ_NUM((23*455 + 32 + 323 - (325-33) * 43 - 32 +883), ret.value.number);
}

TESTCASE(FOT07, "simple div")
{
    char* codeline = "4 / 2";

    object_t ret;
    bool st = Planck_do(codeline, &ret);

    ASSERT_EQ_NUM(true, st);
    ASSERT_EQ_NUM((4 / 2), ret.value.number);
}

TESTCASE(FOT08, "mixed add, sub, mul, div")
{
    char* codeline = "4 / 2 + 2 * 3 + (3 + 2) / 2 * 3";

    object_t ret;
    bool st = Planck_do(codeline, &ret);

    ASSERT_EQ_NUM(true, st);
    ASSERT_EQ_NUM((4.0 / 2.0 + 2 * 3 + (3 + 2) / 2.0 * 3), ret.value.number);
}

TESTCASE(FOT09, "simple mod")
{
    char* codeline = "23424% 323";

    object_t ret;
    bool st = Planck_do(codeline, &ret);

    ASSERT_EQ_NUM(true, st);
    ASSERT_EQ_NUM((23424% 323), ret.value.number);
}

TESTCASE(FOT10, "mixed add, sub, mul, div, mod")
{
    char* codeline = "23424% 323 + 3 / (3 + 4) * 3 - 223 + 3992";

    object_t ret;
    bool st = Planck_do(codeline, &ret);

    ASSERT_EQ_NUM(true, st);
    ASSERT_EQ_NUM((23424% 323 + 3.0 / (3.0 + 4.0) * 3 - 223 + 3992), ret.value.number);
}

TESTCASE(FOT11, "string concatenation")
{
    char* codeline = "'abcd' ## 'efg' ## 23 ## 'deff'";

    object_t ret;
    bool st = Planck_do(codeline, &ret);

    ASSERT_EQ_NUM(true, st);
    ASSERT_EQ_STR("abcdefg23deff", ret.value.str.ptr);
}

TESTCASE(FOT12, "string conc with op")
{
    char* codeline = "'abcd' ## 'efg' ## 20 + 3 ## 'deff'";

    object_t ret;
    bool st = Planck_do(codeline, &ret);

    ASSERT_EQ_NUM(true, st);
    ASSERT_EQ_STR("abcdefg23deff", ret.value.str.ptr);
}