/*
typetab.h

Copyright (c) 5/13/2020, 10:54:55 PM Manwoo Yi

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

/**************************
 * Macro
 **************************/

/**************************
 * Data types, Constants
 **************************/
#define TYPETAB_NO_IDX   ((uint32_t)-1)

/**************************
 * Public function prototypes
 **************************/
void typetab_init(void);
uint32_t typetab_add_type(const char* type_name);
uint32_t typetab_add_member_by_idx(uint32_t type_idx, const char* mem_name, const char* mem_type_name);
uint32_t typetab_get_idx_by_name(const char* type_name);
uint32_t typetab_get_size_by_idx(uint32_t type_idx);

void typetab_debug_dump_type_name(void);

/**************************
 * Inline functions
 **************************/

