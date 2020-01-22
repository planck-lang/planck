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

static int64_t plusminus = 1;
%}

%union {
    int64_t     int_value;
    double      double_value;
}

%start prog

%token<int_value>       INUM
%token<double_value>    DNUM

%token      OPENBR CLOSEBR
%token      PLUS MINUS STAR SLASH

%%
prog    
:   expr                    {plusminus = 1;}
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
| MINUS unary               {plusminus = -1;}
;

primary
: constant
| OPENBR expr CLOSEBR
//| id
;

constant
: INUM                      {codegen_add_num(valtype_int, (val_t)(plusminus * $1));}
| DNUM                      {codegen_add_num(valtype_double, (val_t)(plusminus * $1));}
;
%%