#ifndef _SYNTAXTREE_H
#define _SYNTAXTREE_H

#include <iostream>
#include <list>
using namespace std;

extern int yylineno;
enum NodeType{TERMINAL, NON_TERMINAL};
enum NodeName{EXP, TYPE, ID};
class Node 
{
private:

    NodeType nodeType;
    NodeName nodeName;
    int lineno;
    /*attribute value of numbers*/
    union   
    {
        int int_value;
        float float_value;
    };
    list<Node*> children;
    Node *father;

    void setFather(Node *node);
public:

    Node(NodeType nodeType, NodeName nodeName, int lineno);
    void setValue(int value);
    void setValue(float value);
    int getIntValue();
    float getFloatValue();
    void addChild(Node *node);
    Node* getFather();
    static void deleteTree(Node *root);
};
#endif