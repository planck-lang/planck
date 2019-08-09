/*
00_white_box_test.c

Copyright (c) 7/2/2019 Manwoo Yi

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

#include "symtab.h"
#include "code_gen.h"
#include "planck.h"

REGISTER_SUITE_AUTO(WhiteBox_Test, "00 White Box Test")

TESTCASE(01, "literal table pointer")
{
    uint32_t table_idx_a;
    uint32_t table_idx_b;

    char* ab_ptr = Symtab_add_string_literal("ab", &table_idx_a);
    char* bc_ptr = Symtab_add_string_literal("bc", &table_idx_b);
    ASEERT_NEQ_UINT(ab_ptr, bc_ptr);
    ASEERT_NEQ_UINT(table_idx_a, table_idx_b);

    char* ab_again_ptr = Symtab_add_string_literal("ab", &table_idx_b);
    ASSERT_EQ_UINT(ab_ptr, ab_again_ptr);
    ASSERT_EQ_UINT(table_idx_a, table_idx_b);

    table_idx_a = 1;
    table_idx_b = 2;

    char* cd_ptr = Symtab_add_string_literal("cd", &table_idx_a);
    char* cd_again_ptr = Symtab_add_string_literal("cd", &table_idx_b);

    ASEERT_NEQ_UINT(bc_ptr, cd_ptr);
    ASSERT_EQ_UINT(cd_ptr, cd_again_ptr);
    ASSERT_EQ_UINT(table_idx_a, table_idx_b);
}

TESTCASE(02, "if statement bytecode validation")
{
    char* codeline;
    object_t ret;
    planck_result_t st;

    codeline = "num_t cab = 3";
    st = Planck_do_as_stmt(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);

    codeline = "if cab == 4 {\n\tcab = 2;\n\tnum_t cxb = 10;\n\tcxb += 1;\n}";
    st = Planck_do_as_stmt(codeline, &ret);

    if (planck_result_ok != st)
    {
        char runtime_error_buf[1024] = {0};
        Planck_get_error(runtime_error_buf);
        printf("--> %s", runtime_error_buf);
    }

    ASSERT_EQ_NUM(planck_result_ok, st);

    code_buf_t* pc = CodeGen_get_bytecodes();
    
    // cab == 4
    ASSERT_EQ_NUM(opcode_load, pc->opcode); pc++;
    ASSERT_EQ_NUM(4, pc->value.value.general); pc++;
    ASSERT_EQ_NUM(opcode_push, pc->opcode); pc++;
    ASSERT_EQ_NUM(4, pc->value.value.number); pc++;
    ASSERT_EQ_NUM(opcode_eq, pc->opcode); pc++;

    // if
    ASSERT_EQ_NUM(opcode_cmp, pc->opcode); pc++;
    code_buf_t* jmp_addr = pc; pc++;

    // {
    ASSERT_EQ_NUM(opcode_begin_scope, pc->opcode); pc++;
    
    // cab = 2
    ASSERT_EQ_NUM(opcode_push, pc->opcode); pc++;
    ASSERT_EQ_NUM(2, pc->value.value.number); pc++;
    ASSERT_EQ_NUM(opcode_store, pc->opcode); pc++;
    ASSERT_EQ_NUM(4, pc->value.value.general); pc++;
    
    // num_t cxb = 10
    ASSERT_EQ_NUM(opcode_push, pc->opcode); pc++;
    ASSERT_EQ_NUM(10, pc->value.value.number); pc++;
    ASSERT_EQ_NUM(opcode_decl, pc->opcode); pc++;
    ASSERT_EQ_NUM(5, pc->value.value.general); pc++;

    // cxb += 1
    ASSERT_EQ_NUM(opcode_load, pc->opcode); pc++;
    ASSERT_EQ_NUM(5, pc->value.value.general); pc++;
    ASSERT_EQ_NUM(opcode_push, pc->opcode); pc++;
    ASSERT_EQ_NUM(1, pc->value.value.number); pc++;
    ASSERT_EQ_NUM(opcode_add, pc->opcode); pc++;
    ASSERT_EQ_NUM(opcode_store, pc->opcode); pc++;
    ASSERT_EQ_NUM(5, pc->value.value.general); pc++;

    // }
    ASSERT_EQ_NUM(opcode_end_scope, pc->opcode); pc++;

    ASSERT_EQ_NUM(opcode_nop, pc->opcode); pc++;
    ASSERT_EQ_NUM(opcode_nop, pc->opcode); pc++;

    // validate jumping address
    uint64_t offset = pc - jmp_addr;
    ASSERT_EQ_NUM(jmp_addr->value.value.general, offset);
}

TESTCASE(03, "if-else statement bytecode validation")
{
    char* codeline;
    object_t ret;
    planck_result_t st;

    codeline = "if cab != 3 {\n \
                    cab = 5;\n  \
                } else {\n      \
                    cab = 10;\n \
                }";
    st = Planck_do_as_stmt(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);

    code_buf_t* pc = CodeGen_get_bytecodes();

    // cab != 3
    ASSERT_EQ_NUM(opcode_load, pc->opcode); pc++;
    ASSERT_EQ_NUM(4, pc->value.value.general); pc++;
    ASSERT_EQ_NUM(opcode_push, pc->opcode); pc++;
    ASSERT_EQ_NUM(3, pc->value.value.number); pc++;
    ASSERT_EQ_NUM(opcode_ne, pc->opcode); pc++;

    // if
    ASSERT_EQ_NUM(opcode_cmp, pc->opcode); pc++;
    code_buf_t* jmp_addr = pc; pc++;

    // {
    ASSERT_EQ_NUM(opcode_begin_scope, pc->opcode); pc++;

    // cab = 5;
    ASSERT_EQ_NUM(opcode_push, pc->opcode); pc++;
    ASSERT_EQ_NUM(5, pc->value.value.number); pc++;
    ASSERT_EQ_NUM(opcode_store, pc->opcode); pc++;
    ASSERT_EQ_NUM(4, pc->value.value.general); pc++;

    // }
    ASSERT_EQ_NUM(opcode_end_scope, pc->opcode); pc++;

    // when EXPR is true, run the if-block and skip the else-block
    ASSERT_EQ_NUM(opcode_jmp, pc->opcode); pc++;
    code_buf_t* jmp_addr_else = pc; pc++;

    // else
    // when EXPR is false, the jumping destination address is here
    uint64_t offsetelse = pc - jmp_addr;
    ASSERT_EQ_NUM(jmp_addr->value.value.general, offsetelse);

    ASSERT_EQ_NUM(opcode_nop, pc->opcode); pc++;
    ASSERT_EQ_NUM(opcode_nop, pc->opcode); pc++;

    // {
    ASSERT_EQ_NUM(opcode_begin_scope, pc->opcode); pc++;

    // cab = 10;
    ASSERT_EQ_NUM(opcode_push, pc->opcode); pc++;
    ASSERT_EQ_NUM(10, pc->value.value.number); pc++;
    ASSERT_EQ_NUM(opcode_store, pc->opcode); pc++;
    ASSERT_EQ_NUM(4, pc->value.value.general); pc++;

    // }
    ASSERT_EQ_NUM(opcode_end_scope, pc->opcode); pc++;

    ASSERT_EQ_NUM(opcode_nop, pc->opcode); pc++;
    ASSERT_EQ_NUM(opcode_nop, pc->opcode); pc++;

    // validate jumping address
    uint64_t offset = pc - jmp_addr_else;
    ASSERT_EQ_NUM(jmp_addr_else->value.value.general, offset);
}

TESTCASE(04, "if-elif-elif statement bytecode validation")
{
    char* codeline;
    object_t ret;
    planck_result_t st;

    codeline = "if cab == 3 {\n \
                    cab = 5;\n  \
                } elif cab == 4 {\n      \
                    cab = 10;\n \
                } elif cab == 5 {\n \
                    cab = 20;\n \
                }";
    st = Planck_do_as_stmt(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);

    code_buf_t* pc = CodeGen_get_bytecodes();

    // cab == 3
    ASSERT_EQ_NUM(opcode_load, pc->opcode); pc++;
    ASSERT_EQ_NUM(4, pc->value.value.general); pc++;
    ASSERT_EQ_NUM(opcode_push, pc->opcode); pc++;
    ASSERT_EQ_NUM(3, pc->value.value.number); pc++;
    ASSERT_EQ_NUM(opcode_eq, pc->opcode); pc++;

    // if
    ASSERT_EQ_NUM(opcode_cmp, pc->opcode); pc++;
    code_buf_t* jmp_addr1 = pc; pc++;

    // {
    ASSERT_EQ_NUM(opcode_begin_scope, pc->opcode); pc++;

    // cab = 5;
    ASSERT_EQ_NUM(opcode_push, pc->opcode); pc++;
    ASSERT_EQ_NUM(5, pc->value.value.number); pc++;
    ASSERT_EQ_NUM(opcode_store, pc->opcode); pc++;
    ASSERT_EQ_NUM(4, pc->value.value.general); pc++;

    // }
    ASSERT_EQ_NUM(opcode_end_scope, pc->opcode); pc++;

    // when EXPR is true, run the if-block and skip the else-block
    ASSERT_EQ_NUM(opcode_jmp, pc->opcode); pc++;
    code_buf_t* out_addr1 = pc; pc++;

    // elif
    // when EXPR is false, the jumping destination address is here
    uint64_t offset = pc - jmp_addr1;
    ASSERT_EQ_NUM(jmp_addr1->value.value.general, offset);

    // cab == 4
    ASSERT_EQ_NUM(opcode_load, pc->opcode); pc++;
    ASSERT_EQ_NUM(4, pc->value.value.general); pc++;
    ASSERT_EQ_NUM(opcode_push, pc->opcode); pc++;
    ASSERT_EQ_NUM(4, pc->value.value.number); pc++;
    ASSERT_EQ_NUM(opcode_eq, pc->opcode); pc++;

    ASSERT_EQ_NUM(opcode_cmp, pc->opcode); pc++;
    code_buf_t* jmp_addr2 = pc; pc++;

    // {
    ASSERT_EQ_NUM(opcode_begin_scope, pc->opcode); pc++;

    // cab = 10;
    ASSERT_EQ_NUM(opcode_push, pc->opcode); pc++;
    ASSERT_EQ_NUM(10, pc->value.value.number); pc++;
    ASSERT_EQ_NUM(opcode_store, pc->opcode); pc++;
    ASSERT_EQ_NUM(4, pc->value.value.general); pc++;

    // }
    ASSERT_EQ_NUM(opcode_end_scope, pc->opcode); pc++;

    // when EXPR is true, run the if-block and skip the else-block
    ASSERT_EQ_NUM(opcode_jmp, pc->opcode); pc++;
    code_buf_t* out_addr2 = pc; pc++;

    // elif
    // when EXPR is false, the jumping destination address is here
    offset = pc - jmp_addr2;
    ASSERT_EQ_NUM(jmp_addr2->value.value.general, offset);

    // cab == 5
    ASSERT_EQ_NUM(opcode_load, pc->opcode); pc++;
    ASSERT_EQ_NUM(4, pc->value.value.general); pc++;
    ASSERT_EQ_NUM(opcode_push, pc->opcode); pc++;
    ASSERT_EQ_NUM(5, pc->value.value.number); pc++;
    ASSERT_EQ_NUM(opcode_eq, pc->opcode); pc++;

    ASSERT_EQ_NUM(opcode_cmp, pc->opcode); pc++;
    code_buf_t* jmp_addr3 = pc; pc++;

    // {
    ASSERT_EQ_NUM(opcode_begin_scope, pc->opcode); pc++;

    // cab = 20;
    ASSERT_EQ_NUM(opcode_push, pc->opcode); pc++;
    ASSERT_EQ_NUM(20, pc->value.value.number); pc++;
    ASSERT_EQ_NUM(opcode_store, pc->opcode); pc++;
    ASSERT_EQ_NUM(4, pc->value.value.general); pc++;

    // }
    ASSERT_EQ_NUM(opcode_end_scope, pc->opcode); pc++;

    ASSERT_EQ_NUM(opcode_nop, pc->opcode); pc++;
    ASSERT_EQ_NUM(opcode_nop, pc->opcode); pc++;

    offset = pc - jmp_addr3;
    ASSERT_EQ_NUM(jmp_addr3->value.value.general, offset);

    // validate jumping address
    offset = pc - out_addr1;
    ASSERT_EQ_NUM(out_addr1->value.value.general, offset);

    offset = pc - out_addr2;
    ASSERT_EQ_NUM(out_addr2->value.value.general, offset);
}

TESTCASE(07, "while bytecode")
{
    char* codeline;
    object_t ret;
    planck_result_t st;

    codeline = "while cab < 100 {\n cab += 1;\n}";
    st = Planck_do_as_stmt(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);

    code_buf_t* pc = CodeGen_get_bytecodes();

    ASSERT_EQ_NUM(opcode_begin_loop, pc->opcode); pc++;

    code_buf_t* loop_back_addr = pc;

    // cab < 100
    ASSERT_EQ_NUM(opcode_load, pc->opcode); pc++;
    ASSERT_EQ_NUM(4, pc->value.value.general); pc++;
    ASSERT_EQ_NUM(opcode_push, pc->opcode); pc++;
    ASSERT_EQ_NUM(100, pc->value.value.number); pc++;
    ASSERT_EQ_NUM(opcode_lt, pc->opcode); pc++;

    // while
    ASSERT_EQ_NUM(opcode_cmp, pc->opcode); pc++;
    code_buf_t* jmp_addr = pc; pc++;

    // {
    ASSERT_EQ_NUM(opcode_begin_scope, pc->opcode); pc++;

    // cab += 1
    ASSERT_EQ_NUM(opcode_load, pc->opcode); pc++;
    ASSERT_EQ_NUM(4, pc->value.value.general); pc++;
    ASSERT_EQ_NUM(opcode_push, pc->opcode); pc++;
    ASSERT_EQ_NUM(1, pc->value.value.number); pc++;
    ASSERT_EQ_NUM(opcode_add, pc->opcode); pc++;
    ASSERT_EQ_NUM(opcode_store, pc->opcode); pc++;
    ASSERT_EQ_NUM(4, pc->value.value.general); pc++;

    // }
    ASSERT_EQ_NUM(opcode_end_scope, pc->opcode); pc++;

    ASSERT_EQ_NUM(opcode_jmp, pc->opcode); pc++;
    int64_t back_offset = loop_back_addr - pc;
    ASSERT_EQ_NUM(back_offset, pc->value.value.general); 
    pc++;

    // validate jumping address
    uint64_t offset = pc - jmp_addr;
    ASSERT_EQ_NUM(jmp_addr->value.value.general, offset);
    ASSERT_EQ_NUM(opcode_end_loop, pc->opcode); pc++;
}