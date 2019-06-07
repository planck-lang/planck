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

#include "object.h"
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
    code_buf_t* pc;
    code_buf_t* lr;
    uint32_t    sp;
} s_vm_registers = {0};

static struct _vm_stack_t_ {
    object_t*   stack;
    uint32_t    limit;
} s_vm_stack = {0};

static struct _vm_error_status_t_ {
    char*           msg;
    error_code_t    error_code;
} s_vm_error, s_predefined_error[] = {
        {"Two ops are should be same number type.", error_code_type_mismatch},
        {"No error", error_code_no_error}
};

static void init_registers(void);
static void init_stack(void);
static void check_stack(void);

static void     push_stack(object_t value);
static object_t pop_stack(void);
static void     add_error_msg(error_code_t error_code);
static bool     check_no_error(void);

static object_t op_add(object_t op1, object_t op2);
static object_t op_sub(object_t op1, object_t op2);
static object_t op_mul(object_t op1, object_t op2);
static object_t op_div(object_t op1, object_t op2);
static object_t op_mod(object_t op1, object_t op2);

static bool execute_code(void);

bool VirtualMachine_run_vm(code_buf_t* codes)
{
    init_registers();
    init_stack();

    s_vm_registers.pc = codes;
    return execute_code();
}

object_t  VirtualMachine_get_result(void)
{
    return pop_stack();
}

error_code_t VirtualMachine_get_error_msg(uint64_t* out_msg_ptr)
{
    error_code_t ret = s_vm_error.error_code;
    *out_msg_ptr = (uint64_t)s_vm_error.msg;
    memset(&s_vm_error, 0, sizeof(s_vm_error));
    return ret;
}

static void init_registers(void)
{
    memset(&s_vm_registers, 0, sizeof(s_vm_registers));
}

static void init_stack(void)
{
    if (s_vm_stack.stack == 0)
    {
        s_vm_stack.stack = (object_t*)malloc(sizeof(object_t) * BLOCK_STACK_SIZE);
    }
    else
    {
        s_vm_stack.stack = (object_t*)realloc(s_vm_stack.stack, (sizeof(object_t) * BLOCK_STACK_SIZE));
    }

    s_vm_stack.limit = BLOCK_STACK_SIZE;
}

static void check_stack(void)
{
    const uint32_t margin = 4;

    if (s_vm_registers.sp >= (s_vm_stack.limit - margin))
    {
        uint32_t new_size = s_vm_stack.limit + (sizeof(object_t) * BLOCK_STACK_SIZE);
        s_vm_stack.stack = (object_t*)realloc(s_vm_stack.stack, new_size);
        s_vm_stack.limit = new_size;
    }
}

static void push_stack(object_t value)
{
    check_stack();
    s_vm_stack.stack[s_vm_registers.sp++] = value;
    DEBUG_MSG("push sp[%d], t=%d v=%f\n", s_vm_registers.sp, value.type, value.value.number);
}

static object_t pop_stack(void)
{
    object_t value = {0};

    s_vm_registers.sp--;
    value = s_vm_stack.stack[s_vm_registers.sp];

    DEBUG_MSG("pop sp[%d], t=%d v=%f\n", s_vm_registers.sp, value.type, value.value.number);

    return value;
}

static void add_error_msg(error_code_t error_code)
{
    for(uint32_t i = 0 ; i < error_code_NUM; i++)
    {
        if (s_predefined_error[i].error_code == error_code)
        {
            s_vm_error.msg = s_predefined_error[i].msg;
            s_vm_error.error_code = error_code;
        }
    }
}

static bool check_no_error(void)
{
    bool no_error = true;

    if (s_vm_error.error_code != error_code_no_error)
    {
        no_error = false;
    }

    return no_error;
}

static object_t op_add(object_t op1, object_t op2)
{
    object_t ret;

    if (op1.type == object_type_number && op2.type == object_type_number)
    {
        ret.type = object_type_number;
        ret.value.number = op1.value.number + op2.value.number;
    }
    else
    {
        add_error_msg(error_code_type_mismatch);
    }

    DEBUG_MSG("op1 type=%d op2 type=%d\n", op1.type, op2.type);

    return ret;
}

static object_t op_sub(object_t op1, object_t op2)
{
    object_t ret;

    if (op1.type == object_type_number && op2.type == object_type_number)
    {
        ret.type = object_type_number;
        ret.value.number = op1.value.number - op2.value.number;
    }
    else
    {
        add_error_msg(error_code_type_mismatch);
    }

    return ret;
}

static object_t op_mul(object_t op1, object_t op2)
{
    object_t ret;

    if (op1.type == object_type_number && op2.type == object_type_number)
    {
        ret.type = object_type_number;
        ret.value.number = op1.value.number * op2.value.number;
    }
    else
    {
        add_error_msg(error_code_type_mismatch);
    }

    return ret;
}

static object_t op_div(object_t op1, object_t op2)
{
    object_t ret;

    if (op1.type == object_type_number && op2.type == object_type_number)
    {
        ret.type = object_type_number;
        ret.value.number = op1.value.number / op2.value.number;
    }
    else
    {
        add_error_msg(error_code_type_mismatch);
    }

    return ret;
}

static object_t op_mod(object_t op1, object_t op2)
{
    object_t ret;

    if (op1.type == object_type_number && op2.type == object_type_number)
    {
        ret.type = object_type_number;
        ret.value.number = (uint64_t)op1.value.number % (uint64_t)op2.value.number;
    }
    else
    {
        add_error_msg(error_code_type_mismatch);
    }

    return ret;
}

static bool execute_code(void)
{
    code_buf_t* pc = s_vm_registers.pc;

    while(true)
    {
        opcode_t opcode = pc->opcode;
        switch(opcode)
        {
        case opcode_nop:
        {
            pc++;                       // just execute next code
            break;
        }
        case opcode_push:
        {
            pc++;                       // next code has value which will be pushed into stack
            push_stack(pc->value);      // value has been pushed into stack and increase pc to execute next code
            pc++;
            break;
        }
        case opcode_add:
        case opcode_sub:
        case opcode_mul:
        case opcode_div:
        case opcode_mod:
        {
            object_t op2 = pop_stack(); // get value from stack. Pop the 2nd operand first because it is stack.
            object_t op1 = pop_stack(); // get value from stack. Pop the 1st operand.
            object_t ret = {0};
            switch (opcode)             // calculation
            {
            case opcode_add:
                ret = op_add(op1, op2);
                break;
            case opcode_sub:
                ret = op_sub(op1, op2);
                break;
            case opcode_mul:
                ret = op_mul(op1, op2);
                break;
            case opcode_div:
                ret = op_div(op1, op2);
                break;
            case opcode_mod:
                ret = op_mod(op1, op2);
                break;
            default:
                return check_no_error();
            }
            push_stack(ret);            // result value has been pushed into stack
            pc++;                       // increase pc to execute next code
            break;
        }
        case opcode_halt:
        case opcode_MAXNUM:
            return check_no_error();
        }

        if (check_no_error() == false)
        {
            return false;
        }
    }
}
