#include "common.h"
#include "parseCommon.h"
#include "SemanticAnalyzer.h"
#include "InterCode.h"
#include "MIPS.h"

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
		#ifdef DEBUG
			Node::printTree(treeRoot);
		#endif
		
		SemanticAnalyzer semanticAnalyzer;
		semanticAnalyzer.getSymbolTable()->setSupportNestedScope(false); // nested scope not supported for inter code yet
		semanticAnalyzer.analyse(treeRoot);
		if(semanticAnalyzer.getSemanticErrorFlag() == NO_SEMANTIC_ERROR)
		{
			InterCodeTranslater IRT(semanticAnalyzer.getSymbolTable());
			IRT.translate(treeRoot);
			MIPS32Translater MT;
			MT.translate(IRT.getInterCodeList());
			if(argc >= 3)
			{
				// IRT.output(argv[2]);
				MT.output(argv[2]);
			}
			else
			{
				// IRT.output();
				MT.output();
			}
		}
	}

	Node::deleteTree(treeRoot);
	return 0;
}