#ifndef _SYMBOLTABLE_H
#define _SYMBOLTABLE_H

#include "common.h"
#include <stack>
#include <list>

/*
 * Define of Type
 */ 
typedef struct Type_* Type;
typedef struct FieldList_* FieldList;
typedef struct Structure_* Structure;
typedef struct Function_* Function;

enum BasicType          { INT, FLOAT };
enum TypeKind           { BASIC, ARRAY, STRUCTURE, FUNCTION, ERROR };
enum TypeCompare        { MATCH, NOT_SET, NOT_MATCH, LEFT_SMALLER, RIGHT_SMALLER };
enum AssignType         { LEFT, RIGHT };
enum ScopeType          { COMMON, STRUCT };

struct Type_
{
    TypeKind kind;
    union
    {
        BasicType basic;
        struct 
        {
            Type elem;
            int size;
        }array;
        Structure structure;
        Function function;
    }u;
    /* 
     * Indicate left value or right value.
     * Left value can appear on both side of '='
     * Temporarily, only useful when parsing productions of EXP.
     * Set RIGHT for FunDec.
     * Set defaultly to LEFT elsewhere.
     */
    AssignType assignType;
};

struct FieldList_
{
    string name;
    Type type;
    FieldList tail;
};

struct Structure_
{
    string name;
    FieldList structureFieldList;
};

struct Function_
{
    string name;
    // int paramNum;
    bool isDefined;
    Type returnType;
    FieldList params;
};

// record a declaration of a function
struct FunDecRecord
{
    int lineno;
    Function function;
};

/*
 * Declaration of SymbolTable
 */
struct TableItem
{
    string name;
    Type type;
    TableItem *next;
    int depth;      // depth of nesting scope
    /*
     * Pointer type actually not supported, use this field just to make array/struct params work properly.
     * Defaultly set to false.
     * Only set true when it's a array/struct param in ParamDec.
     */
    bool isPointer; 
};

#define MAX_HASH_SIZE 16384
class SymbolTable
{
private:
    TableItem *hashTable[MAX_HASH_SIZE];
    bool supportNestedScope;
    int scopeDepth;
    stack<ScopeType> scopeStack;
    
    unsigned int hash(const char* name);
    
public:
    list<FunDecRecord> funDecRecords;

    SymbolTable();
    ~SymbolTable();
    void setSupportNestedScope(bool support);
    void clearTable();
    void addItem(TableItem *item);
    void addFieldList(FieldList fieldList);
    void addFieldList(FieldList fieldList, bool isPointer);
    Type addStructureAndGetType(Structure structure);
    Type addFunctionAndGetType(Function function);
    TableItem* getItemByName(string name);
    bool isDuplicatedName(string name);
    bool isDuplicatedNameInCurrentScope(string name);

    void enterScope();
    void enterScope(ScopeType scopeType);
    void exitScope();
    ScopeType getScopeType();

    bool functionAllDefined();

};
#endif