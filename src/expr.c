/*
expr.c

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
SOFTWARE.
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "object.h"
#include "expr.h"
#include "virtual_machine.h"

static object_t op_add(object_t op1, object_t op2);
static object_t op_sub(object_t op1, object_t op2);
static object_t op_xor(object_t op1, object_t op2);
static object_t op_mul(object_t op1, object_t op2);
static object_t op_div(object_t op1, object_t op2);
static object_t op_mod(object_t op1, object_t op2);
static object_t op_con(object_t op1, object_t op2);
static object_t op_lt(object_t op1, object_t op2);
static object_t op_gt(object_t op1, object_t op2);
static object_t op_le(object_t op1, object_t op2);
static object_t op_ge(object_t op1, object_t op2);
static object_t op_eq(object_t op1, object_t op2);
static object_t op_ne(object_t op1, object_t op2);
static object_t op_rshift(object_t op1, object_t op2);
static object_t op_lshift(object_t op1, object_t op2);

object_t Expr_execute(opcode_t opcode, object_t op1, object_t op2)
{   
    object_t ret = {0};

    switch (opcode)             // calculation
    {
    case opcode_add:
        ret = op_add(op1, op2);
        break;
    case opcode_sub:
        ret = op_sub(op1, op2);
        break;
    case opcode_mul:
        ret = op_mul(op1, op2);
        break;
    case opcode_div:
        ret = op_div(op1, op2);
        break;
    case opcode_mod:
        ret = op_mod(op1, op2);
        break;
    case opcode_con:
        ret = op_con(op1, op2);
        break;
    case opcode_lt:
        ret = op_lt(op1, op2);
        break;
    case opcode_gt:
        ret = op_gt(op1, op2);
        break;
    case opcode_le:
        ret = op_le(op1, op2);
        break;
    case opcode_ge:
        ret = op_ge(op1, op2);
        break;
    case opcode_eq:
        ret = op_eq(op1, op2);
        break;
    case opcode_ne:
        ret = op_ne(op1, op2);
        break;
    case opcode_xor:
        ret = op_xor(op1, op2);
        break;
    case opcode_lshift:
        ret = op_lshift(op1, op2);
        break;
    case opcode_rshift:
        ret = op_rshift(op1, op2);
        break;
    default:
        break;
    }

    return ret;
}

static object_t op_add(object_t op1, object_t op2)
{
    object_t ret;

    if (op1.type == object_type_number && op2.type == object_type_number)
    {
        ret.type = object_type_number;
        ret.value.number = op1.value.number + op2.value.number;
    }
    else
    {
        VirtualMachine_add_error_msg(error_code_type_mismatch);
    }

    return ret;
}

static object_t op_sub(object_t op1, object_t op2)
{
    object_t ret;

    if (op1.type == object_type_number && op2.type == object_type_number)
    {
        ret.type = object_type_number;
        ret.value.number = op1.value.number - op2.value.number;
    }
    else
    {
        VirtualMachine_add_error_msg(error_code_type_mismatch);
    }

    return ret;
}

static object_t op_mul(object_t op1, object_t op2)
{
    object_t ret;

    if (op1.type == object_type_number && op2.type == object_type_number)
    {
        ret.type = object_type_number;
        ret.value.number = op1.value.number * op2.value.number;
    }
    else if (op1.type == object_type_number && op2.type == object_type_string)   // number * "string"
    {
        ret = Obj_rept_string(op2, op1);
    }
    else if (op1.type == object_type_string && op2.type == object_type_number)  // "string" * number 
    {
        ret = Obj_rept_string(op1, op2);
    }
    else
    {
        VirtualMachine_add_error_msg(error_code_type_mismatch);
    }

    return ret;
}

static object_t op_div(object_t op1, object_t op2)
{
    object_t ret;

    if (op1.type == object_type_number && op2.type == object_type_number)
    {
        ret.type = object_type_number;
        ret.value.number = op1.value.number / op2.value.number;
    }
    else
    {
        VirtualMachine_add_error_msg(error_code_type_mismatch);
    }

    return ret;
}

static object_t op_mod(object_t op1, object_t op2)
{
    object_t ret;

    if (op1.type == object_type_number && op2.type == object_type_number)
    {
        ret.type = object_type_number;
        ret.value.number = (uint64_t)op1.value.number % (uint64_t)op2.value.number;
    }
    else
    {
        VirtualMachine_add_error_msg(error_code_type_mismatch);
    }

    return ret;
}

static object_t op_con(object_t op1, object_t op2)
{
    object_t op1_str = Obj_to_string(op1);
    object_t op2_str = Obj_to_string(op2);

    object_t ret = Obj_conc_string(op1_str, op2_str);

    if (ret.type == object_type_null)
    {
        VirtualMachine_add_error_msg(error_code_type_mismatch);
    }

    return ret;
}

static object_t op_lt(object_t op1, object_t op2)
{
    object_t ret;

    ret.type = object_type_boolean;

    if (op1.type == object_type_number && op2.type == object_type_number)
    {
        ret.value.boolean = op1.value.number < op2.value.number;
    }
    else if (op1.type == object_type_string && op2.type == object_type_string)
    {
        ret.value.boolean = Obj_comp_string(op1, op2) < 0 ? true : false;
    }
    else
    {
        VirtualMachine_add_error_msg(error_code_type_mismatch);
    }

    return ret;
}

static object_t op_gt(object_t op1, object_t op2)
{
    object_t ret;

    ret.type = object_type_boolean;

    if (op1.type == object_type_number && op2.type == object_type_number)
    {
        ret.value.boolean = op1.value.number > op2.value.number;
    }
    else if (op1.type == object_type_string && op2.type == object_type_string)
    {
        ret.value.boolean = Obj_comp_string(op1, op2) > 0 ? true : false;
    }
    else
    {
        VirtualMachine_add_error_msg(error_code_type_mismatch);
    }

    return ret;
}

static object_t op_le(object_t op1, object_t op2)
{
    object_t ret;

    ret.type = object_type_boolean;

    if (op1.type == object_type_number && op2.type == object_type_number)
    {
        ret.value.boolean = op1.value.number <= op2.value.number;
    }
    else if (op1.type == object_type_string && op2.type == object_type_string)
    {
        ret.value.boolean = Obj_comp_string(op1, op2) <= 0 ? true : false;
    }
    else
    {
        VirtualMachine_add_error_msg(error_code_type_mismatch);
    }
    
    return ret;
}

static object_t op_ge(object_t op1, object_t op2)
{
    object_t ret;

    ret.type = object_type_boolean;

    if (op1.type == object_type_number && op2.type == object_type_number)
    {
        ret.value.boolean = op1.value.number >= op2.value.number;
    }
    else if (op1.type == object_type_string && op2.type == object_type_string)
    {
        ret.value.boolean = Obj_comp_string(op1, op2) >= 0 ? true : false;
    }
    else
    {
        VirtualMachine_add_error_msg(error_code_type_mismatch);
    }
    
    return ret;
}

static object_t op_eq(object_t op1, object_t op2)
{
    object_t ret;

    ret.type = object_type_boolean;

    if (op1.type == object_type_number && op2.type == object_type_number)
    {
        ret.value.boolean = op1.value.number == op2.value.number;
    }
    else if (op1.type == object_type_string && op2.type == object_type_string)
    {
        ret.value.boolean = Obj_comp_string(op1, op2) == 0 ? true : false;
        if (ret.value.boolean)
        {
            ret.value.boolean = strlen(op1.value.string_ptr) == strlen(op2.value.string_ptr) ? true : false;
        }
    }
    else
    {
        VirtualMachine_add_error_msg(error_code_type_mismatch);
    }
    
    return ret;
}

static object_t op_ne(object_t op1, object_t op2)
{
    object_t ret;

    ret.type = object_type_boolean;

    if (op1.type == object_type_number && op2.type == object_type_number)
    {
        ret.value.boolean = op1.value.number != op2.value.number;
    }
    else if (op1.type == object_type_string && op2.type == object_type_string)
    {
        ret.value.boolean = Obj_comp_string(op1, op2) != 0 ? true : false;
    }
    else
    {
        VirtualMachine_add_error_msg(error_code_type_mismatch);
    }
    
    return ret;
}

static object_t op_xor(object_t op1, object_t op2)
{
    object_t ret;

    ret.type = object_type_number;

    if (op1.type == object_type_number && op2.type == object_type_number)
    {
        ret.value.number = (uint64_t)op1.value.number ^ (uint64_t)op2.value.number;
    }
    else
    {
        VirtualMachine_add_error_msg(error_code_type_mismatch);
    }
    
    return ret;
}

static object_t op_rshift(object_t op1, object_t op2)
{
    object_t ret;

    ret.type = object_type_number;

    if (op1.type == object_type_number && op2.type == object_type_number)
    {
        ret.value.number = (uint64_t)op1.value.number >> (uint64_t)op2.value.number;
    }
    else
    {
        VirtualMachine_add_error_msg(error_code_type_mismatch);
    }
    
    return ret;
}

static object_t op_lshift(object_t op1, object_t op2)
{
    object_t ret;

    ret.type = object_type_number;

    if (op1.type == object_type_number && op2.type == object_type_number)
    {
        ret.value.number = (uint64_t)op1.value.number << (uint64_t)op2.value.number;
    }
    else
    {
        VirtualMachine_add_error_msg(error_code_type_mismatch);
    }
    
    return ret;
}