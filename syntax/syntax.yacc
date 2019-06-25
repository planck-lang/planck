%{
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "object.h"
#include "virtual_machine.h"
#include "code_gen.h"

extern int yylex (void);

int yyerror(const char* str)
{
    printf("[Error] %s\n", str);
    return 0;
}
%}

%union {
    uint32_t    int_value;
    double      double_value;
    char*       string_ptr;
}

%start expr

%token                  STRCON
%token                  EQ NE LE GE
%token<double_value>    NUMBER
%token<string_ptr>      STRING

%type<int_value> expr

%left EQ NE '>' '<' LE GE
%left STRCON
%left '+' '-'
%left '^'
%left '*' '/' '%'

%%
expr : NUMBER               {CodeGen_add_number($1); $$ = 0;}
     | STRING               {CodeGen_add_string($1); $$ = 0;}
     | '(' expr ')'         {$$ = $2;}
     | expr '<' expr        {CodeGen_add_opcode(opcode_lt); $$ = 0;}
     | expr '>' expr        {CodeGen_add_opcode(opcode_gt); $$ = 0;}
     | expr LE expr         {CodeGen_add_opcode(opcode_le); $$ = 0;}
     | expr GE expr         {CodeGen_add_opcode(opcode_ge); $$ = 0;}
     | expr EQ expr         {CodeGen_add_opcode(opcode_eq); $$ = 0;}
     | expr NE expr         {CodeGen_add_opcode(opcode_ne); $$ = 0;}
     | expr '+' expr        {CodeGen_add_opcode(opcode_add); $$ = 0;}
     | expr '-' expr        {CodeGen_add_opcode(opcode_sub); $$ = 0;}
     | expr '*' expr        {CodeGen_add_opcode(opcode_mul); $$ = 0;}
     | expr '/' expr        {CodeGen_add_opcode(opcode_div); $$ = 0;}
     | expr '%' expr        {CodeGen_add_opcode(opcode_mod); $$ = 0;}
     | expr '^' expr        {CodeGen_add_opcode(opcode_xor); $$ = 0;}
     | expr STRCON expr     {CodeGen_add_opcode(opcode_con); $$ = 0;}
     ;
%%
