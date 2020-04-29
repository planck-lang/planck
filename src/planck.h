/*
planck.h

Copyright (c) 1/4/2020, 1:28:31 AM Manwoo Yi

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
#include "types.h"
#include "errors.h"
#include "utils.h"

/**************************
 * Macro
 **************************/
 
/**************************
 * Data types, Constants
 **************************/
 
/**************************
 * Public function prototypes
 **************************/
error_code_e planck(const char* str, OUT_PTR data_t* ret);
char* planck_block_buff(char* block_buf, char* line_buf, OUT_PTR uint32_t *block_depth);

/**************************
 * Inline functions
 **************************/
