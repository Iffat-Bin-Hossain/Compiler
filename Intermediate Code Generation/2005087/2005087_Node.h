#include <bits/stdc++.h>
#include "2005087_SymbolInfo.h"

#ifndef NODE_H
#define NODE_H
class Node
{
private:
   SymbolInfo *symbol;
   string rule;
   string returnOrDataType; // Return type for function,datatype for id
   vector<Node *> children;
   Node *next;
   vector<pair<pair<string, string>, bool>> parameterList;
   string functionInfo; // Is it a declaration or a definition
   bool arrayStatus;    // Is it an array or not
   bool isGlobal; // Is it a global or not
   int startLine;
   int endLine;
   int size=1; // For single variable size=1,for an Array size=array_size
   int stackOffset;

public:
   Node(SymbolInfo *symbol, string rule = "", string returnOrDataType = "")
       : symbol(symbol), rule(rule), returnOrDataType(returnOrDataType), next(NULL), functionInfo(""), arrayStatus(false)
   {
   }
   void setName(string name) { symbol->setName(name); }
   string getName() { return symbol->getName(); }
   void setType(string type) { symbol->setType(type); }
   string getType() { return symbol->getType(); }
   void setNext(Node *next) { this->next = next; }
   Node *getNext() { return next; }
   void setRule(string rule) { this->rule = rule; }
   string getRule() { return rule; }
   void setReturnOrDataType(string returnOrDataType) { this->returnOrDataType = returnOrDataType; }
   string getReturnOrDataType() { return returnOrDataType; }
   void setChildren(vector<Node *> children) { this->children = children; }
   vector<Node *> getChildren() { return children; }
   vector<pair<pair<string, string>, bool>> getParameterList() { return parameterList; }
   void addParameter(pair<pair<string, string>, bool> p) { parameterList.push_back(p); }
   void addParameter(vector<pair<pair<string, string>, bool>> paramList) { parameterList = paramList; }
   void setFunctionInfo(string info) { this->functionInfo = info; }
   string getFunctionInfo() { return this->functionInfo; }
   void setArrayStatus(bool status)
   {
      arrayStatus = status;
      if (arrayStatus)
      {
         this->setType("ARRAY");
      }
   }
   bool getArrayStatus() { return arrayStatus; }
   void setIsGlobal(bool isGlobal) { this->isGlobal = isGlobal; }
   bool getIsGlobal() { return isGlobal; }
   void setStartLine(int startLine) { this->startLine = startLine; }
   int getStartLine() { return startLine; }
   void setEndLine(int endLine) { this->endLine = endLine; }
   int getEndLine() { return endLine; }
   void setSize(int size) { this->size = size; }
   int getSize() { return size;}
   void setStackOffset(int stackOffset) { this->stackOffset = stackOffset; }
   int getStackOffset(){return stackOffset;}
   void makeChild(vector<Node *> childrenList)
   {
      for (auto child : childrenList)
      {
         children.push_back(child);
      }
   }
};
#endif