#include <bits/stdc++.h>
using namespace std;
class SymbolInfo
{
private:
    string name, type;
    SymbolInfo *next;

public:
    SymbolInfo()
    {
    }
    SymbolInfo(string name, string type)
    {
        this->name = name;
        this->type = type;
        this->next = NULL;
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
    void setNext(SymbolInfo *next)
    {
        this->next = next;
    }
    SymbolInfo *getNext()
    {
        return next;
    }
    ~SymbolInfo(){
        delete this->next;
    }
};
