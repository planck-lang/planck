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

TESTCASE(06, "string assignment")
{
    char* codeline;
    object_t ret;
    bool st;
    
    codeline = "str_t str = 'asdfg'";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(true, st);

    codeline = "str";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(true, st);
    ASSERT_EQ_NUM(object_type_string, ret.type);
    ASSERT_EQ_STR("asdfg", ret.value.str.ptr);

    codeline = "str = 'asogkel'";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(true, st);
    ASSERT_EQ_NUM(object_type_null, ret.type);

    codeline = "str";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(true, st);
    ASSERT_EQ_NUM(object_type_string, ret.type);
    ASSERT_EQ_STR("asogkel", ret.value.str.ptr);
}

TESTCASE(07, "assignment symbol to symbol")
{
    char* codeline;
    object_t ret;
    bool st;
    
    codeline = "str_t str01 = 'akboekkse'";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(true, st);

    codeline = "str01";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(true, st);
    ASSERT_EQ_NUM(object_type_string, ret.type);
    ASSERT_EQ_STR("akboekkse", ret.value.str.ptr);
    
    codeline = "str";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(true, st);
    ASSERT_EQ_NUM(object_type_string, ret.type);
    ASSERT_EQ_STR("asogkel", ret.value.str.ptr);

    codeline = "str = str01";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(true, st);
    ASSERT_EQ_NUM(object_type_null, ret.type);

    codeline = "str";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(true, st);
    ASSERT_EQ_NUM(object_type_string, ret.type);
    ASSERT_EQ_STR("akboekkse", ret.value.str.ptr);
    
    codeline = "num_t atpp = 33829";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(true, st);

    codeline = "atpp";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(true, st);
    ASSERT_EQ_NUM(object_type_number, ret.type);
    ASSERT_EQ_NUM(33829, ret.value.number);
    
    codeline = "at";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(true, st);
    ASSERT_EQ_NUM(object_type_number, ret.type);
    ASSERT_EQ_NUM(3483, ret.value.number);
    
    codeline = "atpp = at";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(true, st);
    ASSERT_EQ_NUM(object_type_null, ret.type);
    
    codeline = "atpp";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(true, st);
    ASSERT_EQ_NUM(object_type_number, ret.type);
    ASSERT_EQ_NUM(3483, ret.value.number);
}

TESTCASE(08, "assignment type missmatch")
{
    char* codeline;
    object_t ret;
    bool st;
    
    codeline = "atpp = str";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(false, st);
    
    char runtime_error_buf[1024] = {0};
    error_code_t s_error_code = Planck_get_error(runtime_error_buf);
    ASSERT_EQ_NUM(error_code_type_mismatch, s_error_code);
    
    codeline = "str = at";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(false, st);
    
    s_error_code = Planck_get_error(runtime_error_buf);
    ASSERT_EQ_NUM(error_code_type_mismatch, s_error_code);
}

TESTCASE(09, "assignment undefined symbol")
{
    char* codeline;
    object_t ret;
    bool st;
    
    char runtime_error_buf[1024] = {0};
    error_code_t s_error_code;
    
    codeline = "str = atk";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(false, st);
    
    s_error_code = Planck_get_error(runtime_error_buf);
    ASSERT_EQ_NUM(error_code_not_found_symbol, s_error_code);
    
    codeline = "strk = at";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(false, st);
    
    s_error_code = Planck_get_error(runtime_error_buf);
    ASSERT_EQ_NUM(error_code_not_found_symbol, s_error_code);
}

TESTCASE(10, "add assign test")
{
    char* codeline;
    object_t ret;
    bool st;

    codeline = "num_t addassign = 10";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(true, st);

    codeline = "addassign";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(true, st);
    ASSERT_EQ_NUM(object_type_number, ret.type);
    ASSERT_EQ_NUM(10, ret.value.number);

    codeline = "addassign += 110";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(true, st);

    codeline = "addassign";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(true, st);
    ASSERT_EQ_NUM(object_type_number, ret.type);
    ASSERT_EQ_NUM(120, ret.value.number);
}

TESTCASE(11, "op assign test")
{
    char* codeline;
    object_t ret;
    bool st;

    codeline = "num_t opassign = 10";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(true, st);

    codeline = "opassign";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(true, st);
    ASSERT_EQ_NUM(object_type_number, ret.type);
    ASSERT_EQ_NUM(10, ret.value.number);

    // add assignment
    codeline = "opassign += 110";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(true, st);

    codeline = "opassign";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(true, st);
    ASSERT_EQ_NUM(object_type_number, ret.type);
    ASSERT_EQ_NUM(120, ret.value.number);

    // sub assignment
    codeline = "opassign -= 30";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(true, st);

    codeline = "opassign";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(true, st);
    ASSERT_EQ_NUM(object_type_number, ret.type);
    ASSERT_EQ_NUM(90, ret.value.number);

    // mul assignment
    codeline = "opassign *= 4";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(true, st);

    codeline = "opassign";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(true, st);
    ASSERT_EQ_NUM(object_type_number, ret.type);
    ASSERT_EQ_NUM(360, ret.value.number);

    // div assignment
    codeline = "opassign /= 5";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(true, st);

    codeline = "opassign";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(true, st);
    ASSERT_EQ_NUM(object_type_number, ret.type);
    ASSERT_EQ_NUM(72, ret.value.number);

    // mod assignment
    codeline = "opassign %= 7";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(true, st);

    codeline = "opassign";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(true, st);
    ASSERT_EQ_NUM(object_type_number, ret.type);
    ASSERT_EQ_NUM(2, ret.value.number);
}