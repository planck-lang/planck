/*
symtab.h

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

#ifndef SRC_SYMTAB_H_
#define SRC_SYMTAB_H_

#include "planck_types.h"

char* Symtab_add_string_literal(const char* parser_str_ptr, uint32_t* out_idx);
char* Symtab_get_string_literal_by_idx(uint32_t idx);

uint32_t Symtab_get_idx_of_type(const char* type_str_ptr);
uint32_t Symtab_add_variable(uint32_t type_idx, const char* ident_str_ptr);
uint32_t Symtab_get_idx_variable_by_identifier(const char* ident_str_ptr);
bool     Symtab_is_exist_variable(const char* ident_str_ptr);
void     Symtab_store_value_to_symtab(uint32_t symtab_idx, object_t value, bool fail_del_sym);
object_t Symtab_load_value_from_symtab(uint32_t symtab_idx);

#endif /* SRC_SYMTAB_H_ */
