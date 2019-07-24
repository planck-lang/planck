/*
05_condition_stmt_test.c

Copyright (c) 7/22/2019, Manwoo Yi

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

REGISTER_SUITE_AUTO(Condition_statement_Test, "05 Condition statement Test")

TESTCASE(01, "grammer test")
{
    char* codeline;
    object_t ret;
    planck_result_t st;

    codeline = "num_t ca = 3";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);

    codeline = "if ca == 3 { \
                    ca = 2;\n \
                    num_t cx = 10;\n \
                }";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);
}