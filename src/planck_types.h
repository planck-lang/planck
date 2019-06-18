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

/*
 * common
 */
typedef enum _error_code_t_ {
    error_code_no_error = 0,
    error_code_type_mismatch,
    error_code_NUM
} error_code_t;


/*
 * object types
 */
typedef enum _object_type_t_ {
    object_type_null = 0,
    object_type_number,
    object_type_string,
    object_type_MAXNUM
} object_type_t;

typedef struct _object_t_ {
    object_type_t type;

    union _value_t_ {
        double  number;
        char*   string_ptr;
    } value;
} object_t;


/*
 * virtual machine types
 */
typedef enum _opcode_t_ {
    opcode_nop = 0,
    opcode_push,
    opcode_add,
    opcode_sub,
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
    opcode_halt,
    opcode_MAXNUM
} opcode_t;


/*
 * code generator types
 */
typedef union _code_buf_t_ {
    opcode_t    opcode;
    object_t    value;
} code_buf_t;

#endif /* SRC_PLANCK_TYPES_H_ */
