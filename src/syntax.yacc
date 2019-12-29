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

extern int yylex (void);

int yyerror(const char* str)
{
    DEBUG_PRINT("[Parse Error] %s\n", str);
    return 0;
}
%}

%union {
    int64_t     int_value;
    double      double_value;
}

%start prog

%token<int_value>       INUM
%token<double_value>    DNUM

%token      ADD SUB MUL DIV

%left       ADD SUB
%left       MUL DIV

%%
prog    :   expr
        ;

expr    :   INUM            {codegen_add_num(valtype_int,    (val_t)$1);}
        |   DNUM            {codegen_add_num(valtype_double, (val_t)$1);}
        |   expr ADD expr   {codegen_add_opcode(opcode_add);}
        |   expr SUB expr   {codegen_add_opcode(opcode_sub);}
        |   expr MUL expr   {codegen_add_opcode(opcode_mul);}
        |   expr DIV expr   {codegen_add_opcode(opcode_div);}
        ;
%%