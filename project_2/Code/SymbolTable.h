#ifndef _SYMBOLTABLE_H
#define _SYMBOLTABLE_H

#include "common.h"

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

/*
 * Declaration of SymbolTable
 */
struct TableItem
{
    string name;
    Type type;
    TableItem *next;
    int depth;  // depth of nesting scope
};

#define MAX_HASH_SIZE 16384
class SymbolTable
{
private:
    TableItem *hashTable[MAX_HASH_SIZE];
    int scopeDepth;

    unsigned int hash(const char* name);
public:
    SymbolTable();
    ~SymbolTable();
    void clearTable();
    void addItem(TableItem *item);
    void addFieldList(FieldList fieldList);
    Type addStructureAndGetType(Structure structure);
    Type addFunctionAndGetType(Function function);
    TableItem* getItemByName(string name);
    bool isDuplicatedName(string name);
    bool isDuplicatedNameInCurrentScope(string name);

    void enterScope();
    void exitScope();
};
#endif