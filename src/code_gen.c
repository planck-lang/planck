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

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "object.h"
#include "virtual_machine.h"
#include "code_gen.h"
#include "symtab.h"

static struct _generated_code_status_t_ {
    code_buf_t* buffer;
    uint32_t    len;
    uint32_t    limit;
} s_generated_code = {0};

static void check_code_buffer(void);

void CodeGen_reset_bytecodes(void)
{
    s_generated_code.len = 0;
}

code_buf_t* CodeGen_get_bytecodes(void)
{
    return s_generated_code.buffer;
}

void CodeGen_add_opcode(opcode_t opcode)
{
    check_code_buffer();
    s_generated_code.buffer[s_generated_code.len++].opcode = opcode;
}

void CodeGen_add_number(double number)
{
    object_t number_obj = {0};

    number_obj.type = object_type_number;
    number_obj.value.number = number;

    check_code_buffer();
    CodeGen_add_opcode(opcode_push);
    s_generated_code.buffer[s_generated_code.len++].value = number_obj;
}

void CodeGen_add_string(char* str_ptr)
{
    object_t string_obj = {0};

    string_obj.type = object_type_string;
    string_obj.value.string_ptr = Symtab_add_string_literal(str_ptr);

    check_code_buffer();
    CodeGen_add_opcode(opcode_push);
    s_generated_code.buffer[s_generated_code.len++].value = string_obj;
}

static void check_code_buffer(void)
{
    const uint32_t one_time_buffer_uint = 4096;
    const uint32_t margin = 4;

    if (s_generated_code.buffer == 0)
    {
        s_generated_code.buffer = (code_buf_t*)malloc(sizeof(code_buf_t) * one_time_buffer_uint);
        s_generated_code.limit = one_time_buffer_uint;
    }

    if (s_generated_code.len >= (s_generated_code.limit - margin))
    {
        uint32_t new_size = s_generated_code.limit + one_time_buffer_uint;
        s_generated_code.buffer = (code_buf_t*)realloc(s_generated_code.buffer, (sizeof(code_buf_t) * new_size));
        s_generated_code.limit = new_size;
    }
}
