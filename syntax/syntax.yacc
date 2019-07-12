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

static void Identifier_load(char* var_str)
{
    CodeGen_add_opcode(opcode_load);
    CodeGen_read_symtab_variable(var_str);
}

static void Variable_declaration(char* type_str, char* var_str)
{
    CodeGen_add_opcode(opcode_decl); 
    CodeGen_add_variable(type_str, var_str);
}

static void Variable_assignment(char* var_str)
{
    CodeGen_add_opcode(opcode_store);
    CodeGen_read_symtab_variable(var_str);
}

static void Variable_assignment_with_op(opcode_t opcode, char* var_str)
{
    CodeGen_add_opcode(opcode);
    CodeGen_add_opcode(opcode_store);
    CodeGen_read_symtab_variable(var_str);
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
%token                  ADDASSIGN SUBASSIGN MULASSIGN DIVASSIGN MODASSIGN
%token<double_value>    NUMBER
%token<string_ptr>      STRING
%token<string_ptr>      IDENTIFIER

%type<string_ptr>   type
%type<string_ptr>   load_first_var

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
     | stmt_with_semiconlon_or_not
     ;

stmt_with_semiconlon_or_not : stmt ';'
                            | stmt
                            ;

expr : NUMBER               {CodeGen_add_number($1);}
     | STRING               {CodeGen_add_string($1);}
     | IDENTIFIER           {Identifier_load($1); free($1);}
     | expr '<' expr        {CodeGen_add_opcode(opcode_lt);}
     | expr '>' expr        {CodeGen_add_opcode(opcode_gt);}
     | expr LE expr         {CodeGen_add_opcode(opcode_le);}
     | expr GE expr         {CodeGen_add_opcode(opcode_ge);}
     | expr EQ expr         {CodeGen_add_opcode(opcode_eq);}
     | expr NE expr         {CodeGen_add_opcode(opcode_ne);}
     | expr '+' expr        {CodeGen_add_opcode(opcode_add);}
     | expr '-' expr        {CodeGen_add_opcode(opcode_sub);}
     | expr '*' expr        {CodeGen_add_opcode(opcode_mul);}
     | expr '/' expr        {CodeGen_add_opcode(opcode_div);}
     | expr '%' expr        {CodeGen_add_opcode(opcode_mod);}
     | expr '^' expr        {CodeGen_add_opcode(opcode_xor);}
     | expr STRCON expr     {CodeGen_add_opcode(opcode_con);}
     | expr RSHFT expr      {CodeGen_add_opcode(opcode_rshift);}
     | expr LSHFT expr      {CodeGen_add_opcode(opcode_lshift);}
     | expr '|' expr        {CodeGen_add_opcode(opcode_bit_or);}
     | expr '&' expr        {CodeGen_add_opcode(opcode_bit_and);}
     | expr COMAND expr     {CodeGen_add_opcode(opcode_com_and);}
     | expr COMOR expr      {CodeGen_add_opcode(opcode_com_or);}
     | '(' expr ')'         
     ;

stmt : /* empty */
     | decl
     | assign
     ;

decl : type IDENTIFIER '=' expr   {Variable_declaration($1, $2); free($1); free($2);}
     ;

type : IDENTIFIER
     ;

assign : IDENTIFIER '=' expr        {Variable_assignment($1); free($1);}
       | load_first_var ADDASSIGN expr  {Variable_assignment_with_op(opcode_add, $1); free($1);}
       | load_first_var SUBASSIGN expr  {Variable_assignment_with_op(opcode_sub, $1); free($1);}
       | load_first_var MULASSIGN expr  {Variable_assignment_with_op(opcode_mul, $1); free($1);}
       | load_first_var DIVASSIGN expr  {Variable_assignment_with_op(opcode_div, $1); free($1);}
       | load_first_var MODASSIGN expr  {Variable_assignment_with_op(opcode_mod, $1); free($1);}
       ;

load_first_var : IDENTIFIER         {Identifier_load($1);}
%%
