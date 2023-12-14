#include <bits/stdc++.h>
#include "2005087_SymbolTable.cpp"
using namespace std;
int main()
{
    freopen("2005087_input.txt", "r", stdin);
    freopen("2005087_output.txt", "w", stdout);

    unsigned long long bucketSize;

    cin >> bucketSize;
    SymbolTable table(bucketSize);
    string line;
    int cmdNo = 0;
    getline(cin, line);
    while (getline(cin, line))
    {
        cout << "Cmd " << ++cmdNo << ": " << line << endl;
        char operation;
        stringstream ss(line);
        ss >> operation;
        if (operation == 'I')
        {
            string name, type,extra;
            ss >> name >> type>>extra;
            SymbolInfo *sInfo = new SymbolInfo(name, type);
            if (name == "" || type == ""|| extra!="")
            {
                cout << "\tWrong number of arguments for the command I" << endl;
            }
            else
            {
                if (!table.Insert(sInfo))
                {
                    cout << "\t'" << name << "' already exists in the current ScopeTable# " << table.getCurrentScope()->getId() << endl;
                }
            }
        }
       else if (operation == 'S')
       {
           table.EnterScope();
       }
       else if (operation == 'E')
       {
           if (table.getCurrentScope()->getId().length() == 1)
           {
               cout << "\tScopeTable# " << table.getCurrentScope()->getId() << " cannot be deleted" << endl;
           }
           else
           {
               table.ExitScope();
           }
       }
        else if (operation == 'P')
        {
            char attribute;
            ss >> attribute;
            if (attribute == 'A')
            {
                table.PrintAllScope();
            }
            else if (attribute == 'C')
            {
                table.PrintCurrentScope();
            }
            else
            {
                cout << "\tInvalid argument for the command P" << endl;
            }
        }
        else if (operation == 'D')
        {
            string element1, element2;
            ss >> element1 >> element2;
            if (element1 != "" && element2 == "")
            {
                SymbolInfo *toBeDeleted = new SymbolInfo(element1, "");
                unsigned long long bucketNo = table.getCurrentScope()->hashFunction(toBeDeleted);
                int chainIndex = table.getCurrentScope()->Position(toBeDeleted);
                if (!table.Remove(toBeDeleted))
                {
                    cout << "\tNot found in the current ScopeTable# " << table.getCurrentScope()->getId() << endl;
                }
            }
            else
            {
                cout << "\tWrong number of arguments for the command D" << endl;
            }
        }
       else if (operation == 'L')
       {
           string element1, element2;
           ss >> element1 >> element2;
           if ( element1!="" && element2 == "")
           {
               SymbolInfo *searched = new SymbolInfo(element1, "");
               SymbolInfo *returned = table.Lookup(searched);
               if (returned == NULL)
               {
                   cout << "\t'" << element1 << "' not found in any of the ScopeTables" << endl;
               }
           }
           else
           {
               cout << "\tWrong number of arguments for the command L" << endl;
           }
       }
        else if (operation == 'Q')
        {
            //  ScopeTable *current = table.getCurrentScope();
            //  while (current != NULL)
            //  {
            //      ScopeTable *parent = table.getCurrentScope()->getParentScope();
            //      table.ExitScope();
            //      current = parent;
            //  }
            break;
        }
    }
}
