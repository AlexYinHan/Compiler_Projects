#include "SymbolTable.h"

SymbolTable::SymbolTable()
{
    for(int i = 0; i < MAX_HASH_SIZE; i ++)
    {
        hashTable[i] = NULL;
    }

    scopeDepth = 0;
    scopeStack.push(COMMON);
}

void deleteType(Type type)
{
    //TODO:delete type
}
void deleteItem(TableItem *item)
{
    //TODO:delete item
}
SymbolTable::~SymbolTable()
{
    //free spaces in heap
    clearTable();
}

void SymbolTable::clearTable()
{
    //clear table
    for(int i = 0; i < MAX_HASH_SIZE; i ++)
    {
        TableItem *p = hashTable[i];
        while(p)
        {
            TableItem *tmp = p;
            p = p->next;
            deleteItem(p);
        }
    }
}

unsigned int hashPJW(const char *name)
{
	unsigned int val=0, i;
	for(; *name; ++name){
		val = (val<<2)+*name;
		if(i= val& ~0x3fff)
			val = (val^(i>>12))&0x3fff;
	}
	return val;
}
unsigned int SymbolTable::hash(const char *name) 
{
    return hashPJW(name);
}

/*
 * Add an item to the head of the list.
 * Set item->scope to current depth of table.
 */
void SymbolTable::addItem(TableItem *item)
{
    item->depth = this->scopeDepth;
    int hashValue = hash(item->name.c_str());
    item->next = hashTable[hashValue];
    hashTable[hashValue] = item;
}

void SymbolTable::addFieldList(FieldList fieldList)
{
    TableItem *item = new TableItem();
    item->name = fieldList->name;
    item->type = fieldList->type;
    item->next = NULL;
    this->addItem(item);
}

/*
 * Add a structure to the table.
 * Return a Type so there's no need to new a type elsewhere, 
 *  to make sure there's no Type_ or Structure_ that doesn't keep a pointer in this table.
 */
Type SymbolTable::addStructureAndGetType(Structure structure)
{
    TableItem *item = new TableItem();
    item->name = structure->name;
    item->type = new Type_();
    item->type->kind = STRUCTURE;
    item->type->u.structure = structure;
    item->type->assignType = LEFT;
    item->next = NULL;
    this->addItem(item);
    return item->type;
}

/*
 * Similar to the above.
 */ 
Type SymbolTable::addFunctionAndGetType(Function function)
{
    TableItem *item = new TableItem();
    item->name = function->name;
    item->type = new Type_();
    item->type->kind = FUNCTION;
    item->type->u.function = function;
    item->type->assignType = RIGHT;
    item->next = NULL;
    this->addItem(item);
    return item->type;
}


TableItem* SymbolTable::getItemByName(string name)
{
    int hashValue = hash(name.c_str());
    TableItem* p = hashTable[hashValue];
    while(p)
    {
        if(p->name.compare(name) == 0)
        {
            break;
        }
    }
    return p;
}

/*
 * Check if an item with the same name exist in the whole table.
 */
bool SymbolTable::isDuplicatedName(string name)
{
    return getItemByName(name) != NULL;
}
/*
 * Check if an item with the same name exist in table.
 * Return true if an item in current scope is found.
 * As nesting function definition is not supported, this methond should not be called when checking for a function name.
 */
bool SymbolTable::isDuplicatedNameInCurrentScope(string name)
{
    TableItem* item = getItemByName(name);
    return item != NULL && item->depth == this->scopeDepth;
}

/*
 * Change current depth of table.
 * Delete expired items when exit scope.
 * Function definition and declaration is not affected by scope.
 */
void SymbolTable::enterScope()
{
    this->scopeDepth ++;
    scopeStack.push(COMMON);
}
void SymbolTable::enterScope(ScopeType scopeType)
{
    this->scopeDepth ++;
    scopeStack.push(scopeType);
}
ScopeType SymbolTable::getScopeType()
{
    return scopeStack.top();
}

void SymbolTable::exitScope()
{
    if(this->scopeDepth <= 0)
    {
        return;
    }
    for(int i = 0; i < MAX_HASH_SIZE; i ++)
    {
        // delete items with depth of current scope
        TableItem *t = hashTable[i];
        while(t)
        {
            if(t->depth < this->scopeDepth)
            {
                break;
            }
            TableItem *tmp = t;
            t = t->next;
            delete tmp;
        }
        hashTable[i] = t;
    }
    this->scopeDepth --;
    scopeStack.pop();
}

bool SymbolTable::functionAllDefined()
{
   for(int i = 0; i < MAX_HASH_SIZE; i ++)
    {
        TableItem *t = hashTable[i];
        while(t)
        {
            if(t->type->kind == FUNCTION && !t->type->u.function->isDefined)
            {
                return false;
            }
            t = t->next;
        }
    } 
    return true;
}