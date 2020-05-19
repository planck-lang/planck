/*
typetab.c

Copyright (c) 5/13/2020, 10:54:46 PM Manwoo Yi

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
#include <stdlib.h>

/**************************
 * Include project headers
 **************************/
#include "types.h"
#include "typetab.h"
#include "utils.h"
#include "errors.h"

/**************************
 * External references
 **************************/ 

/**************************
 * Macro
 **************************/

/**************************
 * Data types, Constants
 **************************/
#define MAX_TYPE_TAB_NUM     1024
#define MAX_TYPE_MEMBER      1024

struct member {
    const char* name;
    uint32_t type_refer;
};

struct typetab_entry {
    const char* name;
    struct member* members;
    uint32_t num_members;
    uint32_t size;
};

struct typetab {
    struct typetab_entry table[MAX_TYPE_TAB_NUM];
    uint32_t             top;
};

/**************************
 * Private variables
 **************************/
struct typetab global_typetab = {0};

/**************************
 * Private function prototypes
 **************************/

/**************************
 * Public functions
 **************************/
void typetab_init(void)
{
    // index #0 is not used
    global_typetab.top++;

    global_typetab.table[valtype_int].name = "int";
    global_typetab.table[valtype_int].members = NULL;
    global_typetab.table[valtype_int].num_members = 0;
    global_typetab.table[valtype_int].size = 64;
    global_typetab.top++;

    global_typetab.table[valtype_num].name = "num";
    global_typetab.table[valtype_num].members = NULL;
    global_typetab.table[valtype_num].num_members = 0;
    global_typetab.table[valtype_num].size = 64;
    global_typetab.top++;
}

uint32_t typetab_add_type(const char* type_name)
{
    uint32_t idx = global_typetab.top;

    global_typetab.table[idx].name = type_name;
    global_typetab.table[idx].members = NULL;
    global_typetab.table[idx].num_members = 0;
    global_typetab.table[idx].size = 0;
    global_typetab.top++;

    return idx;
}

uint32_t typetab_add_member_by_idx(uint32_t type_idx, const char* mem_name, const char* mem_type_name)
{
    if (NULL == global_typetab.table[type_idx].members)
    {
        global_typetab.table[type_idx].members = alloc_mem(sizeof(struct member), MAX_TYPE_MEMBER);
    }

    uint32_t mem_idx = global_typetab.table[type_idx].num_members;
    uint32_t mem_type_idx = typetab_get_idx_by_name(mem_type_name);

    if (TYPETAB_NO_IDX == mem_type_idx)
    {
        errors_add(error_typetab_no_type_name);
        return TYPETAB_NO_IDX;
    }

    global_typetab.table[type_idx].members[mem_idx].name = mem_name;
    global_typetab.table[type_idx].members[mem_idx].type_refer = mem_type_idx;
    global_typetab.table[type_idx].num_members++;

    return mem_idx;
}

uint32_t typetab_get_idx_by_name(const char* type_name)
{
    for (uint32_t idx = 0 ; idx < global_typetab.top ; idx++)
    {
        if (str_cmp(type_name, global_typetab.table[idx].name))
        {
            return idx;
        }
    }

    return TYPETAB_NO_IDX;
}

/**************************
 * Private functions
 **************************/

