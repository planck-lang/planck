/*
codegen.c

Copyright (c) 12/24/2019, 12:37:16 AM Manwoo Yi

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
#include <stdio.h>

/**************************
 * Include project headers
 **************************/
#include "codegen.h"

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
void codegen_add_num(const valtype_e valtype, const val_t val)
{
    if (valtype == valtype_int)
    {
        int64_t i64val = (int64_t)val.ival;
        printf("TYPE %d VAL %ld\n", (int)valtype, i64val);
    }
    else if (valtype == valtype_double)
    {
        double dval = (double)val.dval;
        printf("TYPE %d VAL %f\n", (int)valtype, dval);
    }
}

void codegen_add_opcode(const opcode_e opcode)
{
    printf("OP %d\n", opcode);
}

/**************************
 * Private functions
 **************************/
