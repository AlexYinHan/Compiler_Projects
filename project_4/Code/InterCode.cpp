#include "InterCode.h"

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

// global variable
int Operand_::tmp_num = 0, Operand_::label_num = 0;

Operand_::Operand_(OperandKind kind)
{
    this->kind = kind;
    if(kind == TMP)
    {
        // TMP
        this->u.tmp_no = ++Operand_::tmp_num;
    }
    else
    {
        // LABEL
        this->u.label_no = ++Operand_::label_num;
    }
}

Operand_::Operand_(OperandKind kind, int value)
{
    this->kind = kind;
    this->u.int_const = value;
}
Operand_::Operand_(OperandKind kind, TableItem* tableItem_p)
{
    this->kind = kind;
    this->u.tableItem_p = tableItem_p;
}

Operand_::Operand_(OperandKind kind, Operand operand)
{
    this->kind = kind;
    this->u.operand = operand;
}

InterCode_::InterCode_(InterCodeKind kind, Operand op)
{
    this->kind = kind;
    this->u.sinop.op = op;
}
InterCode_::InterCode_(InterCodeKind kind, Operand left, Operand right)
{
    this->kind = kind;
    this->u.assign.left = left;
    this->u.assign.right = right;
}
InterCode_::InterCode_(InterCodeKind kind, Operand result, Operand op1, Operand op2)
{
    this->kind = kind;
    this->u.binop.result = result;
    this->u.binop.op1 = op1;
    this->u.binop.op2 = op2;
}
InterCode_::InterCode_(InterCodeKind kind, TableItem* tableItem_p, int size)
{
    this->kind = kind;
    this->u.dec.tableItem_p = tableItem_p;
    this->u.dec.size = size;
}
InterCode_::InterCode_(InterCodeKind kind, IRFunction function)
{
    this->kind = kind;
    this->u.function = function;
}
InterCode_::InterCode_(InterCodeKind kind, IRCondGoto condGoto)
{
    this->kind = kind;
    this->u.condGoto = condGoto;
}
InterCode_::InterCode_(InterCodeKind kind, Operand argOp, int argIndex)
{
    this->kind = kind;
    this->u.arg.argOp = argOp;
    this->u.arg.argIndex = argIndex;
}
InterCode_::InterCode_(InterCodeKind kind, Operand result, IRFunction function)
{
    this->kind = kind;
    this->u.assign_call.result = result;
    this->u.assign_call.function = function;
}

InterCodeTranslater::InterCodeTranslater(SymbolTable* symbolTable)
{
    this->symbolTable = symbolTable;
}

InterCodeTranslater::~InterCodeTranslater()
{

}

void InterCodeTranslater::translate(Node* treeRoot)
{
    Program(treeRoot);
    deleteInvalidCodes();
}

void InterCodeTranslater::output()
{
    list<InterCode>::iterator it;
    for(it = interCodeList.begin(); it != interCodeList.end(); it ++)
    {
        cout << toString(*it) << endl;
    }
}

void InterCodeTranslater::output(string filename)
{
    ofstream file;
    file.open(filename);
    if(!file.is_open())
    {
        return;
    }
    list<InterCode>::iterator it;
    for(it = interCodeList.begin(); it != interCodeList.end(); it ++)
    {
        file << toString(*it) << endl;
    }
    file.close();
}

/**************************** Tool Functions ***************************/
/*
 * Return byte size of type.
 * int and float take up 4 byte;
 */
int InterCodeTranslater::sizeOfType(Type type)
{
    switch(type->kind)
    {
        case BASIC:
            if(type->u.basic == INT || type->u.basic == FLOAT){
                return 4;
            }else{
                return 0;   // other basic types not supported yet
            }
        case ARRAY:
            // multi dimensions
            return type->u.array.size * sizeOfType(type->u.array.elem);
        case STRUCTURE:
        {
            int totalSize = 0;
            FieldList field = type->u.structure->structureFieldList;
            while(field)
            {
                totalSize += sizeOfType(field->type);
                field = field->tail;
            }
            return totalSize;
        }
        case FUNCTION:
        case ERROR:
        default: return 0;
    }
}

void InterCodeTranslater::deleteInvalidCodes()
{
    // delete codes with null operands
    list<InterCode>::iterator it;
    for(it = interCodeList.begin(); it != interCodeList.end();)
    {
        bool invalid = false;
        switch((*it)->kind)
        {
            case PARAM:case RETURN:case ARG:case READ:case WRITE:case LABEL_DEC:case GOTO:
                invalid = ((*it)->u.sinop.op == NULL);
                break;
            case ASSIGN:
                invalid = ((*it)->u.assign.left == NULL || (*it)->u.assign.right == NULL);
                break;
            case ADD:case SUB:case MUL:case DIV:
                invalid = ((*it)->u.binop.result == NULL || (*it)->u.binop.op1 == NULL || (*it)->u.binop.op2 == NULL);
                break;
            case DEC:
                invalid = ((*it)->u.dec.tableItem_p == NULL);
                break;
            case ASSIGN_CALL:
                invalid = ((*it)->u.assign_call.function == NULL || (*it)->u.assign_call.result == NULL);
                break;
            case COND_GOTO:
                invalid = ((*it)->u.condGoto == NULL);
                break;
            case FUNC_DEF:
                invalid = ((*it)->u.function == NULL);
                break;
            default: break;
        }
        if(invalid){
            interCodeList.erase(it++);
        } else {
            it ++;
        }
    }
}

/************************* End of Tool Functions ***********************/
string InterCodeTranslater::toString(Operand operand)
{
    if(operand == NULL)
    {
        return "null operand";
    }

    switch(operand->kind)
    {
        case VARIABLE:
            return operand->u.tableItem_p->name;
        case CONSTANT:
            return "#" + to_string(operand->u.int_const);
        case TMP:
            return "t" + to_string(operand->u.tmp_no);
        case LABEL:
            return "label" + to_string(operand->u.label_no);
        case ADDRESS:
        {
            if(operand->u.operand->kind == DEREFER) {
                // &*t -> t
                return toString(operand->u.operand->u.operand);
            } else {
                return "&" + toString(operand->u.operand);
            }
        }
        case DEREFER:
        {
            if(operand->u.operand->kind == ADDRESS) {
                // *&t -> t
                return toString(operand->u.operand->u.operand);
            } else {
                return "*" + toString(operand->u.operand);
            }
        }
        default:
            return "unrecognized operand";
    }
}
string InterCodeTranslater::toString(InterCode interCode)
{
// cout << "code" << interCode->kind << endl;
    switch(interCode->kind)
    {
        case PARAM:
            return "PARAM " + toString(interCode->u.sinop.op);
        case RETURN:
            return "RETURN " + toString(interCode->u.sinop.op);
        case ARG:
            return "ARG " + toString(interCode->u.arg.argOp);
        case READ:
            return "READ " + toString(interCode->u.sinop.op);
        case WRITE:
            return "WRITE " + toString(interCode->u.sinop.op);
        case LABEL_DEC:
            return "LABEL " + toString(interCode->u.sinop.op) + " : ";
        case GOTO:
            return "GOTO " + toString(interCode->u.sinop.op);
        case ASSIGN:
            return toString(interCode->u.assign.left) + " := " + toString(interCode->u.assign.right);
        case ADD:
            return toString(interCode->u.binop.result) + " := " 
                    + toString(interCode->u.binop.op1) + " + " + toString(interCode->u.binop.op2);
        case SUB:
            return toString(interCode->u.binop.result) + " := " 
                    + toString(interCode->u.binop.op1) + " - " + toString(interCode->u.binop.op2);
        case MUL:
            return toString(interCode->u.binop.result) + " := " 
                    + toString(interCode->u.binop.op1) + " * " + toString(interCode->u.binop.op2);
        case DIV:
            return toString(interCode->u.binop.result) + " := " 
                    + toString(interCode->u.binop.op1) + " / " + toString(interCode->u.binop.op2);
        case DEC:
            return "DEC " + interCode->u.dec.tableItem_p->name + " " + to_string(interCode->u.dec.size);
        case ASSIGN_CALL:
            return toString(interCode->u.assign_call.result) + " := CALL " + interCode->u.assign_call.function->name; 
        case COND_GOTO:
            return "IF " + 
                toString(interCode->u.condGoto->x) + 
                " " + interCode->u.condGoto->relop + " " + 
                toString(interCode->u.condGoto->y) + 
                " GOTO " + 
                toString(interCode->u.condGoto->label);
        case FUNC_DEF:
            return "FUNCTION " + interCode->u.function->name + " :";
        default: return "unrecognized code";
    }
}
/*************************** Semantic Actions **************************/

// TODO: improve efficiency
/* High-level Definitions */
void InterCodeTranslater::Program(Node* node)
{
	showInfo(node);
    // Program -> ExtDefList
    return ExtDefList(node->getChild(0));
}

void InterCodeTranslater::ExtDefList(Node* node)
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

void InterCodeTranslater::ExtDef(Node* node)
{
	showInfo(node);
    switch(node->getProductionNo())
    {
        case 0:
            // ExtDef -> Specifier ExtDecList SEMI
            ExtDecList(node->getChild(1));
            return;
        case 1:
            // ExtDef -> Specifier SEMI
            return;
        case 2:
        {
            // ExtDef -> Specifier FunDec SEMI
            FunDec(node->getChild(1));
            return;
        }
        case 3:
        {
            // ExtDef -> Specifier FunDec CompSt
            Function function = FunDec(node->getChild(1));

            // actuall paramNum only needed when called, not defined, simply set it to 0 here
            InterCode code = new InterCode_(FUNC_DEF, new IRFunction_(function->name, 0));      // code - FUNCTION f :
            interCodeList.push_back(code);
            FieldList param = function->params;
            while(param)
            {
                Operand op = new Operand_(VARIABLE, symbolTable->getItemByName(param->name));   // code - PARAM v
                InterCode paramCode = new InterCode_(PARAM, op);
                interCodeList.push_back(paramCode);
                param = param->tail;
            }

            CompSt(node->getChild(2));
            return;
        }  
        default: return;
    }
}

void InterCodeTranslater::ExtDecList(Node* node)
{
	showInfo(node);
    switch(node->getProductionNo())
    {
        case 0:
            //ExtDecList -> VarDec
            VarDec(node->getChild(0));
            break;
        case 1:
	        //ExtDecList -> VarDec COMMA ExtDecList
            VarDec(node->getChild(0));
            ExtDecList(node->getChild(1));
            break;
        default: return;
    }
}

/* Specifiers */
// no need to generate code for Specifiers
void InterCodeTranslater::Specifier(Node* node)
{
	showInfo(node);
}

void InterCodeTranslater::StructSpecifier(Node* node)
{
	showInfo(node);
}

string OptTag(Node* node)
{
	showInfo(node);
}

string Tag(Node* node)
{
	showInfo(node);
}

/* Declarators */
void InterCodeTranslater::VarDec(Node* node)
{
	showInfo(node);
    switch(node->getProductionNo())
    {
        case 0:
        {
            //VarDec -> ID
            string name = node->getChild(0)->getText();
            TableItem *varItem = symbolTable->getItemByName(name);
            if(varItem != NULL && (varItem->type->kind == ARRAY || varItem->type->kind == STRUCTURE))
            {
                InterCode code = new InterCode_(DEC, 
                                        varItem, sizeOfType(varItem->type)); // code - DEC x [size]
                interCodeList.push_back(code);
            }
            return;
        }
        case 1: 
            //VarDec -> VarDec LB INT RB
            VarDec(node->getChild(0));
        default: return;
    }
}

Function InterCodeTranslater::FunDec(Node* node)
{
	showInfo(node);
    if(node->getProductionNo() == 0)
    {
        //FunDec -> ID LP VarList RP
    }
    else 
    {
        //FunDec -> ID LP RP
    }
    string functionName = node->getChild(0)->getText();
    return symbolTable->getItemByName(functionName)->type->u.function;
}

void InterCodeTranslater::VarList(Node* node)
{
	showInfo(node);
    //VarList -> ParamDec
    ParamDec(node->getChild(0));
    if(node->getProductionNo() == 0)
    {
        //VarList -> ParamDec COMMA VarList
        VarList(node->getChild(2));
    }
}

void InterCodeTranslater::ParamDec(Node* node)
{
	showInfo(node);
    //ParamDec -> Specifier VarDec
    Specifier(node->getChild(0));
    VarDec(node->getChild(1));
}

/* Statements */
void InterCodeTranslater::CompSt(Node* node)
{
	showInfo(node);
    //CompSt -> LC DefList StmtList RC
    DefList(node->getChild(1));
    StmtList(node->getChild(2));
}

void InterCodeTranslater::StmtList(Node* node)
{
	showInfo(node);
    if(node == NULL)
    {
	    //StmtList -> $empty
        return;
    }
    //StmtList -> Stmt StmtList
    Stmt(node->getChild(0));
    StmtList(node->getChild(1));
}

void InterCodeTranslater::Stmt(Node* node)
{
	showInfo(node);
    switch(node->getProductionNo())
    {
        case 0:
            //Stmt -> Exp SEMI
            Exp(node->getChild(0), NULL);
            return;
        case 1:
            //Stmt -> CompSt
            CompSt(node->getChild(0));
            return;
        case 2:
        {
            //Stmt -> RETURN Exp SEMI
            Operand t1 = new Operand_(TMP);
            Exp(node->getChild(1), t1);                     // code 1
            InterCode code2 = new InterCode_(RETURN, t1);   // code 2
            interCodeList.push_back(code2);
            return;
        }
        case 3:
        {
            //Stmt -> IF LP Exp RP Stmt
            Operand label1 = new Operand_(LABEL);
            Operand label2 = new Operand_(LABEL);
            translateCond(node->getChild(2), label1, label2);       // code 1
            InterCode code1_1 = new InterCode_(LABEL_DEC, label1);  // code 1.1 - [LABEL label1]
            interCodeList.push_back(code1_1);
            Stmt(node->getChild(4));                                // code 2
            InterCode code2_1 = new InterCode_(LABEL_DEC, label2);  // code 2.1 - [LABEL label2]
            interCodeList.push_back(code2_1);
            return;
        }
        case 4:
        {
            //Stmt -> IF LP Exp RP Stmt ELSE Stmt
            Operand label1 = new Operand_(LABEL);
            Operand label2 = new Operand_(LABEL);
            Operand label3 = new Operand_(LABEL);
            translateCond(node->getChild(2), label1, label2);       // code 1
            InterCode code1_1 = new InterCode_(LABEL_DEC, label1);  // code 1.1 - [LABEL label1]
            interCodeList.push_back(code1_1);
            Stmt(node->getChild(4));                                // code 2
            InterCode code2_1 = new InterCode_(GOTO, label3);       // code 2.1 - [GOTO label3]
            interCodeList.push_back(code2_1);
            InterCode code2_2 = new InterCode_(LABEL_DEC, label2);  // code 2.2 - [LABEL label2]
            interCodeList.push_back(code2_2);
            Stmt(node->getChild(6));                                // code 3
            InterCode code3_1 = new InterCode_(LABEL_DEC, label3);  // code 3.1
            interCodeList.push_back(code3_1);
            return;
        }
        case 5:
        {
            //Stmt -> WHILE LP Exp RP Stmt
            Operand label1 = new Operand_(LABEL);
            Operand label2 = new Operand_(LABEL);
            Operand label3 = new Operand_(LABEL);
            InterCode code0_1 = new InterCode_(LABEL_DEC, label1);  // code 0.1 - [LABEL label1]
            interCodeList.push_back(code0_1);
            translateCond(node->getChild(2), label2, label3);       // code 1
            InterCode code1_1 = new InterCode_(LABEL_DEC, label2);  // code 1.1 - [LABEL label2]
            interCodeList.push_back(code1_1);
            Stmt(node->getChild(4));                                // code 2
            InterCode code2_1 = new InterCode_(GOTO, label1);       // code 2.1 - [GOTO labe1]
            interCodeList.push_back(code2_1);
            InterCode code2_2 = new InterCode_(LABEL_DEC, label3);  // code 2.2 - [LABEL label3]
            interCodeList.push_back(code2_2);
            return;
        }
        default: return;
    }
}

/* Local Definitons */
void InterCodeTranslater::DefList(Node* node)
{
	showInfo(node);
    if(node == NULL)
    {
        //DefList -> $empty
        return;
    }
    //DefList -> Def DefList
    Def(node->getChild(0));
    DefList(node->getChild(1));
}

void InterCodeTranslater::Def(Node* node)
{
	showInfo(node);
    //Def -> Specifier DecList SEMI
    Specifier(node->getChild(0));
    DecList(node->getChild(1));
}

void InterCodeTranslater::DecList(Node* node)
{
	showInfo(node);
    //DecList -> Dec
    Dec(node->getChild(0));
    if(node->getProductionNo() == 0)
    {
        return;
    }

    //DecList -> Dec COMMA DecList
    DecList(node->getChild(2));
}

void InterCodeTranslater::Dec(Node* node)
{
	showInfo(node);
    VarDec(node->getChild(0));
    if(node->getProductionNo() == 0)
    {
        //Dec -> VarDec
        return;
    }
	//Dec -> VarDec ASSIGNOP Exp
    if(node->getChild(0)->getProductionNo() == 0)
    {
        // VarDec -> ID
        TableItem *IDItem = symbolTable->getItemByName(node->getChild(0)->getChild(0)->getText());
        Operand var = new Operand_(VARIABLE, IDItem);
        Exp(node->getChild(2), var);     // code - var := e
    }
    // Dec -> arrayElem ASSIGNOP Exp
    // got no idea how this shit could be translated
}

/* Expressions */
ExpResult InterCodeTranslater::translateCond(Node* node, Operand label_true, Operand label_false)
{
    switch(node->getProductionNo())
    {
        case 3:
        {
            //Exp -> Exp RELOP Exp
            Operand t1 = new Operand_(TMP);
            Operand t2 = new Operand_(TMP);
            ExpResult expResult = Exp(node->getChild(0), t1);                                       // code 1
            Exp(node->getChild(2), t2);                                                             // code 2
            string op = node->getChild(1)->getText();
            InterCode code3 = new InterCode_(COND_GOTO, new IRCondGoto_(t1, t2, label_true, op));   // code 3
            InterCode code4 = new InterCode_(GOTO, label_false);                                    // code 4
            interCodeList.push_back(code3);
            interCodeList.push_back(code4);
            return expResult;
        }
        case 10:
        {
            //Exp -> NOT Exp
            ExpResult expResult = translateCond(node->getChild(1), label_false, label_true);
            return expResult;
        }
        case 1:
        {
            //Exp -> Exp AND Exp
            Operand label1 = new Operand_(LABEL);
            ExpResult expResult = translateCond(node->getChild(0), label1, label_false);    // code 1
            InterCode code = new InterCode_(LABEL_DEC, label1);                             // code 1.1 - [LABEL label1]
            interCodeList.push_back(code); 
            translateCond(node->getChild(2), label_true, label_false);                      // code 2
            return expResult;
        }
        case 2:
        {
            //Exp -> Exp OR Exp
            Operand label1 = new Operand_(LABEL);
            ExpResult expResult = translateCond(node->getChild(0), label_true, label1); // code 1
            InterCode code = new InterCode_(LABEL_DEC, label1);                         // code 1.1 - [LABEL label1]
            interCodeList.push_back(code); 
            translateCond(node->getChild(2), label_true, label_false);                  // code 2
            return expResult;
        }
        default:
        {
            //common Exp as cond exp
            Operand t1 = new Operand_(TMP);
            ExpResult expResult = Exp(node, t1);                    // code 1
            IRCondGoto condGoto = new IRCondGoto_(t1, new Operand_(CONSTANT, 0), label_true, "!=");
            InterCode code2 = new InterCode_(COND_GOTO, condGoto);  // code 2
            InterCode code3 = new InterCode_(GOTO, label_false);    // code 3
            interCodeList.push_back(code2);
            interCodeList.push_back(code3);
            return expResult;
        }
    }
}
ExpResult InterCodeTranslater::Exp(Node* node, Operand place)
{
    // TODO: remove value assign for ID
	showInfo(node);
    switch(node->getProductionNo())
    {
        case 0:
        {
            //Exp -> Exp ASSIGNOP Exp
            Operand t1 = new Operand_(TMP);
            Exp(node->getChild(2), t1);                                 // code1
            ExpResult expResult = Exp(node->getChild(0), place);        // code2.2
            Operand leftOperand = expResult.operand;
            InterCode code = new InterCode_(ASSIGN, leftOperand, t1);   // code2.1
            interCodeList.insert(--interCodeList.end(), code);          // insert code2.1 before code2.2      
            return expResult;
        }
        case 1:
            //Exp -> Exp AND Exp
        case 10:
            //Exp -> NOT Exp
        case 2:
            //Exp -> Exp OR Exp
        case 3:
        {
            //Exp -> Exp RELOP Exp
            // case 1,2,3,10 share the same actions
            Operand label1 = new Operand_(LABEL);
            Operand label2 = new Operand_(LABEL);
            InterCode code0 = new InterCode_(ASSIGN, place, new Operand_(CONSTANT, 0));     // code 0
            interCodeList.push_back(code0);
            ExpResult expResult = translateCond(node, label1, label2);                      // code 1
            InterCode code2_1 = new InterCode_(LABEL_DEC, label1);                          // code 2.1
            InterCode code2_2 = new InterCode_(ASSIGN, place, new Operand_(CONSTANT, 1));   // code 2.2
            InterCode code3 = new InterCode_(LABEL_DEC, label2);                            // code 3
            interCodeList.push_back(code2_1);
            interCodeList.push_back(code2_2);
            interCodeList.push_back(code3);
            return makeExpResult(expResult.type, place, false);
        }
        case 4:
        {
            //Exp -> Exp PLUS Exp
            Operand t1 = new Operand_(TMP);
            Operand t2 = new Operand_(TMP);
            ExpResult expResult = Exp(node->getChild(0), t1);       // code1
            Exp(node->getChild(2), t2);                             // code2
            InterCode code = new InterCode_(ADD, place, t1, t2);    // code3
            interCodeList.push_back(code);
            return makeExpResult(expResult.type, place, false);
        }
        case 5:
        {
            //Exp -> Exp MINUS Exp
            Operand t1 = new Operand_(TMP);
            Operand t2 = new Operand_(TMP);
            ExpResult expResult = Exp(node->getChild(0), t1);       // code1
            Exp(node->getChild(2), t2);                             // code2
            InterCode code = new InterCode_(SUB, place, t1, t2);    // code3
            interCodeList.push_back(code);
            return makeExpResult(expResult.type, place, false);
        }
        case 6:
        {
            //Exp -> Exp STAR Exp
            Operand t1 = new Operand_(TMP);
            Operand t2 = new Operand_(TMP);
            ExpResult expResult = Exp(node->getChild(0), t1);       // code1
            Exp(node->getChild(2), t2);                             // code2
            InterCode code = new InterCode_(MUL, place, t1, t2);    // code3
            interCodeList.push_back(code);
            return makeExpResult(expResult.type, place, false);
        }
        case 7:
        {
            //Exp -> Exp DIV Exp
            Operand t1 = new Operand_(TMP);
            Operand t2 = new Operand_(TMP);
            ExpResult expResult = Exp(node->getChild(0), t1);       // code1
            Exp(node->getChild(2), t2);                             // code2
            InterCode code = new InterCode_(DIV, place, t1, t2);    // code3
            interCodeList.push_back(code);
            return makeExpResult(expResult.type, place, false);
        }
        case 8:
        {
            //Exp -> LP Exp RP
            ExpResult expResult = Exp(node->getChild(1), place);
            return makeExpResult(expResult.type, expResult.operand, false);
        }
        case 9:
        {
            //Exp -> MINUS Exp
            Operand t1 = new Operand_(TMP);
            ExpResult expResult = Exp(node->getChild(1), t1);                           // code 1
            InterCode code = new InterCode_(SUB, place, new Operand_(CONSTANT, 0), t1); // code 2
            interCodeList.push_back(code);
            return makeExpResult(expResult.type, place, false);
        }
        case 11:
        {
            //Exp -> ID LP Args RP
            string functionName = node->getChild(0)->getText();
            list<Operand> arg_list;
            Args(node->getChild(2), arg_list); // code 1
            if(functionName.compare("write") == 0)
            {
                InterCode code1_1 = new InterCode_(WRITE, *(--arg_list.end()));     // code 1.1 - [WRITE args[0]]
                interCodeList.push_back(code1_1);
                return makeExpResult(NULL, place, false);
            }
            else
            {
                list<Operand>::iterator it;
                int argIndex = arg_list.size() - 1;
                for(it = arg_list.begin(); it != arg_list.end(); ++it)
                {
                    InterCode code2 = new InterCode_(ARG, *it, argIndex);           // code 2 - [ARG args[i]]
                    interCodeList.push_back(code2);
                    argIndex --;
                }
                IRFunction function = new IRFunction_(functionName, arg_list.size());
                InterCode code2_1 = new InterCode_(ASSIGN_CALL, place, function);   // code 2.1 - [place := CALL f]
                interCodeList.push_back(code2_1);
                return makeExpResult(symbolTable->getItemByName(functionName)->type, place, false);
            }
        }
        case 12:
        {
            //Exp -> ID LP RP
            string functionName = node->getChild(0)->getText();
            if(functionName.compare("read") == 0)
            {
                InterCode code = new InterCode_(READ, place);                   // code - [READ place]
                interCodeList.push_back(code);
                return makeExpResult(NULL, place, false);
            } 
            else 
            {
                IRFunction function = new IRFunction_(functionName, 0);
                InterCode code = new InterCode_(ASSIGN_CALL, place, function);  // code - [place := CALL f]
                interCodeList.push_back(code);
                return makeExpResult(symbolTable->getItemByName(functionName)->type, place, false);
            }
        }
        case 13:
        {
            //Exp -> Exp LB Exp RB
            ExpResult expResult = Exp(node->getChild(0), NULL);
            
            Operand baseAddr;
            if(expResult.isPointer || node->getChild(0)->getProductionNo() == 13){
                // if exp1 is of case 13, which means it returns a derefer of an address within an array
                // parse the addr of it and set baseAddr
                baseAddr = expResult.operand;
                if(baseAddr->kind == DEREFER) {
                    baseAddr = baseAddr->u.operand;                                         // t3 := baseAddr
                }
            } else {
                baseAddr = new Operand_(ADDRESS, expResult.operand);                         // t3 := &baseAddr
            }                    
            int size = sizeOfType(expResult.type->u.array.elem);
            Operand t1 = new Operand_(TMP);
            Operand t2 = new Operand_(TMP);
            Operand t3 = new Operand_(TMP);
            Exp(node->getChild(2), t1).operand;                                             // code - t1 := e2
            InterCode code1 = new InterCode_(MUL, t2, t1, new Operand_(CONSTANT, size));    // code - t2 := t1 * e2
            InterCode code2 = new InterCode_(ASSIGN, t3, baseAddr);                         // code - t3 := &baseAddr
                                                                                            // or     t3 := baseAddr
            InterCode code3 = new InterCode_(ADD, t3, t3, t2);                              // code - t3 := t3 + t2
            InterCode code4 = new InterCode_(ASSIGN, place, new Operand_(DEREFER, t3));     // code - place := *t3
            interCodeList.push_back(code1);
            interCodeList.push_back(code2);
            interCodeList.push_back(code3);
            interCodeList.push_back(code4);
            Type type = (expResult.type->kind == ARRAY) ? (expResult.type->u.array.elem) : (expResult.type);
            return makeExpResult(type, new Operand_(DEREFER, t3), false);
        }
        case 14:
        {
            //Exp -> Exp DOT ID
            // Operand t1 = new Operand_(TMP);
            Operand t2 = new Operand_(TMP);
            Operand t3 = new Operand_(TMP);
            // ExpResult expResult = Exp(node->getChild(0), t1);                        
            ExpResult expResult = Exp(node->getChild(0), NULL);
            InterCode code1 = expResult.isPointer ? 
                        new InterCode_(ASSIGN, t2, expResult.operand) :                         // code1 - t2 := e
                        new InterCode_(ASSIGN, t2, new Operand_(ADDRESS, expResult.operand));   // or    - t2 := &e 
            interCodeList.push_back(code1);

            // search for ID in this struct
            string IDName = node->getChild(2)->getText();
            FieldList itr = expResult.type->u.structure->structureFieldList;
            int offset = 0;
            Type elemType = NULL;
            while(itr)
            {
                if(itr->name.compare(IDName) == 0)
                {
                    elemType = itr->type;
                    break;
                }
                offset += sizeOfType(itr->type);
                itr = itr->tail;
            }
            InterCode code2 = new InterCode_(ADD, 
                                t3, t2, new Operand_(CONSTANT, offset));    // code2 - t3 := t2 + offset
            interCodeList.push_back(code2);
            InterCode code3 = new InterCode_(ASSIGN, 
                                place, new Operand_(DEREFER, t3));          // code3 - place := *t3
            interCodeList.push_back(code3);

            return makeExpResult(elemType, new Operand_(DEREFER, t3), false);
        }
        case 15:
        {
            //Exp -> ID
            TableItem *IDItem = symbolTable->getItemByName(node->getChild(0)->getText());
            Operand right = new Operand_(VARIABLE, IDItem);
            InterCode code = new InterCode_(ASSIGN, place, right); // code - [place := variable.name]
            interCodeList.push_back(code);
            return makeExpResult(IDItem->type, right, IDItem->isPointer);
        }
        case 16:
        {    
            //Exp -> INT
            Operand right = new Operand_(CONSTANT, node->getChild(0)->getIntValue());
            InterCode code = new InterCode_(ASSIGN, place, right); // code - [place := #value]
            interCodeList.push_back(code);
            Type type = new Type_();
            type->kind = BASIC;
            type->u.basic = INT;
            type->assignType = RIGHT;
            return makeExpResult(type, right, false);
        }
        case 17:
        {
            //Exp -> FLOAT
            // MARK: InterCode - place := #value
            // float const intercode not supported yet
            Type type = new Type_();
            type->kind = BASIC;
            type->u.basic = FLOAT;
            type->assignType = RIGHT;
            return makeExpResult(type, place, false);
        }
    }
}

void InterCodeTranslater::Args(Node* node, list<Operand>& arg_list)
{
	showInfo(node);
	//Exp
    Operand t1 = new Operand_(TMP);
    ExpResult e = Exp(node->getChild(0), t1);                                   // code 1 - t1 := e      
    InterCode code;
    if(e.type->kind == ARRAY || e.type->kind == STRUCTURE) {
        code = new InterCode_(ASSIGN, t1, new Operand_(ADDRESS, e.operand));    // code 1 - t1 := &a
        interCodeList.pop_back();
        interCodeList.push_back(code);
    }
    arg_list.push_front(t1);
    if(node->getProductionNo() == 0)
    {
        //Exp COMMA Args
        Args(node->getChild(2), arg_list);  // code 2
    }
    return;
}

/*********************** End of Semantic Actions ***********************/