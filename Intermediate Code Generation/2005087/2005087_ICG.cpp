#include <bits/stdc++.h>
#include "2005087_ParseTree.h"
using namespace std;
extern vector<Node *> globals;
extern SymbolTable sTable;
class ICG
{
    std::ofstream icgFout;
    
    int lineNo;
    int offset, popCount, returnLabel;
    int label;
    

public:
    ICG()
    {
        icgFout.open("code.asm");
    }
    ofstream &geticgFout()
    {
        return icgFout;
    }
    void initializeMemorySegment()
    {
        icgFout << ".model small\n.stack 1000h" << endl;
    }
    void initializeDataSegment()
    {
        icgFout << ".Data\n\tnumber DB \"00000$\"\n";
    }
    void initializeCodeSegment()
    {
        icgFout << ".code" << endl;
        PrintProcedure();
        NewlineProcedure();
    }

    void PrintProcedure()
    {
        string print = "Print proc\n\t; Preserve AX,BX,CX,DX register\n\tPUSH ax\n\tPUSH bx\n\tPUSH cx\n\tPUSH dx\n\t; quotient in ax, remainder in dx\n\t; divisor in cx=10\n\t; the length of the number in bl\n\t; the sign bit in bh(1 for negative number)\n\tMOV cx, 10\n\tXOR bl, bl\n\tCMP ax, 0\n\tJGE stackPush   ; for positive number\n\tMOV bh, 1\n\tNEG ax          ; convert to positive\nstackPush:\n\t; This procedure will continue untill ax=0\n\tXOR dx, dx\n\tdiv cx\n\tpush dx         ; Push remainder in stack\n\tINC bl          ; Increment length\n\tCMP ax, 0\n\tJG stackPush    ; continue division\n\tMOV ah, 02      ; display\n\tCMP bh, 1       ; Check if number is negative\n\tJNE stackPop    ; if not negative,start printing\n\tMOV dl, '-'     ; If negative, print a '-' sign first\n\tint 21h\nstackPop:\n\tPOP dx          ; Pop remainder from stack\n\tXOR dh, dh\n\tADD dl, '0'     ; Convert remainder to ASCII\n\tint 21h\n\tDEC bl          ; Decrement length\n\tCMP bl, 0\n\tJG stackPop     ; more to print\n\t; Cleanup AX,BX,CX,DX registers\n\tPOP dx\n\tPOP cx\n\tPOP bx\n\tPOP ax\n\tRET\nPrint endp";
        icgFout << print << endl;
    }
    void NewlineProcedure()
    {
        string newline = "Newline proc\n\tPUSH AX\n\tPUSH DX\n\tMOV AH, 02\n\tMOV DL, 0DH\n\tint 21H\n\tMOV DL, 0AH\n\tint 21H\n\tPOP DX\n\tPOP AX\n\tRET\nNewline ENDP";
        icgFout << newline << endl;
    }

    void print(string id)
    {
        icgFout << "\tPUSH ax\n\tMOV ax," + id + "\n\tCALL Print\n\tCALL Newline\n\tPOP ax" << endl;
    }
    void allocatePlaceForArguments(Node *node, int offset)
    {
        for (Node *child : node->getChildren())
        {
            if (child->getType() == "parameter_list")
            {
                allocatePlaceForArguments(child, offset + 2);
            }
            else if (child->getType() == "ID")
            {
                child->setStackOffset(offset);
            }
        }
    }
    void performArithmeticOperations(Node *node)
    {

        traverse(node->getChildren().at(2));
        icgFout << "\tPUSH AX\n";
        traverse(node->getChildren().at(0));
        icgFout << "\tPOP CX\n";
        if (node->getChildren().at(1)->getName() == "+")
        {
            icgFout << "\tADD AX, CX\n";
        }
        else if (node->getChildren().at(1)->getName() == "-")
        {
            icgFout << "\tSUB AX, CX\n";
        }
        else if (node->getChildren().at(1)->getName() == "*")
        {
            icgFout << "\tCWD\n\tMUL CX\n";
        }
        else if (node->getChildren().at(1)->getName() == "/")
        {
            icgFout << "\tCWD\n\tDIV CX\n";
        }
        else if (node->getChildren().at(1)->getName() == "%")
        {
            icgFout << "\tCWD\n\tDIV CX\n\tMOV AX, DX\n";
        }
    }
    void performRelationalOperations(Node *node)
    {
        string Operator = node->getChildren().at(1)->getName();
        traverse(node->getChildren().at(0));
        icgFout << "\tPUSH AX\n";
        traverse(node->getChildren().at(2));
        icgFout << "\tPOP DX\n\tCMP DX,AX\n";
        if (Operator == "<")
            icgFout << "\tJL ";
        else if (Operator == "<=")
            icgFout << "\tJLE ";
        else if (Operator == ">")
            icgFout << "\tJG ";
        else if (Operator == ">=")
            icgFout << "\tJGE ";
        else if (Operator == "==")
            icgFout << "\tJE ";
        else if (Operator == "!=")
            icgFout << "\tJNE ";
        int truelabel, falselabel;
        truelabel = ++label;
        icgFout << "L" << truelabel << "\n";
        falselabel = ++label;
        icgFout << "\tJMP L" << falselabel << "\n";
        icgFout << "L" << truelabel << ":\n";
        icgFout << "\tMOV AX, 1\n\tJMP L" << ++label << "\n";
        icgFout << "L" << falselabel << ":\n";
        icgFout << "\tMOV AX, 0\n";
        icgFout << "L" << label << ":\n";
    }
    void performLogicalOperations(Node *node)
    {
        bool isOr = node->getChildren().at(1)->getName() == "||";
        int intermediateLabel, finalLabel;
        traverse(node->getChildren().at(0));
        icgFout << "\tCMP AX,0" << endl;
        intermediateLabel = ++label;
        if (isOr)
        {
            icgFout << "\tJNE L" << intermediateLabel << endl;
        }
        else
        {
            icgFout << "\tJE L" << intermediateLabel << endl;
        }
        traverse(node->getChildren().at(2));
        icgFout << "\tCMP AX,0" << endl;
        finalLabel = ++label;
        if (isOr)
        {
            icgFout << "\tJNE L" << intermediateLabel << endl;
            icgFout << "\tMOV AX,0\n\tJMP L" << finalLabel << endl;
            icgFout << "L" << intermediateLabel << ":\n";
            icgFout << "\tMOV AX,1\nL" << finalLabel << ":" << endl;
        }
        else
        {
            icgFout << "\tJE L" << intermediateLabel << endl;
            icgFout << "\tMOV AX,1\n\tJMP L" << finalLabel << endl;
            icgFout << "L" << intermediateLabel << ":\n";
            icgFout << "\tMOV AX,0\nL" << finalLabel << ":" << endl;
        }
    }
    void performAssignmentOperations(Node *node)
    {
        Node *variable = node->getChildren().at(0);
        if (node->getType() == "ARRAY_VARIABLE")
        {
            icgFout << "\tPUSH AX\n";
            traverse(node->getChildren().at(2));
            icgFout << "\tMOV BX, AX\n\tMOV AX, 2\n\tMUL BX\n\tMOV BX, AX\n";
            if (variable->getIsGlobal())
            {
                icgFout << "\tPOP AX\n\tMOV " << variable->getChildren().at(0)->getName() << "[BX], AX\n";
            }
            else
            {
                icgFout << "\tMOV AX," << -variable->getStackOffset() << "\n";
                icgFout << "\tSUB AX,BX\n\tMOV SI, AX\n\tNEG SI\n\tPOP AX\n\tMOV [BP+SI], AX\n";
            }
        }
        else
        {
            if (variable->getIsGlobal())
            {
                icgFout << "\tMOV " << variable->getChildren().at(0)->getName() << ", AX\n";
            }
            else
            {
                icgFout << "\tMOV [BP" << showpos << variable->getStackOffset() << "], AX\n";
            }
            icgFout << noshowpos;
        }
    }
    void performRetrievalOperations(Node *node)
    {
        Node *variable =node->getChildren().at(0);
        variable=sTable.Lookup(variable);
        if (node->getType() == "ARRAY_VARIABLE")
        {
            traverse(node->getChildren().at(2));
            icgFout << "\tMOV BX, AX\n\tMOV AX, 2\n\tMUL BX\n\tMOV BX, AX\n";
            if (variable->getIsGlobal())
            {
                icgFout << "\tMOV AX," << variable->getName() << "[BX]\n";
            }
            else
            {
                icgFout << "\tMOV AX," << -variable->getStackOffset() << "\n";
                icgFout << "\tSUB AX,BX\n\tMOV SI, AX\n\tNEG SI\n\tMOV AX, [BP+SI]\n";
            }
        }
        else
        {
            cout<<variable->getName()<<variable->getIsGlobal()<<endl;
            if (variable->getIsGlobal())
            {
                icgFout << "\tMOV AX," << variable->getName() << "\n";
            }
            else
            {
                icgFout << "\tMOV AX, [BP" << showpos << variable->getStackOffset() << "]\n";
            }
            icgFout << noshowpos;
        }
    }
    void performIncrementOperations(Node *node)
    {
        icgFout << "\tPUSH AX\n\tDEC AX\n";
        performAssignmentOperations(node->getChildren().at(0));
        icgFout << "\tPOP AX\n";
    }
    void performDecrementOperations(Node *node)
    {
        icgFout << "\tPUSH AX\n\tINC AX\n";
        performAssignmentOperations(node->getChildren().at(0));
        icgFout << "\tPOP AX\n";
    }
    void executeIfStatement(Node *node)
    {
        int truelabel;
        int nextlabel;
        traverse(node->getChildren().at(2));
        icgFout << "\tCMP AX, 0\n";
        truelabel = ++label;
        icgFout << "JNE L" << truelabel << "\n";
        if (node->getChildren().size() > 5)
        {
            traverse(node->getChildren().at(6));
        }
        nextlabel = ++label;
        icgFout << "\tJMP L" << nextlabel << endl;
        icgFout << "L" << truelabel << ":\n";
        traverse(node->getChildren().at(4));
        icgFout << "L" << nextlabel << ":\n";
    }
    void executeForLoop(Node *node)
    {
        int startlabel;
        int exitlabel;
        traverse(node->getChildren().at(2));
        startlabel = ++label;
        icgFout << "L" << startlabel << ":\n";
        traverse(node->getChildren().at(3));
        icgFout << "\tCMP AX, 0\n";
        exitlabel = ++label;
        icgFout << "\tJE L" << exitlabel << endl;
        traverse(node->getChildren().at(6));
        traverse(node->getChildren().at(4));
        icgFout << "\tJMP L" << startlabel << "\n";
        icgFout << "L" << exitlabel << ":\n";
    }
    void executeWhileLoop(Node *node)
    {
        int startlabel;
        int exitlabel;
        startlabel = ++label;
        icgFout << "L" << startlabel << ":\n";
        traverse(node->getChildren().at(2));
        icgFout << "\tCMP AX, 0\n";
        exitlabel = ++label;
        icgFout << "\tJE L" << exitlabel << endl;
        traverse(node->getChildren().at(4));
        icgFout << "\tJMP L" << startlabel << "\n";
        icgFout << "L" << exitlabel << ":\n";
    }

    void initializeFunction(Node *node)
    {
        string functionname = node->getChildren().at(1)->getName();
        Node *arguments = node->getChildren().at(3);
        icgFout << functionname << " PROC\n";
        if (functionname == "main")
        {
            icgFout << "\tMOV AX, @DATA\n\tMOV DS, AX\n";
        }
        icgFout << "\tPUSH BP\n\tMOV BP, SP\n";
        offset = 0;
        popCount = 0;
        returnLabel = 0;
        if (arguments->getType() == "parameter_list")
        {
            allocatePlaceForArguments(arguments, 4);
        }
    }
    void terminateFunction(Node *node)
    {
        if (returnLabel != 0)
        {
            icgFout << "L" << returnLabel << ":\n";
        }
        if (popCount != 0)
        {
            icgFout << "\tADD SP, " << popCount << "\n";
        }
        icgFout << "\tPOP BP\n";
        string functionName = node->getChildren().at(1)->getName();
        if (functionName == "main")
        {
            icgFout << "\tMOV AX, 4CH\n\tINT 21H\nmain ENDP\n";
        }
        else
        {
            Node *toBeSearched = new Node(new SymbolInfo(functionName, "ID"));
            Node *searched = sTable.LookupCurrent(toBeSearched);
            int toReturn = searched->getParameterList().size() * 2;
            icgFout << "\tRET ";
            if (toReturn != 0)
            {
                icgFout << toReturn;
            }
            icgFout << "\n" + functionName + " ENDP\n";
        }
    }
    void declareVariable(Node *node)
    {
        for (Node *child : node->getChildren())
        {
            if (child->getType() == "declaration_list")
            {
                nestedVariableDeclaration(child);
            }
            popCount = -offset;
        }
    }
    void nestedVariableDeclaration(Node *node)
    {
        for (Node *child : node->getChildren())
        {
            if (child->getType() == "declaration_list")
            {
                nestedVariableDeclaration(child);
            }
            else if (child->getType() == "ID")
            {
                if (child->getIsGlobal())
                {
                    return;
                }
                offset = offset - 2 * child->getSize();
                child->setStackOffset(offset);
                icgFout << "\tSUB SP, " << 2 * child->getSize() << "\n";
            }
        }
    }

    void traverse(Node *node)
    {
        cout<<node->getRule()<<endl;
        if (node->getStartLine() > lineNo)
        {
            lineNo = node->getStartLine();
            icgFout << "\t; line " << lineNo << "\n";
        }
        if (node->getRule() == "start : program")
        {
            initializeMemorySegment();
            initializeDataSegment();
            for (Node *child : globals)
            {
                
                if (node->getFunctionInfo() == "") // not functions
                {
                    icgFout << "\t" << node->getName() << " DW " << node->getSize() << " DUP (0000H)\n";
                }
            }
            initializeCodeSegment();
            
        }

        if (node->getRule() == "func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement")
        {
            initializeFunction(node);
            
        }
        else if (node->getRule() == "func_definition : type_specifier ID LPAREN RPAREN compound_statement")
        {
            initializeFunction(node);
            
        }

        else if (node->getRule() == "statement : FOR LPAREN expression_statement expression_statement expression RPAREN statement")
        {
            executeForLoop(node);
            return;
        }
        else if (node->getRule() == "statement : IF LPAREN expression RPAREN statement")
        {
            executeIfStatement(node);
            return;
        }
        else if (node->getRule() == "statement : IF LPAREN expression RPAREN statement ELSE statement")
        {
            executeIfStatement(node);
            return;
        }
        else if (node->getRule() == "statement : WHILE LPAREN expression RPAREN statement")
        {
            executeWhileLoop(node);
            return;
        }
        else if (node->getRule() == "expression : variable ASSIGNOP logic_expression")
        {
            traverse(node->getChildren().at(2));
            performAssignmentOperations(node->getChildren().at(0));
            return;
        }
        else if (node->getRule() == "logic_expression : rel_expression LOGICOP rel_expression")
        {
            performLogicalOperations(node);
            return;
        }

        else if (node->getRule() == "rel_expression : simple_expression RELOP simple_expression")
        {
            performRelationalOperations(node);
            return;
        }

        else if (node->getRule() == "simple_expression : simple_expression ADDOP term")
        {
            performArithmeticOperations(node);
            return;
        }

        else if (node->getRule() == "term : term MULOP unary_expression")
        {
            performArithmeticOperations(node);
            return;
        }

        for (Node *child : node->getChildren())
        {
            traverse(child);
        }
        if (node->getRule() == "func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement")
        {
            terminateFunction(node);
        }
        else if (node->getRule() == "func_definition : type_specifier ID LPAREN RPAREN compound_statement")
        {
            terminateFunction(node);
        }
        else if (node->getRule() == "statement : RETURN expression SEMICOLON")
        {
            if (returnLabel == 0)
            {
                returnLabel = ++label;
            }

            icgFout << "\tJMP L" << returnLabel << "\n";
        }
        else if (node->getRule() == "var_declaration : type_specifier declaration_list SEMICOLON")
        {
            declareVariable(node);
            
        }
        else if (node->getRule() == "variable : ID")
        {
            performRetrievalOperations(node);
        }
        else if (node->getRule() == "variable : ID LSQUARE expression RSQUARE")
        {
            performRetrievalOperations(node);
        }
        else if (node->getRule() == "factor : ID LPAREN argument_list RPAREN")
        {
            icgFout << "\tCALL " << node->getChildren().at(0)->getName() << endl;
        }
        else if (node->getRule() == "factor : CONST_INT")
        {
            icgFout << "\tMOV AX, " << node->getChildren().at(0)->getName() << "\n";
        }
        else if (node->getRule() == "factor : variable INCOP")
        {
            performIncrementOperations(node);
        }
        else if (node->getRule() == "factor : variable DECOP")
        {
            performDecrementOperations(node);
        }
        else if (node->getRule() == "unary_expression : ADDOP unary_expression")
        {
            if (node->getChildren().at(0)->getName() == "-")
            {
                icgFout << "\tNEG AX\n";
            }
        }
        else if (node->getRule() == "unary_expression : NOT unary_expression")
        {
            int intermediateLabel, finalLabel;
            icgFout << "\tCMP AX, 0\n";
            intermediateLabel = ++label;
            icgFout << "\tJNE L" << intermediateLabel << "\n";
            icgFout << "\tMOV AX, 1\n";
            finalLabel = ++label;
            icgFout << "\tJMP L" << finalLabel << "\n";
            icgFout << "L" << intermediateLabel << ":\n";
            icgFout << "\tMOV AX, 0\n";
            icgFout << "L" << finalLabel << ":\n";
        }
        else if (node->getRule() == "arguments : arguments COMMA logic_expression" || node->getRule() == "arguments : logic_expression")
        {
            icgFout << "\tPUSH AX" << endl;
        }
        else if (node->getRule() == "statement : PRINTLN LPAREN ID RPAREN SEMICOLON")
        {
            if (node->getChildren().size() != 0)
            {
                icgFout << "\tCALL Print\n\tCALL Newline\n";
            }
        }
    }
    void generateIntermediateCode(ParseTree &pTree)
    {
        traverse(pTree.getCurrentNode());
        icgFout << "END main\n";
        icgFout.close();
    }
};