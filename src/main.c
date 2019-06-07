/*
main.c

Copyright (c) May 31, 2019 Manwoo Yi

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

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "object.h"
#include "planck.h"

int main(int argc, char* argv[])
{
    while (true)
    {
        char* buf = readline(">> ");
        if (buf == NULL)
        {
            puts("\nAnnyuonghi Gaseyo.\n");
            break;
        }

        if (strlen(buf) > 0)
        {
            add_history(buf);

            object_t ret;
            bool st = Planck_do(buf, &ret);
            if (st)
            {
                switch(ret.type)
                {
                case object_type_number:
                    printf("%f\n", ret.value.number);
                    break;
                case object_type_string:
                    printf("%s\n", ret.value.string_ptr);
                    break;
                default:
                    printf("[Type Error]\n");
                }
            }
            else
            {
                printf("[Error] -> %s\n", buf);
            }
        }

        free(buf);
    }
}
