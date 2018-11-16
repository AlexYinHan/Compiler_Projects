#include "SemanticAnalyzer.h"

// #define DEBUG
void showInfo(Node *node)
{
#ifdef DEBUG
    if(node == NULL)
    {
        cout << "NULL" << endl;
    }
    else 
    {
        cout << node->getNodeName() << "@" << node->getLineno() << endl;
    }
#endif
}

SemanticAnalyzer::SemanticAnalyzer()
{
    errorType->kind = ERROR;
}
/*
 * Traver through the syntax tree to check for semantic errors.
 * Return true if no semantic error is found.
 * @param treeRoot
 *  root of syntax tree, passed by syntax analyzer
 */
bool SemanticAnalyzer::analyse(Node* treeRoot)
{
    symbolTable.clearTable();
    Program(treeRoot);
    return true;
}

/*
 * Compare two type to check if they match.
 * Return MATCH if the two match. 
 * (Not implemented) Return LEFT_SMALLER if t1 is 'smaller', which may lead to type conversion.
 * If either is NULL(possibly some error type is found), return NOT_SET.
 */
int compareType(Type t1, Type t2)
{
    if(t1 == NULL || t2 == NULL)
    {
        return NOT_SET;
    }
    if(t1->kind == ERROR || t2->kind == ERROR)
    {
        return NOT_SET;
    }
    if(t1->kind != t2->kind)
    {
        return NOT_MATCH;
    }

    switch(t1->kind)
    {
        case BASIC:
            return t1->u.basic == t2->u.basic ? MATCH : NOT_MATCH;
        case ARRAY:
            return compareType(t1->u.array.elem, t2->u.array.elem);
        case STRUCTURE:
            // Simply check for name now
            return t1->u.structure->name.compare(t2->u.structure->name) == 0 ? MATCH : NOT_MATCH;
        case FUNCTION:
            // Simply check for name now
            return t1->u.function->name.compare(t2->u.function->name) == 0 ? MATCH : NOT_MATCH;
        default: return NOT_SET;
    }
}

/****************** Semantic Actions *********************/
//TODO: free space of Type_() for temporary types
//TODO: use errorType
/* High-level Definitions */
void SemanticAnalyzer::Program(Node* node)
{
    showInfo(node);
    // Program -> ExtDefList
    ExtDefList(node->getChild(0));
}

void SemanticAnalyzer::ExtDefList(Node* node)
{
    showInfo(node);
    if(node == NULL)
    {
        // ExtDefList -> $empty
        return;
    }
    // ExtDefList -> ExtDef ExtDefList
    ExtDef(node->getChild(0));
    ExtDefList(node->getChild(1));
}

void SemanticAnalyzer::ExtDef(Node* node)
{
    showInfo(node);
    Type type = Specifier(node->getChild(0));
    switch(node->getProductionNo())
    {
        case 0:
            // ExtDef -> Specifier ExtDecList SEMI
            ExtDecList(node->getChild(1), type);
            break;
        case 1:
            // ExtDef -> Specifier SEMI
            return;
        case 2:
        {
            // ExtDef -> Specifier FunDec CompSt
            Function function = FunDec(node->getChild(1), type);
            if(symbolTable.isDuplicatedName(function->name))
            {
                cout << "Error type 16 at line" << node->getChild(1)->getLineno()
                        << ":Redefined funtion \"" << function->name << "\"." << endl;
            }
            else
            {
                symbolTable.addFunctionAndGetType(function);
            }
            CompSt(node->getChild(2), type);
            break;
        }  
        default: return;
    }
    
}
void SemanticAnalyzer::ExtDecList(Node* node, Type type)
{
    showInfo(node);
    switch(node->getProductionNo())
    {
        case 0:
            //ExtDecList -> VarDec
            VarDec(node->getChild(0), type);
            break;
        case 1:
	        //ExtDecList -> VarDec COMMA ExtDecList
            VarDec(node->getChild(0), type);
            ExtDecList(node->getChild(1), type);
            break;
        default: return;
    }

}

/* Specifiers */
Type SemanticAnalyzer::Specifier(Node* node)
{
    showInfo(node);
    Type type = new Type_();
    type->assignType = LEFT;
    switch(node->getProductionNo())
    {
        case 0:
            //Specifier -> TYPE
            type->kind = BASIC;
            if(node->getChild(0)->getValue().compare("int") == 0){
                type->u.basic = INT;
            } else if(node->getChild(0)->getValue().compare("float") == 0){
                type->u.basic = FLOAT;
            } else {
                return errorType;
            }
            break;
        case 1:
            //Specifier -> StructSpecifier
            type = StructSpecifier(node->getChild(0));
            break;
        default: return errorType;
    }
    return type;
}

Type SemanticAnalyzer::StructSpecifier(Node* node)
{
    showInfo(node);
    switch(node->getProductionNo())
    {
        case 0:
        {
            //StructSpecifier -> STRUCT OptTag LC DefList RC
            string structureName = OptTag(node->getChild(1));
            if(structureName.compare("") != 0 && symbolTable.isDuplicatedName(structureName))
            {
                cout << "Error type 16 at line" << node->getChild(1)->getLineno()
                        << ":Duplicated name \"" << structureName << "\"." << endl;
                return errorType;
            }
            Structure structure = new Structure_();
            structure->name = structureName;
            structure->structureFieldList = DefList(node->getChild(3));

            // for ananymous struct, simply add it to symbol table with name ""
            Type type = symbolTable.addStructureAndGetType(structure);
            
            return type;
        }
        case 1:
        {
            //StructSpecifier -> STRUCT Tag
            //TODO:search in table
            string tag = Tag(node->getChild(1));
            TableItem *item = symbolTable.getItemByName(tag);
            if(item == NULL || item->type == NULL || item->type->kind != STRUCTURE)
            {
                cout << "Error type 17 at line " << node->getChild(1)->getLineno() 
                    << ": Undefined structure \"" << tag << "\"" << endl;
                return errorType;
            }
            else
            {
                return item->type;
            }
        }
        default: return errorType;
    }
}

string SemanticAnalyzer::OptTag(Node* node)
{
    showInfo(node);
    if(node == NULL)
    {
        //OptTag -> $empty
        return "";
    }
    else 
    {
        //OptTag -> ID
        return node->getValue();
    }
    
}

string SemanticAnalyzer::Tag(Node* node)
{
    showInfo(node);
    //Tag -> ID
    return node->getValue();
}

/* Declarators */
FieldList SemanticAnalyzer::VarDec(Node* node, Type type)
{
    showInfo(node);
    switch(node->getProductionNo())
    {
        case 0:
        {
            //VarDec -> ID
            string name = node->getChild(0)->getValue();
            if(symbolTable.isDuplicatedName(name))
            {
                cout << "Error type 3 at line" << node->getChild(0)->getLineno()
                        << ":Redefined variable \"" << name << "\"." << endl;
                return NULL;
            }
            else 
            {
                FieldList varDec = new FieldList_();
                varDec->name = node->getChild(0)->getValue();
                varDec->type = type;
                varDec->tail = NULL;
                symbolTable.addFieldList(varDec);
                return varDec;
            }
            break;
        }
        case 1:
        {
            //VarDec -> VarDec LB INT RB
            Type varDec = new Type_();
            varDec->assignType = LEFT;
            varDec->kind = ARRAY;
            varDec->u.array.size = node->getChild(2)->getIntValue();
            varDec->u.array.elem = type;
            return VarDec(node->getChild(0), varDec);
            break;
        }
        default: return NULL;
    }
}

Function SemanticAnalyzer::FunDec(Node* node, Type retType)
{
    showInfo(node);
    Function function = new Function_();
    function->name = node->getChild(0)->getValue();
    function->returnType = retType;
    function->returnType->assignType = RIGHT;
    if(node->getProductionNo() == 0)
    {
        //FunDec -> ID LP VarList RP
        function->params = VarList(node->getChild(2));
    }
    else 
    {
        //FunDec -> ID LP RP
        function->params = NULL;
    }
    return function;
}

FieldList SemanticAnalyzer::VarList(Node* node)
{
    showInfo(node);  
    //VarList -> ParamDec
    FieldList varList = ParamDec(node->getChild(0));
    if(node->getProductionNo() == 0)
    {
        //VarList -> ParamDec COMMA VarList
        varList->tail = VarList(node->getChild(2));
    }

    return varList;
}

FieldList SemanticAnalyzer::ParamDec(Node* node)
{
    showInfo(node);
    //ParamDec -> Specifier VarDec
    Type type = Specifier(node->getChild(0));
    FieldList varDec = VarDec(node->getChild(1), type);
    return varDec;
}

/* Statements */
void SemanticAnalyzer::CompSt(Node* node, Type retType)
{
    showInfo(node);
    //CompSt -> LC DefList StmtList RC
    DefList(node->getChild(1));
    StmtList(node->getChild(2), retType);
}

void SemanticAnalyzer::StmtList(Node* node, Type retType)
{
    showInfo(node);
    if(node == NULL)
    {
	    //StmtList -> $empty
        return;
    }
    //StmtList -> Stmt StmtList
    Stmt(node->getChild(0), retType);
    StmtList(node->getChild(1), retType);
}

void SemanticAnalyzer::Stmt(Node* node, Type retType)
{
    showInfo(node);
    switch(node->getProductionNo())
    {
        case 0:
            //Stmt -> Exp SEMI
            Exp(node->getChild(0));
            break;
        case 1:
            //Stmt -> CompSt
            CompSt(node->getChild(0), retType);
            break;
        case 2:
        {
            //Stmt -> RETURN Exp SEMI
            Type resultType = Exp(node->getChild(1));
            if(compareType(resultType, retType) == NOT_MATCH)
            {
                cout << "Error type 8 at line" << node->getChild(1)->getLineno()
                        << ":Type mismatched for return." << endl;
            }
            break;
        }
        case 3:
        {
            //Stmt -> IF LP Exp RP Stmt
            Type expType = Exp(node->getChild(2));
            Stmt(node->getChild(4), retType);
            break;
        }
        case 4:
        {
            //Stmt -> IF LP Exp RP Stmt ELSE Stmt
            Type expType = Exp(node->getChild(2));
            Stmt(node->getChild(4), retType);
            Stmt(node->getChild(6), retType);
            break;
        }
        case 5:
        {
            //Stmt -> WHILE LP Exp RP Stmt
            Exp(node->getChild(2));
            Stmt(node->getChild(4), retType);
            break;
        }
        default: return;
    }
}

/* Local Definitons */
FieldList SemanticAnalyzer::DefList(Node* node)
{
    showInfo(node);
    if(node == NULL)
    {
        //DefList -> $empty
        return NULL;
    }
    //DefList -> Def DefList
    FieldList def = Def(node->getChild(0));
    FieldList defList = DefList(node->getChild(1));
    if(def == NULL)
    {
        return defList;
    }
    else 
    {
        // add defList to tail of def
        FieldList tmp = def;
        while(tmp->tail != NULL)
        {
            tmp = tmp->tail;
        }
        tmp->tail = defList;
        return def;
    }
}

FieldList SemanticAnalyzer::Def(Node* node)
{
    showInfo(node);
    //Def -> Specifier DecList SEMI
    Type type = Specifier(node->getChild(0));
    return DecList(node->getChild(1), type);
}

FieldList SemanticAnalyzer::DecList(Node* node, Type type)
{
    showInfo(node);
    //DecList -> Dec
    FieldList dec = Dec(node->getChild(0), type);
    if(node->getProductionNo() == 0)
    {
        return dec;
    }

    //DecList -> Dec COMMA DecList
    FieldList decList = DecList(node->getChild(2), type);
    if(dec == NULL)
    {
        return decList;
    }
    else 
    {
        // add defList to tail of def
        FieldList tmp = dec;
        while(tmp->tail != NULL)
        {
            tmp = tmp->tail;
        }
        tmp->tail = decList;
        return dec;
    } 
}

FieldList SemanticAnalyzer::Dec(Node* node, Type type)
{
    showInfo(node);
    FieldList varDec = VarDec(node->getChild(0), type);
    if(node->getProductionNo() == 0)
    {
        //Dec -> VarDec
        return varDec;
    }
	//Dec -> VarDec ASSIGNOP Exp
    Type expType = Exp(node->getChild(2));
    if(compareType(type, expType) == NOT_MATCH)
    {
        cout << "Error type 5 at Line " << node->getChild(1)->getLineno()
                        << ":Type mismatched for assignment." << endl;
        return NULL;
    }
    return varDec;
}

/* Expressions */
Type SemanticAnalyzer::Exp(Node* node)
{
    showInfo(node);
    switch(node->getProductionNo())
    {
        case 0:
        {
            //Exp -> Exp ASSIGNOP Exp
            Type tLeft = Exp(node->getChild(0)), tRight = Exp(node->getChild(2));
            if(tLeft->assignType == RIGHT)
            {
                cout << "Error type 6 at Line " << node->getChild(0)->getLineno()
                        << ":The left-hand side of an assignment must be a variable." << endl;
                return errorType;
            }
            if(compareType(tLeft, tRight) == NOT_MATCH)
            {
                cout << "Error type 5 at Line " << node->getChild(0)->getLineno()
                        << ":Type mismatched for assignment." << endl;
                return errorType;
            }
            else
            {
                return tLeft;
            }
        }
        case 1:
            //Exp -> Exp AND Exp
        case 2:
            //Exp -> Exp OR Exp
        case 3:
            //Exp -> Exp RELOP Exp
        case 4:
            //Exp -> Exp PLUS Exp
        case 5:
            //Exp -> Exp MINUS Exp
        case 6:
            //Exp -> Exp STAR Exp
        case 7:
        {
            //Exp -> Exp DIV Exp
            //case 1-7 share the same actions
            Type tLeft = Exp(node->getChild(0)), tRight = Exp(node->getChild(2));
            if(tLeft->kind == BASIC && tRight->kind == BASIC && tLeft->u.basic == tRight->u.basic)
            {
                Type retType = new Type_();
                retType->kind =  BASIC;
                retType->u.basic = tLeft->u.basic;
                retType->assignType = RIGHT;
                return retType;
            }
            else
            {
                cout << "Error type 7 at Line " << node->getChild(0)->getLineno()
                        << ":Type mismatched for operands." << endl;
                return errorType;
            }
        }
        case 8:
            //Exp -> LP Exp RP
            return Exp(node->getChild(1));
        case 9:
        {
            //Exp -> MINUS Exp
            Type typeExp = Exp(node->getChild(1));
            if(typeExp->kind != BASIC)
            {
                cout << "Error type 7 at Line " << node->getChild(0)->getLineno()
                        << ":Type mismatched for operands." << endl;
                return errorType;
            }
            Type retType = new Type_();
            retType->kind =  BASIC;
            retType->u.basic = typeExp->u.basic;
            retType->assignType = RIGHT;
            return retType;
        }
        case 10:
        {
            //Exp -> NOT Exp
            Type typeExp = Exp(node->getChild(1));
            if(typeExp->kind != BASIC || typeExp->u.basic != INT)
            {
                cout << "Error type 7 at Line " << node->getChild(0)->getLineno()
                        << ":Type mismatched for operands." << endl;
                return errorType;
            }
            Type retType = new Type_();
            retType->kind =  BASIC;
            retType->u.basic = typeExp->u.basic;
            retType->assignType = RIGHT;
            return retType;
        }
        case 11:
            //Exp -> ID LP Args RP
        case 12:
        {
            //Exp -> ID LP RP
            // case 11-12 share some same actions
            TableItem *item = symbolTable.getItemByName(node->getChild(0)->getValue());
            Type function = item ? item->type : NULL;
            if(function == NULL)
            {
                cout << "Error type 2 at Line " << node->getChild(0)->getLineno()
                        << ":Undefined function \"" << node->getChild(0)->getValue() << "\"." << endl;
                return errorType;
            }
            if(function->kind != FUNCTION)
            {
                cout << "Error type 11 at Line " << node->getChild(0)->getLineno()
                        << ":\"" << node->getChild(0)->getValue() << "\" is not a function." << endl;
                return errorType;
            }
            // if(function->u.function->isDefined)

            // check param types
            //TODO: output specific info for params
            FieldList param = function->u.function->params;
            if(node->getProductionNo() == 12)
            {
                //Exp -> ID LP RP
                if(param != NULL)
                {
                    cout << "Error type 9 at Line " << node->getChild(0)->getLineno()
                        << ": Function \"" << node->getChild(0)->getValue() << "\" is not applicable for arguments." << endl;
                    return errorType;
                }
            }
            else
            {
                //Exp -> ID LP Args RP
                if(!Args(node->getChild(2), param))
                {
                    cout << "Error type 9 at Line " << node->getChild(0)->getLineno()
                        << ": Function \"" << node->getChild(0)->getValue() << "\" is not applicable for arguments." << endl;
                    return errorType;
                }
            }
            Type retType = new Type_();
            retType->kind = function->u.function->returnType->kind;
            retType->u = function->u.function->returnType->u;
            retType->assignType = RIGHT;
            return retType;
        }
        case 13:
        {
            //Exp -> Exp LB Exp RB
            Type arrayType = Exp(node->getChild(0));
            if(arrayType->kind != ARRAY)
            {
                cout << "Error type 10 at Line " << node->getChild(0)->getLineno()
                        << ": \"" << node->getChild(0)->getValue() << "\" is not an array." << endl;
                return errorType;
            }
            Type indexType = Exp(node->getChild(2));
            if(!(indexType->kind == BASIC && indexType->u.basic == INT))
            {
                cout << "Error type 12 at Line " << node->getChild(0)->getLineno()
                        << ": \"" << node->getChild(2)->getValue() << "\" is not an integer." << endl;
                return errorType;
            }
            Type retType = new Type_();
            retType->kind = arrayType->u.array.elem->kind;
            retType->u = arrayType->u.array.elem->u;
            retType->assignType = LEFT;
            return retType;
        }
        case 14:
        {
            //Exp -> Exp DOT ID
            Type structType = Exp(node->getChild(0));
            if(structType->kind != STRUCTURE)
            {
                cout << "Error type 13 at Line " << node->getChild(0)->getLineno()
                        << ": Illegal use of \".\"" << endl;
                return errorType;
            }

            // search for ID in this struct
            string IDName = node->getChild(2)->getValue();
            FieldList itr = structType->u.structure->structureFieldList;
            while(itr)
            {
                if(itr->name.compare(IDName) == 0)
                {
                    Type retType = new Type_();
                    retType->kind = itr->type->kind;
                    retType->u = itr->type->u;
                    retType->assignType = LEFT;
                    return retType;
                }
                itr = itr->tail;
            }
            cout << "Error type 14 at Line " << node->getChild(0)->getLineno()
                        << ": Non-existent field  \"" << IDName << "\"" << endl;
            return errorType;
        }
        case 15:
        {
            //Exp -> ID
            TableItem *IDItem = symbolTable.getItemByName(node->getChild(0)->getValue());
            if(IDItem == NULL || IDItem->type == NULL)
            {
                cout << "Error type 1 at Line " << node->getChild(0)->getLineno()
                        << ": Undefined variable \"" << node->getChild(0)->getValue() << "\"" << endl;
                return errorType;
            }
            return IDItem->type;
        }
        case 16:
        {    
            //Exp -> INT
            Type type = new Type_();
            type->kind = BASIC;
            type->u.basic = INT;
            type->assignType = RIGHT;
            return type;
        }
        case 17:
        {
            //Exp -> FLOAT
            Type type = new Type_();
            type->kind = BASIC;
            type->u.basic = FLOAT;
            type->assignType = RIGHT;
            return type;
        }
    }
}

/*
 * List of real params.
 * As it's only called when parsing function call, do the type check in passing.
 * Return true if arguments and parametres match.
 */
bool SemanticAnalyzer::Args(Node* node, FieldList param)
{
    showInfo(node);
	//Exp
    if(param == NULL)
    {
        // node couldn't be NULL
        return false;
    }
    Type expType = Exp(node->getChild(0));
    if(compareType(expType, param->type) == NOT_MATCH)
    {
        return false;
    }
    if(node->getProductionNo() == 0)
    {
        //Exp COMMA Args
        return Args(node->getChild(2), param->tail);
    }
    else
    {
        return true;
    }
}

