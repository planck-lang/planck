/*
planck.c

Copyright (c) May 29, 2019 Manwoo Yi

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
SOFT
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "object.h"
#include "virtual_machine.h"
#include "code_gen.h"

typedef struct yy_buffer_state* YY_BUFFER_STATE;
extern int yyparse();
extern YY_BUFFER_STATE yy_scan_string(const char * str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);

bool Planck_do(const char* buf, object_t* out_ret)
{
    CodeGen_reset_bytecodes();

    YY_BUFFER_STATE yyst = yy_scan_string(buf);
    int parse_result = yyparse();
    yy_delete_buffer(yyst);

    if (!parse_result)  // ok
    {
        char* msg = NULL;
        error_code_t error_code = VirtualMachine_get_error_msg(&msg);

        if (error_code != error_code_no_error)
        {
            printf("Compile Error [0x%04X] %s\n", error_code, msg);
            return false;
        }

        CodeGen_add_opcode(opcode_halt);
        if (VirtualMachine_run_vm(CodeGen_get_bytecodes()))
        {
            *out_ret = VirtualMachine_get_result();
            return true;
        }
        else
        {
            error_code = VirtualMachine_get_error_msg(&msg);
            printf("Runtime Error [0x%04X] %s\n", error_code, msg);
            return false;
        }

    }
    // parse error
    return false;
}
