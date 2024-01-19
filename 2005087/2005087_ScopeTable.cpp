#include <bits/stdc++.h>
#include "2005087_Node.h"
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
    Node **scopeTable;
    string id;
    unsigned long long tableSize;
    ScopeTable *parentScope;
    int scopeNo;

public:
    ScopeTable(unsigned long long totalSize)
    {
        tableSize = totalSize;
        scopeTable = new Node *[tableSize];
        for (unsigned long long i = 0; i < tableSize; ++i)
        {
            scopeTable[i] = NULL;
        }
        id = "";
        parentScope = NULL;
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
    unsigned long long hashFunction(Node *node)
    {
        return sdbm(node->getName()) % tableSize;
    }
    bool Insert(Node *node)
    {
        unsigned long long index = hashFunction(node);
        Node *current = scopeTable[index];
        Node *parent = NULL;
        while (current != NULL)
        {
            if (current->getName() == node->getName() && current->getType() == node->getType())
            {
                return false;
            }
            parent = current;
            current = current->getNext();
        }
        if (scopeTable[index] == NULL)
        {
            scopeTable[index] = node;
        }
        else
        {
            parent->setNext(node);
        }
        return true;
    }
    bool Delete(Node *node)
    {

        unsigned long long index = hashFunction(node);
        Node *current = scopeTable[index];
        Node *previous = NULL;
        while (current != NULL && !(current->getName() == node->getName()))
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

    Node *Lookup(Node *node)
    {
        unsigned long long index = hashFunction(node);
        Node *current = scopeTable[index];
        while (current != NULL)
        {
            if (current->getName() == node->getName())
            {
                return current;
            }
            current = current->getNext();
        }
        return NULL;
    }
    unsigned long long Position(Node *node)
    {
        unsigned long long bucketNo = hashFunction(node);
        unsigned long long chainIndex = 0;
        Node *current = scopeTable[bucketNo];
        while (current != NULL)
        {
            if (current->getName() == node->getName())
            {
                break;
            }
            current = current->getNext();
            chainIndex++;
        }
        return chainIndex;
    }
    void Print(ofstream &fout)
    {
        fout << "\tScopeTable# " << id << endl;
        for (unsigned long long i = 0; i < tableSize; i++)
        {
            if (scopeTable[i] != NULL)
            {
                fout <<"\t"<< i + 1 << " --> ";
                Node *current = scopeTable[i];
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
   
};
