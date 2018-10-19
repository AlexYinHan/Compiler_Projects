#ifndef _SYNTAXTREE_H
#define _SYNTAXTREE_H

#include <iostream>
#include <list>
#include <string>
using namespace std;

extern int yylineno;
enum NodeType{TERMINAL, NON_TERMINAL};
class Node 
{
private:

    NodeType nodeType;
    string nodeName;
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

    Node(NodeType nodeType, string nodeName, int lineno);
    void setValue(int value);
    void setValue(float value);
    int getIntValue();
    float getFloatValue();
    void addChild(Node *node);
    Node* getFather();
    static void deleteTree(Node *root);
    static void printTree(Node *root);
};

#endif