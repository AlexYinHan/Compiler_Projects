#ifndef _SYNTAXTREE_H
#define _SYNTAXTREE_H

#include <iostream>
#include <list>
#include <string>
using namespace std;

extern int yylineno;

enum NodeType
{
    /*
     * Imply type of a node in syntax tree
     * Use "NODE_TYPE_" prefix make different to tokens declared in syntax.y
     */
    NODE_TYPE_KEYWORD, // reserved words
    NODE_TYPE_RELOP, NODE_TYPE_BRACKET, NODE_TYPE_PUNCTUATION, NODE_TYPE_OPERATOR, // symbols
    NODE_TYPE_ID, NODE_TYPE_INT, NODE_TYPE_FLOAT, // identifiers and constants
    NODE_TYPE_NON_TERMINAL
};

class Node 
{
private:

    NodeType nodeType;
    string nodeName;
    string value;   //attribute value
    int lineno;

    list<Node*> children;
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
    int getIntValue();
    float getFloatValue();
    void addChild(Node *node);
    Node* getFather();
    static void deleteTree(Node *root);
    static void printTree(Node *root, int depth);
    static void printTree(Node *root);
};

#endif