#include<bits/stdc++.h>
#include "2005087_SymbolInfo.h"
class Node
{
private:
   SymbolInfo *symbol;
   string dataType;
   vector<Node *> children;

public:
   Node(SymbolInfo *symbol, string dataType = "")
   {
      this->symbol = symbol;
      this->dataType = dataType;
   }
   string getName() { return symbol->getName(); }
   string getType() { return symbol->getType(); }
   string getDataType() { return dataType; }
   vector<Node *> getChildren(){ return children;}

};
