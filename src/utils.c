/*
utils.c

Copyright (c) 12/24/2019, 12:19:57 AM Manwoo Yi

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
#include <stdlib.h>
#include <string.h>

/**************************
 * Include project headers
 **************************/
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

/**************************
 * Private variables
 **************************/
 
/**************************
 * Private function prototypes
 **************************/

/**************************
 * Public functions
 **************************/
int64_t str_to_int64(const char* str, uint32_t base)
{
    int64_t ret = 0;

    if (10 == base)
    {
        ret = (int64_t)str_to_double(str);
    }

    return ret;
}

double str_to_double(const char* str)
{
    double ret = 0;
    char* ptr = NULL;

    ret = strtod(str, &ptr);

    return ret;
}

char* str_dup(const char* str, uint32_t len)
{
    char* ret_ptr = malloc(len + 1);
    memset(ret_ptr, 0, len + 1);
    memcpy(ret_ptr, str, len);
    return ret_ptr;
}

char* str_con(const char* front, const char* back, size_t frontlen, size_t backlen, char* delim)
{
    size_t delim_len = str_len(delim);
    size_t all_len = frontlen + backlen + delim_len + 1;

    char* ret_ptr = malloc(all_len);
    memset(ret_ptr, 0, all_len);

    memcpy(ret_ptr, front, frontlen);
    if (NULL != delim) { memcpy((ret_ptr + frontlen), delim, delim_len); }
    memcpy((ret_ptr + frontlen + delim_len), back, backlen);
    
    return ret_ptr;
}

void* alloc_mem(size_t size_of_item, uint32_t num_of_item)
{
    return malloc(size_of_item * num_of_item);
}

void release_mem(void* ptr)
{
    if (ptr != NULL)
    {
        free(ptr);
    }
}

bool str_cmp(const char* s1, const char* s2)
{
    size_t len_s1 = str_len(s1);
    size_t len_s2 = str_len(s2);

    bool check_len = (len_s1 == len_s2);
    bool check_val = (strncmp(s1, s2, len_s1) == 0);

    return (check_len == true && check_val ==  true);
}

size_t str_len(const char* s)
{
    return ((NULL == s) ? 0 : strlen(s));
}

/**************************
 * Private functions
 **************************/
