/*
utils.h

Copyright (c) 12/24/2019, 12:20:10 AM Manwoo Yi

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
#include <stdio.h>
#include <stdbool.h>

/**************************
 * Macro
 **************************/
#define DEBUG_PRINT(_f, ...)  printf((_f), __VA_ARGS__)

/**************************
 * Data types, Constants
 **************************/
 
/**************************
 * Public function prototypes
 **************************/
int64_t     str_to_int64(const char* str, uint32_t base);
double      str_to_double(const char* str);
char*       str_dup(const char* str, uint32_t len);
char*       str_con(const char* front, const char* back, size_t frontlen, size_t backlen, char* delim);
bool        str_cmp(const char* s1, const char* s2);

void        release_mem(void* ptr);

/**************************
 * Inline functions
 **************************/
