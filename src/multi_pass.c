/*
multi_pass.c

Copyright (c) 8/12/2019, Manwoo Yi

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

/**************************
 * Include project headers
 **************************/
#include "multi_pass.h"
#include "code_gen.h"
#include "virtual_machine.h"

/**************************
 * External references
 **************************/ 

/**************************
 * Macro
 **************************/
  
/**************************
 * Data types, Constants
 **************************/
#define LOOP_DEPTH (128)

typedef struct _loop_stack_item_ {
    code_buf_t* begin_addr;
    code_buf_t* end_addr;
} loop_stack_item_t;

typedef struct _loop_stack_ {
    loop_stack_item_t stack[LOOP_DEPTH];
    uint32_t loop_count;
} loop_stack_t;

/**************************
 * Private variables
 **************************/
static loop_stack_t s_loop_stack = {0};

/**************************
 * Private function prototypes
 **************************/
static void Check_loop(code_buf_t* pc);
static bool Check_break_continue(code_buf_t* pc);

/**************************
 * Public functions
 **************************/
void MultiPass_2nd_pass(code_buf_t* base_addr)
{
    s_loop_stack.loop_count = 0;

    code_buf_t* pc = base_addr;

    while (true)
    {
        if (code_buf_opcode == pc->type)
        {
            if (opcode_halt == pc->bytecode.opcode)
            {
                break;
            }
            Check_loop(pc);
        }
        pc++;
    }
}

bool MultiPass_3rd_pass(code_buf_t* base_addr)
{
    s_loop_stack.loop_count = 0;

    code_buf_t* pc = base_addr;

    while (true)
    {
        if (code_buf_opcode == pc->type)
        {
            if (opcode_halt == pc->bytecode.opcode)
            {
                break;
            }
            if (false == Check_break_continue(pc))
            {
                return false;
            }
        }
        pc++;
    }

    return true;
}

/**************************
 * Private functions
 **************************/
static void Check_loop(code_buf_t* pc)
{
    if (opcode_begin_loop == pc->bytecode.opcode)
    {
        s_loop_stack.loop_count++;
        s_loop_stack.stack[s_loop_stack.loop_count].begin_addr = pc;
    }
    else if (opcode_end_loop == pc->bytecode.opcode)
    {
        s_loop_stack.stack[s_loop_stack.loop_count].end_addr = pc;
        s_loop_stack.loop_count--;
    }
}

static bool Check_break_continue(code_buf_t* pc)
{
    int64_t offset = 0;
    code_buf_t jmp_addr = {0};

    if (opcode_begin_loop == pc->bytecode.opcode)
    {
        s_loop_stack.loop_count++;
    }
    else if (opcode_continue == pc->bytecode.opcode)
    {
        if (s_loop_stack.loop_count == 0)
        {
            VirtualMachine_add_error_msg(error_code_not_in_loop);
            return false;
        }

        pc++;
        offset = s_loop_stack.stack[s_loop_stack.loop_count].begin_addr - pc;
        jmp_addr = CodeGen_gen_general_bytecode(offset);
        CodeGen_modify_codebuf(pc, jmp_addr);
    }
    else if (opcode_break == pc->bytecode.opcode)
    {
        if (s_loop_stack.loop_count == 0)
        {
            VirtualMachine_add_error_msg(error_code_not_in_loop);
            return false;
        }

        pc++;
        offset = s_loop_stack.stack[s_loop_stack.loop_count].end_addr - pc;
        jmp_addr = CodeGen_gen_general_bytecode(offset);
        CodeGen_modify_codebuf(pc, jmp_addr);
    }
    else if (opcode_end_loop == pc->bytecode.opcode)
    {
        s_loop_stack.loop_count--;
    }

    return true;
}