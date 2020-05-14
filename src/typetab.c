/*
typetab.c

Copyright (c) 5/13/2020, 10:54:46 PM Manwoo Yi

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

/**************************
 * Include project headers
 **************************/
#include "types.h"
#include "typetab.h"

/**************************
 * External references
 **************************/ 

/**************************
 * Macro
 **************************/

/**************************
 * Data types, Constants
 **************************/
struct typetab_entry {
    const char* name;
    uint32_t    refer_valtype_e;
};

#define MAX_TYPE_TAB_NUM     1024

struct typetab {
    struct typetab_entry table[MAX_TYPE_TAB_NUM];
    uint32_t             top;
    uint32_t             custom_type_idx;
};

/**************************
 * Private variables
 **************************/

/**************************
 * Private function prototypes
 **************************/

/**************************
 * Public functions
 **************************/

/**************************
 * Private functions
 **************************/

