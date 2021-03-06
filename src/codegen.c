/*
codegen.c

Copyright (c) 12/24/2019, 12:37:16 AM Manwoo Yi

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
#include <string.h>

/**************************
 * Include project headers
 **************************/
#include "codegen.h"
#include "utils.h"

/**************************
 * External references
 **************************/ 

/**************************
 * Macro
 **************************/
  
/**************************
 * Data types, Constants
 **************************/
#define BUF_LINES       2048

/**************************
 * Private variables
 **************************/
static objcode_t objcode_buffer[BUF_LINES];
static uint32_t objcode_cur_line;

/**************************
 * Private function prototypes
 **************************/
static void add_objcode(objcode_t objcode);

/**************************
 * Public functions
 **************************/
void codegen_init(void)
{
    objcode_cur_line = 0;
    memset(objcode_buffer, 0, sizeof(objcode_buffer));
}

void codegen_add_opcode(const opcode_e opcode)
{
    objcode_t objcode = {0};

    objcode.opcode = opcode;

    add_objcode(objcode);
}

void codegen_add_num(const valtype_e valtype, const val_t val)
{
    data_t data = {0};

    data.valtype = valtype;
    data.val = val;

    codegen_add_opcode_with_val(opcode_push, data);
}

void codegen_add_store_load(const char op, const uint32_t symtab_idx)
{
    data_t data = {0};

    data.valtype = valtype_tab_idx;
    data.val = symtab_idx;

    opcode_e opcode = opcode_nop;
    if (op == 'S')
    {
        opcode = opcode_store;
    }
    else if (op == 'L') 
    {
        opcode = opcode_load;
    }
    else
    {
        opcode = opcode_nop;
    }

    codegen_add_opcode_with_val(opcode, data);
}

void codegen_add_opcode_with_val(const opcode_e opcode, const data_t data)
{
    objcode_t objcode = {0};

    objcode.opcode = opcode;
    objcode.data = data;

    add_objcode(objcode);
}

objcode_t* codegen_get_objcode(void)
{
    return objcode_buffer;
}

uint32_t codegen_get_objcode_lines(void)
{
    return objcode_cur_line;
}

void codegen_debug_dump(void)
{
    static const char* opcode_name[] = {
        "opcode_nop",
        "opcode_add",
        "opcode_sub",
        "opcode_mul",
        "opcode_div",
        "opcode_push",
        "opcode_store",
        "opcode_load",
    };

    static const char* valtype_name[] = {
        "valtype_none",
        "valtype_int",
        "valtype_double",
        "primitive_max",
        "valtype_tabidx",
        "builtin_max",
    };

    DEBUG_PRINT("%s", "\n");
    for (uint32_t i = 0 ; i < objcode_cur_line ; i++)
    {
        objcode_t objcode = objcode_buffer[i];
        DEBUG_PRINT("%s", opcode_name[objcode.opcode]);

        data_t data = objcode.data;
        if (valtype_none != data.valtype)
        {
            DEBUG_PRINT(" %s", valtype_name[data.valtype]);
            
            switch (data.valtype)
            {
            case valtype_none:
            break;

            case valtype_num:
                DEBUG_PRINT(" %f", data.val);
            break;

            case valtype_tab_idx:
                DEBUG_PRINT(" $%d", (uint32_t)data.val);
            break;

            default:
                DEBUG_PRINT("%s", "Error!");
            break;
            }
        }

        DEBUG_PRINT("%s", "\n");
    }
}

/**************************
 * Private functions
 **************************/
static void add_objcode(objcode_t objcode)
{
    objcode_buffer[objcode_cur_line++] = objcode;
}