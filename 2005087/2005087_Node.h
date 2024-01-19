#include <bits/stdc++.h>
#include "2005087_SymbolInfo.h"



#ifndef NODE_H
#define NODE_H
class Node
{
private:
   SymbolInfo *symbol;
   string rule;
   string returnOrDataType; // return type for function,datatype for id
   vector<Node *> children;
   Node *next;
   vector<pair<string, string>> parameterList;
   string functionInfo;//Is it a declaration or a definition
   bool arrayStatus;

public:
   Node(SymbolInfo *symbol, string rule = "", string returnOrDataType = "")
   {
      this->symbol = symbol;
      next = NULL;
      this->rule = rule;
      this->returnOrDataType = returnOrDataType;
   }
   void setName(string name){symbol->setName(name);}
   string getName() { return symbol->getName(); }
   void setType(string type){symbol->setType(type);}
   string getType() { return symbol->getType(); }
   void setNext(Node *next){this->next = next;}
   Node *getNext(){return next;}
   string setRule(string rule){this->rule = rule;}
   string getRule() { return rule; }
   void setReturnOrDataType(string returnOrDataType) {this->returnOrDataType = returnOrDataType;}
   string getReturnOrDataType() { return returnOrDataType; }
   void setChildren(vector<Node *> children){this->children=children;}
   vector<Node *> getChildren() { return children; }
   vector<pair<string, string>> getParameterList() { return parameterList; }
   void addParameter(pair<string, string> p) { parameterList.push_back(p); }
   void addParameter(vector<pair<string, string>> paramList) { parameterList = paramList; }
   void setFunctionInfo(string info){this->functionInfo = info;}  
   string getFunctionInfo(){return this->functionInfo;}
   void setArrayStatus(bool status){arrayStatus = status;}
   bool getArrayStatus(){return arrayStatus;}
};
#endif
