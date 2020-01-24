/*
errors.h

Copyright (c) 1/23/2020, 11:19:51 PM Manwoo Yi

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
typedef enum _error_code_e_ {
    error_none                     = 0x00,

    // Parser errors
    error_parser_syntax            = 0x00001,

    // VM errors
    error_vm_type_mismatch         = 0x10001,
    error_vm_unknown_opcode        = 0x10002,
} error_code_e;
/**************************
 * Public function prototypes
 **************************/

/**************************
 * Inline functions
 **************************/

