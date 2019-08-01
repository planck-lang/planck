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

static void Modify_jump_addr_with_op(opcode_t opcode, code_buf_t* dst, code_buf_t* jmp)
{
    code_buf_t opcode_bytecode;
    code_buf_t jmp_addr_bytecode;

    opcode_bytecode.opcode = opcode;
    CodeGen_modify_codebuf(dst, opcode_bytecode);
    dst++;

    uint64_t offset = jmp - dst;

    jmp_addr_bytecode.value.type = object_type_general;
    jmp_addr_bytecode.value.value.general = offset;
    CodeGen_modify_codebuf(dst, jmp_addr_bytecode);
    dst++;

    opcode_bytecode.opcode = opcode_begin_scope;
    CodeGen_modify_codebuf(dst, opcode_bytecode);
    dst++;
}
%}

%union {
    int64_t     int_value;
    double      double_value;
    char*       string_ptr;
    void*       code_ptr;
}

%start prog

%token                  STRCON
%token                  COMAND COMOR
%token                  EQ NE LE GE
%token                  RSHFT LSHFT
%token                  ADDASSIGN SUBASSIGN MULASSIGN DIVASSIGN MODASSIGN CONCASSIGN
%token                  RSHTASSIGN LSHTASSIGN
%token                  ORASSIGN ANDASSIGN XORASSIGN
%token<double_value>    NUMBER
%token<string_ptr>      STRING
%token<string_ptr>      IDENTIFIER

%token                  IF

%type<string_ptr>   load_first_var
%type<code_ptr>     jump_index_expr

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
     | stmtlist
     ;

stmtlist : stmt	
         | stmt ';' stmtlist
         ;

expr : NUMBER               {CodeGen_add_number($1);}
     | STRING               {CodeGen_add_string($1);}
     | IDENTIFIER           {Identifier_load($1); free($1);}
     | comparison_expr
     | basic_op_expr
     | bit_op_expr
     | '(' expr ')'         
     ;

basic_op_expr : expr '+' expr        {CodeGen_add_opcode(opcode_add);}
              | expr '-' expr        {CodeGen_add_opcode(opcode_sub);}
              | expr '*' expr        {CodeGen_add_opcode(opcode_mul);}
              | expr '/' expr        {CodeGen_add_opcode(opcode_div);}
              | expr '%' expr        {CodeGen_add_opcode(opcode_mod);}
              | expr '^' expr        {CodeGen_add_opcode(opcode_xor);}
              | expr STRCON expr     {CodeGen_add_opcode(opcode_con);}
              ;

bit_op_expr : expr RSHFT expr      {CodeGen_add_opcode(opcode_rshift);}
            | expr LSHFT expr      {CodeGen_add_opcode(opcode_lshift);}
            | expr '|' expr        {CodeGen_add_opcode(opcode_bit_or);}
            | expr '&' expr        {CodeGen_add_opcode(opcode_bit_and);}
            ;

comparison_expr : expr '<' expr        {CodeGen_add_opcode(opcode_lt);}
                | expr '>' expr        {CodeGen_add_opcode(opcode_gt);}
                | expr LE expr         {CodeGen_add_opcode(opcode_le);}
                | expr GE expr         {CodeGen_add_opcode(opcode_ge);}
                | expr EQ expr         {CodeGen_add_opcode(opcode_eq);}
                | expr NE expr         {CodeGen_add_opcode(opcode_ne);}
                | expr COMAND expr     {CodeGen_add_opcode(opcode_com_and);}
                | expr COMOR expr      {CodeGen_add_opcode(opcode_com_or);}
                ;

stmt : /* empty */
     | decl
     | assign
     | condition_stmt
     ;

decl : IDENTIFIER IDENTIFIER '=' expr   {Variable_declaration($1, $2); free($1); free($2);}
     ;

assign : IDENTIFIER '=' expr            {Variable_assignment($1); free($1);}
       | load_first_var ADDASSIGN expr  {Variable_assignment_with_op(opcode_add, $1); free($1);}
       | load_first_var SUBASSIGN expr  {Variable_assignment_with_op(opcode_sub, $1); free($1);}
       | load_first_var MULASSIGN expr  {Variable_assignment_with_op(opcode_mul, $1); free($1);}
       | load_first_var DIVASSIGN expr  {Variable_assignment_with_op(opcode_div, $1); free($1);}
       | load_first_var MODASSIGN expr  {Variable_assignment_with_op(opcode_mod, $1); free($1);}
       | load_first_var CONCASSIGN expr {Variable_assignment_with_op(opcode_con, $1); free($1);}
       | load_first_var RSHTASSIGN expr {Variable_assignment_with_op(opcode_rshift, $1); free($1);}
       | load_first_var LSHTASSIGN expr {Variable_assignment_with_op(opcode_lshift, $1); free($1);}
       | load_first_var ORASSIGN expr   {Variable_assignment_with_op(opcode_bit_or, $1); free($1);}
       | load_first_var ANDASSIGN expr  {Variable_assignment_with_op(opcode_bit_and, $1); free($1);}
       | load_first_var XORASSIGN expr  {Variable_assignment_with_op(opcode_xor, $1); free($1);}
       ;

load_first_var : IDENTIFIER         {Identifier_load($1);}
               ;

jump_index_expr : comparison_expr   {$$ = CodeGen_current_bytecode_ptr(); CodeGen_skip_bytecode_count(3);}
                ;

condition_stmt : IF jump_index_expr block  {Modify_jump_addr_with_op(opcode_cmp, $2, CodeGen_current_bytecode_ptr());}
               ;
      
block : '{' stmtlist '}'            {CodeGen_add_opcode(opcode_end_scope);}
      ;
%%
