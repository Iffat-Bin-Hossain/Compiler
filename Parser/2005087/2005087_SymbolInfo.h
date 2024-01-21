#include <bits/stdc++.h>
using namespace std;
#ifndef SYMBOL_INFO_H
#define SYMBOL_INFO_H
class SymbolInfo
{
private:
    string name, type;

public:
    SymbolInfo()
    {
    }
    SymbolInfo(string name, string type)
    {
        this->name = name;
        this->type = type;
    }
    void setName(string name)
    {
        this->name = name;
    }
    string getName()
    {
        return name;
    }
    void setType(string type)
    {
        this->type = type;
    }
    string getType()
    {
        return type;
    }
};
#endif