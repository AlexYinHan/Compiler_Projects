%{
    #include "parseCommon.h"

%}

/* declared types */
%union {
    int type_int;
    float type_float;
    double type_double;
    Node* type_node;
}


/* declared tokens */
%token <type_node> INT
%token <type_node> FLOAT
%token <type_node> ID
%token <type_node> SEMI
%token <type_node> COMMA
%token <type_node> ASSIGNOP 
%token <type_node> RELOP
%token <type_node> PLUS 
%token <type_node> MINUS 
%token <type_node> STAR 
%token <type_node> DIV
%token <type_node> AND 
%token <type_node> OR 
%token <type_node> NOT
%token <type_node> DOT
%token <type_node> TYPE
%token <type_node> LP 
%token <type_node> RP 
%token <type_node> LB 
%token <type_node> RB 
%token <type_node> LC 
%token <type_node> RC
%token <type_node> STRUCT
%token <type_node> RETURN
%token <type_node> IF 
%token <type_node> ELSE
%token <type_node> WHILE

/* declared non-terminals */
%type <type_node> Program ExtDefList ExtDef ExtDecList
%type <type_node> Specifier StructSpecifier OptTag Tag
%type <type_node> VarDec FunDec VarList ParamDec
%type <type_node> CompSt StmtList Stmt
%type <type_node> DefList Def DecList Dec
%type <type_node> Exp Args

%right ASSIGNOP
%left OR
%left AND
%left RELOP 
%left PLUS 
%left STAR DIV
%right NOT
%left LP RP LB RB DOT

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%%

/* High-level Definitions */
Program	: ExtDefList { 
            $$ = new Node(NODE_TYPE_NON_TERMINAL, "Program", @$.first_line);
			$$->addChild($1);
			treeRoot = $$;
		}
		;
ExtDefList  : ExtDef ExtDefList {
				$$ = new Node(NODE_TYPE_NON_TERMINAL, "ExtDefList", @$.first_line);
				$$->addChild($1);
				$$->addChild($2);
            }
            |/* empty */ { $$ = NULL; }
            ;
ExtDef	: Specifier ExtDecList SEMI{
	   		$$ = new Node(NODE_TYPE_NON_TERMINAL, "ExtDef", @$.first_line);
			$$->addChild($1);
			$$->addChild($2);
			$$->addChild($3);
		}
		| Specifier SEMI{
	   		$$ = new Node(NODE_TYPE_NON_TERMINAL, "ExtDef", @$.first_line);
			$$->addChild($1);
			$$->addChild($2);
		}
		| Specifier FunDec CompSt{
	   		$$ = new Node(NODE_TYPE_NON_TERMINAL, "ExtDef", @$.first_line);
			$$->addChild($1);
			$$->addChild($2);
			$$->addChild($3);
		}
		| error SEMI { syntaxErrorFlag = NEAR_END_ERROR; }
		;
ExtDecList	: VarDec{
		   		$$ = new Node(NODE_TYPE_NON_TERMINAL, "ExtDecList", @$.first_line);
				$$->addChild($1);
			}
		    | VarDec COMMA ExtDecList{
	   			$$ = new Node(NODE_TYPE_NON_TERMINAL, "ExtDecList", @$.first_line);
				$$->addChild($1);
				$$->addChild($2);
				$$->addChild($3);
			}
			;

/* Specifiers */
Specifier	: TYPE{
		  		$$ = new Node(NODE_TYPE_NON_TERMINAL, "Specifier", @$.first_line);
				$$->addChild($1);
			}
		  	| StructSpecifier{
				$$ = new Node(NODE_TYPE_NON_TERMINAL, "Specifier", @$.first_line);
				$$->addChild($1);
			}
			;
StructSpecifier	: STRUCT OptTag LC DefList RC{
					$$ = new Node(NODE_TYPE_NON_TERMINAL, "StructSpecifier", @$.first_line);
					$$->addChild($1);
					$$->addChild($2);
					$$->addChild($3);
					$$->addChild($4);
					$$->addChild($5);
				}
				| STRUCT Tag{
					$$ = new Node(NODE_TYPE_NON_TERMINAL, "StructSpecifier", @$.first_line);
					$$->addChild($1);
					$$->addChild($2);
				}
				;
OptTag	: ID{
			$$ = new Node(NODE_TYPE_NON_TERMINAL, "OptTag", @$.first_line);
			$$->addChild($1);
		}
	    | /* empty */{ $$=NULL; }
		;
Tag	: ID{
		$$ = new Node(NODE_TYPE_NON_TERMINAL, "Tag", @$.first_line);
		$$->addChild($1);
	}
	;

/* Declarators */
VarDec : ID{
			$$ = new Node(NODE_TYPE_NON_TERMINAL, "VarDec", @$.first_line);
			$$->addChild($1);
		}
	   | VarDec LB INT RB{
			$$ = new Node(NODE_TYPE_NON_TERMINAL, "VarDec", @$.first_line);
			$$->addChild($1);
			$$->addChild($2);
			$$->addChild($3);
			$$->addChild($4);
		}
	   ;
FunDec	: ID LP VarList RP{
			$$ = new Node(NODE_TYPE_NON_TERMINAL, "FunDec", @$.first_line);
			$$->addChild($1);
			$$->addChild($2);
			$$->addChild($3);
			$$->addChild($4);
		}
	   	| ID LP RP{
			$$ = new Node(NODE_TYPE_NON_TERMINAL, "FunDec", @$.first_line);
			$$->addChild($1);
			$$->addChild($2);
			$$->addChild($3);
		}
		| error RP{ syntaxErrorFlag = NEAR_END_ERROR; }
		;
VarList	: ParamDec COMMA VarList{
			$$ = new Node(NODE_TYPE_NON_TERMINAL, "VarList", @$.first_line);
			$$->addChild($1);
			$$->addChild($2);
			$$->addChild($3);
		}
		| ParamDec{
			$$ = new Node(NODE_TYPE_NON_TERMINAL, "VarList", @$.first_line);
			$$->addChild($1);
		}
		;
ParamDec	: Specifier VarDec{
				$$ = new Node(NODE_TYPE_NON_TERMINAL, "ParamDec", @$.first_line);
				$$->addChild($1);
				$$->addChild($2);
			}
		 	;

/* Statements */
CompSt	: LC DefList StmtList RC{
			$$ = new Node(NODE_TYPE_NON_TERMINAL, "CompSt", @$.first_line);
			$$->addChild($1);
			$$->addChild($2);
			$$->addChild($3);
			$$->addChild($4);
		}
		| LC error RC{ syntaxErrorFlag = NEAR_END_ERROR; }
	   	;
StmtList	: Stmt StmtList{
				$$ = new Node(NODE_TYPE_NON_TERMINAL, "StmtList", @$.first_line);
				$$->addChild($1);
				$$->addChild($2);
			}
		 	| /* empty */{ $$ = NULL; }
			;
Stmt	: Exp SEMI{ 
			$$ = new Node(NODE_TYPE_NON_TERMINAL, "Stmt", @$.first_line);
			$$->addChild($1);
			$$->addChild($2);
		}
	 	| CompSt{
			$$ = new Node(NODE_TYPE_NON_TERMINAL, "Stmt", @$.first_line);
			$$->addChild($1);
		}
		| RETURN Exp SEMI{
			$$ = new Node(NODE_TYPE_NON_TERMINAL, "Stmt", @$.first_line);
			$$->addChild($1);
			$$->addChild($2);
			$$->addChild($3);
		}
		| IF LP Exp RP Stmt %prec LOWER_THAN_ELSE{
			$$ = new Node(NODE_TYPE_NON_TERMINAL, "Stmt", @$.first_line);
			$$->addChild($1);
			$$->addChild($2);
			$$->addChild($3);
			$$->addChild($4);
			$$->addChild($5);
		}
		| IF LP Exp RP Stmt ELSE Stmt{
			$$ = new Node(NODE_TYPE_NON_TERMINAL, "Stmt", @$.first_line);
			$$->addChild($1);$$->addChild($2);$$->addChild($3);
			$$->addChild($4);$$->addChild($5);$$->addChild($6);$$->addChild($7);
		}
		| WHILE LP Exp RP Stmt{
			$$ = new Node(NODE_TYPE_NON_TERMINAL, "Stmt", @$.first_line);
			$$->addChild($1);$$->addChild($2);$$->addChild($3);
			$$->addChild($4);$$->addChild($5);
		}
		| error SEMI { syntaxErrorFlag = NEAR_END_ERROR; }
		;

/* Local Definitions */
DefList	: Def DefList{
			$$ = new Node(NODE_TYPE_NON_TERMINAL, "DefList", @$.first_line);
			$$->addChild($1);
			$$->addChild($2);
		}
		| /* empty */{ $$ = NULL; }
		;
Def	: Specifier DecList SEMI{
		$$ = new Node(NODE_TYPE_NON_TERMINAL, "Def", @$.first_line);
		$$->addChild($1);
		$$->addChild($2);
		$$->addChild($3);
	}
	| error SEMI{ syntaxErrorFlag = NEAR_END_ERROR; }
	;
DecList	: Dec{
			$$ = new Node(NODE_TYPE_NON_TERMINAL, "DecList", @$.first_line);
			$$->addChild($1);
		}
		| Dec COMMA DecList{
			$$ = new Node(NODE_TYPE_NON_TERMINAL, "DecList", @$.first_line);
			$$->addChild($1);
			$$->addChild($2);
			$$->addChild($3);
		}
		;
Dec	: VarDec{
		$$ = new Node(NODE_TYPE_NON_TERMINAL, "Dec", @$.first_line);
		$$->addChild($1);
	}
	| VarDec ASSIGNOP Exp{
		$$ = new Node(NODE_TYPE_NON_TERMINAL, "Dec", @$.first_line);
		$$->addChild($1);
		$$->addChild($2);
		$$->addChild($3);
	}

/* Expressions */
Exp	: Exp ASSIGNOP Exp{
		$$ = new Node(NODE_TYPE_NON_TERMINAL, "Exp", @$.first_line);
		$$->addChild($1);
		$$->addChild($2);
		$$->addChild($3);
	}
	| Exp AND Exp{
		$$ = new Node(NODE_TYPE_NON_TERMINAL, "Exp", @$.first_line);
		$$->addChild($1);
	}
	| Exp OR Exp{
		$$ = new Node(NODE_TYPE_NON_TERMINAL, "Exp", @$.first_line);
		$$->addChild($1);
		$$->addChild($2);
		$$->addChild($3);
	}
	| Exp RELOP Exp{
		$$ = new Node(NODE_TYPE_NON_TERMINAL, "Exp", @$.first_line);
		$$->addChild($1);
		$$->addChild($2);
		$$->addChild($3);
	}
	| Exp PLUS Exp{
		$$ = new Node(NODE_TYPE_NON_TERMINAL, "Exp", @$.first_line);
		$$->addChild($1);
		$$->addChild($2);
		$$->addChild($3);
	}
	| Exp MINUS Exp{
		$$ = new Node(NODE_TYPE_NON_TERMINAL, "Exp", @$.first_line);
		$$->addChild($1);
		$$->addChild($2);
		$$->addChild($3);
	}
	| Exp STAR Exp{
		$$ = new Node(NODE_TYPE_NON_TERMINAL, "Exp", @$.first_line);
		$$->addChild($1);
		$$->addChild($2);
		$$->addChild($3);
	}
	| Exp DIV Exp{
		$$ = new Node(NODE_TYPE_NON_TERMINAL, "Exp", @$.first_line);
		$$->addChild($1);
		$$->addChild($2);
		$$->addChild($3);
	}
	| LP Exp RP{
		$$ = new Node(NODE_TYPE_NON_TERMINAL, "Exp", @$.first_line);
		$$->addChild($1);
		$$->addChild($2);
		$$->addChild($3);
	}
	| MINUS Exp{
		$$ = new Node(NODE_TYPE_NON_TERMINAL, "Exp", @$.first_line);
		$$->addChild($1);
		$$->addChild($2);
	}
	| NOT Exp{
		$$ = new Node(NODE_TYPE_NON_TERMINAL, "Exp", @$.first_line);
		$$->addChild($1);
		$$->addChild($2);
	}
	| ID LP Args RP{
		$$ = new Node(NODE_TYPE_NON_TERMINAL, "Exp", @$.first_line);
		$$->addChild($1);
		$$->addChild($2);
		$$->addChild($3);
		$$->addChild($4);
	}
	| ID LP RP{
		$$ = new Node(NODE_TYPE_NON_TERMINAL, "Exp", @$.first_line);
		$$->addChild($1);
		$$->addChild($2);
		$$->addChild($3);
	}
	| Exp LB Exp RB{
		$$ = new Node(NODE_TYPE_NON_TERMINAL, "Exp", @$.first_line);
		$$->addChild($1);
		$$->addChild($2);
		$$->addChild($3);
		$$->addChild($4);
	}
	| Exp DOT ID{
		$$ = new Node(NODE_TYPE_NON_TERMINAL, "Exp", @$.first_line);
		$$->addChild($1);
		$$->addChild($2);
		$$->addChild($3);
	}
	| ID{
		$$ = new Node(NODE_TYPE_NON_TERMINAL, "Exp", @$.first_line);
		$$->addChild($1);
	}
	| INT{
		$$ = new Node(NODE_TYPE_NON_TERMINAL, "Exp", @$.first_line);
		$$->addChild($1);
	}
	| FLOAT{
		$$ = new Node(NODE_TYPE_NON_TERMINAL, "Exp", @$.first_line);
		$$->addChild($1);
	}
	| Exp LB error RB{ syntaxErrorFlag = NEAR_END_ERROR; }
	;
Args	: Exp COMMA Args{
			$$ = new Node(NODE_TYPE_NON_TERMINAL, "Args", @$.first_line);
			$$->addChild($1);
			$$->addChild($2);
			$$->addChild($3);
		}
	 	| Exp{
			$$ = new Node(NODE_TYPE_NON_TERMINAL, "Args", @$.first_line);
			$$->addChild($1);
		}
		;

%%


void yyerror(const char *msg)
{
	fprintf(stderr, "Error type B at Line %d:  %s\n",yylineno,msg);
}