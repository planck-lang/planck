/*
types.h

Copyright (c) 12/24/2019, 12:38:31 AM Manwoo Yi

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
 
 #pragma once
 
/**************************
 * Includes
 **************************/

/**************************
 * Macro
 **************************/
 
/**************************
 * Data types, Constants
 **************************/
typedef union _val_t_ {
    int64_t ival;
    double  dval;
} val_t;

typedef enum _valtype_e_ {
    valtype_none = 0,
    valtype_int  = 1,
    valtype_double,
    valtype_primitive_max,
    valtype_builtin_max,
    valtype_max
} valtype_e;

typedef enum _opcode_e_ {
    opcode_nop  = 0,
    opcode_add  = 1,
    opcode_sub  = 2,
    opcode_mul  = 3,
    opcode_div  = 4,
    opcode_push = 5,
} opcode_e;

typedef struct _data_t_ {
    valtype_e   valtype;
    val_t       val;
} data_t;

typedef struct _objcode_t_ {
    opcode_e    opcode;
    data_t      data;
} objcode_t;

/**************************
 * Public function prototypes
 **************************/
 
/**************************
 * Inline functions
 **************************/
