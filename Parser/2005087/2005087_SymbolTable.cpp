#include "2005087_ScopeTable.cpp"
class SymbolTable
{
private:
    unsigned long long bucketSize;
    ScopeTable *currentScope;
    int globalId = 1;

public:
    SymbolTable(unsigned long long bucketSize)
    {
        this->bucketSize = bucketSize;
        currentScope = new ScopeTable(bucketSize);
        currentScope->setId(globalId);
    }
    ScopeTable *getCurrentScope() { return currentScope; }
    void EnterScope()
    {
        ScopeTable *newScope = new ScopeTable(bucketSize);
        newScope->setParentScope(currentScope);
        newScope->setId(++globalId);
        currentScope = newScope;
    }
    void ExitScope()
    {
        if (currentScope->getId() != 1)
        {
            ScopeTable *temp = currentScope->getParentScope();
            delete currentScope;
            currentScope = temp;
        }
    }
    bool Insert(Node *node)
    {
        return currentScope->Insert(node);
    }
    bool Remove(Node *node)
    {
        return currentScope->Delete(node);
    }
    Node *LookupCurrent(Node *node)
    {
        ScopeTable *current = currentScope;
        Node *searched = current->Lookup(node);
        if (searched != NULL)
        {
            return searched;
        }
        return NULL;
    }
    Node *Lookup(Node *node)
    {
        ScopeTable *current = currentScope;
        while (current != NULL)
        {
            Node *searched = current->Lookup(node);
            if (searched != NULL)
            {
                return searched;
            }
            current = current->getParentScope();
        }
        return NULL;
    }

    void PrintCurrentScope(ofstream &fout)
    {
        currentScope->Print(fout);
    }
    void PrintAllScope(ofstream &fout)
    {
        ScopeTable *current = currentScope;
        while (current != NULL)
        {
            current->Print(fout);
            current = current->getParentScope();
        }
    }
    ~SymbolTable()
    {
        while (currentScope != NULL)
        {
            ScopeTable *parent = currentScope->getParentScope();
            ExitScope();
            currentScope = parent;
        }
    }
};
