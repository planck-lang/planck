%{
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "y.tab.h"
#include "ported_lib.h"
#include "symtab.h"

extern int fileno(FILE *stream);

int yywrap(void)
{
    return 1;
}
%}

%%
[ \t\n]*      ;

"##"        return STRCON;
"=="        return EQ;
"!="        return NE;
"<="        return LE;
">="        return GE;
">>"        return RSHFT;
"<<"        return LSHFT;
" AND "     return COMAND;
" OR "      return COMOR;
"+="        return ADDASSIGN;
"-="        return SUBASSIGN;
"*="        return MULASSIGN;
"/="        return DIVASSIGN;
"%="        return MODASSIGN;
"##="       return CONCASSIGN;
"<<="       return LSHTASSIGN;
">>="       return RSHTASSIGN;
"|="        return ORASSIGN;
"&="        return ANDASSIGN;
"^="        return XORASSIGN;

"if "       {
    Symtab_set_block_input(true);
    return IF;
}

"else "     {
    Symtab_set_block_input(true);
    return ELSE;
}

"elif "     {
    Symtab_set_block_input(true);
    return ELIF;
}

"while "    {
    Symtab_set_block_input(true);
    return WHILE;
}

[0-9]+("."[0-9]*)? {
    yylval.double_value = str_to_number(yytext, 10);
    return NUMBER;
}

"-"?"0"[xX][a-fA-F0-9]+ {
    yylval.double_value = str_to_number(yytext, 16);
    return NUMBER;
}

"-"?"0"[bB][01]+ {
    yylval.double_value = str_to_number(yytext, 2);
    return NUMBER;
}

\"[^\"]*\"  |
\'[^\']*\'  {
    yylval.string_ptr = yytext;
    return STRING;
}

[a-zA-Z_][0-9a-zA-Z_]* {
    yylval.string_ptr = str_dup(yytext, strlen(yytext));
    return IDENTIFIER;
}

.           {       // It must be the last rule because it can match ANY characters
    if (*yytext == '}')
    {
        Symtab_set_block_input(false);
    }
    return *yytext;
}
%%

