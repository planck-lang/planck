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

#include "virtual_machine.h"
#include "code_gen.h"

static struct _generated_code_status_t_ {
    uint64_t*   buffer;
    uint32_t    len;
    uint32_t    limit;
} s_generated_code = {0};


static void check_code_buffer(void);


void CodeGen_add_opcode(opcode_t opcode)
{
    check_code_buffer();
    s_generated_code.buffer[s_generated_code.len++] = (uint64_t)opcode;
    printf("opcode - %d\n", (int)opcode);
}

void CodeGen_add_number(double number)
{
    check_code_buffer();
    CodeGen_add_opcode(opcode_push);
    memcpy(&s_generated_code.buffer[s_generated_code.len++], &number, sizeof(double));;
    printf("addnumber - %f\n", number);
}

static void check_code_buffer(void)
{
    const uint32_t one_time_buffer_uint = 4096;
    const uint32_t margin = 16;

    if (s_generated_code.buffer == 0)
    {
        s_generated_code.buffer = (uint64_t*)malloc(sizeof(uint64_t) * one_time_buffer_uint);
        s_generated_code.limit = one_time_buffer_uint;
    }

    if (s_generated_code.len >= (s_generated_code.limit - margin))
    {
        uint32_t new_size = s_generated_code.limit + one_time_buffer_uint;
        s_generated_code.buffer = (uint64_t*)realloc(s_generated_code.buffer, (sizeof(uint64_t) * new_size));
        s_generated_code.limit = new_size;
    }
}
