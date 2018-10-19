#include "parseCommon.h"

Node* treeRoot = NULL;
SyntaxErrorFlag syntaxErrorFlag = NO_SYNTAX_ERROR;
LexErrorFlag lexErrorFlag = NO_LEX_ERROR;


int main(int argc, char** argv) {
    if(argc <= 1)
    {
        return 1;
    }
	FILE* f = fopen(argv[1], "r");
	if(!f)
	{
		perror(argv[1]);
		return 1;
	}
	yyrestart(f);
	yyparse();

	if(lexErrorFlag == NO_LEX_ERROR && syntaxErrorFlag == NO_SYNTAX_ERROR)
	{
		cout << "here" << endl;
		// Node::printTree(treeRoot);
	}

	return 0;
}