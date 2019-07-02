%{
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "object.h"
#include "virtual_machine.h"
#include "code_gen.h"

extern int yylex (void);

int yyerror(const char* str)
{
    printf("[Parse Error] %s\n", str);
    return 0;
}
%}

%union {
    uint32_t    int_value;
    double      double_value;
    char*       string_ptr;
}

%start prog

%token                  STRCON
%token                  COMAND COMOR
%token                  EQ NE LE GE
%token                  RSHFT LSHFT
%token<double_value>    NUMBER
%token<string_ptr>      STRING
%token<string_ptr>      IDENTIFIER            

%type<int_value>    expr
%type<string_ptr>   type

%left COMAND COMOR
%left EQ NE '>' '<' LE GE
%left STRCON
%left '|' '&'
%left RSHFT LSHFT
%left '+' '-'
%left '^'
%left '*' '/' '%'

%%
prog : expr
     | stmt_with_semiconlon
     ;

stmt_with_semiconlon : stmt ';'
                     ;

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
     | expr RSHFT expr      {CodeGen_add_opcode(opcode_rshift); $$ = 0;}
     | expr LSHFT expr      {CodeGen_add_opcode(opcode_lshift); $$ = 0;}
     | expr '|' expr        {CodeGen_add_opcode(opcode_bit_or); $$ = 0;}
     | expr '&' expr        {CodeGen_add_opcode(opcode_bit_and); $$ = 0;}
     | expr COMAND expr     {CodeGen_add_opcode(opcode_com_and); $$ = 0;}
     | expr COMOR expr      {CodeGen_add_opcode(opcode_com_or); $$ = 0;}
     ;

stmt : /* empty */
     | decl
     ;

decl : type IDENTIFIER '=' expr   {/*CodeGen_add_variable($1, $2); CodeGen_add_opcode(opcode_store); free($1); free($2);*/}
     ;

type : IDENTIFIER
     ;

%%
