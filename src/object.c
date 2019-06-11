/*
object.c

Copyright (c) Jun 3, 2019 Manwoo Yi

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
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "object.h"
#include "symtab.h"

object_t Obj_to_string(object_t obj)
{
    if (obj.type == object_type_string)
    {
        return obj;
    }

    char temp_buffer[50] = {0};
    object_t ret;

    if (obj.type == object_type_number)
    {
        int64_t temp_int = (int64_t)obj.value.number;
        if ((double)temp_int == obj.value.number)   // if value is that like 33.000000, take out below the decimal point -> 33
        {
            sprintf(temp_buffer, "%ld", temp_int);
        }
        else
        {
            sprintf(temp_buffer, "%f", obj.value.number);
        }

        ret.value.string_ptr = Symtab_add_string_literal(temp_buffer);
        ret.type = object_type_string;
    }

    return ret;
}

object_t Obj_conc_string(object_t str_obj1, object_t str_obj2)
{
    object_t ret = {0};

    if (str_obj1.type == object_type_string && str_obj1.type == object_type_string)
    {
        uint32_t temp_con_str_len = strlen(str_obj1.value.string_ptr) + strlen(str_obj2.value.string_ptr) + 1;  // '+1' is for null space
        char*    temp_con_str_buffer = (char*)malloc(temp_con_str_len);

        memset(temp_con_str_buffer, 0, temp_con_str_len);

        sprintf(temp_con_str_buffer, "%s%s", str_obj1.value.string_ptr, str_obj2.value.string_ptr);
        ret.value.string_ptr = Symtab_add_string_literal(temp_con_str_buffer);
        ret.type = object_type_string;

        free(temp_con_str_buffer);
    }

    return ret;
}
