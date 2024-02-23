#include <bits/stdc++.h>
#include "2005087_ParseTree.h"
using namespace std;

class ICG
{
    std::ofstream icgFout;
    int label;
    string currentFunctionName;
    vector<pair<pair<string, string>, bool>> currentArgumentList;
    vector<int> endList;
    bool isGlobal = true;
    map<int, string> labelMap;
    Node *currentExpression;

public:
    ICG()
    {
        icgFout.open("code.asm");
        label = 0;
    }
    ofstream &getIcgFout()
    {
        return icgFout;
    }
    void initializeMemorySegment()
    {
        icgFout << ".model small\n.stack 1000h" << endl;
    }
    void initializeDataSegment()
    {
        icgFout << ".data" << endl;
        icgFout << "\n"
                << endl;
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

    string VariableAddress(Node *node)
    {
        int offset = node->getStackOffset();
        string name = node->getName();

        if (offset == -1)
        {
            return name; // Global variable
        }
        else // Local variable
        {
            if (offset == 0)
            {
                return "[BP]";
            }

            else if (offset > 0)
            {
                return "[BP-" + to_string(offset) + "]";
            }
            else
            {
                return "[BP+" + to_string(abs(offset)) + "]";
            }
        }
    }
    string VariableAddress(string name, int offset)
    {
        if (offset == -1)
        {
            return name;
        }

        else
        {
            return "[BP" + (offset ? ((offset > 0 ? "-" : "") + to_string(offset)) : "") + "]";
        }
    }

    void performArithmeticOperation(string Operator)
    {
        if (Operator == "+")
        {
            icgFout << "\tADD ax,bx" << endl;
        }
        else if (Operator == "-")
        {
            icgFout << "\tSUB ax,bx" << endl;
        }
        else if (Operator == "*")
        {
            icgFout << "\tMUL BX" << endl;
        }
        else if (Operator == "/")
        {
            icgFout << "\tCWD\n\tDIV BX" << endl;
        }
        else if (Operator == "%")
        {
            icgFout << "\tCWD\n\tDIV BX\n\tMOV AX,DX" << endl;
        }
    }

    void createNewLabel()
    {
        label++;
        icgFout << "L" + to_string(label) << ":" << endl;
    }

    void performIncrementOperation(string Operator, Node *node)
    {
        if (node->getType() != "ARRAY_VARIABLE")
        {
            string address = VariableAddress(node);
            icgFout << "\tMOV ax," + address << endl;
            icgFout << "PUSH ax" << endl;
            icgFout << Operator + " W ." + address << endl;
        }
        else
        {
            if (node->getStackOffset() == -1)
            {
                icgFout << "\tPOP cx\n\tLEA SI," + node->getName() + "\n\tSHL cx,1\n\tADD SI,cx\n\tMOV ax,[SI]\n\tPUSH ax\n\t" + Operator + " W.[SI]" << endl;
            }
            else
            {
                icgFout << "\tPOP cx\n\tSHL cx,1\n\tADD cx," + to_string(node->getStackOffset()) + "\n\tMOV DI,BP\n\tSUB DI,CX\n\tMOV AX,[DI]\n\tPUSH ax\n\t" + Operator + " W.[DI]" << endl;
            }
        }
    }

    void performRelationalOperation(string Operator, Node *node)
    {
        if (Operator == "<")
        {
            icgFout << "\tCMP ax,bx\n\tJL " << endl;
        }
        else if (Operator == ">")
        {
            icgFout << "\tCMP ax,bx\n\tJG " << endl;
        }
        else if (Operator == "==")
        {
            icgFout << "\tCMP ax,bx\n\tJE " << endl;
        }
        else if (Operator == "!=")
        {
            icgFout << "\tCMP ax,bx\n\tJNE " << endl;
        }
        else if (Operator == ">=")
        {
            icgFout << "\tCMP ax,bx\n\tJGE " << endl;
        }
        else if (Operator == "<=")
        {
            icgFout << "\tCMP ax,bx\n\tJLE " << endl;
        }
        icgFout << "\tJMP " << endl;
    }

    void performLogicalOperation(string Operator)
    {
        if (Operator == "NOT")
        {
            icgFout << "\tCMP AX, 0\n\t";
            int trueLabel = label;
            icgFout << "JE L" + to_string(trueLabel) + "\n\t";
            int falseLabel = label + 1;
            icgFout << "JMP L" + to_string(falseLabel) + "\n\t";
            icgFout << "L" + to_string(trueLabel) << ":" << endl;
            icgFout << "MOV AX, 1";
            icgFout << "JMP L" + to_string(falseLabel + 1) + "\n\t";
            icgFout << "L" + to_string(falseLabel) << ":" << endl;
            icgFout << "XOR AX, AX";
            icgFout << "L" + to_string(falseLabel + 1) << ":" << endl;
            label += 3;
        }
        else
        {
            if (Operator == "&&")
                icgFout << "AND AX, BX" << endl;
            else if (Operator == "||")
                icgFout << "OR AX, BX" << endl;
        }
    }

    void initializeFunction(string functionName)
    {
        if (functionName == "main")
        {
            icgFout << "Main proc\n\tMOV ax,@data\n\tMOV ds,ax" << endl;
        }
        else
        {
            icgFout << functionName + " PROC" << endl;
        }
        icgFout << "\tPUSH BP" << endl;
        icgFout << "\tMOV BP,SP" << endl;
    }

    void returnFromFunction(string functionName, int argumentsCount)
    {
        icgFout << "\tMOV SP, BP\n\tPOP BP" << endl;
        if (functionName == "main")
        {
            icgFout << "\n\tExit:\n\tMOV ax,4ch\n\tint 21h\nMain ENDP" << endl;
        }
        else
        {
            if (argumentsCount == 0)
            {
                icgFout << "\tRET" << endl;
            }

            else
            {
                icgFout << "\tRET " + to_string(argumentsCount * 2) << endl;
            }

            icgFout << functionName + " ENDP" << endl;
        }
    }
    void performUnaryBooleanOperation(Node *node)
    {
        int trueLabel = label;
        icgFout << "L" + to_string(trueLabel) + ":" << endl;
        icgFout << "MOV AX, 1";
        int falseLabel = label + 1;
        icgFout << "JMP L" + to_string(falseLabel) + "\n\t";
        icgFout << "L" + to_string(falseLabel) << ":" << endl;
        icgFout << "XOR AX, AX";
        icgFout << "L" + to_string(falseLabel + 1) << ":" << endl;
        label += 3;
    }

    void backPatch(const vector<int> &list, const string label)
    {
        for (int i : list)
        {
            labelMap[i] = label;
        }
    }

    void CheckExpressionEvaluatedOrNot(Node *node)
    {
        if (!node->getIsEvaluated())
        {
            icgFout << "\tPOP ax\n\tCMP AX,0\n\tJNE" << endl;
            // add code here
            icgFout << "\tPUSH ax" << endl;
        }
        node->setIsEvaluated(true);
    }

    void traverse(Node *node)
    {
        cout << node->getRule() << endl;
        if (node->getRule() == "func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement")
        {
            traverse(node->getChildren().at(0));
            traverse(node->getChildren().at(1));
            traverse(node->getChildren().at(2));
            traverse(node->getChildren().at(3));
            traverse(node->getChildren().at(4));
            currentFunctionName = node->getChildren().at(1)->getName();
            currentArgumentList = node->getChildren().at(3)->getParameterList();
            initializeFunction(currentFunctionName);
            traverse(node->getChildren().at(5));
            createNewLabel();
            backPatch(endList, to_string(label));
            endList.clear();
            returnFromFunction(currentFunctionName, currentArgumentList.size());
        }
        else if (node->getRule() == "func_definition : type_specifier ID LPAREN RPAREN compound_statement")
        {
            traverse(node->getChildren().at(0));
            traverse(node->getChildren().at(1));
            traverse(node->getChildren().at(2));
            traverse(node->getChildren().at(3));
            string currentFunctionName = node->getChildren().at(1)->getName();
            initializeFunction(currentFunctionName);
            traverse(node->getChildren().at(4));
            createNewLabel();
            backPatch(endList, to_string(label));
            endList.clear();
            returnFromFunction(currentFunctionName, 0);
        }
        else if (node->getRule() == "compound_statement : LCURL statements RCURL")
        {
            traverse(node->getChildren().at(0));
            traverse(node->getChildren().at(1));
            createNewLabel();
            traverse(node->getChildren().at(2));
            backPatch(node->getChildren().at(1)->getNextList(), to_string(label));
            node->setNextList(node->getChildren().at(1)->getNextList());
        }
        else if (node->getRule() == "compound_statement : LCURL RCURL")
        {
            traverse(node->getChildren().at(0));
            traverse(node->getChildren().at(1));
            if (!node->getCompareOffset())
            {
                icgFout << "\tADD SP," + to_string(node->getOffsetDifference()) << endl;
            }
        }

        else if (node->getRule() == "statements : statement")
        {
            traverse(node->getChildren().at(0));
            node->setNextList(node->getChildren().at(0)->getNextList());
        }
        else if (node->getRule() == "statements : statements statement")
        {
            traverse(node->getChildren().at(0));
            createNewLabel();
            traverse(node->getChildren().at(1));
            backPatch(node->getChildren().at(0)->getNextList(), to_string(label));
            node->setNextList(node->getChildren().at(1)->getNextList());
        }
        else if (node->getRule() == "statement : expression_statement")
        {
            traverse(node->getChildren().at(0));
            node->setNextList(node->getChildren().at(0)->getNextList());
        }
        else if (node->getRule() == "statement : compound_statement")
        {
            traverse(node->getChildren().at(0));
            node->setNextList(node->getChildren().at(0)->getNextList());
        }
        else if (node->getRule() == "statement : FOR LPAREN expression_statement expression_statement expression RPAREN statement")
        {
            traverse(node->getChildren().at(0));
            traverse(node->getChildren().at(1));
            traverse(node->getChildren().at(2));
            createNewLabel();
            int l1 = label;
            traverse(node->getChildren().at(3));
            createNewLabel();
            int l2 = label;
            traverse(node->getChildren().at(4));
            icgFout << "\tPOP ax" << endl;
            icgFout << "\tJMP" << endl;
            vector<int> list1;

            // add code here

            traverse(node->getChildren().at(5));
            createNewLabel();
            int l3 = label;
            traverse(node->getChildren().at(6));
            icgFout << "\tJMP" << endl;
            vector<int> list2;

            // add code here

            node->setNextList(node->getChildren().at(3)->getFalseList());
            backPatch(node->getChildren().at(2)->getNextList(), to_string(l1));
            backPatch(node->getChildren().at(3)->getTrueList(), to_string(l3));
            backPatch(node->getChildren().at(6)->getNextList(), to_string(l2));
            backPatch(node->getChildren().at(4)->getNextList(), to_string(l1));
            backPatch(list2, to_string(l2));
            backPatch(list1, to_string(l1));
        }
        else if (node->getRule() == "statement : IF LPAREN expression RPAREN statement")
        {
            traverse(node->getChildren().at(0));
            traverse(node->getChildren().at(1));
            traverse(node->getChildren().at(2));
            CheckExpressionEvaluatedOrNot(currentExpression);
            traverse(node->getChildren().at(3));
            createNewLabel();
            traverse(node->getChildren().at(4));
        }
        else if (node->getRule() == "statement : IF LPAREN expression RPAREN statement ELSE statement")
        {
            traverse(node->getChildren().at(0));
            traverse(node->getChildren().at(1));
            traverse(node->getChildren().at(2));
            CheckExpressionEvaluatedOrNot(currentExpression);
            traverse(node->getChildren().at(3));
            createNewLabel();
            traverse(node->getChildren().at(4));
            traverse(node->getChildren().at(5));
            icgFout << "\tJMP" << endl;
            createNewLabel();
            traverse(node->getChildren().at(6));
        }
        else if (node->getRule() == "statement : WHILE LPAREN expression RPAREN statement")
        {
            traverse(node->getChildren().at(0));
            createNewLabel();
            int l1 = label;
            traverse(node->getChildren().at(1));
            traverse(node->getChildren().at(2));
            CheckExpressionEvaluatedOrNot(currentExpression);
            traverse(node->getChildren().at(3));
            createNewLabel();
            traverse(node->getChildren().at(4));
            icgFout << "JMP " + to_string(l1) << endl;
        }
        else if (node->getRule() == "statement : PRINTLN LPAREN ID RPAREN SEMICOLON")
        {
            for (Node *child : node->getChildren())
            {
                traverse(child);
            }
            int offset = node->getStackOffset();
            print(VariableAddress(node->getChildren().at(2)->getName(), offset));
        }
        else if (node->getRule() == "statement : RETURN expression SEMICOLON")
        {
            traverse(node->getChildren().at(0));
            traverse(node->getChildren().at(1));
            if (!(node->getChildren().at(1)->getTrueList().empty()) || !(node->getChildren().at(1)->getFalseList().empty()) || !(node->getChildren().at(1)->getNextList().empty()))
            {
                icgFout << "\tPOP ax" << endl;
                performUnaryBooleanOperation(node->getChildren().at(1));
                icgFout << "\tPUSH ax" << endl;
                traverse(node->getChildren().at(2));
                icgFout << "POP ax" << endl;
                icgFout << "JMP" << endl;
            }
        }

        else if (node->getRule() == "expression_statement : SEMICOLON")
        {
            traverse(node->getChildren().at(0));
            icgFout << "\tPOP ax" << endl;
        }
        else if (node->getRule() == "expression_statement : expression SEMICOLON")
        {
            traverse(node->getChildren().at(0));
            traverse(node->getChildren().at(1));
            node->setTrueList(node->getChildren().at(0)->getTrueList());
            node->setFalseList(node->getChildren().at(0)->getFalseList());
            node->setNextList(node->getChildren().at(0)->getNextList());
            node->setIsEvaluated(node->getChildren().at(0)->getIsEvaluated());
            icgFout << "POP ax" << endl;
        }

        else if (node->getRule() == "expression : logic_expression")
        {
            traverse(node->getChildren().at(0));
            node->setTrueList(node->getChildren().at(0)->getTrueList());
            node->setFalseList(node->getChildren().at(0)->getFalseList());
            node->setNextList(node->getChildren().at(0)->getNextList());
            node->setIsEvaluated(node->getChildren().at(0)->getIsEvaluated());
            currentExpression = node;
        }

        else if (node->getRule() == "expression : variable ASSIGNOP logic_expression")
        {
            if (node->getChildren().at(0)->getReturnOrDataType() == "INT")
            {
                traverse(node->getChildren().at(0));
                traverse(node->getChildren().at(1));
                traverse(node->getChildren().at(2));
                node->setTrueList(node->getChildren().at(2)->getTrueList());
                node->setFalseList(node->getChildren().at(2)->getFalseList());
                node->setNextList(node->getChildren().at(2)->getNextList());
                icgFout << "\tPOP ax" << endl;
                if (!(node->getChildren().at(2)->getTrueList().empty()) || !(node->getChildren().at(2)->getFalseList().empty()) || !(node->getChildren().at(2)->getNextList().empty()))
                {
                    icgFout << "\tPOP ax" << endl;
                    performUnaryBooleanOperation(node->getChildren().at(2));
                    icgFout << "\tPUSH ax" << endl;
                }
                if (node->getChildren().at(0)->getType() != "ARRAY_VARIABLE")
                {
                    icgFout << "\tMOV " + VariableAddress(node->getChildren().at(0)) + ",ax" << endl;
                }
                else
                {
                    if (node->getChildren().at(0)->getStackOffset() == -1)
                    {
                        icgFout << "\tPOP cx\n\tLEA SI," + node->getChildren().at(0)->getName() + "\n\tSHL cx,1\n\tADD SI,cx\n\tMOV [SI],ax" << endl;
                    }
                    else
                    {
                        icgFout << "POP cx\n\tSHL cx,1\n\tADD cx," + to_string(node->getChildren().at(0)->getStackOffset()) + "\n\tMOV DI, BP\n\tSUB DI, CX\n\tMOV [DI], AX" << endl;
                    }
                    node->setIsEvaluated(node->getChildren().at(2)->getIsEvaluated());
                }
            }
            currentExpression = node;
        }
        else if (node->getRule() == "logic_expression : rel_expression")
        {
            traverse(node->getChildren().at(0));
            node->setTrueList(node->getChildren().at(0)->getTrueList());
            node->setFalseList(node->getChildren().at(0)->getFalseList());
            node->setNextList(node->getChildren().at(0)->getNextList());
            node->setIsEvaluated(node->getChildren().at(0)->getIsEvaluated());
        }


        else if (node->getRule() == "factor : ID LPAREN argument_list RPAREN")
        {
            icgFout << "\tCALL " + node->getChildren().at(0)->getName() + "\n\tPUSH ax" << endl;
        }
        else if (node->getRule() == "factor : variable")
        {
            traverse(node->getChildren().at(0));
            if (node->getChildren().at(0)->getType() != "ARRAY_VARIABLE")
            {
                icgFout << "\tMOV ax," + VariableAddress(node->getChildren().at(0)) << endl;
            }
            else
            {
                // global array
                int offset = node->getChildren().at(0)->getStackOffset();
                string name = node->getChildren().at(0)->getName();
                if (offset == -1)
                {
                    icgFout << "\tPOP cx\n\tLEA SI," + name + "\n\tSHL cx,1\n\tADD SI,cx\n\tMOV ax,[SI]" << endl;
                }
                else
                {
                    // local array
                    icgFout << "\tPOP cx\n\tSHL cx,1\n\tADD cx," + to_string(offset) + "\n\tMOV DI, BP\n\tSUB DI, CX\n\tMOV AX, [DI]" << endl;
                }
            }
            icgFout << "\tPUSH ax" << endl;
        }
        else if (node->getRule() == "factor : ID LPAREN argument_list RPAREN")
        {
            traverse(node->getChildren().at(0));
            traverse(node->getChildren().at(1));
            traverse(node->getChildren().at(2));
            traverse(node->getChildren().at(3));
            icgFout << "\tCALL " + node->getChildren().at(1)->getName() << endl;
            icgFout << "\tPUSH ax" << endl;
        }
        else if (node->getRule() == "factor : LPAREN expression RPAREN")
        {
            traverse(node->getChildren().at(0));
            traverse(node->getChildren().at(1));
            traverse(node->getChildren().at(2));
            node->setTrueList(node->getChildren().at(1)->getTrueList());
            node->setFalseList(node->getChildren().at(1)->getFalseList());
            node->setNextList(node->getChildren().at(1)->getNextList());
            node->setIsEvaluated(node->getChildren().at(1)->getIsEvaluated());
            if (!(node->getChildren().at(1)->getTrueList().empty()) || !(node->getChildren().at(1)->getFalseList().empty()) || !(node->getChildren().at(1)->getNextList().empty()))
            {
                icgFout << "\tPOP ax" << endl;
                performUnaryBooleanOperation(node->getChildren().at(1));
                icgFout << "\tPUSH ax" << endl;
            }
        }

        else if (node->getRule() == "factor : CONST_INT")
        {
            traverse(node->getChildren().at(0));
            icgFout << "MOV ax," + node->getChildren().at(0)->getName() << endl;
            icgFout << "PUSH ax" << endl;
        }
        else if (node->getRule() == "factor : variable INCOP")
        {
            traverse(node->getChildren().at(0));
            traverse(node->getChildren().at(1));
            performIncrementOperation("INC", node->getChildren().at(0));
        }
        else if (node->getRule() == "factor : variable DECOP")
        {
            traverse(node->getChildren().at(0));
            traverse(node->getChildren().at(1));
            performIncrementOperation("DEC", node->getChildren().at(0));
        }

        
        else if (node->getRule() == "logic_expression : rel_expression LOGICOP rel_expression")
        {
            traverse(node->getChildren().at(0));
            if (!node->getChildren().at(0)->getIsEvaluated())
            {
                icgFout << "\tPOP ax" << endl;
                icgFout << "\tCMP AX, 0" << endl;
                icgFout << "\tJNE" << endl;
                icgFout<<"\tJMP"<<endl;
                // add code here
                icgFout << "\tPUSH ax" << endl;
                node->getChildren().at(0)->setIsEvaluated(true);
            }
            traverse(node->getChildren().at(1));
            createNewLabel();
            traverse(node->getChildren().at(2));
            if (!node->getChildren().at(2)->getIsEvaluated())
            {
                icgFout << "\tPOP ax" << endl;
                icgFout << "\tCMP AX, 0" << endl;
                icgFout << "\tJNE" << endl;
                icgFout<<"\tJMP"<<endl;
                // add code here
                icgFout << "\tPUSH ax" << endl;
                node->getChildren().at(2)->setIsEvaluated(true);
            }
            icgFout << "POP bx\n\tPOP ax" << endl;
            icgFout << "\tPUSH ax" << endl;
        }
        else if (node->getRule() == "rel_expression : simple_expression")
        {
            traverse(node->getChildren().at(0));
            node->setTrueList(node->getChildren().at(0)->getTrueList());
            node->setFalseList(node->getChildren().at(0)->getFalseList());
            node->setNextList(node->getChildren().at(0)->getNextList());
            node->setIsEvaluated(node->getChildren().at(0)->getIsEvaluated());
        }
        else if (node->getRule() == "rel_expression : simple_expression RELOP simple_expression")
        {
            for (Node *child : node->getChildren())
            {
                traverse(child);
            }
            icgFout << "\tPOP bx\n\tPOP ax" << endl;
            performRelationalOperation(node->getChildren().at(1)->getName(), node);
            icgFout << "PUSH ax" << endl;
            node->setIsEvaluated(true);
        }

        else if (node->getRule() == "simple_expression : term")
        {
            traverse(node->getChildren().at(0));
            node->setTrueList(node->getChildren().at(0)->getTrueList());
            node->setFalseList(node->getChildren().at(0)->getFalseList());
            node->setNextList(node->getChildren().at(0)->getNextList());
            node->setIsEvaluated(node->getChildren().at(0)->getIsEvaluated());
        }
        else if (node->getRule() == "simple_expression : simple_expression ADDOP term")
        {
            for (Node *child : node->getChildren())
            {
                traverse(child);
            }
            icgFout << "\tPOP bx\n\tPOP ax" << endl;
            performArithmeticOperation(node->getChildren().at(1)->getName());
            icgFout << "\tPUSH ax" << endl;
        }
        else if (node->getRule() == "term : unary_expression")
        {
            traverse(node->getChildren().at(0));
            node->setTrueList(node->getChildren().at(0)->getTrueList());
            node->setFalseList(node->getChildren().at(0)->getFalseList());
            node->setNextList(node->getChildren().at(0)->getNextList());
            node->setIsEvaluated(node->getChildren().at(0)->getIsEvaluated());
        }
        else if (node->getRule() == "term : term MULOP unary_expression")
        {
            for (Node *child : node->getChildren())
            {
                traverse(child);
            }
            icgFout << "\tPOP bx" << endl;
            icgFout << "\tPOP ax" << endl;
            performArithmeticOperation(node->getChildren().at(1)->getName());
            icgFout << "\tPUSH ax" << endl;
        }
        else if (node->getRule() == "unary_expression : ADDOP unary_expression")
        {
            for (Node *child : node->getChildren())
            {
                traverse(child);
            }
            if (node->getChildren().at(0)->getName() == "-")
            {
                icgFout << "\tPOP ax" << endl;
                icgFout << "\tNEG ax" << endl;
                icgFout << "\tPUSH ax" << endl;
            }
        }
        else if (node->getRule() == "unary_expression : NOT unary_expression")
        {
            for (Node *child : node->getChildren())
            {
                traverse(child);
            }
            if (!(node->getChildren().at(1)->getTrueList().empty()) || !(node->getChildren().at(1)->getFalseList().empty()) || !(node->getChildren().at(1)->getNextList().empty()))
            {
                node->setTrueList(node->getChildren().at(1)->getFalseList());
                node->setFalseList(node->getChildren().at(1)->getTrueList());
            }
            else
            {
                icgFout << "POP ax" << endl;
                performLogicalOperation("NOT");
                icgFout << "PUSH ax" << endl;
            }
        }
        else if (node->getRule() == "unary_expression : factor")
        {
            traverse(node->getChildren().at(0));
            node->setTrueList(node->getChildren().at(0)->getTrueList());
            node->setFalseList(node->getChildren().at(0)->getFalseList());
            node->setNextList(node->getChildren().at(0)->getNextList());
            node->setIsEvaluated(node->getChildren().at(0)->getIsEvaluated());
        }

        else
        {
            for (Node *child : node->getChildren())
            {
                traverse(child);
            }
        }
    }
    void generateIntermediateCode(ParseTree &pTree)
    {
        initializeMemorySegment();
        initializeDataSegment();
        initializeCodeSegment();
        traverse(pTree.getCurrentNode());
    }
};