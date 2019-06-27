%{
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "y.tab.h"

extern int fileno(FILE *stream);

int yywrap(void)
{
    return 1;
}
%}
%%
[ \t]*      ;

"##"        return STRCON;
"=="        return EQ;
"!="        return NE;
"<="        return LE;
">="        return GE;
">>"        return RSHFT;
"<<"        return LSHFT;

[0-9]+("."[0-9]*)? {
    char* ptr;
    yylval.double_value = strtod(yytext, &ptr);
    return NUMBER;
}

\"[^\"]*\"  |
\'[^\']*\'  {
    yylval.string_ptr = yytext;
    return STRING;
}

.           {       // It must be the last rule because it can match ANY characters
    return *yytext;
}
%%

