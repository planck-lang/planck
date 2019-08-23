/*
planck_types.h

Copyright (c) Jun 5, 2019 Manwoo Yi

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

#ifndef SRC_PLANCK_TYPES_H_
#define SRC_PLANCK_TYPES_H_

/**************************
 * Includes
 **************************/
#include "stdbool.h"

/**************************
 * Data types, Constants
 **************************/
/*
 * common
 */
typedef enum _error_code_t_ {
    error_code_no_error = 0,
    error_code_type_mismatch,
    error_code_undefined_type,
    error_code_not_found_symbol,
    error_code_redefinition,
    error_code_must_be_bool,
    error_code_not_in_loop,
    error_code_MAXNUM
} error_code_t;


/*
 * object types
 */
typedef enum _object_type_t_ {
    object_type_null = 0,
    object_type_general,
    object_type_number,
    object_type_string,
    object_type_boolean,
    object_type_array,
    object_type_MAXNUM
} object_type_t;

typedef struct _str_t_ {
    char*    ptr;
    uint32_t table_idx;
} str_t;

typedef struct _list_t_ {
    struct _list_t_ *next;
    void* ptr_value;
} list_t;

typedef struct _object_t_ {
    object_type_t type;

    union _value_t_ {
        int64_t  general;
        double   number;
        str_t    str;
        bool     boolean;
        list_t*  combined; 
    } value;
} object_t;


/*
 * virtual machine types
 */
typedef enum _opcode_t_ {
    opcode_nop = 0,
    opcode_push,
    opcode_decl,
    opcode_store,
    opcode_load,
    opcode_add,
    opcode_sub,
    opcode_xor,
    opcode_mul,
    opcode_div,
    opcode_mod,
    opcode_con,
    opcode_lt,
    opcode_gt,
    opcode_le,
    opcode_ge,
    opcode_eq,
    opcode_ne,
    opcode_rshift,
    opcode_lshift,
    opcode_bit_or,
    opcode_bit_and,
    opcode_com_or,
    opcode_com_and,
    opcode_cmp,
    opcode_jmp,
    opcode_begin_scope,
    opcode_end_scope,
    opcode_begin_loop,
    opcode_end_loop,
    opcode_break,
    opcode_continue,
    opcode_array,
    opcode_halt,
    opcode_MAXNUM
} opcode_t;

typedef enum _code_buf_type_ {
    code_buf_opcode,
    code_buf_value
} code_buf_type_t;

/*
 * code generator types
 */
typedef struct _code_buf_t_ {
    code_buf_type_t type;
    union {
        opcode_t    opcode;
        object_t    value;
    } bytecode;
} code_buf_t;

#endif /* SRC_PLANCK_TYPES_H_ */
