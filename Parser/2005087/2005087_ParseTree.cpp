#include "2005087_Node.h"
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
    void addChildren(vector<Node *> childList)
    {
        for (Node *childNode : childList)
        {
            currentNode->getChildren().push_back(childNode);
        }
    }
    void printParseTree(ofstream &fout, int level = 0)
    {
        Node *originalNode = currentNode; // Preserve the original currentNode

        for (int j = 0; j < level; j++)
        {
            fout << " ";
        }
        if (currentNode->getRule() != "")
        {
            fout << currentNode->getRule() << "\t";
            fout << "<Line: " << currentNode->getStartLine() << "-" << currentNode->getEndLine() << ">\n";
        }
        else
        {
            fout << currentNode->getType() << " : " << currentNode->getName() << "\t";
            fout << "<Line: " << currentNode->getStartLine() << ">\n";
        }

        for (Node *child : currentNode->getChildren())
        {
            currentNode = child;
            printParseTree(fout, level + 1);
        }

        currentNode = originalNode;
    }
};
