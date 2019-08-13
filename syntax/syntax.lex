%{
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "y.tab.h"
#include "ported_lib.h"

extern int fileno(FILE *stream);

static uint32_t block_depth = 0;

uint32_t yy_get_block_depth(void)
{
    return block_depth;
}

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

"if "       return IF;
"else "     return ELSE;
"elif "     return ELIF;
"while "    return WHILE;
"break"     return BREAK;
"continue"  return CONTINUE;

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

. {       // It must be the last rule because it can match ANY characters
    if (*yytext == '{')
    {
        block_depth++;
    }
    if (*yytext == '}')
    {
        block_depth--;
    }
    return *yytext;
}
%%

