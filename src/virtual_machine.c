/*
virtual_machine.c

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

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "virtual_machine.h"
#include "code_gen.h"

#define DEBUG_VM    0
#if DEBUG_VM
#define DEBUG_MSG(_f, ...)  printf((_f), __VA_ARGS__);
#else
#define DEBUG_MSG(_f, ...)
#endif

#define BLOCK_STACK_SIZE     4096

static struct _vm_registers_t_ {
    uint64_t*   pc;
    uint64_t*   lr;
    uint32_t    sp;
} s_vm_registers = {0};

static struct _vm_stack_t_ {
    double*   stack;
    uint32_t    limit;
} s_vm_stack = {0};

static void init_registers(void);
static void init_stack(void);
static void check_stack(void);

static void     push_stack(double value);
static double pop_stack(void);

static void execute_code(void);

void VirtualMachine_run_vm(uint64_t* codes)
{
    init_registers();
    init_stack();

    s_vm_registers.pc = codes;
    execute_code();
}

double  VirtualMachine_get_result(void)
{
    return pop_stack();
}

static void init_registers(void)
{
    memset(&s_vm_registers, 0, sizeof(s_vm_registers));
}

static void init_stack(void)
{
    if (s_vm_stack.stack == 0)
    {
        s_vm_stack.stack = (double*)malloc(sizeof(double) * BLOCK_STACK_SIZE);
    }
    else
    {
        s_vm_stack.stack = (double*)realloc(s_vm_stack.stack, (sizeof(double) * BLOCK_STACK_SIZE));
    }

    s_vm_stack.limit = BLOCK_STACK_SIZE;
}

static void check_stack(void)
{
    const uint32_t margin = 4;

    if (s_vm_registers.sp >= (s_vm_stack.limit - margin))
    {
        uint32_t new_size = s_vm_stack.limit + (sizeof(double) * BLOCK_STACK_SIZE);
        s_vm_stack.stack = (double*)realloc(s_vm_stack.stack, new_size);
        s_vm_stack.limit = new_size;
    }
}

static void push_stack(double value)
{
    check_stack();
    s_vm_stack.stack[s_vm_registers.sp++] = value;
    DEBUG_MSG("push sp[%d], v=%f\n", s_vm_registers.sp, value);
}

static double pop_stack(void)
{
    double value = 0;

    s_vm_registers.sp--;
    value = s_vm_stack.stack[s_vm_registers.sp];

    DEBUG_MSG("pop sp[%d], v=%f\n", s_vm_registers.sp, value);

    return value;
}

static void execute_code(void)
{
    uint64_t* pc = s_vm_registers.pc;

    while(true)
    {
        opcode_t opcode = *pc;
        switch(opcode)
        {
        case opcode_nop:
        {
            pc++;                       // just execute next code
            break;
        }
        case opcode_push:
        {
            double value;

            pc++;                       // next code has value which will be pushed into stack
            memcpy(&value, pc, sizeof(double));
            push_stack(value);          // value has been pushed into stack and increase pc to execute next code
            pc++;
            break;
        }
        case opcode_add:
        case opcode_sub:
        case opcode_mul:
        case opcode_div:
        case opcode_mod:
        {
            double op2 = pop_stack(); // get value from stack. Pop the 2nd operand first because it is stack.
            double op1 = pop_stack(); // get value from stack. Pop the 1st operand.
            double ret = 0;
            switch (opcode)             // calculation
            {
            case opcode_add:
                ret = op1 + op2;
                break;
            case opcode_sub:
                ret = op1 - op2;
                break;
            case opcode_mul:
                ret = op1 * op2;
                break;
            case opcode_div:
                ret = op1 / op2;
                break;
            case opcode_mod:
                ret = (uint64_t)op1 % (uint64_t)op2;
                break;
            default:
                return;
            }
            push_stack(ret);            // result value has been pushed into stack
            pc++;                       // increase pc to execute next code
            break;
        }
        case opcode_halt:
        case opcode_MAXNUM:
            return;
        }
    }
}
