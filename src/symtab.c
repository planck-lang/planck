/*
symtab.c

Copyright (c) 4/1/2020, 11:34:30 PM Manwoo Yi

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
#include <stdbool.h>

/**************************
 * Include project headers
 **************************/
#include "types.h"
#include "symtab.h"
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
struct symtab_entry {
    const char* name;
    data_t      value;
};

#define MAX_SYM_TAB_NUM     1024

struct symtab {
    struct symtab_entry table[MAX_SYM_TAB_NUM];
    uint32_t            top;
};

/**************************
 * Private variables
 **************************/
static struct symtab global_symtab = {0};

/**************************
 * Private function prototypes
 **************************/
static uint32_t push_symbol(struct symtab* ptr_symtab, const char* symname);
static bool set_value_to_symbol(struct symtab* ptr_symtab, const uint32_t idx, const data_t data);
static data_t get_value_from_symbol(struct symtab* ptr_symtab, const uint32_t idx);
static uint32_t get_idx_by_name(struct symtab* ptr_symtab, const char* sym_name);

/**************************
 * Public functions
 **************************/
uint32_t symtab_add_symbol(const char* sym_name)
{
    return push_symbol(&global_symtab, sym_name);
}

uint32_t symtab_add_symbol_type(const char* sym_name, const char* type_name)
{
    return push_symbol(&global_symtab, sym_name);
}

bool symtab_set_value_to_symbol_idx(const uint32_t sym_idx, const data_t data)
{
    return set_value_to_symbol(&global_symtab, sym_idx, data);
}

data_t symtab_get_value_from_symbol_idx(const uint32_t sym_idx)
{
    return get_value_from_symbol(&global_symtab, sym_idx);
}

uint32_t symtab_get_idx_by_name(const char* sym_name)
{
    return get_idx_by_name(&global_symtab, sym_name);
}

/**************************
 * Private functions
 **************************/
static uint32_t push_symbol(struct symtab* ptr_symtab, const char* symname)
{
    if (ptr_symtab->top >= MAX_SYM_TAB_NUM)
    {
        return SYMTAB_NO_IDX;
    }

    uint32_t idx = ptr_symtab->top;
    ptr_symtab->table[idx].name = symname;
    
    ptr_symtab->top++;

    return idx;
}

static bool set_value_to_symbol(struct symtab* ptr_symtab, const uint32_t idx, const data_t data)
{
    if (idx >= ptr_symtab->top)
    {
        return false;
    }

    ptr_symtab->table[idx].value = data;

    return true;
}

static data_t get_value_from_symbol(struct symtab* ptr_symtab, const uint32_t idx)
{
    data_t ret_value = {0};

    if (idx >= ptr_symtab->top)
    {
        return ret_value;
    }

    ret_value = ptr_symtab->table[idx].value;

    return ret_value;
}

static uint32_t get_idx_by_name(struct symtab* ptr_symtab, const char* sym_name)
{
    for (uint32_t idx = 0 ; idx < ptr_symtab->top ; idx++)
    {
        if (str_cmp(sym_name, ptr_symtab->table[idx].name))
        {
            return idx;
        }
    }

    return SYMTAB_NO_IDX;
}