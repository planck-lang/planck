/*
main.c

Copyright (c) 12/12/2019, 12:28:08 AM Manwoo Yi

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
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

/**************************
 * Include project headers
 **************************/
#include "planck.h"

/**************************
 * External references
 **************************/ 

/**************************
 * Macro
 **************************/
#define NORMAL_PROMPT   ">> "
#define BLOCK_PROMPT    ".. "

/**************************
 * Data types, Constants
 **************************/

/**************************
 * Private variables
 **************************/
static char* s_prompt_ptr;

/**************************
 * Private function prototypes
 **************************/
static void print_result(data_t data);
static void print_error(error_code_e e);

/**************************
 * Public functions
 **************************/
int main(int argc, char* argv[])
{
    char* line = NULL;

    s_prompt_ptr = NORMAL_PROMPT;

    planck_init();
    
    while(true) 
    {
        char* buf = readline(s_prompt_ptr);
        if (buf == NULL)
        {
            puts("\nAnnyuonghi Gaseyo.\n");
            break;
        }

        add_history(buf);

        line = planck_block_buff(line, buf);
        free(buf);

        s_prompt_ptr = NORMAL_PROMPT;

        if (0 != planck_get_block_depth())
        {
            s_prompt_ptr = BLOCK_PROMPT;
            continue;
        }

        if (strlen(line) > 0) 
        {
            data_t ret = {0};
            error_code_e error = planck(line, &ret);

            if (error_none == error)
            {
                print_result(ret);
            }
            else
            {
                print_error(error);
            }
        }

        free(line);
        line = NULL;
    }
}

/**************************
 * Private functions
 **************************/
static void print_result(data_t data)
{
    if (valtype_int == data.valtype)
    {
        printf("%ld\n", (uint64_t)data.val);
    }
    else if (valtype_num == data.valtype)
    {
        printf("%f\n", data.val);
    }
}

static void print_error(error_code_e e)
{
    if (e > error_start_idx)
    {
        printf("Error: %s\n", errors_get_text(e));
    }
}
