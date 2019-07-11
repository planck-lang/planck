/*
04_variable_test.c

Copyright (c) 7/1/2019 Manwoo Yi

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

REGISTER_SUITE_AUTO(Variable_Test, "04 Variable Test")

TESTCASE(01, "basic assign")
{
    char* codeline = "num_t a = 3;";

    object_t ret;
    bool st = Planck_do(codeline, &ret);

    ASSERT_EQ_NUM(true, st);
    ASSERT_EQ_NUM(object_type_null, ret.type);

    codeline = "a";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(true, st);
    ASSERT_EQ_NUM(object_type_number, ret.type);
    ASSERT_EQ_NUM(3, ret.value.number);
}

TESTCASE(02, "declaration duplication")
{
    char* codeline = "num_t b = 3;";

    object_t ret;
    bool st = Planck_do(codeline, &ret);

    ASSERT_EQ_NUM(true, st);
    ASSERT_EQ_NUM(object_type_null, ret.type);

    codeline = "num_t b = 10;";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(false, st);
}

TESTCASE(03, "type miss match")
{
    char* codeline;    
    object_t ret;
    bool st;
    
    codeline = "str_t bs = 3;";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(false, st);

    codeline = "str_t bs = 'fdoksem'";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(true, st);
    ASSERT_EQ_NUM(object_type_null, ret.type);

    codeline = "bs";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(true, st);
    ASSERT_EQ_STR("fdoksem", ret.value.str.ptr);
}

TESTCASE(04, "undefined type")
{
    char* codeline;    
    object_t ret;
    bool st;
    
    codeline = "slgkje_t kls = 3;";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(false, st);

    char runtime_error_buf[1024] = {0};
    error_code_t s_error_code = Planck_get_error(runtime_error_buf);
    ASSERT_EQ_NUM(error_code_undefined_type, s_error_code);
}

TESTCASE(05, "assignment")
{
    char* codeline;    
    object_t ret;
    bool st;
    
    codeline = "num_t at = 3";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(true, st);

    codeline = "at";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(true, st);
    ASSERT_EQ_NUM(object_type_number, ret.type);
    ASSERT_EQ_NUM(3, ret.value.number);

    codeline = "at = 3483";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(true, st);
    ASSERT_EQ_NUM(object_type_null, ret.type);

    codeline = "at";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(true, st);
    ASSERT_EQ_NUM(object_type_number, ret.type);
    ASSERT_EQ_NUM(3483, ret.value.number);
}

// TODO:
// string assignment
// assignment symbol to symbol
// assignment type missmatch
// assignment undefined symbol