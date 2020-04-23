/*
errors.c

Copyright (c) 1/23/2020, 11:20:07 PM Manwoo Yi

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
#define ERR_LIST_NUM 128

struct error_text
{
    error_code_e e;
    char* text;
};

/**************************
 * Private variables
 **************************/
static error_code_e error_list[ERR_LIST_NUM];
static uint32_t error_list_idx;

static struct error_text error_text_dict[] = 
{
    // Parser errors
    {error_parser_syntax, "Syntax Error"},

    // VM errors
    {error_vm_type_mismatch, "Type mismatch"},
    {error_vm_unknown_opcode, "Unkown opcode"},

    // Symtab errors
    {error_symtab_no_sym_name, "Not found symbol name"},
    {error_symtab_dup_sym_name, "Duplicated declaration"},
};
static const uint32_t num_of_error_text_dict = sizeof(error_text_dict) / sizeof(struct error_text);

/**************************
 * Private function prototypes
 **************************/

/**************************
 * Public functions
 **************************/
void errors_reset(void)
{
    error_list_idx = 0;
    memset(error_list, 0, sizeof(error_list));
}

void errors_add(error_code_e e)
{
    error_list[error_list_idx++] = e;
}

bool errors_has_error(void)
{
    return (error_list_idx != 0);
}

error_code_e errors_get(void)
{
    if (errors_has_error())
    {
        return error_list[--error_list_idx];
    }

    return error_none;
}

char* errors_get_text(error_code_e e)
{
    // Linear search
    for(uint32_t i = 0 ; i < num_of_error_text_dict ; i++)
    {
        if (e == error_text_dict[i].e)
        {
            return error_text_dict[i].text;
        }
    }

    return NULL;
}

/**************************
 * Private functions
 **************************/

