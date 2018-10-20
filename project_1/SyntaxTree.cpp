#include "SyntaxTree.h"

Node::Node(NodeType nodeType, string nodeName, int lineno)
{
    this->nodeType = nodeType;
    this->nodeName = nodeName;
    this->lineno = lineno;
    this->value = " "; // set default value;
}

Node::Node(NodeType nodeType, string nodeName, string value, int lineno)
{
    this->nodeType = nodeType;
    this->nodeName = nodeName;
    this->lineno = lineno;
    this->value = value; // set default value;
}

void Node::setValue(int value)
{
    this->value = to_string(value);
}
void Node::setValue(float value)
{
    this->value = to_string(value);
}
void Node::setValue(string value)
{
    this->value = value;
}
int Node::getIntValue()
{
    return atoi(this->value.c_str());
}
float Node::getFloatValue()
{
    return atof(this->value.c_str());
}
void Node::addChild(Node *node)
{
    this->children.push_back(node);
}
void Node::setFather(Node *node)
{
    // as a node's father would only be set when it's added to some node's children list
    // this function should be private, and only be called in addChild()
    node->father = node;
}
Node* Node::getFather()
{
    return this->father;
}
void Node::deleteTree(Node*root)
{
    /**
     * delete the whole sub tree
     * which means delete/free this node, along with all its descendants
    */
    for (list<Node*>::iterator it = root->children.begin(); it != root->children.end();)
    {
        Node *p = *it;
        it ++;
        deleteTree(p);
    }
    delete root;
}
    

