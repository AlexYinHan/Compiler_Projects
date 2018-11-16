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
%type <type_node> Program 
%type <type_node> ExtDefList ExtDef ExtDecList
%type <type_node> Specifier StructSpecifier OptTag Tag
%type <type_node> VarDec FunDec VarList ParamDec
%type <type_node> CompSt StmtList Stmt
%type <type_node> DefList Def DecList Dec
%type <type_node> Exp Args

/* combination principles */
%right ASSIGNOP
%left OR
%left AND
%left RELOP 
%left PLUS MINUS
%left STAR DIV
%right NOT
%left DOT LP RP LB RB

/* avoid if-else shift/reduce conflict */
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%%

/* High-level Definitions */
Program	: ExtDefList { 
            $$ = new Node(NODE_TYPE_NON_TERMINAL, "Program", @$.first_line);
			$$->addChild($1);
			$$->setProductionNo(0);
			treeRoot = $$;
		}
		;
ExtDefList  : ExtDef ExtDefList {
				$$ = new Node(NODE_TYPE_NON_TERMINAL, "ExtDefList", @$.first_line);
				$$->addChild($1);
				$$->addChild($2);
				$$->setProductionNo(0);
            }
            |/* empty */ { $$ = NULL; }
            ;
ExtDef	: Specifier ExtDecList SEMI{
	   		$$ = new Node(NODE_TYPE_NON_TERMINAL, "ExtDef", @$.first_line);
			$$->addChild($1);
			$$->addChild($2);
			$$->addChild($3);
			$$->setProductionNo(0);
		}
		| Specifier SEMI{
	   		$$ = new Node(NODE_TYPE_NON_TERMINAL, "ExtDef", @$.first_line);
			$$->addChild($1);
			$$->addChild($2);
			$$->setProductionNo(1);
		}
		| Specifier FunDec CompSt{
	   		$$ = new Node(NODE_TYPE_NON_TERMINAL, "ExtDef", @$.first_line);
			$$->addChild($1);
			$$->addChild($2);
			$$->addChild($3);
			$$->setProductionNo(2);
		}
		| error SEMI { syntaxErrorFlag = NEAR_END_ERROR; }
		;
ExtDecList	: VarDec{
		   		$$ = new Node(NODE_TYPE_NON_TERMINAL, "ExtDecList", @$.first_line);
				$$->addChild($1);
				$$->setProductionNo(0);
			}
		    | VarDec COMMA ExtDecList{
	   			$$ = new Node(NODE_TYPE_NON_TERMINAL, "ExtDecList", @$.first_line);
				$$->addChild($1);
				$$->addChild($2);
				$$->addChild($3);
				$$->setProductionNo(1);
			}
			;

/* Specifiers */
Specifier	: TYPE{
		  		$$ = new Node(NODE_TYPE_NON_TERMINAL, "Specifier", @$.first_line);
				$$->addChild($1);
				$$->setProductionNo(0);
			}
		  	| StructSpecifier{
				$$ = new Node(NODE_TYPE_NON_TERMINAL, "Specifier", @$.first_line);
				$$->addChild($1);
				$$->setProductionNo(1);
			}
			;
StructSpecifier	: STRUCT OptTag LC DefList RC{
					$$ = new Node(NODE_TYPE_NON_TERMINAL, "StructSpecifier", @$.first_line);
					$$->addChild($1);
					$$->addChild($2);
					$$->addChild($3);
					$$->addChild($4);
					$$->addChild($5);
					$$->setProductionNo(0);
				}
				| STRUCT Tag{
					$$ = new Node(NODE_TYPE_NON_TERMINAL, "StructSpecifier", @$.first_line);
					$$->addChild($1);
					$$->addChild($2);
					$$->setProductionNo(1);
				}
				;
OptTag	: ID{
			$$ = new Node(NODE_TYPE_NON_TERMINAL, "OptTag", @$.first_line);
			$$->addChild($1);
			$$->setProductionNo(0);
		}
	    | /* empty */{ $$=NULL; }
		;
Tag	: ID{
		$$ = new Node(NODE_TYPE_NON_TERMINAL, "Tag", @$.first_line);
		$$->addChild($1);
		$$->setProductionNo(0);
	}
	;

/* Declarators */
VarDec : ID{
			$$ = new Node(NODE_TYPE_NON_TERMINAL, "VarDec", @$.first_line);
			$$->addChild($1);
			$$->setProductionNo(0);
		}
	   	| VarDec LB INT RB{
			$$ = new Node(NODE_TYPE_NON_TERMINAL, "VarDec", @$.first_line);
			$$->addChild($1);
			$$->addChild($2);
			$$->addChild($3);
			$$->addChild($4);
			$$->setProductionNo(1);
		}
	   	;
FunDec	: ID LP VarList RP{
			$$ = new Node(NODE_TYPE_NON_TERMINAL, "FunDec", @$.first_line);
			$$->addChild($1);
			$$->addChild($2);
			$$->addChild($3);
			$$->addChild($4);
			$$->setProductionNo(0);
		}
	   	| ID LP RP{
			$$ = new Node(NODE_TYPE_NON_TERMINAL, "FunDec", @$.first_line);
			$$->addChild($1);
			$$->addChild($2);
			$$->addChild($3);
			$$->setProductionNo(1);
		}
		| error RP{ syntaxErrorFlag = NEAR_END_ERROR; }
		;
VarList	: ParamDec COMMA VarList{
			$$ = new Node(NODE_TYPE_NON_TERMINAL, "VarList", @$.first_line);
			$$->addChild($1);
			$$->addChild($2);
			$$->addChild($3);
			$$->setProductionNo(0);
		}
		| ParamDec{
			$$ = new Node(NODE_TYPE_NON_TERMINAL, "VarList", @$.first_line);
			$$->addChild($1);
			$$->setProductionNo(1);
		}
		;
ParamDec	: Specifier VarDec{
				$$ = new Node(NODE_TYPE_NON_TERMINAL, "ParamDec", @$.first_line);
				$$->addChild($1);
				$$->addChild($2);
				$$->setProductionNo(0);
			}
		 	;

/* Statements */
CompSt	: LC DefList StmtList RC{
			$$ = new Node(NODE_TYPE_NON_TERMINAL, "CompSt", @$.first_line);
			$$->addChild($1);
			$$->addChild($2);
			$$->addChild($3);
			$$->addChild($4);
			$$->setProductionNo(0);
		}
		| error RC{ syntaxErrorFlag = NEAR_END_ERROR; }
	   	;
StmtList	: Stmt StmtList{
				$$ = new Node(NODE_TYPE_NON_TERMINAL, "StmtList", @$.first_line);
				$$->addChild($1);
				$$->addChild($2);
				$$->setProductionNo(0);
			}
		 	| /* empty */{ $$ = NULL; }
			;
Stmt	: Exp SEMI{ 
			$$ = new Node(NODE_TYPE_NON_TERMINAL, "Stmt", @$.first_line);
			$$->addChild($1);
			$$->addChild($2);
			$$->setProductionNo(0);
		}
	 	| CompSt{
			$$ = new Node(NODE_TYPE_NON_TERMINAL, "Stmt", @$.first_line);
			$$->addChild($1);
			$$->setProductionNo(1);
		}
		| RETURN Exp SEMI{
			$$ = new Node(NODE_TYPE_NON_TERMINAL, "Stmt", @$.first_line);
			$$->addChild($1);
			$$->addChild($2);
			$$->addChild($3);
			$$->setProductionNo(2);
		}
		| IF LP Exp RP Stmt %prec LOWER_THAN_ELSE{
			$$ = new Node(NODE_TYPE_NON_TERMINAL, "Stmt", @$.first_line);
			$$->addChild($1);
			$$->addChild($2);
			$$->addChild($3);
			$$->addChild($4);
			$$->addChild($5);
			$$->setProductionNo(3);
		}
		| IF LP Exp RP Stmt ELSE Stmt{
			$$ = new Node(NODE_TYPE_NON_TERMINAL, "Stmt", @$.first_line);
			$$->addChild($1);$$->addChild($2);$$->addChild($3);
			$$->addChild($4);$$->addChild($5);$$->addChild($6);$$->addChild($7);
			$$->setProductionNo(4);
		}
		| WHILE LP Exp RP Stmt{
			$$ = new Node(NODE_TYPE_NON_TERMINAL, "Stmt", @$.first_line);
			$$->addChild($1);$$->addChild($2);$$->addChild($3);
			$$->addChild($4);$$->addChild($5);
			$$->setProductionNo(5);
		}
		| error RP { syntaxErrorFlag = NEAR_END_ERROR; }
		| error SEMI { syntaxErrorFlag = NEAR_END_ERROR; }
		;

/* Local Definitions */
DefList	: Def DefList{
			$$ = new Node(NODE_TYPE_NON_TERMINAL, "DefList", @$.first_line);
			$$->addChild($1);
			$$->addChild($2);
			$$->setProductionNo(0);
		}
		| /* empty */{ $$ = NULL; }
		;
Def	: Specifier DecList SEMI{
		$$ = new Node(NODE_TYPE_NON_TERMINAL, "Def", @$.first_line);
		$$->addChild($1);
		$$->addChild($2);
		$$->addChild($3);
		$$->setProductionNo(0);
	}
	;
DecList	: Dec{
			$$ = new Node(NODE_TYPE_NON_TERMINAL, "DecList", @$.first_line);
			$$->addChild($1);
			$$->setProductionNo(0);
		}
		| Dec COMMA DecList{
			$$ = new Node(NODE_TYPE_NON_TERMINAL, "DecList", @$.first_line);
			$$->addChild($1);
			$$->addChild($2);
			$$->addChild($3);
			$$->setProductionNo(1);
		}
		;
Dec	: VarDec{
		$$ = new Node(NODE_TYPE_NON_TERMINAL, "Dec", @$.first_line);
		$$->addChild($1);
		$$->setProductionNo(0);
	}
	| VarDec ASSIGNOP Exp{
		$$ = new Node(NODE_TYPE_NON_TERMINAL, "Dec", @$.first_line);
		$$->addChild($1);
		$$->addChild($2);
		$$->addChild($3);
		$$->setProductionNo(1);
	}

/* Expressions */
Exp	: Exp ASSIGNOP Exp{
		$$ = new Node(NODE_TYPE_NON_TERMINAL, "Exp", @$.first_line);
		$$->addChild($1);
		$$->addChild($2);
		$$->addChild($3);
		$$->setProductionNo(0);
	}
	| Exp AND Exp{
		$$ = new Node(NODE_TYPE_NON_TERMINAL, "Exp", @$.first_line);
		$$->addChild($1);
		$$->setProductionNo(1);
	}
	| Exp OR Exp{
		$$ = new Node(NODE_TYPE_NON_TERMINAL, "Exp", @$.first_line);
		$$->addChild($1);
		$$->addChild($2);
		$$->addChild($3);
		$$->setProductionNo(2);
	}
	| Exp RELOP Exp{
		$$ = new Node(NODE_TYPE_NON_TERMINAL, "Exp", @$.first_line);
		$$->addChild($1);
		$$->addChild($2);
		$$->addChild($3);
		$$->setProductionNo(3);
	}
	| Exp PLUS Exp{
		$$ = new Node(NODE_TYPE_NON_TERMINAL, "Exp", @$.first_line);
		$$->addChild($1);
		$$->addChild($2);
		$$->addChild($3);
		$$->setProductionNo(4);
	}
	| Exp MINUS Exp{
		$$ = new Node(NODE_TYPE_NON_TERMINAL, "Exp", @$.first_line);
		$$->addChild($1);
		$$->addChild($2);
		$$->addChild($3);
		$$->setProductionNo(5);
	}
	| Exp STAR Exp{
		$$ = new Node(NODE_TYPE_NON_TERMINAL, "Exp", @$.first_line);
		$$->addChild($1);
		$$->addChild($2);
		$$->addChild($3);
		$$->setProductionNo(6);
	}
	| Exp DIV Exp{
		$$ = new Node(NODE_TYPE_NON_TERMINAL, "Exp", @$.first_line);
		$$->addChild($1);
		$$->addChild($2);
		$$->addChild($3);
		$$->setProductionNo(7);
	}
	| LP Exp RP{
		$$ = new Node(NODE_TYPE_NON_TERMINAL, "Exp", @$.first_line);
		$$->addChild($1);
		$$->addChild($2);
		$$->addChild($3);
		$$->setProductionNo(8);
	}
	| MINUS Exp{
		$$ = new Node(NODE_TYPE_NON_TERMINAL, "Exp", @$.first_line);
		$$->addChild($1);
		$$->addChild($2);
		$$->setProductionNo(9);
	}
	| NOT Exp{
		$$ = new Node(NODE_TYPE_NON_TERMINAL, "Exp", @$.first_line);
		$$->addChild($1);
		$$->addChild($2);
		$$->setProductionNo(10);
	}
	| ID LP Args RP{
		$$ = new Node(NODE_TYPE_NON_TERMINAL, "Exp", @$.first_line);
		$$->addChild($1);
		$$->addChild($2);
		$$->addChild($3);
		$$->addChild($4);
		$$->setProductionNo(11);
	}
	| ID LP RP{
		$$ = new Node(NODE_TYPE_NON_TERMINAL, "Exp", @$.first_line);
		$$->addChild($1);
		$$->addChild($2);
		$$->addChild($3);
		$$->setProductionNo(12);
	}
	| Exp LB Exp RB{
		$$ = new Node(NODE_TYPE_NON_TERMINAL, "Exp", @$.first_line);
		$$->addChild($1);
		$$->addChild($2);
		$$->addChild($3);
		$$->addChild($4);
		$$->setProductionNo(13);
	}
	| Exp DOT ID{
		$$ = new Node(NODE_TYPE_NON_TERMINAL, "Exp", @$.first_line);
		$$->addChild($1);
		$$->addChild($2);
		$$->addChild($3);
		$$->setProductionNo(14);
	}
	| ID{
		$$ = new Node(NODE_TYPE_NON_TERMINAL, "Exp", @$.first_line);
		$$->addChild($1);
		$$->setProductionNo(15);
	}
	| INT{
		$$ = new Node(NODE_TYPE_NON_TERMINAL, "Exp", @$.first_line);
		$$->addChild($1);
		$$->setProductionNo(16);
	}
	| FLOAT{
		$$ = new Node(NODE_TYPE_NON_TERMINAL, "Exp", @$.first_line);
		$$->addChild($1);
		$$->setProductionNo(17);
	}
	| Exp LB error RB{ syntaxErrorFlag = NEAR_END_ERROR; }
	;
Args	: Exp COMMA Args{
			$$ = new Node(NODE_TYPE_NON_TERMINAL, "Args", @$.first_line);
			$$->addChild($1);
			$$->addChild($2);
			$$->addChild($3);
			$$->setProductionNo(0);
		}
	 	| Exp{
			$$ = new Node(NODE_TYPE_NON_TERMINAL, "Args", @$.first_line);
			$$->addChild($1);
			$$->setProductionNo(1);
		}
		;

%%


void yyerror(const char *msg)
{
	fprintf(stderr, "Error type B at Line %d: %s\n",yylineno,msg);
}