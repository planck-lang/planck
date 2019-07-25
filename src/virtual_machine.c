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

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "object.h"
#include "virtual_machine.h"
#include "code_gen.h"
#include "expr.h"
#include "symtab.h"
#include "ported_lib.h"

#define DEBUG_VM    0
#if DEBUG_VM
#define DEBUG_MSG(_f, ...)  printf((_f), __VA_ARGS__);
#else
#define DEBUG_MSG(_f, ...)
#endif

#define BLOCK_STACK_SIZE     4096

static struct _vm_registers_t_ {
    code_buf_t* pc;
    // code_buf_t* lr; // avoid lint error this member must be used when implement function return feature
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
        {"Type mismatch error on this operator", error_code_type_mismatch},
        {"Type is not defined (undefined type)", error_code_undefined_type},
        {"Symbol not found from symtab", error_code_not_found_symbol},
        {"Redefined of variable", error_code_redefinition},
        {"No error", error_code_no_error}
};

static void init_registers(void);
static void check_stack(void);

static void     push_stack(object_t value);
static object_t pop_stack(void);
static bool     check_no_error(void);

static bool execute_code(void);

bool VirtualMachine_run_vm(code_buf_t* codes)
{
    init_registers();
    check_stack();

    s_vm_registers.pc = codes;
    return execute_code();
}

object_t VirtualMachine_get_result(void)
{
    return pop_stack();
}

void VirtualMachine_add_error_msg(error_code_t error_code)
{
    for(uint32_t i = 0 ; i < error_code_MAXNUM; i++)
    {
        if (s_predefined_error[i].error_code == error_code)
        {
            s_vm_error.msg = s_predefined_error[i].msg;
            s_vm_error.error_code = error_code;
        }
    }
}

error_code_t VirtualMachine_get_error_msg(char** out_msg_ptr)
{
    error_code_t ret = s_vm_error.error_code;
    *out_msg_ptr = s_vm_error.msg;
    memset(&s_vm_error, 0, sizeof(s_vm_error)); // reset s_vm_error
    return ret;
}

static void init_registers(void)
{
    memset(&s_vm_registers, 0, sizeof(s_vm_registers));
}

static void check_stack(void)
{
    s_vm_stack.stack = (object_t*)limited_malloc(s_vm_stack.stack, sizeof(object_t), BLOCK_STACK_SIZE, s_vm_registers.sp, &s_vm_stack.limit);
}

static void push_stack(object_t value)
{
    check_stack();

    if (value.type == object_type_string)
    {
        value.value.str.ptr = Symtab_get_string_literal_by_idx(value.value.str.table_idx);
    }
    s_vm_stack.stack[s_vm_registers.sp++] = value;
    DEBUG_MSG("push sp[%d], t=%d v=%f\n", s_vm_registers.sp, value.type, value.value.number);
}

static object_t pop_stack(void)
{
    object_t value = {0};
    
    if (s_vm_registers.sp == 0)
    {
        return value;
    }

    s_vm_registers.sp--;
    value = s_vm_stack.stack[s_vm_registers.sp];

    DEBUG_MSG("pop sp[%d], t=%d v=%f\n", s_vm_registers.sp, value.type, value.value.number);

    return value;
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
        case opcode_store:
        case opcode_decl:
        {
            pc++;
            object_t symtab_idx = pc->value;
            object_t value = pop_stack();
            Symtab_store_value_to_symtab(symtab_idx.value.general, value, (opcode == opcode_store ? false : true));
            pc++;
            break;
        }
        case opcode_load:
        {
            pc++;
            object_t symtab_idx = pc->value;
            object_t value = Symtab_load_value_from_symtab(symtab_idx.value.general);
            push_stack(value);
            pc++;
            break;
        }
        case opcode_add:     case opcode_sub:    case opcode_mul:    case opcode_div:    case opcode_mod:
        case opcode_lt:      case opcode_gt:     case opcode_le:     case opcode_ge:     case opcode_eq:      case opcode_ne:
        case opcode_xor:     case opcode_lshift: case opcode_rshift: case opcode_bit_or: case opcode_bit_and: 
        case opcode_com_and: case opcode_com_or:
        case opcode_con:
        {
            object_t op2 = pop_stack(); // get value from stack. Pop the 2nd operand first because it is stack.
            object_t op1 = pop_stack(); // get value from stack. Pop the 1st operand.
            object_t ret = Expr_execute(opcode, op1, op2);
            push_stack(ret);            // result value has been pushed into stack
            pc++;                       // increase pc to execute next code
            break;
        }
        case opcode_cmp:
        {
            pc++;
            printf("OPCODE CMP -> %lx\n", pc->value.value.general);
            pc++;
            break;
        }

        case opcode_halt:
        case opcode_MAXNUM:
            return check_no_error();

        default:
            printf("Critical Error %d\n", opcode);
        }

        if (check_no_error() == false)
        {
            return false;
        }
    }
}
