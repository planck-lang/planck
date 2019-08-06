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

    codeline = "if ca == 3 {\n\tca = 2;\n\tnum_t cx = 10;\n}";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);
}

TESTCASE(02, "separated input line")
{
    char* codeline;
    object_t ret;
    planck_result_t st;

    codeline = "if ca == 4";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_block_input, st);

    codeline = "{";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_block_input, st);

    codeline = "   ca = 5;";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_block_input, st);

    codeline = "   ca = 7;";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_block_input, st);

    codeline = "   str_t k3lxi='kk3kxl';";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_block_input, st);

    codeline = "}";
    st = Planck_do(codeline, &ret);

    if (planck_result_ok != st)
    {
        char runtime_error_buf[1024] = {0};
        Planck_get_error(runtime_error_buf);
        printf("--> %s", runtime_error_buf);
    }

    ASSERT_EQ_NUM(planck_result_ok, st);
}

TESTCASE(03, "if block result")
{
    char* codeline;
    object_t ret;
    planck_result_t st;

    codeline = "ca";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);
    ASSERT_EQ_NUM(object_type_number, ret.type);
    ASSERT_EQ_NUM(2, ret.value.number);
}

TESTCASE(04, "scope test")
{
    char* codeline;
    object_t ret;
    planck_result_t st;

    codeline = "k3lxi";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_fail, st);

    codeline = "str_t k3lxi='tttake'";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);
}

TESTCASE(05, "scope test2")
{
    char* codeline;
    object_t ret;
    planck_result_t st;

    codeline = "num_t can = 3";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);

    codeline = "if can == 3 {\n\tcan = 2;\n\tnum_t cx = 10;\ncan=cx;}";
    st = Planck_do(codeline, &ret);
    if (planck_result_ok != st)
    {
        char runtime_error_buf[1024] = {0};
        Planck_get_error(runtime_error_buf);
        printf("--> %s", runtime_error_buf);
    }
    ASSERT_EQ_NUM(planck_result_ok, st);
    
    codeline = "can";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);
    ASSERT_EQ_NUM(object_type_number, ret.type);
    ASSERT_EQ_NUM(10, ret.value.number);
    
    codeline = "cx";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_fail, st);
    
    codeline = "num_t cx = 3423";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);
    
    codeline = "cx";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);
    ASSERT_EQ_NUM(object_type_number, ret.type);
    ASSERT_EQ_NUM(3423, ret.value.number);
}

TESTCASE(06, "else test")
{
    char* codeline;
    object_t ret;
    planck_result_t st;

    codeline = "num_t eel = 3";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);

    codeline = "if can == 3 {\n \
                    eel = 5;\n  \
                } else {\n      \
                    eel = 10;\n \
                }";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);

    codeline = "eel";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);
    ASSERT_EQ_NUM(object_type_number, ret.type);
    ASSERT_EQ_NUM(10, ret.value.number);
}

TESTCASE(07, "else test2")
{
    char* codeline;
    object_t ret;
    planck_result_t st;

    codeline = "if can != 3 {\n \
                    eel = 5;\n  \
                } else {\n      \
                    eel = 10;\n \
                }";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);

    codeline = "eel";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);
    ASSERT_EQ_NUM(object_type_number, ret.type);
    ASSERT_EQ_NUM(5, ret.value.number);
}

TESTCASE(08, "if-elif-elif-else")
{
    char* base_test_code;
    char* codeline;
    object_t ret;
    planck_result_t st;

    codeline = "num_t ielelse = 332";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);

    codeline = "if ielelse == 3 {\n \
                    can = 5;\n  \
                } elif ielelse == 10 {\n      \
                    can = 10;\n \
                } else {\n \
                    can = 15;\n \
                }";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);

    base_test_code = (char*)malloc(strlen(codeline) + 1);
    strncpy(base_test_code, codeline, strlen(codeline));

    codeline = "can";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);
    ASSERT_EQ_NUM(object_type_number, ret.type);
    ASSERT_EQ_NUM(15, ret.value.number);

    // Modify the first line to if ielelse != 3 {
    base_test_code[11] = '!';
    st = Planck_do(base_test_code, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);
    codeline = "can";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);
    ASSERT_EQ_NUM(object_type_number, ret.type);
    ASSERT_EQ_NUM(5, ret.value.number);

    // Modify the second comparison to elif ielelse != 10
    base_test_code[11] = '=';   // restore original op
    base_test_code[81] = '!';
    st = Planck_do(base_test_code, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);
    codeline = "can";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);
    ASSERT_EQ_NUM(object_type_number, ret.type);
    ASSERT_EQ_NUM(10, ret.value.number);

    free(base_test_code);
}

TESTCASE(09, "if { if - elif - elif - else }")
{
    char* base_test_code;
    char* codeline;
    object_t ret;
    planck_result_t st;

    codeline = "num_t ififeeelse = 8283";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);

    codeline = "if ififeeelse == 29 { \n \
                    if ielelse == 3 {\n \
                        can = 51;\n  \
                    } elif ielelse == 10 {\n      \
                        can = 101;\n \
                    } else {\n \
                        can = 151;\n \
                    }\n \
                }";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);

    base_test_code = (char*)malloc(strlen(codeline) + 1);
    strncpy(base_test_code, codeline, strlen(codeline));

    codeline = "can";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);
    ASSERT_EQ_NUM(object_type_number, ret.type);
    ASSERT_EQ_NUM(10, ret.value.number);

    // Modify the first line to if ififeeelse != 29 {
    base_test_code[14] = '!';
    st = Planck_do(base_test_code, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);
    codeline = "can";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);
    ASSERT_EQ_NUM(object_type_number, ret.type);
    ASSERT_EQ_NUM(151, ret.value.number);

    // Modify the second comparison to if ielelse != 3 {
    base_test_code[55] = '!';
    st = Planck_do(base_test_code, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);
    codeline = "can";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);
    ASSERT_EQ_NUM(object_type_number, ret.type);
    ASSERT_EQ_NUM(51, ret.value.number);

    free(base_test_code);
}