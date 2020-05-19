/*
errors.h

Copyright (c) 1/23/2020, 11:19:51 PM Manwoo Yi

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
typedef enum _error_code_e_ {
    error_none                     = 0x00,
    error_vm_stack_empty           = 0x01,

    error_start_idx                = 0x10000,

    // Parser errors
    error_parser_syntax            = 0x10001,

    // VM errors
    error_vm_type_mismatch         = 0x20001,
    error_vm_unknown_opcode        = 0x20002,

    // Symtab errors
    error_symtab_no_sym_name       = 0x30001,
    error_symtab_dup_sym_name      = 0x30002,

    // Typetab errors
    error_typetab_no_type_name     = 0x40001,
} error_code_e;

/**************************
 * Public function prototypes
 **************************/
void errors_reset(void);
void errors_add(error_code_e e);
bool errors_has_error(void);
error_code_e errors_get(void);
char* errors_get_text(error_code_e e);

/**************************
 * Inline functions
 **************************/

