/*
ported_lib.c

Copyright (c) 7/8/2019, 10:27:30 PM Manwoo Yi

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

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "ported_lib.h"

void* new_malloc(size_t size)
{
    return malloc(size);
}

void release_mem(void* ptr)
{
    return free(ptr);
}

void* limited_malloc(void* current_ptr, size_t size, uint32_t count, uint32_t current_len, uint32_t* out_limit)
{
    void* ret_ptr = NULL;

    if (current_ptr == NULL)
    {
        ret_ptr = malloc(size * count);
        *out_limit = count;
        return ret_ptr;
    }
    
    if (current_len >= *out_limit)
    {
        uint32_t new_limit = *out_limit + count;
        ret_ptr = realloc(current_ptr, (size * new_limit));
        *out_limit = new_limit;
        return ret_ptr;
    }

    return current_ptr;
}

char* str_dup(const char* orig, size_t len)
{
    char* ret_ptr = malloc(len + 1);
    memset(ret_ptr, 0, len + 1);
    memcpy(ret_ptr, orig, len);
    return ret_ptr;
}

double str_to_number(const char* strnum, uint32_t base)
{
    double ret = 0;
    const char* targetstr = strnum;
    char* ptr = NULL;
    bool negative = false;

    if (strnum[0] == '-')
    {
        targetstr = strnum + 1;
        negative = true;
    }

    if (base == 10)
    {
        ret = strtod(targetstr, &ptr);
    }
    else if (base == 16)
    {
        if (targetstr[0] == '0')
        {
            if (targetstr[1] == 'x' || targetstr[1] == 'X') // "0x" or "0X"
            {
                int64_t longint = 0;
                longint = strtol(targetstr, &ptr, 16);
                ret = (double)longint;
            }
        }
    }
    else if (base == 2)
    {

    }
    else
    {
        ret = 0;
    }

    ret *= (negative) ? -1 : 1;
    
    return ret;
}