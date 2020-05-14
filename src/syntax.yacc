/*
syntax.yacc

Copyright (c) 12/23/2019, 11:38:08 PM Manwoo Yi

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

%{
#include <stdint.h>
#include <stdio.h>

#include "codegen.h"
#include "utils.h"
#include "symtab.h"

extern int yylex (void);

int yyerror(const char* str)
{
    //DEBUG_PRINT("[Parse Error] %s\n", str);
    return 0;
}

%}

%union {
    int64_t     int_value;
    double      double_value;
    char*       identifier;
}

%start prog

%token<int_value>       INUM
%token<double_value>    DNUM
%token<identifier>      IDENTIFIER

%token      OPENBR CLOSEBR OPENBLOCK CLOSEBLOCK
%token      PLUS MINUS STAR SLASH EQUAL
%token      K_LET K_AS K_STRUCT

%%
prog
: expr
| declaration
| assignment
| struct_declaration
;

declaration
: K_LET IDENTIFIER EQUAL expr
                            {codegen_add_store_load('S', symtab_add_symbol($2));}
| K_LET IDENTIFIER EQUAL expr K_AS IDENTIFIER
                            {codegen_add_store_load('S', symtab_add_symbol_type($2, $6));}
;

member_decl_list
: member_decl
| member_decl member_decl_list
;

member_decl
: K_LET IDENTIFIER K_AS IDENTIFIER
;

struct_declaration
: K_STRUCT IDENTIFIER OPENBLOCK member_decl_list CLOSEBLOCK
;

assignment
: IDENTIFIER EQUAL expr     {codegen_add_store_load('S', symtab_get_idx_by_name($1));
                             release_mem($1);
                            }
;

expr
: term
| term PLUS term            {codegen_add_opcode(opcode_add);}
| term MINUS term           {codegen_add_opcode(opcode_sub);}
;

term
: factor
| factor STAR factor        {codegen_add_opcode(opcode_mul);}
| factor SLASH factor       {codegen_add_opcode(opcode_div);}
//| factor '%' factor
;

factor
: unary
//| unary '^' factor
;

unary
: primary
| PLUS unary
| MINUS {codegen_add_num(valtype_int, (val_t)(-1));} unary {codegen_add_opcode(opcode_mul);}
;

primary
: constant
| OPENBR expr CLOSEBR
| identifier
;

identifier
: IDENTIFIER                {codegen_add_store_load('L', symtab_get_idx_by_name($1));
                             release_mem($1);
                            }
;

constant
: INUM                      {codegen_add_num(valtype_int, (val_t)($1));}
| DNUM                      {codegen_add_num(valtype_num, (val_t)($1));}
;
%%