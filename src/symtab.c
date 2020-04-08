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

/**************************
 * Include project headers
 **************************/
#include "vm.h"
#include "types.h"

/**************************
 * External references
 **************************/ 

/**************************
 * Macro
 **************************/

/**************************
 * Data types, Constants
 **************************/

/**************************
 * Private variables
 **************************/

/**************************
 * Private function prototypes
 **************************/

/**************************
 * Public functions
 **************************/
void symtab_add_symbol(const char* sym_name)
{
    data_t data = {0};
    vm_get_last_stack(&data);
    printf("%s : %s -> %ld\n", __FUNCTION__, sym_name, data.val.ival);
}

void symtab_add_symbol_type(const char* sym_name, const char* type_name)
{
    data_t data = {0};
    vm_get_last_stack(&data);
    printf("%s : %s - %s -> %ld\n", __FUNCTION__, sym_name, type_name, data.val.ival);
}

/**************************
 * Private functions
 **************************/

