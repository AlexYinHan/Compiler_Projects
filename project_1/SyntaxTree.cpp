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

/*
 * Do not delete father. 
 */
Node::~Node()
{
    
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

/*
 * Add a child to this node, and let the child node's father point to this node
 * Note: 
 *  If a NULL node is added as child, this should only be the case where an empty string is detected in some grammar rule.
 *  As it is useless in syntax tree(at least for now), just return; 
 */
void Node::addChild(Node *node)
{
    if(node == NULL)
    {
        return;
    }
    this->children.push_back(node);
    node->setFather(this);
}

/*
 * Let the father of this node point to a certain node.
 * As a node's father would only be set when it's added to some node's children list
 * this function should be private, and only be called in addChild()  
 */
void Node::setFather(Node *node)
{
    node->father = node;
}
Node* Node::getFather()
{
    return this->father;
}

/**
 * Delete the whole sub tree
 * which means delete/free this node, along with all its descendants
 */
void Node::deleteTree(Node*root)
{
    if(root == NULL) 
    {
        // just in case, shouldn't really reach here.
        return;
    }

    for (list<Node*>::iterator it = root->children.begin(); it != root->children.end();)
    {
        Node *p = *it;
        it ++;
        deleteTree(p);
    }
    delete root;
}

/*
 * Print node info for debugging.
 */
void Node::printNode()
{
    printf("%s (%d)\n", this->nodeName.c_str(), this->lineno);
}
/**
 * Traverse the sub tree and print it, only for project_1 requirements.
 * @param depth
 *  Indicates the depth of the current node and how much indent should be put before it.  
 */
void Node::printTree(Node*p, int depth)
{
    if(p == NULL)
    {
        return;
    }

    for(int i =0; i < depth;i ++)
    {
        printf("  ");   // two space
    }
    printf("%s (%d)\n", p->nodeName.c_str(), p->lineno);
    // switch(p->nodeType)
    // {
    //     case NODE_TYPE_NON_TERMINAL:
    //         printf("%s (%d)\n", p->nodeName.c_str(), p->lineno);
    //     default:
    //         break;
    // }

    for (list<Node*>::iterator it = p->children.begin(); it != p->children.end(); it ++)
    {
        printTree(*it, depth + 1);
    }
}

/**
 * Traverse the whole tree and print it, only for project_1 requirements.
 */
void Node::printTree(Node*root)
{
    printTree(root, 0);
}
    

