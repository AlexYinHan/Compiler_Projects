%{
    #include <stdio.h>
	#include <stdlib.h>

    int chars = 0;
    int words = 0;
    int lines = 0;
%}

%option yylineno

Ws [ \t]+
Digit [0-9]
DigitSequence {Digit}+
Sign [+-]
NonzeroDigit [1-9]
Letter [a-zA-zA]
DecimalInteger 0|({NonzeroDigit}{Digit}*)
OctalDigit [0-7]
OctalInteger 0({OctalDigit}+)
OctalInteger_Error 0[0-9A-WYZa-wyz][0-9A-Za-z]*
HexDigit [0-9a-fA-F]
HexInteger 0[xX]({HexDigit}+)
HexInteger_Error [0][xX][0-9A-Za-z]*
UnsignedIntegerConstant {DecimalInteger}|{OctalInteger}|{HexInteger}
UnsignedIntegerConstantLike [0-9][0-9a-zA-F]*
ExponentPart [eE]{Sign}?{DigitSequence}
FractionalConstant ({DigitSequence}?\.{DigitSequence})|({DigitSequence}\.)
DecimalFloatingConstant ({FractionalConstant}{ExponentPart}?)|({DigitSequence}{ExponentPart})
FloatingConstantLike [0-9][0-9a-zA-F+-\.]*
Relop >|<|>=|<=|==|!=
Type int|float
Identifier ({Letter}|_)({Letter}|{Digit}|_)*
LineFeed \n
%%
{Ws} {/*tabs and whitespaces*/}     
";" {
        
    } 
"," {

    }   
"=" {

    }      
{Relop} {

        }
"+" {

    }
"-" {

    }
"*" {

    } 
"/" {

    }   
"&&"    {

        }
"||"    {

        }
"." {

    }
"!" {

    }
{Type}  {

        }
"(" {

    }
")" {

    }
"[" {

    }
"]" {

    }
"{" {

    }    
"}" {

    }
"struct"    {

            }
"return"    {

            }   
"if"    {

        }
"else"  {

        }   
"while" {

        }               
{UnsignedIntegerConstant}   {
                                printf("INT\n");
                            }
{DecimalFloatingConstant}   {
                                printf("FLOAT\n");
                            }
{UnsignedIntegerConstantLike}   { 
                                    printf("Error type A at line %d: Illegal integer \'%s\'\n", yylineno, yytext); 
                                    // errorLexFlag = 1;
                                } 
{FloatingConstantLike}  { 
                            printf("Error type A at line %d: Illegal float point number \'%s\'\n", yylineno, yytext); 
                            // errorLexFlag = 1;
                        }
{Identifier}    {
                    printf("ID\n");
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