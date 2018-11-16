#ifndef _SEMANTICANALYZER_H
#define _SEMANTICANALYZER_H

#include "common.h"
#include "SyntaxTree.h"
#include "SymbolTable.h"

class SemanticAnalyzer
{
private:
    SymbolTable symbolTable;

    const Type errorType = new Type_();
public:
    SemanticAnalyzer();
    bool analyse(Node* treeRoot);

    /****************** Semantic Actions *********************/

    /* High-level Definitions */
    void Program(Node* node);
    void ExtDefList(Node* node);
    void ExtDef(Node* node);
    void ExtDecList(Node* node, Type type);

    /* Specifiers */
    Type Specifier(Node* node);
    Type StructSpecifier(Node* node);
    string OptTag(Node* node);
    string Tag(Node* node);

    /* Declarators */
    FieldList VarDec(Node* node, Type type);
    Function FunDec(Node* node, Type retType);
    FieldList VarList(Node* node);
    FieldList ParamDec(Node* node);

    /* Statements */
    void CompSt(Node* node, Type retType);
    void StmtList(Node* node, Type retType);
    void Stmt(Node* node, Type retType);

    /* Local Definitons */
    FieldList DefList(Node* node);
    FieldList Def(Node* node);
    FieldList DecList(Node* node, Type type);
    FieldList Dec(Node* node, Type type);

    /* Expressions */
    Type Exp(Node* node);
    bool Args(Node* node, FieldList param);

    /**************** End of Semantic Actions ****************/
};



#endif