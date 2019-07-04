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
