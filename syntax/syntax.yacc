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

static void Modify_jump_addr(code_buf_t* dst, code_buf_t* jmp)
{
    code_buf_t jmp_addr_bytecode;
    uint64_t offset = jmp - dst;
    jmp_addr_bytecode.value.type = object_type_general;
    jmp_addr_bytecode.value.value.general = offset;
    CodeGen_modify_codebuf(dst, jmp_addr_bytecode);
}

static void Modify_op(code_buf_t* dst, opcode_t opcode)
{
    code_buf_t opcode_bytecode;
    opcode_bytecode.opcode = opcode;
    CodeGen_modify_codebuf(dst, opcode_bytecode);
}

static void Modify_jump_addr_with_op(opcode_t opcode, code_buf_t* dst, code_buf_t* jmp)
{
    Modify_op(dst, opcode);
    dst++;

    Modify_jump_addr(dst, jmp);
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

%token                  IF ELSE ELIF

%type<string_ptr>   load_first_var
%type<code_ptr>     _current_pc_ block elif_list begin_block end_block

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

condition_stmt : IF comparison_expr block           {Modify_jump_addr_with_op(opcode_cmp, $3, CodeGen_current_bytecode_ptr());}
               | IF comparison_expr block elif_list {Modify_jump_addr_with_op(opcode_cmp, $3, $4);}
               ;

_current_pc_ : {$$ = CodeGen_current_bytecode_ptr();}
             ;

elif_list : ELSE _current_pc_ block                           {$$ = $2; Modify_jump_addr_with_op(opcode_jmp, ((code_buf_t*)$2 - 2), CodeGen_current_bytecode_ptr());}
          | ELIF _current_pc_ comparison_expr block           {$$ = $2; Modify_jump_addr_with_op(opcode_jmp, ((code_buf_t*)$2 - 2), CodeGen_current_bytecode_ptr());
                                                                        Modify_jump_addr_with_op(opcode_cmp, $4, CodeGen_current_bytecode_ptr());}
          | ELIF _current_pc_ comparison_expr block elif_list {$$ = $2; Modify_jump_addr_with_op(opcode_jmp, ((code_buf_t*)$2 - 2), CodeGen_current_bytecode_ptr());
                                                                        Modify_jump_addr_with_op(opcode_cmp, $4, $5);}     
          ;
      
block : begin_block stmtlist end_block  {$$ = ((code_buf_t*)$1 - 2);}
      ;

begin_block : '{'   {CodeGen_skip_bytecode_count(2); $$ = CodeGen_current_bytecode_ptr(); CodeGen_add_opcode(opcode_begin_scope);}
            ;
end_block : '}'     {CodeGen_add_opcode(opcode_end_scope); CodeGen_skip_bytecode_count(2); $$ = CodeGen_current_bytecode_ptr();}
          ;
%%
