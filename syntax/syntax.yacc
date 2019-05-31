%{
#include <stdint.h>
#include <stdio.h>

#include "virtual_machine.h"
#include "code_gen.h"

extern int yylex (void);

int yyerror(const char* str)
{
    extern char* yytext;
    printf("Parse Error : %s\n", yytext);
    return 0;
}
%}

%union {
    uint32_t    int_value;
    double      double_value;
}

%start expr

%token<double_value>    NUMBER

%type<int_value> expr

%left '+' '-'
%left '*' '/' '%'

%%
expr : NUMBER               {CodeGen_add_number($1); $$ = 0;}
     | '(' expr ')'         {$$ = $2;}
     | expr '+' expr        {CodeGen_add_opcode(opcode_add); $$ = 0;}
     | expr '-' expr        {CodeGen_add_opcode(opcode_sub); $$ = 0;}
     | expr '*' expr        {CodeGen_add_opcode(opcode_mul); $$ = 0;}
     | expr '/' expr        {CodeGen_add_opcode(opcode_div); $$ = 0;}
     | expr '%' expr        {CodeGen_add_opcode(opcode_mod); $$ = 0;}
     ;
%%
