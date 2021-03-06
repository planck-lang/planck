/*
vm.c

Copyright (c) 12/26/2019, 1:24:27 AM Manwoo Yi

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
#include <string.h>

/**************************
 * Include project headers
 **************************/
#include "vm.h"
#include "types.h"
#include "utils.h"
#include "errors.h"
#include "symtab.h"

/**************************
 * External references
 **************************/ 

/**************************
 * Macro
 **************************/
  
/**************************
 * Data types, Constants
 **************************/
#define STACK_MAX   4096

/**************************
 * Private variables
 **************************/
static data_t data_stack[STACK_MAX];
static uint32_t data_stack_pointer;

static const objcode_t* code_ptr;
static uint32_t code_line;
static uint32_t code_pc;
/**************************
 * Private function prototypes
 **************************/
static objcode_t fetch_code(void);
static void execute_code(objcode_t code);

static void arithmetic(opcode_e opcode);
static void store_after_pop(data_t data);
static void load_after_push(data_t data);

static void stack_push(data_t data);
static data_t stack_pop(void);

/**************************
 * Public functions
 **************************/
void vm_init(const objcode_t* pcode, const uint32_t lines)
{
    data_stack_pointer = 0;
    memset(data_stack, 0, sizeof(data_stack));

    code_ptr = pcode;
    code_line = lines;
    code_pc = 0;
}

void vm_run(void)
{
    while (code_pc < code_line)
    {
        objcode_t code = fetch_code();
        execute_code(code);

        if (errors_has_error())
        {
            return;
        }
    }
}

bool vm_get_last_stack(OUT_PTR data_t* ret)
{
    if (data_stack_pointer == 0)
    {
        return false;
    }

    *ret = stack_pop();
    return true;
}

/**************************
 * Private functions
 **************************/
static objcode_t fetch_code(void)
{
    return code_ptr[code_pc++];
}

static void execute_code(objcode_t code)
{
    opcode_e opcode = code.opcode;

    switch(opcode)
    {
        case opcode_add:
        case opcode_sub:
        case opcode_mul:
        case opcode_div:
            arithmetic(opcode);
        break;

        case opcode_push:
            stack_push(code.data);
        break;

        case opcode_store:
            store_after_pop(code.data);
        break;

        case opcode_load:
            load_after_push(code.data);
        break;

        case opcode_nop:
        break;
        default:
            errors_add(error_vm_unknown_opcode);
        break;
    }
}

static void arithmetic(opcode_e opcode)
{
    data_t val2 = stack_pop();
    data_t val1 = stack_pop();

    data_t result = {0};
    result.valtype = ((val1.valtype == val2.valtype) ? val1.valtype : valtype_num);

    switch(opcode)
    {
    case opcode_add:
        result.val = val1.val + val2.val;
    break;

    case opcode_sub:
        result.val = val1.val - val2.val;
    break;

    case opcode_mul:
        result.val = val1.val * val2.val;
    break;

    case opcode_div:
        if (result.valtype == valtype_int)
        {
            result.val = (uint64_t)val1.val / (uint64_t)val2.val;
        }
        else
        {
            result.val = val1.val / val2.val;
        }
    break;

    default:
        // TODO: Critical Error, Should be never happened
    break;
    }

    stack_push(result);
}

static void store_after_pop(data_t data)
{
    uint32_t sym_idx = (uint32_t)data.val;
    data_t val = stack_pop();

    if (sym_idx == SYMTAB_NO_IDX)
    {
        errors_add(error_symtab_no_sym_name);
        return;
    }

    bool result = symtab_set_value_to_symbol_idx(sym_idx, val);

    if (false == result)
    {
        errors_add(error_symtab_no_sym_name);
    }
}

static void load_after_push(data_t data)
{
    uint32_t sym_idx = (uint32_t)data.val;

    if (sym_idx == SYMTAB_NO_IDX)
    {
        errors_add(error_symtab_no_sym_name);
        return;
    }

    data_t val = symtab_get_value_from_symbol_idx(sym_idx);
    stack_push(val);
}

static void stack_push(data_t data)
{
    data_stack[data_stack_pointer++] = data;
}

static data_t stack_pop(void)
{
    return data_stack[--data_stack_pointer];
}