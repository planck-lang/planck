/*
planck.c

Copyright (c) 1/4/2020, 1:28:14 AM Manwoo Yi

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
#include "planck.h"
#include "codegen.h"
#include "vm.h"
#include "utils.h"
#include "errors.h"

/**************************
 * External references
 **************************/ 
typedef struct yy_buffer_state* YY_BUFFER_STATE;
extern int yyparse();
extern YY_BUFFER_STATE yy_scan_string(const char * str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);
extern int yylex(void);

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
error_code_e planck(const char* str, OUT_PTR data_t* ret)
{
    codegen_init();
    errors_reset();

    YY_BUFFER_STATE yyst = yy_scan_string(str);
    int parse_result = yyparse();
    yy_delete_buffer(yyst);

    if (!parse_result) // OK
    {
        //codegen_debug_dump();

        vm_init(
            codegen_get_objcode(),
            codegen_get_objcode_lines()
        );
        vm_run();
        
        if (!errors_has_error())
        {
            *ret = vm_get_last_stack();
        }
    }
    else
    {
        errors_add(error_parser_syntax);
    }

    return errors_get();
}

/**************************
 * Private functions
 **************************/
