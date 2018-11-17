#include "SymbolTable.h"

SymbolTable::SymbolTable()
{
    for(int i = 0; i < MAX_HASH_SIZE; i ++)
    {
        hashTable[i] = NULL;
    }
}

SymbolTable::~SymbolTable()
{
    //TODO:return space
}

void SymbolTable::clearTable()
{
    //TODO:clear table
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
 * Add an item to the head of the list
 */
void SymbolTable::addItem(TableItem *item)
{
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

bool SymbolTable::isDuplicatedName(string name)
{
    return getItemByName(name) != NULL;
}