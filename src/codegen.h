/*
codegen.h

Copyright (c) 12/24/2019, 12:37:08 AM Manwoo Yi

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
#include <stdint.h>

#include "types.h"

/**************************
 * Macro
 **************************/
 
/**************************
 * Data types, Constants
 **************************/
 
/**************************
 * Public function prototypes
 **************************/
void codegen_init(void);
void codegen_add_opcode(const opcode_e opcode);
void codegen_add_num(const valtype_e valtype, const val_t val);
void codegen_add_store(const uint32_t symtab_idx);
void codegen_add_opcode_with_val(const opcode_e opcode, const data_t data);
objcode_t* codegen_get_objcode(void);
uint32_t codegen_get_objcode_lines(void);

void codegen_debug_dump(void);
/**************************
 * Inline functions
 **************************/
