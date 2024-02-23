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
   int startLine;
   int endLine;
   int stackOffset = -1; //-1 means global variable
   bool compareOffset = true; // If previousOffset==currentOffset
   int offsetDifference; // Previous offset ~ current Offset
   bool isEvaluated = false; // Is a expression evaluated or not
   vector<int> trueList, falseList, nextList;

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
   void setStartLine(int startLine) { this->startLine = startLine; }
   int getStartLine() { return startLine; }
   void setEndLine(int endLine) { this->endLine = endLine; }
   int getEndLine() { return endLine; }
   void setStackOffset(int stackOffset) { this->stackOffset = stackOffset; }
   int getStackOffset() { return stackOffset; }
   void setCompareOffset(bool compareOffset) { this->compareOffset = compareOffset; }
   bool getCompareOffset() { return compareOffset; }
   void setOffsetDifference(int offsetDifference) { this->offsetDifference = offsetDifference; }
   int getOffsetDifference() { return offsetDifference; }
   void setIsEvaluated(bool isEvaluated) { this->isEvaluated = isEvaluated; }
   bool getIsEvaluated() { return isEvaluated; }
   void setTrueList(vector<int> trueList) { this->trueList = trueList; }
   void setFalseList(vector<int> falseList) { this->falseList = falseList; }
   void setNextList(vector<int> nextList) { this->nextList = nextList; }
   vector<int> getTrueList() { return trueList; }
   vector<int> getFalseList() { return falseList; }
   vector<int> getNextList() { return nextList; }
   void makeChild(vector<Node *> childrenList)
   {
      for (auto child : childrenList)
      {
         children.push_back(child);
      }
   }
};
#endif