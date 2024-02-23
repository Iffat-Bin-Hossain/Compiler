#include "2005087_Node.h"
#ifndef PARSETREE_H
#define PARSETREE_H
class ParseTree
{
private:
    Node *currentNode;

public:
    ParseTree()
    {
        currentNode = NULL;
    }
    void setCurrentNode(Node *node) { currentNode = node; }
    Node *getCurrentNode() { return currentNode; }
    void printParseTree(int level = 0)
    {
        Node *originalNode = currentNode; // Preserve the original currentNode

        for (int j = 0; j < level; j++)
        {
            cout << " ";
        }
        if (currentNode->getRule() != "")
        {
            cout << currentNode->getRule() << "\t";
            cout << "<Line: " << currentNode->getStartLine() << "-" << currentNode->getEndLine() << ">\n";
        }
        else
        {
            cout << currentNode->getType() << " : " << currentNode->getName() << "\t";
            cout << "<Line: " << currentNode->getStartLine() << ">\n";
        }

        for (Node *child : currentNode->getChildren())
        {
            currentNode = child;
            printParseTree( level + 1);
        }

        currentNode = originalNode;
    }
};
#endif
