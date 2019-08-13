/*
code_gen.c

Copyright (c) May 29, 2019 Manwoo Yi

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

/**************************
 * Include system headers
 **************************/
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

/**************************
 * Include project headers
 **************************/
#include "object.h"
#include "code_gen.h"
#include "symtab.h"
#include "ported_lib.h"

/**************************
 * Data types, Constants
 **************************/
static struct _generated_code_status_t_ {
    code_buf_t* buffer;
    uint32_t    len;
    uint32_t    limit;
} s_generated_code = {0};

/**************************
 * Private function prototypes
 **************************/
static void check_code_buffer(void);
static char* trim_quote_in_memory(char* orig_str);

/**************************
 * Public functions
 **************************/
void CodeGen_reset_bytecodes(void)
{
    s_generated_code.len = 0;
}

code_buf_t* CodeGen_get_bytecodes(void)
{
    return s_generated_code.buffer;
}

code_buf_t* CodeGen_current_bytecode_ptr(void)
{
    return &s_generated_code.buffer[s_generated_code.len];
}

void CodeGen_skip_bytecode_count(uint32_t count)
{
    while(count--)
    {
        check_code_buffer();
        s_generated_code.buffer[s_generated_code.len].type = code_buf_opcode;
        s_generated_code.buffer[s_generated_code.len].bytecode.opcode = opcode_nop;    
        s_generated_code.len++;
    }
}

void CodeGen_add_opcode(opcode_t opcode)
{
    check_code_buffer();
    s_generated_code.buffer[s_generated_code.len].type = code_buf_opcode;
    s_generated_code.buffer[s_generated_code.len].bytecode.opcode = opcode;
    s_generated_code.len++;
}

// cppcheck-suppress unusedFunction
void CodeGen_add_number(double number)
{
    object_t number_obj = {0};

    number_obj.type = object_type_number;
    number_obj.value.number = number;

    check_code_buffer();
    CodeGen_add_opcode(opcode_push);
    s_generated_code.buffer[s_generated_code.len].type = code_buf_value;
    s_generated_code.buffer[s_generated_code.len].bytecode.value = number_obj;
    s_generated_code.len++;
}

// cppcheck-suppress unusedFunction
void CodeGen_add_string(char* str_ptr)
{
    object_t string_obj = {0};

    str_ptr = trim_quote_in_memory(str_ptr);    

    string_obj.type = object_type_string;
    string_obj.value.str.ptr = Symtab_add_string_literal(str_ptr, &string_obj.value.str.table_idx);

    check_code_buffer();
    CodeGen_add_opcode(opcode_push);
    s_generated_code.buffer[s_generated_code.len].type = code_buf_value;
    s_generated_code.buffer[s_generated_code.len].bytecode.value = string_obj;
    s_generated_code.len++;
}

// cppcheck-suppress unusedFunction
void CodeGen_add_variable(const char* type_str, const char* ident_str)
{
    uint32_t type_tab_idx = Symtab_get_idx_of_type(type_str);
    uint32_t literal_index = 0;
    Symtab_add_string_literal(ident_str, &literal_index);

    object_t general_obj = {0};
    general_obj.type = object_type_general;
    general_obj.value.general = ((uint64_t)type_tab_idx << 32) | (literal_index);

    check_code_buffer();
    s_generated_code.buffer[s_generated_code.len].type = code_buf_value;
    s_generated_code.buffer[s_generated_code.len].bytecode.value = general_obj;
    s_generated_code.len++;
}

void CodeGen_read_symtab_variable(const char* ident_str)
{
    uint32_t literal_index = 0;
    Symtab_add_string_literal(ident_str, &literal_index);
    
    object_t general_obj = {0};
    general_obj.type = object_type_general;
    general_obj.value.general = literal_index;

    check_code_buffer();
    s_generated_code.buffer[s_generated_code.len].type = code_buf_value;
    s_generated_code.buffer[s_generated_code.len].bytecode.value = general_obj;
    s_generated_code.len++;
}

void CodeGen_modify_codebuf(code_buf_t* dst, code_buf_t src)
{
    *dst = src;
}

/**************************
 * Private functions
 **************************/
static void check_code_buffer(void)
{
    const uint32_t one_time_buffer_unit = 4096;

    s_generated_code.buffer = (code_buf_t*)limited_malloc(s_generated_code.buffer, 
                                                            sizeof(code_buf_t), 
                                                            one_time_buffer_unit, 
                                                            s_generated_code.len, 
                                                            &s_generated_code.limit);
}

static char* trim_quote_in_memory(char* orig_str)
{
    if (orig_str[0] == '\'' || orig_str[0] == '\"')
    {
        size_t len = strlen(orig_str);
        if (orig_str[len-1] == '\'' || orig_str[len-1] == '\"')
        {
            // trim ending quote
            len -= 1;
            orig_str[len] = '\0';

            // trim beginning quote
            orig_str += 1;
        }
    }

    return orig_str;
}
