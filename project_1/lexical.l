%{
    #include <stdio.h>
	#include <stdlib.h>

    int chars = 0;
    int words = 0;
    int lines = 0;
%}

%option yylineno

DecimalDigit [0-9]
NonzeroDigit [1-9]
Letter [a-zA-zA]
DecimalInteger 0|({NonzeroDigit}{DecimalDigit}+)
OctalDigit [0-7]
OctalInteger 0({OctalDigit}+)
HexDigit [0-9a-fA-F]
HexInteger 0[xX]({HexDigit}+)
Int ({DecimalInteger})|({OctalInteger})|({HexInteger})
LineFeed \n
%%
{Int}   {
            printf("INT\n");
        }

{LineFeed}  {

            }
.   {
        printf("Error type A at Line %d:  Mysterious characters \'%s\'\n", yylineno, yytext);
    }
%%
int main(int argc, char** argv) {
    if (argc > 1) {
        if (!(yyin = fopen(argv[1], "r"))) {
            perror(argv[1]);
            return 1;
        }
    }
    yylineno = 1;
    yylex();
    // while (yylex() != 0) {
        printf("%8d%8d%8d total\n", lines, words, chars);
    // }
	return 0;
}