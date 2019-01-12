#ifndef _INTER_CODE_H
#define _INTER_CODE_H

#include "common.h"
#include <list>
#include <fstream>
#include "SyntaxTree.h"
#include "SymbolTable.h"

enum OperandKind    {   VARIABLE, TMP, CONSTANT, ADDRESS, DEREFER, LABEL };
enum InterCodeKind  {   ASSIGN, DEC, 
                        ADD, SUB, MUL, DIV, 
                        FUNC_DEF, PARAM, RETURN, ARG, READ, WRITE, 
                        ASSIGN_CALL, 
                        LABEL_DEC, GOTO, COND_GOTO };

typedef struct Operand_*    Operand;
typedef struct InterCode_*  InterCode;
typedef struct IRCondGoto_* IRCondGoto;
typedef struct IRFunction_* IRFunction;

struct Operand_
{
    static int tmp_num, label_num; // defined in InterCode.cpp
    OperandKind kind;
    union
    {
        TableItem* tableItem_p; // VARIABLE
        int int_const;          // CONSTANT
        int tmp_no;             // TMP
        int label_no;           // LABEL
        Operand operand;        // ADDRESS, DEREFER
    } u;

    // Constructors
    Operand_(OperandKind kind);
    Operand_(OperandKind kind, int int_const);
    Operand_(OperandKind kind, TableItem* tableItem_p);
    Operand_(OperandKind kind, Operand operand);
};

struct InterCode_
{
    InterCodeKind kind;
    union
    {
        struct {Operand op;} sinop;                        // PARAM, RETURN, ARG, READ, WRITE, LABEL_DEC, GOTO
        struct {Operand left, right;} assign;                       // ASSIGN
        struct {Operand result, op1, op2;} binop;                   // ADD, SUB, MUL, DIV
        struct {TableItem* tableItem_p; int size;} dec;             // DEC
        struct {Operand result; IRFunction function;} assign_call;  // ASSIGN_CALL
        IRCondGoto condGoto;                                        // COND_GOTO
        IRFunction function;                                        // FUNC_DEF
    } u;

    // Constructors
    InterCode_(InterCodeKind kind, Operand op);
    InterCode_(InterCodeKind kind, Operand right, Operand left);
    InterCode_(InterCodeKind kind, Operand result, Operand op1, Operand op2);
    InterCode_(InterCodeKind kind, TableItem* tableItem_p, int size);
    InterCode_(InterCodeKind kind, IRFunction function);
    InterCode_(InterCodeKind kind, IRCondGoto condGoto);
    InterCode_(InterCodeKind kind, Operand result, IRFunction function);

    // InterCode pre, next;
};

struct IRCondGoto_
{
    // if x [relop] y goto label
    Operand x, y, label;
    string relop;

    IRCondGoto_(Operand x, Operand y, Operand label, string relop)
    {
        this->x = x;
        this->y = y;
        this->label = label;
        this->relop = relop;
    }
};
struct IRFunction_
{
    string name;
    IRFunction_(string name)
    {
        this->name = name;
    }
};

struct ExpResult
{
    Type type;
    Operand operand;
    /*
     * Pointer type actually not supported, use this field just to make array/struct params work properly.
     * Defaultly set to false.
     * Only set true when it's a array/struct param in ParamDec.
     */
    bool isPointer;
};

class InterCodeTranslater
{
private:
    list<InterCode> interCodeList;
    list<InterCode> emptyIRList; // empty code list
    SymbolTable* symbolTable;

    /**************************** Tool Functions ***************************/
    int sizeOfType(Type type);
    void deleteInvalidCodes();
    ExpResult makeExpResult(const Type type, Operand operand, bool isPointer)
    {
        ExpResult r;
        r.type = type;
        r.operand = operand;
        r.isPointer = isPointer;
        return r;
    }
    /************************* End of Tool Functions ***********************/

public:
    InterCodeTranslater(SymbolTable* symbolTable);
    ~InterCodeTranslater();
    
    void translate(Node* treeRoot);
    void output();
    void output(string filename);
    list<InterCode> getInterCodeList() {
        return this->interCodeList;
    }
    static string toString(Operand operand);
    static string toString(InterCode interCode);

    /*************************** Semantic Actions **************************/

    /* High-level Definitions */
    void Program(Node* node);
    void ExtDefList(Node* node);
    void ExtDef(Node* node);
    void ExtDecList(Node* node);

    /* Specifiers */
    void Specifier(Node* node);
    void StructSpecifier(Node* node);
    string OptTag(Node* node);
    string Tag(Node* node);

    /* Declarators */
    void VarDec(Node* node);
    Function FunDec(Node* node);
    void VarList(Node* node);
    void ParamDec(Node* node);

    /* Statements */
    void CompSt(Node* node);
    void StmtList(Node* node);
    void Stmt(Node* node);

    /* Local Definitons */
    void DefList(Node* node);
    void Def(Node* node);
    void DecList(Node* node);
    void Dec(Node* node);

    /* Expressions */
    ExpResult translateCond(Node* node, Operand label1, Operand label2);
    ExpResult Exp(Node* node, Operand place);
    void Args(Node* node, list<Operand>& arg_list);

    /*********************** End of Semantic Actions ***********************/
};

#endif