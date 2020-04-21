/*
symtab.h

Copyright (c) 4/1/2020, 11:34:50 PM Manwoo Yi

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
 
#pragma once

/**************************
 * Includes
 **************************/
#include <stdbool.h>

/**************************
 * Macro
 **************************/

/**************************
 * Data types, Constants
 **************************/
#define SYMTAB_NO_IDX   ((uint32_t)-1)

/**************************
 * Public function prototypes
 **************************/
uint32_t symtab_add_symbol(const char* sym_name);
uint32_t symtab_add_symbol_type(const char* sym_name, const char* type_name);
bool symtab_set_value_to_symbol_idx(const uint32_t sym_idx, const data_t data);
data_t symtab_get_value_from_symbol_idx(const uint32_t sym_idx);
uint32_t symtab_get_idx_by_name(const char* sym_name);

/**************************
 * Inline functions
 **************************/

