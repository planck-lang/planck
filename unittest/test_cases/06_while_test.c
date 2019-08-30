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

#include "code_gen.h"
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

TESTCASE(03, "continue test")
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

    codeline = "while idxw < 10 {\n caw += 1;\n if idxw == 5 { idxw = 8; continue; } idxw += 1;\n}";
    st = Planck_do_as_stmt(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);

    codeline = "caw";
    st = Planck_do_as_stmt(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);
    ASSERT_EQ_NUM(object_type_number, ret.type);
    ASSERT_EQ_NUM(11, ret.value.number);
}

TESTCASE(04, "single use error")
{
    char* codeline;
    object_t ret;
    planck_result_t st;

    codeline = "break";
    st = Planck_do_as_stmt(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_fail, st);

    char runtime_error_buf[1024] = {0};
    error_code_t s_error_code = Planck_get_error(runtime_error_buf);
    ASSERT_EQ_NUM(error_code_not_in_loop, s_error_code);

    codeline = "continue";
    st = Planck_do_as_stmt(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_fail, st);

    s_error_code = Planck_get_error(runtime_error_buf);
    ASSERT_EQ_NUM(error_code_not_in_loop, s_error_code);
}

TESTCASE(05, "while bytecode")
{
    char* codeline;
    object_t ret;
    planck_result_t st;

    codeline = "while cab < 100 {\n cab += 1;\n}";
    st = Planck_do_as_stmt(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);

    code_buf_t* pc = CodeGen_get_bytecodes();

    ASSERT_EQ_NUM(opcode_begin_loop, pc->bytecode.opcode); pc++;

    code_buf_t* loop_back_addr = pc;

    // cab < 100
    ASSERT_EQ_NUM(opcode_load, pc->bytecode.opcode); pc++;
    ASSERT_EQ_NUM(45, pc->bytecode.value.value.general); pc++;
    ASSERT_EQ_NUM(opcode_push, pc->bytecode.opcode); pc++;
    ASSERT_EQ_NUM(100, pc->bytecode.value.value.number); pc++;
    ASSERT_EQ_NUM(opcode_lt, pc->bytecode.opcode); pc++;

    // while
    ASSERT_EQ_NUM(opcode_cmp, pc->bytecode.opcode); pc++;
    code_buf_t* jmp_addr = pc; pc++;

    // {
    ASSERT_EQ_NUM(opcode_begin_scope, pc->bytecode.opcode); pc++;

    // cab += 1
    ASSERT_EQ_NUM(opcode_load, pc->bytecode.opcode); pc++;
    ASSERT_EQ_NUM(45, pc->bytecode.value.value.general); pc++;
    ASSERT_EQ_NUM(opcode_push, pc->bytecode.opcode); pc++;
    ASSERT_EQ_NUM(1, pc->bytecode.value.value.number); pc++;
    ASSERT_EQ_NUM(opcode_add, pc->bytecode.opcode); pc++;
    ASSERT_EQ_NUM(opcode_store, pc->bytecode.opcode); pc++;
    ASSERT_EQ_NUM(45, pc->bytecode.value.value.general); pc++;

    // }
    ASSERT_EQ_NUM(opcode_end_scope, pc->bytecode.opcode); pc++;

    ASSERT_EQ_NUM(opcode_jmp, pc->bytecode.opcode); pc++;
    int64_t back_offset = loop_back_addr - pc;
    ASSERT_EQ_NUM(back_offset, pc->bytecode.value.value.general); 
    pc++;

    // validate jumping address
    uint64_t offset = pc - jmp_addr;
    ASSERT_EQ_NUM(jmp_addr->bytecode.value.value.general, offset);
    ASSERT_EQ_NUM(opcode_end_loop, pc->bytecode.opcode); pc++;
}

TESTCASE(06, "break bytecode")
{
    char* codeline;
    object_t ret;
    planck_result_t st;

    codeline = "while cab < 10 {\n cab += 1;\n if cab == 5 { cab = 8; break; } cab += 1;\n}";
    st = Planck_do_as_stmt(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);

    code_buf_t* pc = CodeGen_get_bytecodes();

    ASSERT_EQ_NUM(opcode_begin_loop, pc->bytecode.opcode); pc++;
    
    code_buf_t* loop_back_addr = pc;
    // cab < 10
    ASSERT_EQ_NUM(opcode_load, pc->bytecode.opcode); pc++;
    ASSERT_EQ_NUM(45, pc->bytecode.value.value.general); pc++;
    ASSERT_EQ_NUM(opcode_push, pc->bytecode.opcode); pc++;
    ASSERT_EQ_NUM(10, pc->bytecode.value.value.number); pc++;
    ASSERT_EQ_NUM(opcode_lt, pc->bytecode.opcode); pc++;

    // while
    ASSERT_EQ_NUM(opcode_cmp, pc->bytecode.opcode); pc++;
    code_buf_t* jmp_addr = pc; pc++;

    // {
    ASSERT_EQ_NUM(opcode_begin_scope, pc->bytecode.opcode); pc++;

    // cab += 1
    ASSERT_EQ_NUM(opcode_load, pc->bytecode.opcode); pc++;
    ASSERT_EQ_NUM(45, pc->bytecode.value.value.general); pc++;
    ASSERT_EQ_NUM(opcode_push, pc->bytecode.opcode); pc++;
    ASSERT_EQ_NUM(1, pc->bytecode.value.value.number); pc++;
    ASSERT_EQ_NUM(opcode_add, pc->bytecode.opcode); pc++;
    ASSERT_EQ_NUM(opcode_store, pc->bytecode.opcode); pc++;
    ASSERT_EQ_NUM(45, pc->bytecode.value.value.general); pc++;

    //      cab == 5
    ASSERT_EQ_NUM(opcode_load, pc->bytecode.opcode); pc++;
    ASSERT_EQ_NUM(45, pc->bytecode.value.value.general); pc++;
    ASSERT_EQ_NUM(opcode_push, pc->bytecode.opcode); pc++;
    ASSERT_EQ_NUM(5, pc->bytecode.value.value.number); pc++;
    ASSERT_EQ_NUM(opcode_eq, pc->bytecode.opcode); pc++;

    //      if
    ASSERT_EQ_NUM(opcode_cmp, pc->bytecode.opcode); pc++;
    code_buf_t* jmp_addr1 = pc; pc++;

    //      {    
    ASSERT_EQ_NUM(opcode_begin_scope, pc->bytecode.opcode); pc++;

    //          cab = 8
    ASSERT_EQ_NUM(opcode_push, pc->bytecode.opcode); pc++;
    ASSERT_EQ_NUM(8, pc->bytecode.value.value.number); pc++;
    ASSERT_EQ_NUM(opcode_store, pc->bytecode.opcode); pc++;
    ASSERT_EQ_NUM(45, pc->bytecode.value.value.general); pc++;

    //          break
    ASSERT_EQ_NUM(opcode_break, pc->bytecode.opcode); pc++;
    code_buf_t* break_jmp = pc; pc++;

    //      }
    ASSERT_EQ_NUM(opcode_end_scope, pc->bytecode.opcode); pc++;

    ASSERT_EQ_NUM(opcode_nop, pc->bytecode.opcode); pc++;
    ASSERT_EQ_NUM(opcode_nop, pc->bytecode.opcode); pc++;

    // validate jumping address
    uint64_t end_if = pc - jmp_addr1;
    ASSERT_EQ_NUM(jmp_addr1->bytecode.value.value.general, end_if);

    // cab += 1
    ASSERT_EQ_NUM(opcode_load, pc->bytecode.opcode); pc++;
    ASSERT_EQ_NUM(45, pc->bytecode.value.value.general); pc++;
    ASSERT_EQ_NUM(opcode_push, pc->bytecode.opcode); pc++;
    ASSERT_EQ_NUM(1, pc->bytecode.value.value.number); pc++;
    ASSERT_EQ_NUM(opcode_add, pc->bytecode.opcode); pc++;
    ASSERT_EQ_NUM(opcode_store, pc->bytecode.opcode); pc++;
    ASSERT_EQ_NUM(45, pc->bytecode.value.value.general); pc++;

    // }
    ASSERT_EQ_NUM(opcode_end_scope, pc->bytecode.opcode); pc++;

    ASSERT_EQ_NUM(opcode_jmp, pc->bytecode.opcode); pc++;
    int64_t back_offset = loop_back_addr - pc;
    ASSERT_EQ_NUM(back_offset, pc->bytecode.value.value.general); 
    pc++;

    // validate jumping address
    uint64_t out_while = pc - jmp_addr;
    ASSERT_EQ_NUM(jmp_addr->bytecode.value.value.general, out_while);
    int64_t out_break = pc - break_jmp;
    ASSERT_EQ_NUM(break_jmp->bytecode.value.value.general, out_break);
    ASSERT_EQ_NUM(opcode_end_loop, pc->bytecode.opcode); 
    pc++;
}

TESTCASE(07, "continue bytecode")
{
    char* codeline;
    object_t ret;
    planck_result_t st;

    codeline = "while cab < 10 {\n cab += 1;\n if cab == 5 { cab = 8; continue; } cab += 1;\n}";
    st = Planck_do_as_stmt(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);

    code_buf_t* pc = CodeGen_get_bytecodes();

    code_buf_t* out_continue = pc;
    ASSERT_EQ_NUM(opcode_begin_loop, pc->bytecode.opcode); pc++;
    
    code_buf_t* loop_back_addr = pc;
    // cab < 10
    ASSERT_EQ_NUM(opcode_load, pc->bytecode.opcode); pc++;
    ASSERT_EQ_NUM(45, pc->bytecode.value.value.general); pc++;
    ASSERT_EQ_NUM(opcode_push, pc->bytecode.opcode); pc++;
    ASSERT_EQ_NUM(10, pc->bytecode.value.value.number); pc++;
    ASSERT_EQ_NUM(opcode_lt, pc->bytecode.opcode); pc++;

    // while
    ASSERT_EQ_NUM(opcode_cmp, pc->bytecode.opcode); pc++;
    code_buf_t* jmp_addr = pc; pc++;

    // {
    ASSERT_EQ_NUM(opcode_begin_scope, pc->bytecode.opcode); pc++;

    // cab += 1
    ASSERT_EQ_NUM(opcode_load, pc->bytecode.opcode); pc++;
    ASSERT_EQ_NUM(45, pc->bytecode.value.value.general); pc++;
    ASSERT_EQ_NUM(opcode_push, pc->bytecode.opcode); pc++;
    ASSERT_EQ_NUM(1, pc->bytecode.value.value.number); pc++;
    ASSERT_EQ_NUM(opcode_add, pc->bytecode.opcode); pc++;
    ASSERT_EQ_NUM(opcode_store, pc->bytecode.opcode); pc++;
    ASSERT_EQ_NUM(45, pc->bytecode.value.value.general); pc++;

    //      cab == 5
    ASSERT_EQ_NUM(opcode_load, pc->bytecode.opcode); pc++;
    ASSERT_EQ_NUM(45, pc->bytecode.value.value.general); pc++;
    ASSERT_EQ_NUM(opcode_push, pc->bytecode.opcode); pc++;
    ASSERT_EQ_NUM(5, pc->bytecode.value.value.number); pc++;
    ASSERT_EQ_NUM(opcode_eq, pc->bytecode.opcode); pc++;

    //      if
    ASSERT_EQ_NUM(opcode_cmp, pc->bytecode.opcode); pc++;
    code_buf_t* jmp_addr1 = pc; pc++;

    //      {    
    ASSERT_EQ_NUM(opcode_begin_scope, pc->bytecode.opcode); pc++;

    //          cab = 8
    ASSERT_EQ_NUM(opcode_push, pc->bytecode.opcode); pc++;
    ASSERT_EQ_NUM(8, pc->bytecode.value.value.number); pc++;
    ASSERT_EQ_NUM(opcode_store, pc->bytecode.opcode); pc++;
    ASSERT_EQ_NUM(45, pc->bytecode.value.value.general); pc++;

    //          continue
    ASSERT_EQ_NUM(opcode_continue, pc->bytecode.opcode); pc++;
    int64_t continue_jmp = out_continue - pc;
    ASSERT_EQ_NUM(pc->bytecode.value.value.general, continue_jmp);
    pc++;

    //      }
    ASSERT_EQ_NUM(opcode_end_scope, pc->bytecode.opcode); pc++;

    ASSERT_EQ_NUM(opcode_nop, pc->bytecode.opcode); pc++;
    ASSERT_EQ_NUM(opcode_nop, pc->bytecode.opcode); pc++;

    // validate jumping address
    uint64_t end_if = pc - jmp_addr1;
    ASSERT_EQ_NUM(jmp_addr1->bytecode.value.value.general, end_if);

    // cab += 1
    ASSERT_EQ_NUM(opcode_load, pc->bytecode.opcode); pc++;
    ASSERT_EQ_NUM(45, pc->bytecode.value.value.general); pc++;
    ASSERT_EQ_NUM(opcode_push, pc->bytecode.opcode); pc++;
    ASSERT_EQ_NUM(1, pc->bytecode.value.value.number); pc++;
    ASSERT_EQ_NUM(opcode_add, pc->bytecode.opcode); pc++;
    ASSERT_EQ_NUM(opcode_store, pc->bytecode.opcode); pc++;
    ASSERT_EQ_NUM(45, pc->bytecode.value.value.general); pc++;

    // }
    ASSERT_EQ_NUM(opcode_end_scope, pc->bytecode.opcode); pc++;

    ASSERT_EQ_NUM(opcode_jmp, pc->bytecode.opcode); pc++;
    int64_t back_offset = loop_back_addr - pc;
    ASSERT_EQ_NUM(back_offset, pc->bytecode.value.value.general); 
    pc++;

    // validate jumping address
    uint64_t out_while = pc - jmp_addr;
    ASSERT_EQ_NUM(jmp_addr->bytecode.value.value.general, out_while);
    ASSERT_EQ_NUM(opcode_end_loop, pc->bytecode.opcode); 
    pc++;
}
