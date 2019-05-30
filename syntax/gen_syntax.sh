#!/bin/bash
yacc -d syntax.yacc
lex syntax.lex
mv lex.yy.c ../src
mv y.tab.* ../src
