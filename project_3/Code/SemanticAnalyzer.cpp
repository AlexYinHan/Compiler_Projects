#include "SemanticAnalyzer.h"

// #define DEBUG
static void showInfo(Node *node)
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

SemanticErrorFlag SemanticAnalyzer::getSemanticErrorFlag()
{
    return this->semanticErrorFlag;
}


/*
 * Traver through the syntax tree to check for semantic errors.
 * @param treeRoot
 *  root of syntax tree, passed by syntax analyzer
 */
void SemanticAnalyzer::analyse(Node* treeRoot)
{
    symbolTable.clearTable();
    this->semanticErrorFlag = NO_SEMANTIC_ERROR;
    Program(treeRoot);
    checkUndefinedFunctions();
}

bool SemanticAnalyzer::functionAllDefined()
{
    return symbolTable.functionAllDefined();
}

/**************************** Tool Functions ***************************/
/*
 * List types of fields, separated by comma and a space.
 */
string SemanticAnalyzer::toString(FieldList fieldList)
{
    string s = "";
    FieldList t = fieldList;
    while(t)
    {
        s += toString(t->type);
        t = t->tail;
        if(t)
        {
            s += ", ";
        }
    }
    return s;
}
/*
 * Return a string representing a type.
 * BASIC as int, float.
 * STRUCTURE as struct id.
 * FUNCTION as func id.
 * ARRAY as type[][]...
 * ERROR as error_type
 */
string SemanticAnalyzer::toString(Type type)
{
    if(type == NULL)
    {
        return "";
    }
    switch(type->kind)
    {
        case BASIC:
            return type->u.basic == INT ? "int" : "float";
        case STRUCTURE:
            return "struct " + type->u.structure->name;
        case FUNCTION:
            return "func " + type->u.function->name;
        case ARRAY:
            return toString(type->u.array.elem) + "[" + to_string(type->u.array.size) + "]";
        case ERROR:
        default:
            return "error_type";
    }
}

/*
 * Compare two fieldList to check if they match.
 */
bool SemanticAnalyzer::matchedFieldlist(FieldList f1, FieldList f2)
{
    if(f1 == NULL && f2 == NULL)
    {
        return true;
    }
    if(f1 == NULL || f2 == NULL)
    {
        return false;
    }
    if(compareType(f1->type, f2->type) == NOT_MATCH)
    {
        return false;
    }
    return matchedFieldlist(f1->tail, f2->tail);
}

/*
 * Compare two type to check if they match.
 * Return MATCH if the two match. 
 * (Not implemented) Return LEFT_SMALLER if t1 is 'smaller', which may lead to type conversion.
 * If either is NULL(possibly some error type is found), return NOT_SET.
 */
TypeCompare SemanticAnalyzer::compareType(Type t1, Type t2)
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
            return matchedStructure(t1->u.structure, t2->u.structure) ? MATCH : NOT_MATCH;
        case FUNCTION:
            // It shouldn't really happend where we need to compare two functions in this method.
            // Simply check for name now.
            return t1->u.function->name.compare(t2->u.function->name) == 0 ? MATCH : NOT_MATCH;
        default: return NOT_SET;
    }
}

/*
 * Return true if the two share the same field types, regardless of the names.
 */
bool SemanticAnalyzer::matchedStructure(Structure s1, Structure s2)
{
    return matchedFieldlist(s1->structureFieldList, s2->structureFieldList);
}

/*
 * Check if it's ok to add a function.
 */
AddFunctionResult SemanticAnalyzer::checkAndAddFunction(Function function)
{
    TableItem *item = symbolTable.getItemByName(function->name);
    if(item == NULL)
    {
        // add a new item to symbol table.
        symbolTable.addFunctionAndGetType(function);
        return function->isDefined ? NEW_DEF_ADDED : NEW_DEC_ADDED;
    }
    if(item->type->kind != FUNCTION)
    {
        // redeclared as different kind symbol.
        return DIFFERENT_KIND;
    }
    Function functionInTable = item->type->u.function;
    bool consistent = matchedFieldlist(function->params, functionInTable->params) && 
                        compareType(function->returnType, functionInTable->returnType) == MATCH;
    if(function->isDefined)
    {
        if(functionInTable->isDefined)
        {
            return REDEFINED;
        }
        else
        {
            if(consistent)
            {
                functionInTable->isDefined = true;
                return NEWLY_DEFINED;
            }
            else
            {
                return INCONSISTENT_DEFINE;
            }
        }
    }
    else
    {
        return consistent ? CONSISTENT_DECLARE : INCONSISTENT_DECLARE;
    }
}

void SemanticAnalyzer::dealWithAddFunctionResult(AddFunctionResult result, int lineno, Function function)
{
    switch(result)
    {
        case REDEFINED:
            cout << "Error type 4 at line " << lineno
                    << ": Redefined function \"" << function->name << "\"." << endl;
            this->semanticErrorFlag = SEMANTIC_ERROR;
            return;
        case DIFFERENT_KIND:
            cout << "Error type 19 at line " << lineno
                    << ": \"" << function->name << "\" redeclared as different kind of symbol." << endl;
            this->semanticErrorFlag = SEMANTIC_ERROR;
            return;
        case INCONSISTENT_DECLARE:
            cout << "Error type 19 at line " << lineno
                    << ": Inconsistent declaration of function \"" << function->name << "\"." << endl;
            this->semanticErrorFlag = SEMANTIC_ERROR;
            return;
        case INCONSISTENT_DEFINE:
            cout << "Error type 19 at line " << lineno
                    << ": Inconsistent definition of function \"" << function->name << "\"." << endl;
            this->semanticErrorFlag = SEMANTIC_ERROR;
            return;

        // results without error
        case NEW_DEF_ADDED:
        case NEW_DEC_ADDED:
        {
            FunDecRecord record;
            record.lineno = lineno;
            record.function = function;
            symbolTable.funDecRecords.push_back(record);
            return;
        }
        case CONSISTENT_DECLARE:
        case NEWLY_DEFINED:
        {
            FunDecRecord record;
            record.lineno = lineno;
            record.function = symbolTable.getItemByName(function->name)->type->u.function; // function in table
            symbolTable.funDecRecords.push_back(record);
        }
        default: return;
    }
}

void SemanticAnalyzer::checkUndefinedFunctions()
{
    for (list<FunDecRecord>::iterator it = symbolTable.funDecRecords.begin(); it != symbolTable.funDecRecords.end(); it ++)
    {
        if(!(*it).function->isDefined)
        {
            cout << "Error type 18 at line " << (*it).lineno 
                << ": Undefined function \"" << (*it).function->name << "\"." << endl;
            this->semanticErrorFlag = SEMANTIC_ERROR;
        }
    }
}

SymbolTable* SemanticAnalyzer::getSymbolTable()
{
    return &(this->symbolTable);
}

/********************** End of Tool Functions *************************/

/************************ Semantic Actions ****************************/
//TODO: free space of Type_() for temporary types
//TODO: add variable type
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
            return;
        case 1:
            // ExtDef -> Specifier SEMI
            return;
        case 2:
        {
            // ExtDef -> Specifier FunDec SEMI

            /* We don't want params in pure func declaration be put into symbol table.
                To make it simple, assume this declaration has a nested scope, 
                so the params will be added into table and be deleted right away.*/
            // EnterScopeNote:
            symbolTable.enterScope();
            Function function = FunDec(node->getChild(1), type);
            function->isDefined = false;
            // Try to add function to symbol, and print messages if error is detected
            // The function itself is in the outer scope (than params scope).
            // Write the below code inside this scope so that recursive call is supported;
            dealWithAddFunctionResult(checkAndAddFunction(function), node->getChild(1)->getLineno(), function);
            
            symbolTable.exitScope();

            return;
        }
        case 3:
        {
            // ExtDef -> Specifier FunDec CompSt

            /* EnterScopeNote: Params and var declarations happened in CompSt are in a nested scope.*/
            symbolTable.enterScope();
            Function function = FunDec(node->getChild(1), type);
            function->isDefined = true;
            // Try to add function to symbol, and print messages if error is detected
            // The function itself is in the outer scope (than params scope).
            // Write the below code inside this scope so that recursive call is supported;
            dealWithAddFunctionResult(checkAndAddFunction(function), node->getChild(1)->getLineno(), function);
            CompSt(node->getChild(2), type);
            symbolTable.exitScope();
        
            return;
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
            VarDec(node->getChild(0), type, false);
            break;
        case 1:
	        //ExtDecList -> VarDec COMMA ExtDecList
            VarDec(node->getChild(0), type, false);
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
            if(node->getChild(0)->getText().compare("int") == 0){
                type->u.basic = INT;
            } else if(node->getChild(0)->getText().compare("float") == 0){
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
            if(structureName.compare("") != 0 && symbolTable.isDuplicatedNameInCurrentScope(structureName))
            {
                cout << "Error type 16 at line " << node->getChild(1)->getLineno()
                        << ": Duplicated name \"" << structureName << "\"." << endl;
                this->semanticErrorFlag = SEMANTIC_ERROR;
                return errorType;
            }
            Structure structure = new Structure_();
            structure->name = structureName;

            // EnterScopeNote: Var declarations in struct definition are in a scope.
            symbolTable.enterScope(STRUCT);
            structure->structureFieldList = DefList(node->getChild(3));
            symbolTable.exitScope();

            // for ananymous struct, simply add it to symbol table with name ""
            Type type = symbolTable.addStructureAndGetType(structure);
            
            return type;
        }
        case 1:
        {
            //StructSpecifier -> STRUCT Tag
            string tag = Tag(node->getChild(1));
            TableItem *item = symbolTable.getItemByName(tag);
            if(item == NULL || item->type == NULL || item->type->kind != STRUCTURE)
            {
                cout << "Error type 17 at line " << node->getChild(1)->getLineno() 
                    << ": Undefined structure \"" << tag << "\"." << endl;
                this->semanticErrorFlag = SEMANTIC_ERROR;
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
        return node->getText();
    }
    
}

string SemanticAnalyzer::Tag(Node* node)
{
    showInfo(node);
    //Tag -> ID
    return node->getText();
}

/* Declarators */
FieldList SemanticAnalyzer::VarDec(Node* node, Type type, bool fromParam)
{
    showInfo(node);
    switch(node->getProductionNo())
    {
        case 0:
        {
            //VarDec -> ID
            string name = node->getChild(0)->getText();
            if(symbolTable.isDuplicatedNameInCurrentScope(name))
            {
                if(symbolTable.getScopeType() == STRUCT)
                {   
                    cout << "Error type 15 at line " << node->getChild(0)->getLineno()
                        << ": Redefined field \"" << name << "\"." << endl;
                }
                else
                {
                    cout << "Error type 3 at line " << node->getChild(0)->getLineno()
                            << ": Redefined variable \"" << name << "\"." << endl;
                }
                this->semanticErrorFlag = SEMANTIC_ERROR;
                return NULL;
            }
            else 
            {
                FieldList varDec = new FieldList_();
                varDec->name = node->getChild(0)->getText();
                varDec->type = type;
                varDec->tail = NULL;
                // for array/struct parameter, its address is passed, so the symbol is actually a pointer 
                if(fromParam && (type->kind == ARRAY || type->kind == STRUCTURE))
                {
                    symbolTable.addFieldList(varDec, true);
                }
                else
                {
                    symbolTable.addFieldList(varDec, false);
                }
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
            return VarDec(node->getChild(0), varDec, fromParam);
            break;
        }
        default: return NULL;
    }
}

Function SemanticAnalyzer::FunDec(Node* node, Type retType)
{
    showInfo(node);
    Function function = new Function_();
    function->name = node->getChild(0)->getText();
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
    FieldList varDec = VarDec(node->getChild(1), type, true);
    return varDec;
}

/* Statements */
void SemanticAnalyzer::CompSt(Node* node, Type retType)
{
    /*
     * Note: Don't enter and exit scope here.
     * Write change scope code where CompSt appear on the right side of production.
     * To make sure params in () like func(int p) or for(int i) are in the correct scope.
     */
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
            /* EnterScopeNote: Var declrations in CompSt are in a nested scope.*/
            symbolTable.enterScope();
            CompSt(node->getChild(0), retType);
            symbolTable.exitScope();
            break;
        case 2:
        {
            //Stmt -> RETURN Exp SEMI
            Type resultType = Exp(node->getChild(1));
            if(compareType(resultType, retType) == NOT_MATCH)
            {
                cout << "Error type 8 at line " << node->getChild(1)->getLineno()
                        << ": Type mismatched for return." << endl;
                this->semanticErrorFlag = SEMANTIC_ERROR;
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
    FieldList varDec = VarDec(node->getChild(0), type, false);
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
                << ": Type mismatched for assignment." << endl;
        this->semanticErrorFlag = SEMANTIC_ERROR;
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
                        << ": The left-hand side of an assignment must be a variable." << endl;
                this->semanticErrorFlag = SEMANTIC_ERROR;
                return errorType;
            }
            if(compareType(tLeft, tRight) == NOT_MATCH)
            {
                cout << "Error type 5 at Line " << node->getChild(0)->getLineno()
                        << ": Type mismatched for assignment." << endl;
                this->semanticErrorFlag = SEMANTIC_ERROR;
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
                if(compareType(tLeft, tRight) == NOT_MATCH)
                {
                    cout << "Error type 7 at Line " << node->getChild(0)->getLineno()
                            << ": Type mismatched for operands." << endl;
                    this->semanticErrorFlag = SEMANTIC_ERROR;
                }
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
                if(typeExp->kind != ERROR) // don't consider error type as Error type 7
                {
                    cout << "Error type 7 at Line " << node->getChild(0)->getLineno()
                            << ": Type mismatched for operands." << endl;
                    this->semanticErrorFlag = SEMANTIC_ERROR;
                }
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
                if(typeExp->kind != ERROR) // don't consider error type as Error type 7
                {
                    cout << "Error type 7 at Line " << node->getChild(0)->getLineno()
                            << ": Type mismatched for operands." << endl;
                    this->semanticErrorFlag = SEMANTIC_ERROR;
                }
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
            string functionName = node->getChild(0)->getText();
            TableItem *item = symbolTable.getItemByName(functionName);
            Type function = item ? item->type : NULL;
            if(function == NULL)
            {
                if(!(functionName.compare("read") == 0 || functionName.compare("write") == 0))
                {
                    cout << "Error type 2 at Line " << node->getChild(0)->getLineno()
                            << ": Undefined function \"" << node->getChild(0)->getText() << "\"." << endl;
                    this->semanticErrorFlag = SEMANTIC_ERROR;
                }
                 // read and write have no return type, treated as errorType
                return errorType;
            }
            if(function->kind != FUNCTION)
            {
                cout << "Error type 11 at Line " << node->getChild(0)->getLineno()
                        << ": \"" << node->getChild(0)->getText() << "\" is not a function." << endl;
                this->semanticErrorFlag = SEMANTIC_ERROR;
                return errorType;
            }
            // if(function->u.function->isDefined)

            // check param types
            FieldList param = function->u.function->params;
            if(node->getProductionNo() == 12)
            {
                //Exp -> ID LP RP
                if(param != NULL)
                {
                    cout << "Error type 9 at Line " << node->getChild(0)->getLineno()
                        << ": Function \"" << node->getChild(0)->getText() << "\" is not applicable for arguments." << endl;
                    this->semanticErrorFlag = SEMANTIC_ERROR;
                    return errorType;
                }
            }
            else
            {
                //Exp -> ID LP Args RP
                FieldList tmpArgs = Args(node->getChild(2));
                if(!matchedFieldlist(tmpArgs, param))
                {
                    cout << "Error type 9 at Line " << node->getChild(0)->getLineno()
                        << ": Function \"" << node->getChild(0)->getText() << "(" << toString(param) << ")"
                        << "\" is not applicable for arguments \"(" << toString(tmpArgs) << ")\"." << endl;
                    this->semanticErrorFlag = SEMANTIC_ERROR;
                    // tmpArgs won't be added to symbol table, delete it in time
                    delete tmpArgs;
                    tmpArgs = NULL;
                    return errorType;
                }
                // tmpArgs won't be added to symbol table, delete it in time
                delete tmpArgs;
                tmpArgs = NULL;
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
                        << ": \"" << node->getChild(0)->getText() << "\" is not an array." << endl;
                this->semanticErrorFlag = SEMANTIC_ERROR;
                return errorType;
            }
            Type indexType = Exp(node->getChild(2));
            if(!(indexType->kind == BASIC && indexType->u.basic == INT))
            {
                cout << "Error type 12 at Line " << node->getChild(0)->getLineno()
                        << ": \"" << node->getChild(2)->getText() << "\" is not an integer." << endl;
                this->semanticErrorFlag = SEMANTIC_ERROR;
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
                        << ": Illegal use of \".\"." << endl;
                this->semanticErrorFlag = SEMANTIC_ERROR;
                return errorType;
            }

            // search for ID in this struct
            string IDName = node->getChild(2)->getText();
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
                        << ": Non-existent field \"" << IDName << "\"." << endl;
            this->semanticErrorFlag = SEMANTIC_ERROR;
            return errorType;
        }
        case 15:
        {
            //Exp -> ID
            TableItem *IDItem = symbolTable.getItemByName(node->getChild(0)->getText());
            if(IDItem == NULL || IDItem->type == NULL)
            {
                cout << "Error type 1 at Line " << node->getChild(0)->getLineno()
                        << ": Undefined variable \"" << node->getChild(0)->getText() << "\"." << endl;
                this->semanticErrorFlag = SEMANTIC_ERROR;
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

FieldList SemanticAnalyzer::Args(Node* node)
{
    showInfo(node);
    FieldList args = new FieldList_();
	//Exp
    Type expType = Exp(node->getChild(0));
    args->type = expType;
    if(node->getProductionNo() == 0)
    {
        //Exp COMMA Args
        args->tail = Args(node->getChild(2));
    }
    return args;
}

