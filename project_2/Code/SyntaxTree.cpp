#include "SyntaxTree.h"

/*
 * Constructor.
 * @param value
 *  For non-terminals, set to " ", because the value field is meaningless.
 *  For a termianl symbol, value is the actual text of it.
 */
Node::Node(NodeType nodeType, string nodeName, int lineno)
{
    this->nodeType = nodeType;
    this->nodeName = nodeName;
    this->lineno = lineno;
    this->value = " "; // set default value;
    this->father = NULL;
}

Node::Node(NodeType nodeType, string nodeName, string value, int lineno)
{
    this->nodeType = nodeType;
    this->nodeName = nodeName;
    this->lineno = lineno;
    this->value = value;
    this->father = NULL;
}

/*
 * Destructor.
 * Do not free 'father' pointer. 
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
    int int_value = 0;
    stringstream ss;
    switch(this->nodeType)
    {
        case NODE_TYPE_DEC_INT:
            ss << this->value;
            ss >> int_value;
            break;
        case NODE_TYPE_OCT_INT:
            ss << std::oct << this->value;
            ss >> std::oct >> int_value;
            break;
        case NODE_TYPE_HEX_INT:
            ss << std::hex << this->value;
            ss >> std::hex >> int_value;
            break;
        default: ;
    }
    return int_value;
    // return atoi(this->value.c_str());
}
float Node::getFloatValue()
{
    return atof(this->value.c_str());
}
string Node::getValue()
{
    return this->value;
}

/*
 * Add a child to this node, and let the child node's father point to this node
 * Note: 
 *  If a NULL node is added as child, this should only be the case where an empty string is detected in some grammar rule.
 *  As it is useless in syntax tree(at least for now), just return; 
 */
void Node::addChild(Node *node)
{
    
    this->children.push_back(node);
    if(node != NULL)
    {
        node->setFather(this);
    }
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

Node* Node::getChild(int index)
{
    // cout << this->nodeName << index << "," << this->children.size() << endl;
    try
    {
        return this->children.at(index);
    }
    catch(exception& e)
    {
        return NULL;
    }
}

/**
 * Delete the whole sub tree
 * which means delete/free this node, along with all its descendants
 */
void Node::deleteTree(Node*root)
{
    if(root == NULL) 
    {
        return;
    }

    for (vector<Node*>::iterator it = root->children.begin(); it != root->children.end();)
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
    // printf("%s (%d)\n", p->nodeName.c_str(), p->lineno);
    stringstream ss;
    int int_value;
    float float_value;
    switch(p->nodeType)
    {
        case NODE_TYPE_NON_TERMINAL:
        case NODE_TYPE_RELOP:
            printf("%s (%d)\n", p->nodeName.c_str(), p->lineno);
            break;
        case NODE_TYPE_DEC_INT:
            ss << p->value;
            ss >> int_value;
            printf("%s: %d\n", p->nodeName.c_str(), int_value);
            break;
        case NODE_TYPE_OCT_INT:
            ss << std::oct << p->value;
            ss >> std::oct >> int_value;
            printf("%s: %d\n", p->nodeName.c_str(), int_value);
            break;
        case NODE_TYPE_HEX_INT:
            ss << std::hex << p->value;
            ss >> std::hex >> int_value;
            printf("%s: %d\n", p->nodeName.c_str(), int_value);
            break;
        case NODE_TYPE_FLOAT:
            ss << p->value;
            ss >> float_value;
            printf("%s: %f\n", p->nodeName.c_str(), float_value);
            break;
        case NODE_TYPE_ID:
        case NODE_TYPE_KEYWORD:
            printf("%s: %s\n", p->nodeName.c_str(), p->value.c_str());
            break;
        case NODE_TYPE_BRACKET:
        case NODE_TYPE_PUNCTUATION:
        case NODE_TYPE_OPERATOR:
            printf("%s\n", p->nodeName.c_str());
            break;
        default:
            printf("%s (%d)\n", p->nodeName.c_str(), p->lineno);
            break;
    }

    for (vector<Node*>::iterator it = p->children.begin(); it != p->children.end(); it ++)
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
    

