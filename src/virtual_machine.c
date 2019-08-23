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

/**************************
 * Include system headers
 **************************/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/**************************
 * Include project headers
 **************************/
#include "object.h"
#include "virtual_machine.h"
#include "code_gen.h"
#include "expr.h"
#include "symtab.h"
#include "ported_lib.h"
#include "error.h"

/**************************
 * Macro
 **************************/
#define DEBUG_VM    0
#if DEBUG_VM
#define DEBUG_MSG(_f, ...)  printf((_f), __VA_ARGS__);
#else
#define DEBUG_MSG(_f, ...)
#endif

/**************************
 * Data types, Constants
 **************************/
#define BLOCK_STACK_SIZE     4096

typedef struct _vm_registers_t_ {
    code_buf_t* pc;
    // code_buf_t* lr; // avoid lint error this member must be used when implement function return feature
    uint32_t    sp;
} vm_registers_t;

typedef struct _vm_stack_t_ {
    object_t*   stack;
    uint32_t    limit;
} vm_stack_t;

/**************************
 * Private variables
 **************************/
static vm_registers_t s_vm_registers = {0};
static vm_stack_t s_vm_stack = {0};

/**************************
 * Private function prototypes
 **************************/
static void init_registers(void);
static void check_stack(void);

static void     push_stack(object_t value);
static object_t pop_stack(void);
static object_t pop_combine(object_type_t type, uint32_t count);

static code_buf_t* cmp_false_jmp(code_buf_t* pc, object_t result, uint64_t offset);

static bool execute_code(void);

/**************************
 * Public functions
 **************************/
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

/**************************
 * Private functions
 **************************/
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

static code_buf_t* cmp_false_jmp(code_buf_t* pc, object_t result, uint64_t offset)
{
    if (result.type == object_type_boolean)
    {
        if (result.value.boolean == true)
        {
            return (pc + 1);
        }
        else
        {
            return (pc + offset);
        }
    }

    Error_add_error_msg(error_code_must_be_bool);
    return NULL;
}

static object_t pop_combine(object_type_t type, uint32_t count)
{
    object_t combined_value = {0};
    
    combined_value.type = type;
    list_t** current = &combined_value.value.combined;
    
    for (uint32_t i = 0 ; i < count ; i++)
    {
        object_t value = pop_stack();
        list_t* list_item = Obj_conv_list_item(value);
        
        *current = list_item;
        current = &list_item->next;
    }
    
    return combined_value;
}

static bool execute_code(void)
{
    code_buf_t* pc = s_vm_registers.pc;

    while(true)
    {
        opcode_t opcode = pc->bytecode.opcode;

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
            push_stack(pc->bytecode.value);      // value has been pushed into stack and increase pc to execute next code
            pc++;
            break;
        }
        case opcode_decl:
        {
            pc++;
            object_t identify_info = pc->bytecode.value;
            uint32_t type_idx = identify_info.value.general >> 32;
            uint32_t literal_idx = identify_info.value.general & 0xFFFFFFFF;
            
            char* identify_ptr = Symtab_get_string_literal_by_idx(literal_idx);

            if (Symtab_is_exist_variable(identify_ptr))
            {
                Error_add_error_msg(error_code_redefinition);
                return false;
            }

            uint32_t symtab_idx = Symtab_add_variable(type_idx, identify_ptr);
            if (symtab_idx == 0)
            {
                return false;
            }
            object_t value = pop_stack();
            Symtab_store_value_to_symtab(symtab_idx, value, true);
            pc++;
            break;
        }
        case opcode_load:
        case opcode_store:
        {
            pc++;
            object_t literal_idx = pc->bytecode.value;
            char* identify_ptr = Symtab_get_string_literal_by_idx(literal_idx.value.general);

            if (Symtab_is_exist_variable(identify_ptr) == false)
            {
                Error_add_error_msg(error_code_not_found_symbol);
                return false;
            }

            uint32_t variable_symtab_index = Symtab_get_idx_variable_by_identifier(identify_ptr);
            if (opcode == opcode_load)
            {
                object_t value = Symtab_load_value_from_symtab(variable_symtab_index);
                push_stack(value);
            }
            else    // opcode_store
            {
                object_t value = pop_stack();
                Symtab_store_value_to_symtab(variable_symtab_index, value, false);
            }
            
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
            object_t result = pop_stack();
            
            pc++;
            uint64_t offset = pc->bytecode.value.value.general;
            pc = cmp_false_jmp(pc, result, offset);
            break;
        }
        case opcode_jmp:
        case opcode_break:
        case opcode_continue:
        {
            pc++;
            int64_t offset = pc->bytecode.value.value.general;
            pc += offset;
            break;
        }
        case opcode_begin_scope:
        case opcode_end_scope:
        {
            Symtab_set_block_input(opcode == opcode_begin_scope ? true : false);
            pc++;
            break;
        }
        case opcode_begin_loop:
        case opcode_end_loop:
        {
            pc++;
            break;
        }
        case opcode_array:
        {
            pc++;
            uint32_t combine_count = pc->bytecode.value.value.general;
            object_t result = pop_combine(object_type_array, combine_count);
            push_stack(result);
            pc++;
            break;
        }
        case opcode_halt:
        case opcode_MAXNUM:
            return (!Error_is_happened());

        default:
            printf("Critical Error %d\n", opcode);
            printf("%lx %lx %lx\n", (uint64_t)(*(uint64_t*)(pc - 2)), (uint64_t)(*(uint64_t*)(pc - 1)), (uint64_t)(*(uint64_t*)pc));
            return false;
        }

        if (Error_is_happened() == true)
        {
            return false;
        }
    }
}
