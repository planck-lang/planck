/*
06_while_test.c.c

Copyright (c) 8/6/2019, 10:32:18 PM Manwoo Yi

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

REGISTER_SUITE_AUTO(Condition_statement_Test, "06 while loop Test")

TESTCASE(01, "grammer test")
{
    char* codeline;
    object_t ret;
    planck_result_t st;

    codeline = "num_t caw = 3";
    st = Planck_do_as_stmt(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);

    codeline = "num_t idxw = 0";
    st = Planck_do_as_stmt(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);

    codeline = "while idxw < 10 {\n caw += 1;\n idxw += 1;\n}";
    st = Planck_do_as_stmt(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);

    codeline = "caw";
    st = Planck_do_as_stmt(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);
    ASSERT_EQ_NUM(object_type_number, ret.type);
    ASSERT_EQ_NUM(13, ret.value.number);
}

TESTCASE(02, "break test")
{
    char* codeline;
    object_t ret;
    planck_result_t st;

    codeline = "caw = 3";
    st = Planck_do_as_stmt(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);

    codeline = "idxw = 0";
    st = Planck_do_as_stmt(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);

    codeline = "while idxw < 10 {\n caw += 1;\n if idxw == 5 { caw = 8; break; } idxw += 1;\n}";
    st = Planck_do_as_stmt(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);

    codeline = "caw";
    st = Planck_do_as_stmt(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);
    ASSERT_EQ_NUM(object_type_number, ret.type);
    ASSERT_EQ_NUM(8, ret.value.number);
}