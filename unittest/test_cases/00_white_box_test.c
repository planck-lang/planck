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
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);

    codeline = "if cab == 4 {\n\tcab = 2;\n\tnum_t cxb = 10;\n\tcxb += 1;\n}";
    st = Planck_do(codeline, &ret);
    ASSERT_EQ_NUM(planck_result_ok, st);

    code_buf_t* pc = CodeGen_get_bytecodes();
    
    // cab == 4
    ASSERT_EQ_NUM(opcode_load, pc->opcode); pc++;
    ASSERT_EQ_NUM(1, pc->value.value.general); pc++;
    ASSERT_EQ_NUM(opcode_push, pc->opcode); pc++;
    ASSERT_EQ_NUM(4, pc->value.value.number); pc++;
    ASSERT_EQ_NUM(opcode_eq, pc->opcode); pc++;

    // if
    ASSERT_EQ_NUM(opcode_cmp, pc->opcode); pc++;
    code_buf_t* jmp_addr = pc; pc++;

    // cab = 2
    ASSERT_EQ_NUM(opcode_push, pc->opcode); pc++;
    ASSERT_EQ_NUM(2, pc->value.value.number); pc++;
    ASSERT_EQ_NUM(opcode_store, pc->opcode); pc++;
    ASSERT_EQ_NUM(1, pc->value.value.general); pc++;
    
    // num_t cxb = 10
    ASSERT_EQ_NUM(opcode_push, pc->opcode); pc++;
    ASSERT_EQ_NUM(10, pc->value.value.number); pc++;
    ASSERT_EQ_NUM(opcode_decl, pc->opcode); pc++;
    ASSERT_EQ_NUM(1, pc->value.value.general); pc++;

    // cxb += 1
    ASSERT_EQ_NUM(opcode_load, pc->opcode); pc++;
    ASSERT_EQ_NUM(1, pc->value.value.general); pc++;
    ASSERT_EQ_NUM(opcode_push, pc->opcode); pc++;
    ASSERT_EQ_NUM(1, pc->value.value.number); pc++;
    ASSERT_EQ_NUM(opcode_add, pc->opcode); pc++;
    ASSERT_EQ_NUM(opcode_store, pc->opcode); pc++;
    ASSERT_EQ_NUM(1, pc->value.value.general); pc++;

    // validate jumping address
    uint64_t offset = pc - jmp_addr;
    ASSERT_EQ_NUM(jmp_addr->value.value.general, offset);
}
