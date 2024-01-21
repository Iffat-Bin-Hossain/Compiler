%{
#include<bits/stdc++.h>
#include "2005087_SymbolTable.cpp"
#include "2005087_Utility.cpp"
#include "2005087_ParseTree.cpp"
using namespace std;
int yyparse(void);
int yylex(void);
extern int yylineno;
extern FILE* yyin;
void yyerror(const std::string& message) {
    cout << "Error at line " << yylineno << ": " << message << endl;
}

ParseTree pTree;
SymbolTable sTable(11);
Utility util;
int startLine;
string currentFunctionReturnType="";
vector<pair<pair<string,string>,bool>>currentFunctionParameters={};

void declareFunction(string functionName, string returnType, int startLine, vector<pair<pair<string, string>, bool>> parameterList = {})
{
	Node *toBeInserted = new Node(new SymbolInfo(functionName, "FUNCTION"));
	bool beInserted = sTable.Insert(toBeInserted);
	Node *searched = sTable.Lookup(toBeInserted);
	if (beInserted)
	{
		searched->setFunctionInfo("FUNCTION_DECLARATION");
		searched->addParameter(parameterList);
		searched->setReturnOrDataType(returnType);
	}
	else
	{
		if (searched->getFunctionInfo() == "FUNCTION_DECLARATION")
		{
			util.printError("redeclaration of " + functionName, startLine);
			return;
		}
	}
}
void defineFunction(string functionName, string returnType, int startLine, vector<pair<pair<string, string>, bool>> parameterList = {})
{
	currentFunctionParameters.clear();
	currentFunctionParameters = parameterList;
	Node *toBeChecked = new Node(new SymbolInfo(functionName, "FUNCTION"));
	Node *searched = sTable.Lookup(toBeChecked);
	if (searched == NULL)
	{
		bool b = sTable.Insert(toBeChecked);
		searched = sTable.Lookup(toBeChecked);
		searched->setFunctionInfo("FUNCTION_DEFINITION");
		searched->addParameter(parameterList);
		searched->setReturnOrDataType(returnType);
	}
	else
	{

		if (searched->getFunctionInfo() == "FUNCTION_DECLARATION")
		{

			if (searched->getReturnOrDataType() != returnType)
			{
				util.printError("Conflicting types for \'" + functionName + "\'", startLine);
				// return;
			}

			if (searched->getParameterList().size() != parameterList.size())
			{
				util.printError("Conflicting types for \'" + functionName + "\'", startLine);
			}

			searched->setFunctionInfo("FUNCTION_DEFINITION");
			searched->addParameter(parameterList);
			searched->setReturnOrDataType(returnType);
		}
		else
		{
			util.printError("\'" + functionName + "\' redeclared as different kind of symbol", startLine);
			return;
		}
	}
	currentFunctionParameters.clear();
}
void callFunction(Node *actualNode, Node *nameNode, Node *argNode)
{
	auto parameterList = argNode->getParameterList();
	Node *searched = sTable.Lookup(nameNode);
	if (searched == NULL)
	{
		util.printError("Undeclared function \'" + nameNode->getName() + "\'", actualNode->getStartLine());
	}
	else if (!(searched->getFunctionInfo() == "FUNCTION_DECLARATION" || searched->getFunctionInfo() == "FUNCTION_DEFINITION"))
	{
		util.printError("\'" + nameNode->getName() + "\' is not a function", actualNode->getStartLine());
	}
	else if (searched->getParameterList().size() < parameterList.size())
	{
		util.printError("Too many arguments to function \'" + nameNode->getName() + "\'", actualNode->getStartLine());
		return;
	}
	else if (searched->getParameterList().size() > parameterList.size())
	{
		util.printError("Too few arguments to function \'" + nameNode->getName() + "\'", actualNode->getStartLine());
		return;
	}
	else
	{
		for (int i = 0; i < searched->getParameterList().size(); i++)
		{

			
			if ((searched->getParameterList().at(i).first.first != parameterList[i].first.first))
			{
				util.printError("Type mismatch for argument " + to_string(i + 1) + " of \'" + nameNode->getName() + "\'", actualNode->getStartLine());
				
			}
		}
		actualNode->setReturnOrDataType(searched->getReturnOrDataType());
	}
}
void SaveData(string dataType, Node *node, int startLine)
{
	auto list = node->getParameterList();
	for (auto element : list)
	{
		if (dataType == "VOID")
		{
			util.printError("Variable or field \'" + element.first.second + "\' declared void", startLine);
			return;
		}
		Node *toBeSearched = new Node(new SymbolInfo(element.first.second, ""), "", dataType);
		Node *searched = sTable.LookupCurrent(toBeSearched);

		if (searched == NULL)
		{
			Node *toBeInserted = new Node(new SymbolInfo(element.first.second, "ID"), "", dataType);

			if (element.second)
			{
				toBeInserted->setArrayStatus(true);
			}
			sTable.Insert(toBeInserted);
			element.first.first = dataType;
		}
		else if (searched->getReturnOrDataType() != dataType)
		{
			util.printError("Conflicting types for \'" + element.first.second + "\'", startLine);
		}
		else
		{
			util.printError("Redefinition of variable \'" + element.first.second + "\'", startLine);
		}
	}
}

void CheckVariableDeclaredOrNot(Node *node)
{
	string variable = node->getName();
	Node *toBeSearched = new Node(new SymbolInfo(variable, "ID"));
	Node *searched = sTable.Lookup(toBeSearched);
	if (searched == NULL)
	{
		util.printError("Undeclared variable \'" + variable + "\'", node->getStartLine());
		return;
	}
	node->setReturnOrDataType(searched->getReturnOrDataType());
	node->setArrayStatus(searched->getArrayStatus());
}
void CheckReturnIssues(Node *node)
{
	// cout<<currentFunctionReturnType;
	//  if (currentFunctionReturnType == "VOID")
	//  {
	//  	util.printError("Function return type void", node->getStartLine());
	//  }
	if (currentFunctionReturnType == "INT" && node->getReturnOrDataType() == "FLOAT")
	{
		util.printError("Warning: possible loss of data in assignment of FLOAT to INT", node->getStartLine());
	}
}
void CheckArrayIssues(Node *actualNode, Node *nameNode, Node *indexNode)
{
	string name = nameNode->getName();
	Node *toBeSearched = new Node(new SymbolInfo(name, "ID"));
	Node *searched = sTable.Lookup(toBeSearched);
	if (searched == NULL)
	{
		util.printError("Undeclared variable \'" + name + "\'", actualNode->getStartLine());
		return;
	}
	if (!searched->getArrayStatus())
	{
		util.printError("\'" + name + "\' is not an array", actualNode->getStartLine());
		return;
	}
	if (indexNode->getReturnOrDataType() != "INT")
	{
		util.printError("Array subscript is not an integer", actualNode->getStartLine());
		return;
	}
	else
	{
		actualNode->setReturnOrDataType(searched->getReturnOrDataType());
		actualNode->setArrayStatus(false);
	}
}
void CheckAssignmentIssues(Node *actualNode, Node *leftNode, Node *rightNode)
{
	if (leftNode->getArrayStatus() && !rightNode->getArrayStatus())
	{
		util.printError("Type mismatch for \'" + leftNode->getName() + "\' , is an array", actualNode->getStartLine());
		return;
	}
	if (leftNode->getReturnOrDataType() == "VOID" || rightNode->getReturnOrDataType() == "VOID")
	{
		util.printError("Void cannot be used in expression", actualNode->getStartLine());
		return;
	}
	if (leftNode->getReturnOrDataType() == "INT")
	{
		if (rightNode->getReturnOrDataType() == "FLOAT")
		{
			util.printError("Warning: possible loss of data in assignment of FLOAT to INT", actualNode->getStartLine());
		}
		actualNode->setReturnOrDataType("INT");
		return;
	}
	actualNode->setReturnOrDataType("FLOAT");
}
void CheckLogicalIssues(Node *actualNode, Node *leftNode, Node *rightNode)
{
	if (leftNode->getReturnOrDataType() == "VOID" || rightNode->getReturnOrDataType() == "VOID")
	{
		util.printError("Void cannot be used in expression", actualNode->getStartLine());
		return;
	}
	if (leftNode->getReturnOrDataType() == "FLOAT" || rightNode->getReturnOrDataType() == "FLOAT")
	{
		util.printError("Warning: Void cannot be used in expression", actualNode->getStartLine());
		actualNode->setReturnOrDataType("INT");
	}
	actualNode->setReturnOrDataType("INT");
}
void CheckRelationalIssues(Node *actualNode, Node *leftNode, Node *rightNode)
{
	if (leftNode->getReturnOrDataType() == "VOID" || rightNode->getReturnOrDataType() == "VOID")
	{
		util.printError("Void cannot be used in expression", actualNode->getStartLine());
	}
	else
	{
		actualNode->setReturnOrDataType("INT");
	}
}
bool CheckingZerOperand(string str)
{
	bool hasNonZeroDigit = false;

	for (int i = 0; i < str.size(); ++i)
	{
		char c = str[i];

		if (c == 'e' || c == 'E')
		{
			break;
		}

		if (c != '0' && c != '.')
		{
			hasNonZeroDigit = true;
			break;
		}

		if (c == '.')
		{
			for (++i; i < str.size(); ++i)
			{
				if (str[i] != '0')
				{
					hasNonZeroDigit = true;
					break;
				}
			}
			break;
		}
	}

	return !hasNonZeroDigit;
}
// string TypeCasting(string  x, string y){
//     if (x == "VOID" || y == "VOID" || x == "ERROR" || y == "ERROR") {
//         return "ERROR";
//     }
// 		if(x == y)
// 			return x;
// 		if(x == "INT" && y->getDataType() == "float"){
// 			x->setDataType("float");
// 			return "float";
// 		}else if(x->getDataType() == "float" && y->getDataType() == "int"){
// 			y->setDataType("float");
// 			return "float";
// 		}
// 		if(x->getDataType()!="void"){
// 			return x->getDataType();
// 		}
// 		return y->getDataType();
// 	}

string TypeCasting(string s1, const string s2)
{
	// if (s1 == "VOID" || s2 == "VOID" || s1 == "ERROR" || s2 == "ERROR")
	// {
	// 	return "ERROR";
	// }
	if (s1 == "FLOAT" || s2 == "FLOAT")
	{
		return "FLOAT";
	}
	else
	{
		return "INT";
	}
}

void CheckMultiplicationIssues(Node *actualNode, Node *leftNode, Node *midNode, Node *rightNode)
{
	if (leftNode->getReturnOrDataType() == "VOID" || rightNode->getReturnOrDataType() == "VOID")
	{
		util.printError("Void cannot be used in expression", actualNode->getStartLine());
	}

	else if (midNode->getName() == "%")
	{
		if (leftNode->getReturnOrDataType() == "FLOAT" || rightNode->getReturnOrDataType() == "FLOAT")
		{
			util.printError("Operands of modulus must be integers", actualNode->getStartLine());
		}
		else if (CheckingZerOperand(rightNode->getName()))
		{
			util.printError("Warning: division by zero i=0f=1Const=0", actualNode->getStartLine());
		}
		else
		{
			actualNode->setReturnOrDataType("INT");
		}
	}
	else if (midNode->getName() == "/")
	{
		if (CheckingZerOperand(rightNode->getName()))
		{
			util.printError("Warning: division by zero i=0f=1Const=0", actualNode->getStartLine());
		}
		else
		{
			actualNode->setReturnOrDataType(TypeCasting(leftNode->getReturnOrDataType(), rightNode->getReturnOrDataType()));
		}
	}
	else if (midNode->getName() == "*")
	{
		actualNode->setReturnOrDataType(TypeCasting(leftNode->getReturnOrDataType(), rightNode->getReturnOrDataType()));
	}
}
void CheckUnaryExpression(Node *actualNode, Node *node)
{
	if (node->getReturnOrDataType() == "VOID")
	{
		util.printError("Void cannot be used in expression", actualNode->getStartLine());
		return;
	}
	else if (node->getReturnOrDataType() == "FLOAT")
	{
		util.printError("Warning :Operands of bitwise operation should be integers", actualNode->getStartLine());
	}
	actualNode->setReturnOrDataType("INT");
}
void CheckUnaryOperandIssues(Node *actualNode, Node *operand)
{
	if (operand->getReturnOrDataType() == "VOID")
	{
		util.printError("Void cannot be used in expression", actualNode->getStartLine());
		actualNode->setReturnOrDataType("ERROR");
		return;
	}
	else if (operand->getReturnOrDataType() == "ERROR")
	{
		actualNode->setReturnOrDataType("ERROR");
		return;
	}
	actualNode->setReturnOrDataType(operand->getReturnOrDataType());
}
 

%}

%union {
    Node *node;
}

%token<node> IF ELSE SWITCH CASE BREAK DEFAULT FOR WHILE DO CONTINUE     
%token<node> MAIN VOID INT FLOAT DOUBLE CHAR ID RETURN PRINTLN
%token<node> CONST_INT CONST_FLOAT CONST_CHAR 
%token<node> NOT LPAREN RPAREN LCURL RCURL LSQUARE RSQUARE COMMA SEMICOLON
%token<node> ADDOP MULOP INCOP DECOP RELOP LOGICOP BITOP ASSIGNOP 
%type<node> start program unit var_declaration func_declaration func_definition type_specifier parameter_list compound_statement statements declaration_list statement expression expression_statement logic_expression rel_expression simple_expression term unary_expression factor variable argument_list arguments
%nonassoc THEN
%nonassoc ELSE

%%

start : program {
    
        SymbolInfo *sInfo=new SymbolInfo("","start");
        $$ =new Node(sInfo,"start : program");
        pTree.setCurrentNode($$);
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($1->getEndLine());
        $$->makeChild({$1});
        util.printGrammar("start : program");
        pTree.printParseTree(util.getTreeFout());
    
}
;   
program : program unit{
    
        SymbolInfo *sInfo=new SymbolInfo("","program");
        $$=new Node(sInfo,"program : program unit");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($2->getEndLine());
        pTree.setCurrentNode($$);
        $$->makeChild({$1,$2});
        util.printGrammar("program : program unit");
     
    
}
    | unit {
   
        SymbolInfo *sInfo=new SymbolInfo("","program");
        $$=new Node(sInfo,"program : unit");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($1->getEndLine());
        pTree.setCurrentNode($$);
        $$->makeChild({$1});
        util.printGrammar("program : unit");
   
}    
;
unit : var_declaration {
   
        SymbolInfo *sInfo=new SymbolInfo("","unit");
        $$=new Node(sInfo,"unit : var_declaration");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($1->getEndLine());
        pTree.setCurrentNode($$);
        $$->makeChild({$1});
        util.printGrammar("unit : var_declaration");
    
}
    | func_declaration {
    
        SymbolInfo *sInfo=new SymbolInfo("","unit");
        $$=new Node(sInfo,"unit : func_declaration");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($1->getEndLine());
        pTree.setCurrentNode($$);
        $$->makeChild({$1});
        util.printGrammar("unit : func_declaration");
   
}
    | func_definition {
        
		SymbolInfo *sInfo = new SymbolInfo("", "unit");
        $$=new Node(sInfo,"unit : func_definition");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($1->getEndLine());
        pTree.setCurrentNode($$);
        $$->makeChild({$1});
		util.printGrammar("unit : func_definition");
       
}
;
func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON {
     
    currentFunctionParameters.clear();
        SymbolInfo *sInfo = new SymbolInfo("", "func_declaration");
        $$=new Node(sInfo,"func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($6->getEndLine());
        pTree.setCurrentNode($$);
        $$->makeChild({$1,$2,$3,$4,$5,$6});
        util.printGrammar("func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON");
        //Inserting the function in SymbolTable
        string functionName=$2->getName();
        string returnType=$1->getReturnOrDataType();
        vector<pair<pair<string, string>,bool>> parameterList = $4->getParameterList();
        declareFunction(functionName, returnType,$$->getStartLine(), parameterList);
       
        
}
    | type_specifier ID LPAREN RPAREN SEMICOLON {
        
        currentFunctionParameters.clear();
        SymbolInfo *sInfo = new SymbolInfo("", "func_declaration");
        $$=new Node(sInfo,"func_declaration : type_specifier ID LPAREN RPAREN SEMICOLON");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($5->getEndLine());
        pTree.setCurrentNode($$);
        $$->makeChild({$1,$2,$3,$4,$5});
        util.printGrammar("func_declaration : type_specifier ID LPAREN RPAREN SEMICOLON");
        //Inserting the function in SymbolTable
        string functionName=$2->getName();
        string returnType=$1->getReturnOrDataType();
        declareFunction(functionName, returnType,$$->getStartLine());
         
        
}
;

func_definition : type_specifier ID LPAREN parameter_list RPAREN {
        currentFunctionReturnType=$1->getReturnOrDataType();
}
 compound_statement {
     
        SymbolInfo *sInfo = new SymbolInfo("", "func_definition");
        $$=new Node(sInfo,"func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($7->getEndLine());
        pTree.setCurrentNode($$);
        $$->makeChild({$1,$2,$3,$4,$5,$7});
        util.printGrammar("func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement");
        string functionName=$2->getName();
        string returnType=$1->getReturnOrDataType();  
        vector<pair<pair<string, string>,bool>> parameterList = $4->getParameterList();
        defineFunction(functionName,returnType,$$->getStartLine(),parameterList);
    
    }
    | type_specifier ID LPAREN RPAREN {
        currentFunctionReturnType=$1->getReturnOrDataType();
    }
    compound_statement { 
    
        SymbolInfo *sInfo = new SymbolInfo("", "func_definition");
        $$=new Node(sInfo,"func_definition : type_specifier ID LPAREN RPAREN compound_statement");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($6->getEndLine()); 
        pTree.setCurrentNode($$);
        $$->makeChild({$1,$2,$3,$4,$6});
        util.printGrammar("func_definition : type_specifier ID LPAREN RPAREN compound_statement");
        string functionName=$2->getName();
        string returnType=$1->getReturnOrDataType();
        defineFunction(functionName,returnType,$$->getStartLine());
    
}
;




parameter_list : parameter_list COMMA type_specifier ID {

        SymbolInfo *sInfo = new SymbolInfo("", "parameter_list");
        $$=new Node(sInfo,"parameter_list : parameter_list COMMA type_specifier ID");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($4->getEndLine());
        $$->addParameter($1->getParameterList());
        $$->addParameter({{$3->getReturnOrDataType(),$4->getName()},false});
        currentFunctionParameters=$$->getParameterList();
        pTree.setCurrentNode($$);
        $$->makeChild({$1,$2,$3,$4});
        util.printGrammar("parameter_list : parameter_list COMMA type_specifier ID");
   
}
    | parameter_list COMMA type_specifier {

        SymbolInfo *sInfo = new SymbolInfo("", "parameter_list");
        $$=new Node(sInfo,"parameter_list : parameter_list COMMA type_specifier");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($3->getEndLine());
        $$->addParameter($1->getParameterList());
        $$->addParameter({{$3->getReturnOrDataType(),""},false});
        currentFunctionParameters=$$->getParameterList();
        pTree.setCurrentNode($$);
        $$->makeChild({$1,$2,$3});
        util.printGrammar("parameter_list : parameter_list COMMA type_specifier");
       
}
    | type_specifier ID{

        SymbolInfo *sInfo = new SymbolInfo("", "parameter_list");
        $$=new Node(sInfo,"parameter_list : type_specifier ID");
        $$->addParameter({{$1->getReturnOrDataType(),$2->getName()},false});
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($2->getEndLine());
        currentFunctionParameters=$$->getParameterList();
        pTree.setCurrentNode($$);
        $$->makeChild({$1,$2});
        util.printGrammar("parameter_list : type_specifier ID");
  
}
    | type_specifier {

        SymbolInfo *sInfo = new SymbolInfo("", "parameter_list");
        $$=new Node(sInfo,"parameter_list : type_specifier");
        $$->addParameter({{$1->getReturnOrDataType(),""},false});
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($1->getEndLine());
        currentFunctionParameters=$$->getParameterList();
        pTree.setCurrentNode($$);
        $$->makeChild({$1});
        util.printGrammar("parameter_list : type_specifier");
    
}
;
compound_statement : LCURL{
        sTable.EnterScope();
        for (auto functionArgument : currentFunctionParameters) {
        string argumentName=functionArgument.first.second;
			if (argumentName == "") {
				continue;
			}
			Node* toBeInserted = new Node(new SymbolInfo(argumentName, "ID"),"", functionArgument.first.first);
			toBeInserted->setArrayStatus(functionArgument.second);
			if (!sTable.Insert(toBeInserted)) {
                util.printError("Redefinition of parameter \'"+toBeInserted->getName() +"\'",$1->getStartLine() );
				break;
			}
		}
        currentFunctionParameters.clear();
    } 
     statements RCURL {

        SymbolInfo *sInfo = new SymbolInfo("", "compound_statement");
        $$=new Node(sInfo,"compound_statement : LCURL statements RCURL");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($4->getEndLine());
        pTree.setCurrentNode($$);
        $$->makeChild({$1,$3,$4});
        util.printGrammar("compound_statement : LCURL statements RCURL");  
        sTable.PrintAllScope(util.getLogFout());
        sTable.ExitScope();
     
}
    | LCURL{
        sTable.EnterScope();
        for (auto functionArgument : currentFunctionParameters) {
        string argumentName=functionArgument.first.second;
            if (argumentName == "") {
                continue;
            }
            Node* toBeInserted = new Node(new SymbolInfo(argumentName, "ID"),"", functionArgument.first.first);
            toBeInserted->setArrayStatus(functionArgument.second);
            if (!sTable.Insert(toBeInserted)) {
                util.printError("Redefinition of parameter \'"+toBeInserted->getName() +"\'",$1->getStartLine() );
                break;
            }
        }
        currentFunctionParameters.clear();
    } 
    RCURL {

        SymbolInfo *sInfo = new SymbolInfo("", "compound_statement");
        $$=new Node(sInfo,"compound_statement : LCURL RCURL");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($3->getEndLine());
        pTree.setCurrentNode($$);
        $$->makeChild({$1,$3});
        util.printGrammar("compound_statement : LCURL RCURL"); 
        sTable.PrintAllScope(util.getLogFout());
        sTable.ExitScope();
     
    }
    ;
var_declaration : type_specifier declaration_list SEMICOLON {

        SymbolInfo *sInfo = new SymbolInfo("", "var_declaration");
        $$=new Node(sInfo,"var_declaration : type_specifier declaration_list SEMICOLON",$1->getReturnOrDataType());
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($3->getEndLine());
        SaveData($1->getReturnOrDataType(),$2,$$->getStartLine());
        pTree.setCurrentNode($$);
        $$->makeChild({$1,$2,$3});
        util.printGrammar("var_declaration : type_specifier declaration_list SEMICOLON");
     
}
;

type_specifier : INT {

        SymbolInfo *sInfo = new SymbolInfo("", "type_specifier");
        $$=new Node(sInfo,"type_specifier : INT","INT");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($1->getEndLine());
        pTree.setCurrentNode($$);
        $$->makeChild({$1});
        util.printGrammar("type_specifier : INT");
       
}
    | FLOAT {

        SymbolInfo *sInfo = new SymbolInfo("", "type_specifier");
        $$=new Node(sInfo,"type_specifier : FLOAT","FLOAT");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($1->getEndLine());
        pTree.setCurrentNode($$);
        $$->makeChild({$1});
        util.printGrammar("type_specifier : FLOAT");
        
}
    | VOID {

        SymbolInfo *sInfo = new SymbolInfo("", "type_specifier");
        $$=new Node(sInfo,"type_specifier : VOID","VOID");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($1->getEndLine());
        pTree.setCurrentNode($$);
        $$->makeChild({$1});
        util.printGrammar("type_specifier : VOID");
        
}
;
declaration_list : declaration_list COMMA ID {

        SymbolInfo *sInfo= new SymbolInfo("", "declaration_list");
        $$=new Node(sInfo,"declaration_list : declaration_list COMMA ID");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($3->getEndLine());
        $$->addParameter($1->getParameterList());
        $$->addParameter({{"",$3->getName()},false});
        pTree.setCurrentNode($$);
        $$->makeChild({$1,$2,$3});
        util.printGrammar("declaration_list : declaration_list COMMA ID");
      
}
    | declaration_list COMMA ID LSQUARE CONST_INT RSQUARE {

        SymbolInfo *sInfo = new SymbolInfo("", "declaration_list");
        $$=new Node(sInfo,"declaration_list : declaration_list COMMA ID LSQUARE CONST_INT RSQUARE");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($6->getEndLine());
        $$->addParameter($1->getParameterList());
        $$->addParameter({{"",$3->getName()},true});
        $$->setArrayStatus(true);
        pTree.setCurrentNode($$);
        $$->makeChild({$1,$2,$3,$4,$5,$6});
        util.printGrammar("declaration_list : declaration_list COMMA ID LSQUARE CONST_INT RSQUARE");
 
}
    | ID {

        SymbolInfo *sInfo = new SymbolInfo("", "declaration_list");
        $$=new Node(sInfo,"declaration_list : ID");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($1->getEndLine()); 
        $$->addParameter({{$1->getReturnOrDataType(),$1->getName()},false});
        pTree.setCurrentNode($$);
        $$->makeChild({$1});
        util.printGrammar("declaration_list : ID");  
 
}
    | ID LSQUARE CONST_INT RSQUARE {

        SymbolInfo *sInfo= new SymbolInfo("", "declaration_list");
        $$=new Node(sInfo,"declaration_list : ID LSQUARE CONST_INT RSQUARE");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($4->getEndLine());
        $$->addParameter({{$1->getReturnOrDataType(),$1->getName()},true});
        pTree.setCurrentNode($$);
        $$->makeChild({$1,$2,$3,$4});
        util.printGrammar("declaration_list : ID LSQUARE CONST_INT RSQUARE"); 

}
;
statements : statement {

        SymbolInfo *sInfo = new SymbolInfo("", "statement");
        $$=new Node(sInfo,"statements : statement");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($1->getEndLine()); 
        pTree.setCurrentNode($$);
        $$->makeChild({$1});
        util.printGrammar("statements : statement");
  
}
    | statements statement {

        SymbolInfo *sInfo = new SymbolInfo("", "statement");
        $$=new Node(sInfo,"statements : statements statement");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($2->getEndLine());
        pTree.setCurrentNode($$);
        $$->makeChild({$1,$2});
        util.printGrammar("statements : statements statement"); 
 
}
;
statement : var_declaration {

        SymbolInfo *sInfo= new SymbolInfo("", "statement");
        $$=new Node(sInfo,"statement : var_declaration",$1->getReturnOrDataType());
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($1->getEndLine());
        pTree.setCurrentNode($$);
        $$->makeChild({$1});
        util.printGrammar("statement : var_declaration");
      
}
    | expression_statement {

        SymbolInfo *sInfo = new SymbolInfo("", "statement");
        $$=new Node(sInfo,"statement : expression_statement",$1->getReturnOrDataType());
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($1->getEndLine());
        pTree.setCurrentNode($$);
        $$->makeChild({$1});
        util.printGrammar("statement : expression_statement");
     
}
    | compound_statement {

        SymbolInfo *sInfo = new SymbolInfo("", "statement");
        $$=new Node(sInfo,"statement : compound_statement",$1->getReturnOrDataType());
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($1->getEndLine());
        pTree.setCurrentNode($$);
        $$->makeChild({$1});
        util.printGrammar("statement : compound_statement");
   
        
}
    | FOR LPAREN expression_statement expression_statement expression RPAREN statement {

        SymbolInfo *sInfo = new SymbolInfo("", "statement");
        $$=new Node(sInfo,"statement : FOR LPAREN expression_statement expression_statement expression RPAREN statement");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($7->getEndLine());
        pTree.setCurrentNode($$);
        $$->makeChild({$1,$2,$3,$4,$5,$6,$7});
        util.printGrammar("statement : FOR LPAREN expression_statement expression_statement expression RPAREN statement");
 
}
    | IF LPAREN expression RPAREN statement %prec THEN {

        SymbolInfo *sInfo = new SymbolInfo("", "statement");
        $$=new Node(sInfo,"statement : IF LPAREN expression RPAREN statement");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($5->getEndLine());
        pTree.setCurrentNode($$);
        $$->makeChild({$1,$2,$3,$4,$5});
        util.printGrammar("statement : IF LPAREN expression RPAREN statement");
     
}
    | IF LPAREN expression RPAREN statement ELSE statement {

        SymbolInfo *sInfo = new SymbolInfo("", "statement");
        $$=new Node(sInfo,"statement : IF LPAREN expression RPAREN statement ELSE statement");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($7->getEndLine());
        pTree.setCurrentNode($$);
        $$->makeChild({$1,$2,$3,$4,$5,$6,$7});
        util.printGrammar("statement : IF LPAREN expression RPAREN statement ELSE statement");
 
}
    | WHILE LPAREN expression RPAREN statement {
        SymbolInfo *sInfo= new SymbolInfo("", "statement");
        $$=new Node(sInfo,"statement : WHILE LPAREN expression RPAREN statement");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($5->getEndLine());
        pTree.setCurrentNode($$);
        $$->makeChild({$1,$2,$3,$4,$5});
        util.printGrammar("statement : WHILE LPAREN expression RPAREN statement");  
 
}
    | PRINTLN LPAREN ID RPAREN SEMICOLON {
        SymbolInfo *sInfo = new SymbolInfo("", "statement");
        $$=new Node(sInfo,"statement : PRINTLN LPAREN ID RPAREN SEMICOLON");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($5->getEndLine());
        CheckVariableDeclaredOrNot($3);
        pTree.setCurrentNode($$);
        $$->makeChild({$1,$2,$3,$4,$5});
        util.printGrammar("statement : PRINTLN LPAREN ID RPAREN SEMICOLON");
}
    | RETURN expression SEMICOLON {

        SymbolInfo *sInfo = new SymbolInfo("", "statement");
        $$=new Node(sInfo,"statement : RETURN expression SEMICOLON");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($3->getEndLine()); 
        CheckReturnIssues($2);
        pTree.setCurrentNode($$);
        $$->makeChild({$1,$2,$3});
        util.printGrammar("statement : RETURN expression SEMICOLON");  
 
}
;
expression_statement : SEMICOLON {

        SymbolInfo *sInfo = new SymbolInfo("", "expression_statement");
        $$=new Node(sInfo,"expression_statement : SEMICOLON");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($1->getEndLine());
        pTree.setCurrentNode($$);
        $$->makeChild({$1});
        util.printGrammar("expression_statement : SEMICOLON"); 
     
}
	| expression SEMICOLON {

        SymbolInfo *sInfo = new SymbolInfo("", "expression_statement");
        $$=new Node(sInfo,"expression_statement : expression SEMICOLON");  
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($2->getEndLine());
        pTree.setCurrentNode($$);
        $$->makeChild({$1,$2});
        util.printGrammar("expression_statement : expression SEMICOLON"); 
       
}
;
variable : ID {

        SymbolInfo *sInfo = new SymbolInfo($1->getName(), "VARIABLE");
        CheckVariableDeclaredOrNot($1);
        $$=new Node(sInfo,"variable : ID",$1->getReturnOrDataType()); 
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($1->getEndLine());
        pTree.setCurrentNode($$);
        $$->makeChild({$1});
        util.printGrammar("variable : ID");
  
        		
}
    | ID LSQUARE expression RSQUARE {

        SymbolInfo *sInfo = new SymbolInfo($1->getName(), "VARIABLE");
        $$=new Node(sInfo,"variable : ID LSQUARE expression RSQUARE",$1->getReturnOrDataType());
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($4->getEndLine()); 
        CheckArrayIssues($$,$1,$3);
        pTree.setCurrentNode($$);
        $$->makeChild({$1,$2,$3,$4});
        util.printGrammar("variable : ID LSQUARE expression RSQUARE");     
      
}
;
expression : logic_expression {
 
        SymbolInfo *sInfo = new SymbolInfo($1->getName(), "expression");
        $$=new Node(sInfo,"expression : logic_expression",$1->getReturnOrDataType());
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($1->getEndLine());
        $$->setArrayStatus($1->getArrayStatus());
        pTree.setCurrentNode($$);
        $$->makeChild({$1});
        util.printGrammar("expression : logic_expression");
        
}
    | variable ASSIGNOP logic_expression {

        SymbolInfo *sInfo = new SymbolInfo("", "expression");
        $$=new Node(sInfo,"expression : variable ASSIGNOP logic_expression");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($3->getEndLine());
        CheckAssignmentIssues($$,$1,$3);
        pTree.setCurrentNode($$);
        $$->makeChild({$1,$2,$3});
        util.printGrammar("expression : variable ASSIGNOP logic_expression");
 
}
;
logic_expression : rel_expression {

        SymbolInfo *sInfo = new SymbolInfo($1->getName(), "logic_expression");
        $$=new Node(sInfo,"logic_expression : rel_expression",$1->getReturnOrDataType());
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($1->getEndLine());
        $$->setArrayStatus($1->getArrayStatus());
        pTree.setCurrentNode($$);
        $$->makeChild({$1});
        util.printGrammar("logic_expression : rel_expression");
 
}
    | rel_expression LOGICOP rel_expression {

		SymbolInfo *sInfo = new SymbolInfo("", "logic_expression");
        CheckLogicalIssues($$,$1,$3);
        $$=new Node(sInfo,"logic_expression : rel_expression LOGICOP rel_expression");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($3->getEndLine());
        pTree.setCurrentNode($$);
        $$->makeChild({$1,$2,$3});
        util.printGrammar("logic_expression : rel_expression LOGICOP rel_expression");

}
;
rel_expression : simple_expression {
 
        SymbolInfo *sInfo = new SymbolInfo($1->getName(), "rel_expression");
        $$=new Node(sInfo,"rel_expression : simple_expression",$1->getReturnOrDataType());
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($1->getEndLine());
        $$->setArrayStatus($1->getArrayStatus());
        pTree.setCurrentNode($$);
        $$->makeChild({$1});
        util.printGrammar("rel_expression : simple_expression");
       
}
    | simple_expression RELOP simple_expression {

        SymbolInfo *sInfo= new SymbolInfo("", "rel_expression");
        $$=new Node(sInfo,"rel_expression : simple_expression RELOP simple_expression");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($3->getEndLine());
        CheckRelationalIssues($$,$1,$3);
        pTree.setCurrentNode($$);
        $$->makeChild({$1,$2,$3});
        util.printGrammar("rel_expression : simple_expression RELOP simple_expression");

}
;
simple_expression : term {

        SymbolInfo *sInfo = new SymbolInfo($1->getName(), "simple_expression");
        $$=new Node(sInfo,"simple_expression : term",$1->getReturnOrDataType());
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($1->getEndLine());
        $$->setArrayStatus($1->getArrayStatus());
        pTree.setCurrentNode($$);
        $$->makeChild({$1});
        util.printGrammar("simple_expression : term");
       
}
	| simple_expression ADDOP term {

        SymbolInfo *sInfo = new SymbolInfo("", "simple_expression");
        $$=new Node(sInfo,"simple_expression : simple_expression ADDOP term",TypeCasting($1->getReturnOrDataType(), $3->getReturnOrDataType()));
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($3->getEndLine());
        if ($1->getReturnOrDataType() == "VOID" || $3->getReturnOrDataType() == "VOID") {
			util.printError("Void cannot be used in expression", $$->getStartLine());
		}
        pTree.setCurrentNode($$);
        $$->makeChild({$1,$2,$3});
        util.printGrammar("simple_expression : simple_expression ADDOP term");
	
}
;
term : unary_expression {

       SymbolInfo *sInfo = new SymbolInfo($1->getName(), "term");
        $$=new Node(sInfo,"term : unary_expression",$1->getReturnOrDataType());
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($1->getEndLine());
        $$->setArrayStatus($1->getArrayStatus());
        pTree.setCurrentNode($$);
        $$->makeChild({$1});
        util.printGrammar("term : unary_expression");
       
}
    | term MULOP unary_expression {

        SymbolInfo *sInfo = new SymbolInfo("", "term");  
        $$=new Node(sInfo,"term : term MULOP unary_expression");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($3->getEndLine());
        CheckMultiplicationIssues($$,$1,$2,$3);
        pTree.setCurrentNode($$);
        $$->makeChild({$1,$2,$3});
        util.printGrammar("term : term MULOP unary_expression");  

}
;
unary_expression : ADDOP unary_expression {

        SymbolInfo *sInfo = new SymbolInfo("", "unary_expression");
        $$=new Node(sInfo,"unary_expression : ADDOP unary_expression");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($2->getEndLine());
        if ($2->getReturnOrDataType() == "VOID") {
                util.printError("Void cannot be used in expression", $$->getStartLine());
            }
        else {$$->setReturnOrDataType($2->getReturnOrDataType());}
        pTree.setCurrentNode($$);
        $$->makeChild({$1,$2});
        util.printGrammar("unary_expression : ADDOP unary_expression");

		
}
    | NOT unary_expression {

        SymbolInfo *sInfo = new SymbolInfo("", "unary_expression");
        $$=new Node(sInfo,"unary_expression : NOT unary_expression");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($2->getEndLine());
        CheckUnaryExpression($$,$2);
        pTree.setCurrentNode($$);
        $$->makeChild({$1,$2});
        util.printGrammar("unary_expression : NOT unary_expression");
 

}
    | factor {

        SymbolInfo *sInfo = new SymbolInfo($1->getName(), "unary_expression");
        $$=new Node(sInfo,"unary_expression : factor",$1->getReturnOrDataType());
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($1->getEndLine());
        $$->setArrayStatus($1->getArrayStatus());
        pTree.setCurrentNode($$);
        $$->makeChild({$1});
        util.printGrammar("unary_expression : factor");
       
}
;


factor : variable {

        SymbolInfo *sInfo = new SymbolInfo($1->getName(), "factor");
        $$=new Node(sInfo,"factor : variable",$1->getReturnOrDataType());
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($1->getEndLine());
        $$->setArrayStatus($1->getArrayStatus());
        pTree.setCurrentNode($$);
        $$->makeChild({$1});
        util.printGrammar("factor : variable");

        
}
    | ID LPAREN argument_list RPAREN {

        SymbolInfo *sInfo = new SymbolInfo("", "factor");
        $$=new Node(sInfo,"factor : ID LPAREN argument_list RPAREN");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($4->getEndLine());
        callFunction($$,$1,$3);
        pTree.setCurrentNode($$);
        $$->makeChild({$1,$2,$3,$4});
        util.printGrammar("factor : ID LPAREN argument_list RPAREN");

}

    | LPAREN expression RPAREN {

        SymbolInfo *sInfo = new SymbolInfo($2->getName(), "factor");
        $$=new Node(sInfo,"factor : LPAREN expression RPAREN",$2->getReturnOrDataType());
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($2->getEndLine());
        pTree.setCurrentNode($$);
        $$->makeChild({$1,$2,$3});
        util.printGrammar("factor : LPAREN expression RPAREN");
        
}
    | CONST_INT {

        SymbolInfo *sInfo = new SymbolInfo($1->getName(), "factor");	
        $$=new Node(sInfo,"factor : CONST_INT","INT");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($1->getEndLine());
        pTree.setCurrentNode($$);
        $$->makeChild({$1});
        util.printGrammar("factor : CONST_INT");

}
    | CONST_FLOAT {

        SymbolInfo *sInfo = new SymbolInfo($1->getName(), "factor");
        $$=new Node(sInfo,"factor : CONST_FLOAT","FLOAT");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($1->getEndLine());
        pTree.setCurrentNode($$);
        $$->makeChild({$1});
        util.printGrammar("factor : CONST_FLOAT");


}
    | variable INCOP {

        SymbolInfo *sInfo = new SymbolInfo("", "factor");
        $$=new Node(sInfo,"factor : variable INCOP");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($2->getEndLine());
        CheckUnaryOperandIssues($$,$1);
        pTree.setCurrentNode($$);
        $$->makeChild({$1,$2});
        util.printGrammar("factor : variable INCOP");

}
    | variable DECOP {

        SymbolInfo *sInfo= new SymbolInfo("", "factor");
        $$=new Node(sInfo,"factor : variable DECOP");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($2->getEndLine());
        CheckUnaryOperandIssues($$,$1);
        pTree.setCurrentNode($$);
        $$->makeChild({$1,$2});
        util.printGrammar("factor : variable DECOP");

}
;

argument_list : arguments {
	
        SymbolInfo *sInfo = new SymbolInfo("", "argument_list");
        $$=new Node(sInfo,"argument_list : arguments");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($1->getEndLine());
        $$->addParameter($1->getParameterList());
        pTree.setCurrentNode($$);
        $$->makeChild({$1});
        util.printGrammar("argument_list : arguments");	

}
    | {
        SymbolInfo *sInfo= new SymbolInfo("", "argument_list");
        $$=new Node(sInfo,"argument_list : ");
        util.printGrammar("argument_list : ");
}
;
arguments : arguments COMMA logic_expression {

        SymbolInfo *sInfo = new SymbolInfo("", "arguments");
        $$=new Node(sInfo,"arguments : arguments COMMA logic_expression");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($3->getEndLine());
        $$->addParameter($1->getParameterList());
        $$->addParameter({{$3->getReturnOrDataType(),$3->getName()},$3->getArrayStatus()});
        pTree.setCurrentNode($$);
        $$->makeChild({$1,$2,$3});
        util.printGrammar("arguments : arguments COMMA logic_expression");

}
    | logic_expression {

        SymbolInfo *sInfo = new SymbolInfo("", "arguments");
        $$=new Node(sInfo,"arguments : logic_expression");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($1->getEndLine());
        $$->addParameter({{$1->getReturnOrDataType(),$1->getName()},$1->getArrayStatus()});
        pTree.setCurrentNode($$);
        $$->makeChild({$1});
        util.printGrammar("arguments : logic_expression");

}
;

%%

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE* fin = fopen(argv[1], "r");
    if (fin == NULL) {
        fprintf(stderr, "Cannot open specified file: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    yyin = fin;
    yyparse();
    fclose(yyin);

    util.getLogFout()<<"Total Lines: "<<yylineno<<endl;
    util.getLogFout()<<"Total Errors: "<<util.getErrorCount()<<endl;
    return EXIT_SUCCESS;
}