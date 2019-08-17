/*
error.c

Copyright (c) 8/16/2019 Manwoo Yi

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
#include <string.h>

/**************************
 * Include project headers
 **************************/
#include "error.h"

/**************************
 * External references
 **************************/ 

/**************************
 * Macro
 **************************/
  
/**************************
 * Data types, Constants
 **************************/
typedef struct _vm_error_status_t_ {
    char*           msg;
    error_code_t    error_code;
} vm_error_status_t;

/**************************
 * Private variables
 **************************/
 static vm_error_status_t s_vm_error, s_predefined_error[] = {
        {"Type mismatch error on this operator", error_code_type_mismatch},
        {"Type is not defined (undefined type)", error_code_undefined_type},
        {"Symbol not found from symtab", error_code_not_found_symbol},
        {"Redefined of variable", error_code_redefinition},
        {"Condition statement expects boolean expression", error_code_must_be_bool},
        {"Loop not found", error_code_not_in_loop},
        {"No error", error_code_no_error}
};

/**************************
 * Private function prototypes
 **************************/
 
/**************************
 * Public functions
 **************************/
void Error_add_error_msg(error_code_t error_code)
{
    for(uint32_t i = 0 ; i < error_code_MAXNUM; i++)
    {
        if (s_predefined_error[i].error_code == error_code)
        {
            s_vm_error.msg = s_predefined_error[i].msg;
            s_vm_error.error_code = error_code;
        }
    }
}

error_code_t Error_get_error_msg(char** out_msg_ptr)
{
    error_code_t ret = s_vm_error.error_code;
    *out_msg_ptr = s_vm_error.msg;
    memset(&s_vm_error, 0, sizeof(s_vm_error)); // reset s_vm_error
    return ret;
}

bool Error_is_happened(void)
{
    return (s_vm_error.error_code != error_code_no_error);
}

/**************************
 * Private functions
 **************************/
