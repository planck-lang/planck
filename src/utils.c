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
/**************************
 * Private functions
 **************************/
