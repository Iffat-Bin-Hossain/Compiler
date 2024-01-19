#include <bits/stdc++.h>
#include "2005087_SymbolInfo.h"
class Node
{
private:
   SymbolInfo *symbol;
   string rule;
   string returnOrDataType; //return type for function,datatype for id
   vector<Node*> children;
   vector<pair<string,string>> parameterList;

public:
   Node(SymbolInfo *symbol, string rule = "", string returnOrDataType = "")
   {
      this->symbol = symbol;
      this->rule = rule;
      this->dataType = dataType;
   }
   string getName() { return symbol->getName();}
   string getType() { return symbol->getType();}
   string getRule() { return rule;}
   string getDataType() { return dataType;}
   string getReturnType() { return returnType;}
   vector<Node *> getChildren() { return children;}
   vector<pair<string,string>> getParameterList() { return parameterList;}
   void addParameter(pair<string,string> p){ parameterList.push_back(p);}
   void addParameter(vector<pair<string,string>> paramList){parameterList=paramList;}
   
};

