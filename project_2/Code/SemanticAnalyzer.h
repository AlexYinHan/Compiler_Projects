#ifndef _SEMANTICANALYZER_H
#define _SEMANTICANALYZER_H

#include "common.h"
#include "SyntaxTree.h"
#include "SymbolTable.h"


enum AddFunctionResult   { NEW_ITEM_ADDED, DIFFERENT_KIND, REDEFINED, 
                            CONSISTENT_DECLARE, INCONSISTENT_DECLARE,
                            NEWLY_DEFINED, INCONSISTENT_DEFINE};

class SemanticAnalyzer
{
private:
    SymbolTable symbolTable;
    const Type errorType = new Type_();

    /**************************** Tool Functions ***************************/
    string toString(FieldList fieldList);
    string toString(Type type);
    bool matchedFieldlist(FieldList f1, FieldList f2);
    bool matchedStructure(Structure s1, Structure s2);
    TypeCompare compareType(Type t1, Type t2);
    AddFunctionResult checkAndAddFunction(Function function);
    void dealWithAddFunctionResult(AddFunctionResult result, int lineno, Function function);
    void checkUndefinedFunctions();
    /************************* End of Tool Functions ***********************/
    
public:
    SemanticAnalyzer();
    void analyse(Node* treeRoot);
    bool functionAllDefined();

    /*************************** Semantic Actions **************************/

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
    FieldList Args(Node* node);

    /*********************** End of Semantic Actions ***********************/
};



#endif