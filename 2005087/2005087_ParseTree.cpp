#include "2005087_Node.h"
class ParseTree{
   private:
   Node* currentNode;
   public:
   ParseTree(){
    currentNode = NULL;
   }
   void setCurrentNode(Node* node) { currentNode =node; }
   Node* getCurrentNode() { return currentNode;}
   void addChildren(list<Node*> childList) {
    for (Node* childNode : childList) {
        currentNode->getChildren().push_back(childNode);
    }
}
};
