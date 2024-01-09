#include "2005087_ScopeTable.cpp"
class SymbolTable
{
private:
    unsigned long long bucketSize;
    ScopeTable *currentScope;

public:
    SymbolTable(unsigned long long bucketSize)
    {
        this->bucketSize = bucketSize;
        currentScope = new ScopeTable(bucketSize);
        currentScope->setId("1");

    }
    ScopeTable *getCurrentScope() { return currentScope; }
    void EnterScope()
    {
        ScopeTable *newScope = new ScopeTable(bucketSize);
        newScope->setParentScope(currentScope);
        currentScope->setScopeNo(currentScope->getScopeNo() + 1);
        newScope->setId(currentScope->getId() + "." + to_string(currentScope->getScopeNo()));
        currentScope = newScope;

    }
    void ExitScope()
    {
        if (currentScope->getId() != "1")
        {
            ScopeTable *temp = currentScope->getParentScope();
            delete currentScope;
            currentScope = temp;
        }
    }
    bool Insert(SymbolInfo *sInfo)
    {
        return currentScope->Insert(sInfo);
    }
    bool Remove(SymbolInfo *sInfo)
    {
        return currentScope->Delete(sInfo);
    }
    SymbolInfo *Lookup(SymbolInfo *sInfo)
    {
        ScopeTable *current = currentScope;
        while (current != NULL)
        {
            SymbolInfo *searched = current->Lookup(sInfo);
            if (searched != NULL)
            {
                cout << "\t'" << sInfo->getName() << "' found at position <" << current->hashFunction(sInfo) + 1 << ", " << current->Position(sInfo) + 1 << "> of ScopeTable# " << current->getId() << endl;
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
