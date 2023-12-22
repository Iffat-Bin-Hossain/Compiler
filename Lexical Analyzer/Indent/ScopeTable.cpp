#include <bits/stdc++.h>
#include "SymbolInfo.cpp"
using namespace std;

static unsigned long long sdbm(string str)
{
    unsigned long long hash = 0;

    for (int i = 0; i < str.length(); i++)
    {
        hash = str[i] + (hash << 6) + (hash << 16) - hash;
    }

    return hash;
}
class ScopeTable
{
private:
    SymbolInfo **scopeTable;
    string id;
    unsigned long long tableSize;
    ScopeTable *parentScope;
    int scopeNo;

public:
    ScopeTable(unsigned long long totalSize)
    {
        tableSize = totalSize;
        scopeTable = new SymbolInfo *[tableSize];
        for (unsigned long long i = 0; i < tableSize; ++i)
        {
            scopeTable[i] = NULL;
        }
        id = "";
        parentScope=NULL;
        scopeNo = 0;
    }
    void setParentScope(ScopeTable *ParentScope)
    {
        this->parentScope = ParentScope;
    }
    ScopeTable *getParentScope() { return this->parentScope; }
    void setScopeNo(int scopeNo) { this->scopeNo = scopeNo; }
    int getScopeNo() { return this->scopeNo; }
    void setId(string id)
    {
        this->id = id;
    }
    string getId()
    {
        return this->id;
    }
    unsigned long long hashFunction(SymbolInfo *sInfo)
    {
        return sdbm(sInfo->getName()) % tableSize;
    }
    bool Insert(SymbolInfo *sInfo)
    {
        unsigned long long index = hashFunction(sInfo);
        SymbolInfo *current = scopeTable[index];
        SymbolInfo *parent = NULL;
        while (current != NULL)
        {
            if (current->getName() == sInfo->getName() && current->getType() == sInfo->getType())
            {
                return false;
            }
            parent = current;
            current = current->getNext();
        }
        if (scopeTable[index] == NULL)
        {
            scopeTable[index] = sInfo;
        }
        else
        {
            parent->setNext(sInfo);
        }
        //cout << "\tInserted  at position <" << hashFunction(sInfo) + 1 << ", " << Position(sInfo) + 1 << "> of ScopeTable# " << id << endl;
        return true;
    }
    bool Delete(SymbolInfo *sInfo)
    {

        unsigned long long index = hashFunction(sInfo);
        SymbolInfo *current = scopeTable[index];
        SymbolInfo *previous = NULL;
        while (current != NULL && !(current->getName() == sInfo->getName()))
        {
            previous = current;
            current = current->getNext();
        }
        if (current == NULL)
        {
            return false;
        }
        else
        {
            //cout << "\tDeleted '" << sInfo->getName() << "' from position <" << hashFunction(sInfo) + 1 << ", " << Position(sInfo) + 1 << "> of ScopeTable# " << id << endl;
            if (previous == NULL)
            {
                scopeTable[index] = current->getNext();
            }
            else
            {
                previous->setNext(current->getNext());
            }
            delete current;
            return true;
        }
    }

    SymbolInfo *Lookup(SymbolInfo *sInfo)
    {
        unsigned long long index = hashFunction(sInfo);
        SymbolInfo *current = scopeTable[index];
        while (current != NULL)
        {
            if (current->getName() == sInfo->getName())
            {
                return current;
            }
            current = current->getNext();
        }
        return NULL;
    }
    unsigned long long Position(SymbolInfo *sInfo)
    {
        unsigned long long bucketNo = hashFunction(sInfo);
        unsigned long long chainIndex = 0;
        SymbolInfo *current = scopeTable[bucketNo];
        while (current != NULL)
        {
            if (current->getName() == sInfo->getName())
            {
                break;
            }
            current = current->getNext();
            chainIndex++;
        }
        return chainIndex;
    }
    void Print(ofstream& fout)
    {
        fout << "\tScopeTable# " << id << endl;
        for (unsigned long long i = 0; i < tableSize; i++)
        {
            fout << "\t";
            if (scopeTable[i] == NULL)
            {
                fout << i + 1 << endl;
            }
            else
            {
                fout << i + 1 << " --> ";
                SymbolInfo *current = scopeTable[i];
                while (current != NULL)
                {
                    if (current->getNext() == NULL)
                    {
                        fout << "(" << current->getName() << "," << current->getType() << ")" << endl;
                    }
                    else
                    {
                        fout << "(" << current->getName() << "," << current->getType() << ") --> ";
                    }
                    current = current->getNext();
                }
            }
        }
    }
    // ~ScopeTable()
    // {
    //     for (unsigned long long i = 0; i < tableSize; ++i)
    //     {
    //         SymbolInfo *current = scopeTable[i];
    //         while (current != NULL)
    //         {
    //             SymbolInfo *temp = current;
    //             current = current->getNext();
    //             delete temp;
    //         }
    //         scopeTable[i] = NULL;
    //     }
    //     delete[] scopeTable;
    // }
};
