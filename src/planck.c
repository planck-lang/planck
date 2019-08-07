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

/**************************
 * Include system headers
 **************************/
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/**************************
 * Include project headers
 **************************/
#include "object.h"
#include "virtual_machine.h"
#include "code_gen.h"
#include "planck.h"
#include "ported_lib.h"

/**************************
 * External references
 **************************/ 
typedef struct yy_buffer_state* YY_BUFFER_STATE;
extern int yyparse();
extern YY_BUFFER_STATE yy_scan_string(const char * str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);
extern int yylex(void);
extern uint32_t yy_get_block_depth(void);

/**************************
 * Data types, Constants
 **************************/
#define BLOCK_BUF_DEF_LEN (8 * 1024)     // 8KB

/**************************
 * Private variables
 **************************/
static char* s_error_msg_ptr = NULL;
static error_code_t s_error_code;

static char* s_block_buf = NULL;
static uint32_t s_block_buf_limit = BLOCK_BUF_DEF_LEN;

/**************************
 * Private function prototypes
 **************************/
static void check_block_input_mode(const char* str);

/**************************
 * Public functions
 **************************/
planck_result_t Planck_do_as_stmt(const char* buf, object_t* out_ret)
{
    check_block_input_mode(buf);

    const char* input_code_buf = buf;

    if (yy_get_block_depth() > 0)
    {
        if (s_block_buf == NULL)
        {
            s_block_buf = (char*)new_malloc(s_block_buf_limit);
        }

        size_t expected_len = strlen(s_block_buf) + strlen(buf);
        if (expected_len > s_block_buf_limit)
        {
            s_block_buf = (char*)limited_malloc(s_block_buf, 1, BLOCK_BUF_DEF_LEN, expected_len, &s_block_buf_limit);
        }
        
        strncat(s_block_buf, buf, strlen(buf));

        return planck_result_block_input;
    }

    if (s_block_buf != NULL)
    {
        strncat(s_block_buf, buf, strlen(buf));
        input_code_buf = s_block_buf;
    }

    CodeGen_reset_bytecodes();

    YY_BUFFER_STATE yyst = yy_scan_string(input_code_buf);
    int parse_result = yyparse();    
    yy_delete_buffer(yyst);

    if (s_block_buf != NULL)
    {
        release_mem(s_block_buf);
        s_block_buf = NULL;
    }

    s_error_code = VirtualMachine_get_error_msg(&s_error_msg_ptr);

    if (!parse_result)  // ok
    {
        if (s_error_code != error_code_no_error)
        {
            return planck_result_fail;
        }

        CodeGen_add_opcode(opcode_halt);
        if (VirtualMachine_run_vm(CodeGen_get_bytecodes()))
        {
            *out_ret = VirtualMachine_get_result();
            return planck_result_ok;
        }
        else
        {
            s_error_code = VirtualMachine_get_error_msg(&s_error_msg_ptr);
            return planck_result_fail;
        }

    }
    // parse error
    return planck_result_fail;
}

error_code_t Planck_get_error(char* out_error)
{
    sprintf(out_error, "Runtime Error [0x%04X] %s\n", s_error_code, s_error_msg_ptr);
    return s_error_code;
}

/**************************
 * Private functions
 **************************/
static void check_block_input_mode(const char* str)
{
    YY_BUFFER_STATE buf;
    buf = yy_scan_string(str);
    while (true)
    {
        /* The Lexical analyzer changes s_block_depth value by calling the Planck_set_block_input() function.
           Refer the syntax.lex file */
        if (yylex() == 0)
        {
            break;
        }
    }
    yy_delete_buffer(buf);
}
