/*
07_array_test.c

Copyright (c) 8/16/2019 Manwoo Yi

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

REGISTER_SUITE_AUTO(Condition_statement_Test, "07 Array Test")

TESTCASE(01, "array of number test")
{
    char* codeline;
    object_t ret;
    planck_result_t st;

    codeline = "num_t[3] arr = [1,2,3]";
    st = Planck_do_as_stmt(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);

    codeline = "arr[1]";
    st = Planck_do_as_stmt(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);
    ASSERT_EQ_NUM(object_type_number, ret.type);
    ASSERT_EQ_NUM(2, ret.value.number);

    codeline = "num_t idx = 2";
    st = Planck_do_as_stmt(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);

    codeline = "arr[idx]";
    st = Planck_do_as_stmt(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);
    ASSERT_EQ_NUM(object_type_number, ret.type);
    ASSERT_EQ_NUM(3, ret.value.number);
}

TESTCASE(02, "array of string test")
{
    char* codeline;
    object_t ret;
    planck_result_t st;

    codeline = "str_t[3] arrstr = ['hi', 'hello', 'annyung']";
    st = Planck_do_as_stmt(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);

    codeline = "arrstr[2]";
    st = Planck_do_as_stmt(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);
    ASSERT_EQ_NUM(object_type_string, ret.type);
    ASSERT_EQ_STR("annyung", ret.value.str.ptr);
}

TESTCASE(03, "multi dimension test")
{
    char* codeline;
    object_t ret;
    planck_result_t st;

    codeline = "num_t[2][2] arrm = [[1,2], [3,4]]";
    st = Planck_do_as_stmt(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);

    codeline = "arrm[1][1]";
    st = Planck_do_as_stmt(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);
    ASSERT_EQ_NUM(object_type_number, ret.type);
    ASSERT_EQ_NUM(3, ret.value.number);

    codeline = "idx = 1";
    st = Planck_do_as_stmt(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);

    codeline = "arrm[0][idx]";
    st = Planck_do_as_stmt(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);
    ASSERT_EQ_NUM(object_type_number, ret.type);
    ASSERT_EQ_NUM(2, ret.value.number);
}

TESTCASE(04, "oversize test")
{
    char* codeline;
    object_t ret;
    planck_result_t st;

    codeline = "num_t[10] arr_over = [1,2,3]";
    st = Planck_do_as_stmt(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);

    codeline = "arr_over[2]";
    st = Planck_do_as_stmt(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);
    ASSERT_EQ_NUM(object_type_number, ret.type);
    ASSERT_EQ_NUM(3, ret.value.number);

    codeline = "arr_over[7]";
    st = Planck_do_as_stmt(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);
    ASSERT_EQ_NUM(object_type_null, ret.type);
    ASSERT_EQ_NUM(0, ret.value.number);
}

TESTCASE(05, "oversize test for string")
{
    char* codeline;
    object_t ret;
    planck_result_t st;

    codeline = "str_t[14] arrstr_over = ['hi', 'hello', 'annyung']";
    st = Planck_do_as_stmt(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);

    codeline = "arrstr_over[2]";
    st = Planck_do_as_stmt(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);
    ASSERT_EQ_NUM(object_type_string, ret.type);
    ASSERT_EQ_STR("annyung", ret.value.str.ptr);

    codeline = "arrstr_over[7]";
    st = Planck_do_as_stmt(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);
    ASSERT_EQ_NUM(object_type_null, ret.type);
    ASSERT_EQ_NUM(0, ret.value.number);
}

TESTCASE(06, "array expression")
{
    char* codeline;
    object_t ret;
    planck_result_t st;

    codeline = "[1,2,3,4,5,6]";
    st = Planck_do_as_stmt(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);

    codeline = "[[1,2],[3,4],[5,6]]";
    st = Planck_do_as_stmt(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);

    codeline = "[[[1,2],[3,4]],[[5,6],[7,8]]]";
    st = Planck_do_as_stmt(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);
}
