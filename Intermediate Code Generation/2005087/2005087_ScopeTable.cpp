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
    int id;
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
        id = 0;
        parentScope = NULL;
        scopeNo = 0;
    }
    void setParentScope(ScopeTable *ParentScope)
    {
        this->parentScope = ParentScope;
    }
    ScopeTable *getParentScope() { return this->parentScope; }
    int getScopeNo() { return this->scopeNo; }
    void setId(int id)
    {
        this->id = id;
    }
    int getId()
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
    void Print()
    {
        cout << "\tScopeTable# " << id << endl;
        for (unsigned long long i = 0; i < tableSize; i++)
        {
            if (scopeTable[i] != NULL)
            {
                cout << "\t" << i + 1 << " --> ";
                Node *current = scopeTable[i];
                while (current != NULL)
                {

                    if (current->getType() == "FUNCTION")
                    {
                        if (current->getNext() == NULL)
                        {
                            cout << "<" << current->getName() << "," << current->getType() << "," << current->getReturnOrDataType() << ">" << endl;
                        }
                        else
                        {
                            cout << "<" << current->getName() << "," << current->getType() << "," << current->getReturnOrDataType() << "> ";
                        }
                    }
                    else if (current->getType() == "ARRAY")
                    {

                        if (current->getNext() == NULL)
                        {
                            cout << "<" << current->getName() << "," << current->getType() << ">" << endl;
                        }
                        else
                        {
                            cout << "<" << current->getName() << "," << current->getType() << "> ";
                        }
                    }
                    else
                    {

                        if (current->getNext() == NULL)
                        {
                            cout << "<" << current->getName() << "," << current->getReturnOrDataType() << ">" << endl;
                        }
                        else
                        {
                            cout << "<" << current->getName() << "," << current->getReturnOrDataType() << "> ";
                        }
                    }

                    current = current->getNext();
                }
            }
        }
    }
};
