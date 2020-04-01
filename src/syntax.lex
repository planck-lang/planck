/*
syntax.lex

Copyright (c) 12/23/2019, 11:38:08 PM Manwoo Yi

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */
%{

#include "y.tab.h"
#include "utils.h"

extern int fileno(FILE *stream);

int yywrap(void)
{
    return 1;
}

%}

%%
[ \t\n]*        ;

"("             return OPENBR;
")"             return CLOSEBR;
"+"             return PLUS;
"-"             return MINUS;
"*"             return STAR;
"/"             return SLASH;
"="             return EQUAL;

"let"           return K_LET;

[0-9]+ {
    yylval.int_value = str_to_int64(yytext, 10);
    return INUM;
}

[0-9]+"."([0-9]*)? {
    yylval.double_value = str_to_double(yytext);
    return DNUM;
}

[a-zA-Z_][0-9a-zA-Z_]* {
    yylval.identifier = str_dup(yytext, strlen(yytext));
    return IDENTIFIER;
}
%%