#ifndef _SYNTAXTREE_H
#define _SYNTAXTREE_H

#include "common.h"
#include <list>
#include <vector>
#include <sstream>
#include <exception>

extern int yylineno;

enum NodeType
{
    /*
     * Imply type of a node in syntax tree
     * Use "NODE_TYPE_" prefix make different to tokens declared in syntax.y
     */
    NODE_TYPE_KEYWORD, // reserved words
    NODE_TYPE_RELOP, NODE_TYPE_BRACKET, NODE_TYPE_PUNCTUATION, NODE_TYPE_OPERATOR, // symbols
    NODE_TYPE_ID, NODE_TYPE_DEC_INT, NODE_TYPE_OCT_INT, NODE_TYPE_HEX_INT, NODE_TYPE_FLOAT, // identifiers and constants
    NODE_TYPE_NON_TERMINAL
};

class Node 
{
private:

    NodeType nodeType;
    string nodeName;
    string value;   //attribute value
    int lineno;
    int productionNo;   //start from 0

    vector<Node*> children;
    Node *father;

    void setFather(Node *node);
    void printNode();
public:

    Node(NodeType nodeType, string nodeName, int lineno);
    Node(NodeType nodeType, string nodeName, string value, int lineno);
    ~Node();
    void setValue(int value);
    void setValue(float value);
    void setValue(string value);
    NodeType getNodeType(){ return this->nodeType;}
    string getNodeName(){ return this->nodeName;}
    int getIntValue();
    float getFloatValue();
    string getValue();
    string getText();
    int getLineno(){ return this->lineno;}
    void setProductionNo(int no){ this->productionNo = no;}
    int getProductionNo(){ return this->productionNo;}
    void addChild(Node *node);
    Node* getFather();
    Node* getChild(int index);
    static void deleteTree(Node *root);
    static void printTree(Node *root, int depth);
    static void printTree(Node *root);
};

#endif