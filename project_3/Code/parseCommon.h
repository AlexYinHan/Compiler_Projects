#ifndef _PARSE_COMMON_H
#define _PARSE_COMMON_H

#include <stdlib.h>
#include "SyntaxTree.h"


/*
 * Functions and variables provided by flex.
 */
extern int yyparse(void);
extern void yyerror(const char*);
extern FILE* yyin;
// extern int yylex(void);
extern "C" int yylex(void);
extern void yyrestart(FILE *);
extern int yylineno;

/*
 * Globally used variables.
 * Declared in: syntax.y
 * Defined in: main.cpp 
 */
extern Node* treeRoot;	
enum SyntaxErrorFlag{NO_SYNTAX_ERROR, NEAR_END_ERROR};
extern SyntaxErrorFlag syntaxErrorFlag;
enum LexErrorFlag{NO_LEX_ERROR, LEX_ERROR};
extern LexErrorFlag lexErrorFlag;

#endif