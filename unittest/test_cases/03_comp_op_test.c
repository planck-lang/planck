/*
03_comp_op_test.c

Copyright (c) 6/26/2019 Manwoo Yi

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

REGISTER_SUITE_AUTO(Comp_OP_Test, "03 Comparison Operation Test")

TESTCASE(01, "less than")
{
    char* codeline = "3 < 4";

    object_t ret;
    planck_result_t st = Planck_do_as_stmt(codeline, &ret);

    ASSERT_EQ_NUM(planck_result_ok, st);
    ASSERT_EQ_NUM(true, ret.value.boolean);

    codeline = "323 < 3";
    st = Planck_do_as_stmt(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);
    ASSERT_EQ_NUM(false, ret.value.boolean);
}

TESTCASE(02, "grater than")
{
    char* codeline = "3 > 4";

    object_t ret;
    planck_result_t st = Planck_do_as_stmt(codeline, &ret);

    ASSERT_EQ_NUM(planck_result_ok, st);
    ASSERT_EQ_NUM(false, ret.value.boolean);

    codeline = "323 > 3";
    st = Planck_do_as_stmt(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);
    ASSERT_EQ_NUM(true, ret.value.boolean);
}

TESTCASE(03, "less equal")
{
    char* codeline = "3 <= 3";

    object_t ret;
    planck_result_t st = Planck_do_as_stmt(codeline, &ret);

    ASSERT_EQ_NUM(planck_result_ok, st);
    ASSERT_EQ_NUM(true, ret.value.boolean);

    codeline = "323 <= 3";
    st = Planck_do_as_stmt(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);
    ASSERT_EQ_NUM(false, ret.value.boolean);
}

TESTCASE(04, "grater equal")
{
    char* codeline = "3 >= 3";

    object_t ret;
    planck_result_t st = Planck_do_as_stmt(codeline, &ret);

    ASSERT_EQ_NUM(planck_result_ok, st);
    ASSERT_EQ_NUM(true, ret.value.boolean);

    codeline = "323 >= 3";
    st = Planck_do_as_stmt(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);
    ASSERT_EQ_NUM(true, ret.value.boolean);
}

TESTCASE(05, "equal")
{
    char* codeline = "3 == 1 + 2";

    object_t ret;
    planck_result_t st = Planck_do_as_stmt(codeline, &ret);

    ASSERT_EQ_NUM(planck_result_ok, st);
    ASSERT_EQ_NUM(true, ret.value.boolean);

    codeline = "323 == 3";
    st = Planck_do_as_stmt(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);
    ASSERT_EQ_NUM(false, ret.value.boolean);
}

TESTCASE(06, "not equal")
{
    char* codeline = "3 != 1 + 2";

    object_t ret;
    planck_result_t st = Planck_do_as_stmt(codeline, &ret);

    ASSERT_EQ_NUM(planck_result_ok, st);
    ASSERT_EQ_NUM(false, ret.value.boolean);

    codeline = "323 != 3";
    st = Planck_do_as_stmt(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);
    ASSERT_EQ_NUM(true, ret.value.boolean);
}

TESTCASE(07, "string comparison")
{
    char* codeline = "'ab' > 'bc'";

    object_t ret;
    planck_result_t st = Planck_do_as_stmt(codeline, &ret);

    ASSERT_EQ_NUM(planck_result_ok, st);
    ASSERT_EQ_NUM(false, ret.value.boolean);

    codeline = "'ab' < 'bc'";
    st = Planck_do_as_stmt(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);
    ASSERT_EQ_NUM(true, ret.value.boolean);

    codeline = "'ab' <= 'ab'";
    st = Planck_do_as_stmt(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);
    ASSERT_EQ_NUM(true, ret.value.boolean);

    codeline = "'ab' >= 'ab'";
    st = Planck_do_as_stmt(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);
    ASSERT_EQ_NUM(true, ret.value.boolean);

    codeline = "'ab' == 'ab'";
    st = Planck_do_as_stmt(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);
    ASSERT_EQ_NUM(true, ret.value.boolean);

    codeline = "'ab' != 'ab'";
    st = Planck_do_as_stmt(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);
    ASSERT_EQ_NUM(false, ret.value.boolean);

    codeline = "'ab' != 'de'";
    st = Planck_do_as_stmt(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);
    ASSERT_EQ_NUM(true, ret.value.boolean);
}

TESTCASE(08, "and comparison")
{
    char* codeline = "3> 2 AND 4 > 3";

    object_t ret;
    planck_result_t st = Planck_do_as_stmt(codeline, &ret);

    ASSERT_EQ_NUM(planck_result_ok, st);
    ASSERT_EQ_NUM(true, ret.value.boolean);

    codeline = "3> 2 AND 4 < 3";
    st = Planck_do_as_stmt(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);
    ASSERT_EQ_NUM(false, ret.value.boolean);

    codeline = "3 < 2 AND 4 > 3";
    st = Planck_do_as_stmt(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);
    ASSERT_EQ_NUM(false, ret.value.boolean);
}

TESTCASE(09, "or comparison")
{
    char* codeline = "3> 2 OR 4 > 3";

    object_t ret;
    planck_result_t st = Planck_do_as_stmt(codeline, &ret);

    ASSERT_EQ_NUM(planck_result_ok, st);
    ASSERT_EQ_NUM(true, ret.value.boolean);

    codeline = "3> 2 OR 4 < 3";
    st = Planck_do_as_stmt(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);
    ASSERT_EQ_NUM(true, ret.value.boolean);

    codeline = "3 < 2 OR 4 > 3";
    st = Planck_do_as_stmt(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);
    ASSERT_EQ_NUM(true, ret.value.boolean);

    codeline = "3 < 2 OR 4 < 3";
    st = Planck_do_as_stmt(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);
    ASSERT_EQ_NUM(false, ret.value.boolean);
}

TESTCASE(10, "mixed and or")
{
    char* codeline = "3+ 4 == 7 AND 4+2==6";

    object_t ret;
    planck_result_t st = Planck_do_as_stmt(codeline, &ret);

    ASSERT_EQ_NUM(planck_result_ok, st);
    ASSERT_EQ_NUM(true, ret.value.boolean);

    codeline = "3+ 4 == 7 AND 4+3==6";
    st = Planck_do_as_stmt(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);
    ASSERT_EQ_NUM(false, ret.value.boolean);

    codeline = "3+ 4 == 7 OR 4+3==6";
    st = Planck_do_as_stmt(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);
    ASSERT_EQ_NUM(true, ret.value.boolean);
}
