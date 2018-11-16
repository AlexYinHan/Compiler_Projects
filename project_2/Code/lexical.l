%{
    #include "parseCommon.h"
    #include "syntax.tab.h"

    int yycolumn = 1;
	/* define YY_USER_ACTION */
	#define YY_USER_ACTION \
		yylloc.first_line = yylloc.last_line = yylineno; \
		yylloc.first_column = yycolumn; \
		yylloc.last_column = yycolumn + yyleng - 1; \
		yycolumn += yyleng;
    
%}

%option yylineno
%x IN_COMMENT

Ws [ \t]+
Digit [0-9]
DigitSequence {Digit}+
Sign [+-]
NonzeroDigit [1-9]
Letter [a-zA-zA]
DecimalInteger 0|({NonzeroDigit}{Digit}*)
OctalDigit [0-7]
OctalInteger 0({OctalDigit}+)
HexDigit [0-9a-fA-F]
HexInteger 0[xX]({HexDigit}+)
UnsignedIntegerConstantLike [0-9][0-9a-zA-Z]*
ExponentPart [eE]{Sign}?{DigitSequence}
FractionalConstant ({DigitSequence}?\.{DigitSequence})|({DigitSequence}\.)
DecimalFloatingConstant ({FractionalConstant}{ExponentPart}?)|({DigitSequence}{ExponentPart})
FloatingConstantLike [0-9][0-9a-zA-Z+\-\.]*
Relop >|<|>=|<=|==|!=
Type int|float
Identifier ({Letter}|_)({Letter}|{Digit}|_)*
LineFeed \n
LineComment "//"[^\r\n]*
Comment {BlockComment}|{LineComment}
%%
{Ws} {/*tabs and whitespaces*/}  
{LineComment} {/*comments*/}  
<INITIAL>"/*"         BEGIN(IN_COMMENT);
<IN_COMMENT>"*/"      BEGIN(INITIAL);
<IN_COMMENT>[^*\n]+   // eat comment in chunks
<IN_COMMENT>"*"       // eat the lone star
<IN_COMMENT>\n        
";" {
        yylval.type_node = new Node(NODE_TYPE_PUNCTUATION, "SEMI", yytext, yylineno);
        return SEMI; 
    } 
"," {
        yylval.type_node = new Node(NODE_TYPE_PUNCTUATION, "COMMA", yytext, yylineno);
        return COMMA;
    }   
"=" {
        yylval.type_node = new Node(NODE_TYPE_OPERATOR, "ASSIGNOP", yytext, yylineno);
        return ASSIGNOP;
    }      
{Relop} {
            yylval.type_node = new Node(NODE_TYPE_RELOP, "RELOP", yytext, yylineno);
            return RELOP;
        }
"+" {
        yylval.type_node = new Node(NODE_TYPE_OPERATOR, "PLUS", yytext, yylineno);
        return PLUS;
    }
"-" {
        yylval.type_node = new Node(NODE_TYPE_OPERATOR, "MINUS", yytext, yylineno);
        return MINUS;
    }
"*" {
        yylval.type_node = new Node(NODE_TYPE_OPERATOR, "STAR", yytext, yylineno);
        return STAR;
    } 
"/" {
        yylval.type_node = new Node(NODE_TYPE_OPERATOR, "DIV", yytext, yylineno);
        return DIV;
    }   
"&&"    {
            yylval.type_node = new Node(NODE_TYPE_OPERATOR, "AND", yytext, yylineno);
            return AND;
        }
"||"    {
            yylval.type_node = new Node(NODE_TYPE_OPERATOR, "OR", yytext, yylineno);
            return OR;
        }
"." {
        yylval.type_node = new Node(NODE_TYPE_PUNCTUATION, "DOT", yytext, yylineno);
        return DOT;
    }
"!" {
        yylval.type_node = new Node(NODE_TYPE_OPERATOR, "NOT", yytext, yylineno);
        return NOT;
    }
{Type}  {
            yylval.type_node = new Node(NODE_TYPE_KEYWORD, "TYPE", yytext, yylineno);
            return TYPE;
        }
"(" {
        yylval.type_node = new Node(NODE_TYPE_BRACKET, "LP", yytext, yylineno);
        return LP;
    }
")" {
        yylval.type_node = new Node(NODE_TYPE_BRACKET, "RP", yytext, yylineno);
        return RP;
    }
"[" {
        yylval.type_node = new Node(NODE_TYPE_BRACKET, "LB", yytext, yylineno);
        return LB;
    }
"]" {
        yylval.type_node = new Node(NODE_TYPE_BRACKET, "RB", yytext, yylineno);
        return RB;
    }
"{" {
        yylval.type_node = new Node(NODE_TYPE_BRACKET, "LC", yytext, yylineno);
        return LC;
    }    
"}" {
        yylval.type_node = new Node(NODE_TYPE_BRACKET, "RC", yytext, yylineno);
        return RC;
    }
"struct"    {
                yylval.type_node = new Node(NODE_TYPE_KEYWORD, "STRUCT", yytext, yylineno);
                return STRUCT;
            }
"return"    {
                yylval.type_node = new Node(NODE_TYPE_KEYWORD, "RETURN", yytext, yylineno);
                return RETURN;
            }   
"if"    {
            yylval.type_node = new Node(NODE_TYPE_KEYWORD, "IF", yytext, yylineno);
            return IF;
        }
"else"  {
            yylval.type_node = new Node(NODE_TYPE_KEYWORD, "ELSE", yytext, yylineno);
            return ELSE;
        }   
"while" {
            yylval.type_node = new Node(NODE_TYPE_KEYWORD, "WHILE", yytext, yylineno);
            return WHILE;
        }               
{DecimalInteger}    {
                        yylval.type_node = new Node(NODE_TYPE_DEC_INT, "INT", yytext, yylineno);
                        return INT;
                    }
{OctalInteger}      {
                        yylval.type_node = new Node(NODE_TYPE_OCT_INT, "INT", yytext, yylineno);
                        return INT;
                    }
{HexInteger}    {
                    yylval.type_node = new Node(NODE_TYPE_HEX_INT, "INT", yytext, yylineno);
                    return INT;
                }
{DecimalFloatingConstant}   {
                                yylval.type_node = new Node(NODE_TYPE_FLOAT, "FLOAT", yytext, yylineno);
                                return FLOAT;
                            }
{UnsignedIntegerConstantLike}   { 
                                    printf("Error type A at line %d: Illegal integer \'%s\'\n", yylineno, yytext); 
                                    lexErrorFlag = LEX_ERROR;
                                    /* for a error number, store it in node anyway
                                     * so that it won't raise an extra syntax error 
                                     */
                                    yylval.type_node = new Node(NODE_TYPE_DEC_INT, "INT", yytext, yylineno);
                                    return INT;
                                } 
{FloatingConstantLike}  { 
                            printf("Error type A at line %d: Illegal float point number \'%s\'\n", yylineno, yytext); 
                            lexErrorFlag = LEX_ERROR;
                            // same as above
                            yylval.type_node = new Node(NODE_TYPE_FLOAT, "FLOAT", yytext, yylineno);
                            return FLOAT;
                        }
{Identifier}    {
                    yylval.type_node = new Node(NODE_TYPE_ID, "ID", yytext, yylineno);
                    return ID;
                }
{LineFeed}  {
                yycolumn = 1;
            }
.   {
        printf("Error type A at Line %d:  Mysterious characters \'%s\'\n", yylineno, yytext);
        lexErrorFlag = LEX_ERROR;
    }
%%
// int main(int argc, char** argv) {
//     if (argc > 1) {
//         if (!(yyin = fopen(argv[1], "r"))) {
//             perror(argv[1]);
//             return 1;
//         }
//     }
//     yylineno = 1;
//     yylex();
//     // while (yylex() != 0) {
//         printf("%8d%8d%8d total\n", lines, words, chars);
//     // }
// 	return 0;
// }