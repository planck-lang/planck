/*
exe.c

Copyright (c) 12/26/2019, 2:55:38 AM Manwoo Yi

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
#include <stdint.h>

/**************************
 * Include project headers
 **************************/
#include "exe.h"

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
val_t exe_add(valtype_e valtype, val_t v1, val_t v2)
{
    val_t result = {0};

    if (valtype_int == valtype)
    {
        result.ival = v1.ival + v2.ival;
    }
    else if (valtype_double == valtype)
    {
        result.dval = v1.dval + v2.dval;
    }

    return result;
}

val_t exe_sub(valtype_e valtype, val_t v1, val_t v2)
{
    val_t result = {0};

    if (valtype_int == valtype)
    {
        result.ival = v1.ival - v2.ival;
    }
    else if (valtype_double == valtype)
    {
        result.dval = v1.dval - v2.dval;
    }

    return result;
}
val_t exe_mul(valtype_e valtype, val_t v1, val_t v2)
{
    val_t result = {0};

    if (valtype_int == valtype)
    {
        result.ival = v1.ival * v2.ival;
    }
    else if (valtype_double == valtype)
    {
        result.dval = v1.dval * v2.dval;
    }

    return result;
}

val_t exe_div(valtype_e valtype, val_t v1, val_t v2)
{
    val_t result = {0};

    if (valtype_int == valtype)
    {
        result.ival = v1.ival / v2.ival;
    }
    else if (valtype_double == valtype)
    {
        result.dval = v1.dval / v2.dval;
    }

    return result;
}

/**************************
 * Private functions
 **************************/
