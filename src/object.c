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

/**************************
 * Include system headers
 **************************/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/**************************
 * Include project headers
 **************************/
#include "object.h"
#include "symtab.h"
#include "ported_lib.h"

/**************************
 * Public functions
 **************************/
object_t Obj_to_string(object_t obj)
{
    if (obj.type == object_type_string)
    {
        return obj;
    }

    object_t ret = {0};

    if (obj.type == object_type_number)
    {
        char temp_buffer[50] = {0};
        int64_t temp_int = (int64_t)obj.value.number;
        if ((double)temp_int == obj.value.number)   // if value is that like 33.000000, take out below the decimal point -> 33
        {
            sprintf(temp_buffer, "%ld", temp_int);
        }
        else
        {
            sprintf(temp_buffer, "%f", obj.value.number);
        }

        ret.value.str.ptr = Symtab_add_string_literal(temp_buffer, &ret.value.str.table_idx);
        ret.type = object_type_string;
    }

    return ret;
}

object_t Obj_conc_string(object_t str_obj1, object_t str_obj2)
{
    object_t ret = {0};

    if (str_obj1.type == object_type_string && str_obj2.type == object_type_string)
    {
        size_t temp_con_str_len = strlen(str_obj1.value.str.ptr) + strlen(str_obj2.value.str.ptr) + 1;  // '+1' is for null space
        char*  temp_con_str_buffer = (char*)new_malloc(temp_con_str_len);

        sprintf(temp_con_str_buffer, "%s%s", str_obj1.value.str.ptr, str_obj2.value.str.ptr);
        ret.value.str.ptr = Symtab_add_string_literal(temp_con_str_buffer, &ret.value.str.table_idx);
        ret.type = object_type_string;

        release_mem(temp_con_str_buffer);
    }

    return ret;
}

object_t Obj_rept_string(object_t str_obj, object_t num_obj)
{
    object_t ret = {0};

    if (str_obj.type == object_type_string && num_obj.type == object_type_number)
    {
        size_t temp_con_str_len = (strlen(str_obj.value.str.ptr) * num_obj.value.number) + 1;  // '+1' is for null space
        char*  temp_con_str_buffer = (char*)new_malloc(temp_con_str_len);

        *temp_con_str_buffer = 0;
        for(uint64_t i = 0 ; i < (uint64_t)num_obj.value.number ; i++)
        {
            strncat(temp_con_str_buffer, str_obj.value.str.ptr, strlen(str_obj.value.str.ptr));
        }

        ret.value.str.ptr = Symtab_add_string_literal(temp_con_str_buffer, &ret.value.str.table_idx);
        ret.type = object_type_string;

        release_mem(temp_con_str_buffer);
    }

    return ret;
}

int Obj_comp_string(object_t str_obj1, object_t str_obj2)
{
    int ret = -1;
    
    if (str_obj1.type == object_type_string && str_obj2.type == object_type_string)
    {
        ret = strncmp(str_obj1.value.str.ptr, str_obj2.value.str.ptr, strlen(str_obj1.value.str.ptr));
    }

    return ret;
}

list_t* Obj_conv_list_item(object_t obj)
{
    list_t* list_item = (list_t*)new_malloc(sizeof(list_t));
    
    object_t* obj_item = (object_t*)new_malloc(sizeof(object_t));
    memcpy(obj_item, &obj, sizeof(object_t));
    
    list_item->ptr_value = obj_item;
    
    return list_item;
}
