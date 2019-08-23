/*
code_gen.h

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

#ifndef SRC_CODE_GEN_H_
#define SRC_CODE_GEN_H_

/**************************
 * Includes
 **************************/
#include "planck_types.h"

/**************************
 * Public function prototypes
 **************************/
void        CodeGen_reset_bytecodes(void);
code_buf_t* CodeGen_get_bytecodes(void);
code_buf_t* CodeGen_current_bytecode_ptr(void);
void        CodeGen_skip_bytecode_count(uint32_t count);
void        CodeGen_add_opcode(opcode_t opcode);
void        CodeGen_add_number(double number);
void        CodeGen_add_string(char* str_ptr);
void        CodeGen_add_variable(const char* type_str, const char* ident_str);
void        CodeGen_add_argument(int64_t general);
void        CodeGen_read_symtab_variable(const char* ident_str);
code_buf_t  CodeGen_gen_general_bytecode(int64_t genvalue);
void        CodeGen_modify_codebuf(code_buf_t* dst, code_buf_t src);

#endif /* SRC_CODE_GEN_H_ */
