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
    int64_t offset = jmp - dst;
    code_buf_t jmp_addr = CodeGen_gen_general_bytecode(offset);
    
    CodeGen_modify_codebuf(dst, jmp_addr);
}

static void Modify_op(code_buf_t* dst, opcode_t opcode)
{
    code_buf_t opcode_addr;
    opcode_addr.type = code_buf_opcode;
    opcode_addr.bytecode.opcode = opcode;
    CodeGen_modify_codebuf(dst, opcode_addr);
}

static void Modify_jump_addr_with_op(opcode_t opcode, code_buf_t* dst, code_buf_t* jmp)
{
    Modify_op(dst, opcode);
    dst++;
    Modify_jump_addr(dst, jmp);
}

static void Jmp_cmp(code_buf_t* jmp_dst, code_buf_t* jmp_offset, code_buf_t* cmp_dst, code_buf_t* cmp_offset)
{
    if (jmp_dst != NULL)
    {
        Modify_jump_addr_with_op(opcode_jmp, jmp_dst, jmp_offset);
    }

    if (cmp_dst != NULL)
    {
        Modify_jump_addr_with_op(opcode_cmp, cmp_dst, cmp_offset);
    }
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

%token                  IF ELSE ELIF WHILE
%token                  BREAK CONTINUE

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
prog : exprlist
     | stmtlist
     ;

stmtlist : stmt	
         | stmt ';' stmtlist
         | condition_stmt stmtlist
         | loop_stmt stmtlist
         ;

exprlist : expr
         | expr ',' exprlist
         ;

expr : NUMBER               {CodeGen_add_number($1);}
     | STRING               {CodeGen_add_string($1);}
     | IDENTIFIER           {Identifier_load($1); free($1);}
     | comparison_expr
     | basic_op_expr
     | bit_op_expr
     | arr_list_expr
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

arr_list_expr : '[' exprlist ']'
              ;

stmt : /* empty */
     | decl
     | assign
     | BREAK        {CodeGen_add_opcode(opcode_break);    CodeGen_add_opcode(opcode_nop);}
     | CONTINUE     {CodeGen_add_opcode(opcode_continue); CodeGen_add_opcode(opcode_nop);}
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

condition_stmt : IF comparison_expr block           {Jmp_cmp(NULL, NULL, $3, CodeGen_current_bytecode_ptr());}
               | IF comparison_expr block elif_list {Jmp_cmp(NULL, NULL, $3, $4);}
               ;

loop_stmt : WHILE _begin_loop_ _current_pc_ comparison_expr block _end_loop_ {Jmp_cmp((CodeGen_current_bytecode_ptr() - 3), $3, 
                                                                                       $5, (CodeGen_current_bytecode_ptr() - 1));}
          ;

_current_pc_ : {$$ = CodeGen_current_bytecode_ptr();}
             ;
_begin_loop_ : {CodeGen_add_opcode(opcode_begin_loop);}
             ;
_end_loop_ :   {CodeGen_add_opcode(opcode_end_loop);}
           ;

elif_list : ELSE _current_pc_ block                           {$$ = $2; Jmp_cmp(((code_buf_t*)$2 - 2), CodeGen_current_bytecode_ptr(), NULL, NULL);}
          | ELIF _current_pc_ comparison_expr block           {$$ = $2; Jmp_cmp(((code_buf_t*)$2 - 2), CodeGen_current_bytecode_ptr(), $4, CodeGen_current_bytecode_ptr());}
          | ELIF _current_pc_ comparison_expr block elif_list {$$ = $2; Jmp_cmp(((code_buf_t*)$2 - 2), CodeGen_current_bytecode_ptr(), $4, $5);}
          ;
      
block : begin_block stmtlist end_block  {$$ = ((code_buf_t*)$1 - 2);}
      ;

begin_block : '{'   {CodeGen_skip_bytecode_count(2); $$ = CodeGen_current_bytecode_ptr(); CodeGen_add_opcode(opcode_begin_scope);}
            ;
end_block : '}'     {CodeGen_add_opcode(opcode_end_scope); CodeGen_skip_bytecode_count(2); $$ = CodeGen_current_bytecode_ptr();}
          ;
%%
