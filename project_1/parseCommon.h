#ifndef _PARSE_COMMON_H
#define _PARSE_COMMON_H

#include <iostream>
#include <stdlib.h>
#include "SyntaxTree.h"
using namespace std;

/*
 * Functions and variables generated in lex.yy.c
 */
// extern "C"
// {
//     int yylex(void);
//     int yyparse(void);
//     void yyrestart(FILE*);
//     void yyerror(char*);
// }
// extern int yylex(void);
// extern int yyparse(void);
// extern void yyrestart(FILE*);
// extern void yyerror(char*);
extern int yylineno;

/*
 * Functions and variables provided by flex.
 */
extern FILE* yyin;
// extern int yylex(void);
extern void yyrestart(FILE *);

/*
 * Globally used variables.
 * Declared in: syntax.y
 * Defined in: main.cpp 
 */
extern Node* treeRoot;	
enum SyntaxErrorFlag{NO_SYNTAX_ERROR, NEAR_END_ERROR};
extern SyntaxErrorFlag syntaxErrorFlag;
enum LexErrorFlag{NO_LEX_ERROR};
extern LexErrorFlag lexErrorFlag;
#endif