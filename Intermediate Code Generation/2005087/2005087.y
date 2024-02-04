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
            cout<<"Line# " << startLine <<": " << "redeclaration of " << functionName << std::endl;
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
                cout<<"Line# " << startLine <<": " << "Conflicting types for \'" << functionName << "\'"<< std::endl;
			}

			if (searched->getParameterList().size() != parameterList.size())
			{
				cout<<"Line# " << startLine <<": " << "Conflicting types for \'" << functionName << "\'"<< std::endl;
			}

			searched->setFunctionInfo("FUNCTION_DEFINITION");
			searched->addParameter(parameterList);
			searched->setReturnOrDataType(returnType);
		}
		else
		{
            cout<<"Line# " << startLine <<": \'" << functionName + "\' redeclared as different kind of symbol"<< std::endl;
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
        cout<<"Line# " << actualNode->getStartLine() <<": " << "Undeclared function \'" + nameNode->getName() + "\'"<< std::endl;
	}
	else if (!(searched->getFunctionInfo() == "FUNCTION_DECLARATION" || searched->getFunctionInfo() == "FUNCTION_DEFINITION"))
	{
        cout<<"Line# " << actualNode->getStartLine() <<": " <<"\'" + nameNode->getName() + "\' is not a function"<<std::endl;
	}
	else if (searched->getParameterList().size() < parameterList.size())
	{
        cout<<"Line# " << actualNode->getStartLine() <<": " <<"Too many arguments to function \'" + nameNode->getName() + "\'"<<std::endl;
		return;
	}
	else if (searched->getParameterList().size() > parameterList.size())
	{
        cout<<"Line# " << actualNode->getStartLine() <<": " <<"Too few arguments to function \'" + nameNode->getName() + "\'"<<std::endl;
		return;
	}
	else
	{
		for (int i = 0; i < searched->getParameterList().size(); i++)
		{

			
			if ((searched->getParameterList().at(i).first.first != parameterList[i].first.first))
			{
                cout<<"Line# " << actualNode->getStartLine() <<": " <<"Type mismatch for argument " + to_string(i + 1) + " of \'" + nameNode->getName() + "\'"<<std::endl;
				
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
            cout<<"Line# " << startLine <<": " <<"Variable or field \'" << element.first.second << "\' declared void"<<std::endl;
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
            cout<<"Line# " << startLine <<": " <<"Conflicting types for \'" + element.first.second + "\'"<<std::endl;
		}
		else
		{
            cout<<"Line# " << startLine <<": " <<"Redefinition of variable \'" + element.first.second + "\'"<<std::endl;
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
        cout<<"Line# " << node->getStartLine() <<": " <<"Undeclared variable \'" + variable + "\'"<<std::endl;
		return;
	}
	node->setReturnOrDataType(searched->getReturnOrDataType());
	node->setArrayStatus(searched->getArrayStatus());
}
void CheckReturnIssues(Node *node)
{
	if (currentFunctionReturnType == "INT" && node->getReturnOrDataType() == "FLOAT")
	{
        cout<<"Line# " << node->getStartLine() <<": " <<"Warning: possible loss of data in assignment of FLOAT to INT"<<std::endl;
	}
}
void CheckArrayIssues(Node *actualNode, Node *nameNode, Node *indexNode)
{
	string name = nameNode->getName();
	Node *toBeSearched = new Node(new SymbolInfo(name, "ID"));
	Node *searched = sTable.Lookup(toBeSearched);
	if (searched == NULL)
	{
        cout<<"Line# " << actualNode->getStartLine() <<": " <<"Undeclared variable \'" + name + "\'"<<std::endl;
		return;
	}
	if (!searched->getArrayStatus())
	{
        cout<<"Line# " << actualNode->getStartLine() <<": " <<"\'" + name + "\' is not an array"<<std::endl;
		return;
	}
	if (indexNode->getReturnOrDataType() != "INT")
	{
        cout<<"Line# " << actualNode->getStartLine() <<": " <<"Array subscript is not an integer"<<std::endl;
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
        cout<<"Line# " << actualNode->getStartLine() <<": " <<"Type mismatch for \'" + leftNode->getName() + "\' , is an array"<<std::endl;
		return;
	}
	if (leftNode->getReturnOrDataType() == "VOID" || rightNode->getReturnOrDataType() == "VOID")
	{
        cout<<"Line# " << actualNode->getStartLine() <<": " <<"Void cannot be used in expression"<<std::endl;
		return;
	}
	if (leftNode->getReturnOrDataType() == "INT")
	{
		if (rightNode->getReturnOrDataType() == "FLOAT")
		{
            cout<<"Line# " << actualNode->getStartLine() <<": " <<"Warning: possible loss of data in assignment of FLOAT to INT"<<std::endl;
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
        cout<<"Line# " << actualNode->getStartLine() <<": " <<"Void cannot be used in expression"<<std::endl;
		return;
	}
	if (leftNode->getReturnOrDataType() == "FLOAT" || rightNode->getReturnOrDataType() == "FLOAT")
	{
        cout<<"Line# " << actualNode->getStartLine() <<": " <<"Warning: float cannot be used in expression"<<std::endl;
		actualNode->setReturnOrDataType("INT");
	}
	actualNode->setReturnOrDataType("INT");
}
void CheckRelationalIssues(Node *actualNode, Node *leftNode, Node *rightNode)
{
	if (leftNode->getReturnOrDataType() == "VOID" || rightNode->getReturnOrDataType() == "VOID")
	{
        cout<<"Line# " << actualNode->getStartLine() <<": " <<"Void cannot be used in expression"<<std::endl;
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
string TypeCasting(string s1, const string s2)
{

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
        cout<<"Line# " << actualNode->getStartLine() <<": " <<"Void cannot be used in expression"<<std::endl;
	}

	else if (midNode->getName() == "%")
	{
		if (leftNode->getReturnOrDataType() == "FLOAT" || rightNode->getReturnOrDataType() == "FLOAT")
		{
            cout<<"Line# " << actualNode->getStartLine() <<": " <<"Operands of modulus must be integers"<<std::endl;
		}
		else if (CheckingZerOperand(rightNode->getName()))
		{
            cout<<"Line# " << actualNode->getStartLine() <<": " <<"Warning: division by zero i=0f=1Const=0"<<std::endl;
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
            cout<<"Line# " << actualNode->getStartLine() <<": " <<"Warning: division by zero i=0f=1Const=0"<<std::endl;
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
        cout<<"Line# " << actualNode->getStartLine() <<": " <<"Void cannot be used in expression"<<std::endl;
		return;
	}
	else if (node->getReturnOrDataType() == "FLOAT")
	{
        cout<<"Line# " << actualNode->getStartLine() <<": " <<"Warning :Operands of bitwise operation should be integers"<<std::endl;
	}
	actualNode->setReturnOrDataType("INT");
}
void CheckUnaryOperandIssues(Node *actualNode, Node *operand)
{
	if (operand->getReturnOrDataType() == "VOID")
	{
        cout<<"Line# " << actualNode->getStartLine() <<": " <<"Void cannot be used in expression"<<std::endl;
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
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($1->getEndLine());
        $$->makeChild({$1});
        pTree.setCurrentNode($$);
    
}
;   
program : program unit{
    
        SymbolInfo *sInfo=new SymbolInfo("","program");
        $$=new Node(sInfo,"program : program unit");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($2->getEndLine());
        $$->makeChild({$1,$2});
     
    
}
    | unit {
   
        SymbolInfo *sInfo=new SymbolInfo("","program");
        $$=new Node(sInfo,"program : unit");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($1->getEndLine());
        $$->makeChild({$1});

   
}  
;
unit : var_declaration {
   
        SymbolInfo *sInfo=new SymbolInfo("","unit");
        $$=new Node(sInfo,"unit : var_declaration");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($1->getEndLine());
        $$->makeChild({$1});
    
}
    | func_declaration {
    
        SymbolInfo *sInfo=new SymbolInfo("","unit");
        $$=new Node(sInfo,"unit : func_declaration");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($1->getEndLine());
        $$->makeChild({$1});

   
}
    | func_definition {
        
		SymbolInfo *sInfo = new SymbolInfo("", "unit");
        $$=new Node(sInfo,"unit : func_definition");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($1->getEndLine());
        $$->makeChild({$1});

       
}
;
func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON {
     
    currentFunctionParameters.clear();
        SymbolInfo *sInfo = new SymbolInfo("", "func_declaration");
        $$=new Node(sInfo,"func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($6->getEndLine());
        $$->makeChild({$1,$2,$3,$4,$5,$6});
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
        $$->makeChild({$1,$2,$3,$4,$5});
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
        $$->makeChild({$1,$2,$3,$4,$5,$7});
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
        $$->makeChild({$1,$2,$3,$4,$6});
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
        $$->makeChild({$1,$2,$3,$4});
   
}
    | parameter_list COMMA type_specifier {

        SymbolInfo *sInfo = new SymbolInfo("", "parameter_list");
        $$=new Node(sInfo,"parameter_list : parameter_list COMMA type_specifier");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($3->getEndLine());
        $$->addParameter($1->getParameterList());
        $$->addParameter({{$3->getReturnOrDataType(),""},false});
        currentFunctionParameters=$$->getParameterList();
        $$->makeChild({$1,$2,$3});
       
}
    | type_specifier ID{

        SymbolInfo *sInfo = new SymbolInfo("", "parameter_list");
        $$=new Node(sInfo,"parameter_list : type_specifier ID");
        $$->addParameter({{$1->getReturnOrDataType(),$2->getName()},false});
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($2->getEndLine());
        currentFunctionParameters=$$->getParameterList();
        $$->makeChild({$1,$2});
  
}
    | type_specifier {

        SymbolInfo *sInfo = new SymbolInfo("", "parameter_list");
        $$=new Node(sInfo,"parameter_list : type_specifier");
        $$->addParameter({{$1->getReturnOrDataType(),""},false});
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($1->getEndLine());
        currentFunctionParameters=$$->getParameterList();
        $$->makeChild({$1});
    
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
                cout<<"Line# " << $1->getStartLine() <<": " <<"Redefinition of parameter \'"+toBeInserted->getName() +"\'"<<std::endl;
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
        $$->makeChild({$1,$3,$4}); 
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
                cout<<"Line# " << $1->getStartLine() <<": " <<"Redefinition of parameter \'"+toBeInserted->getName() +"\'"<<std::endl;
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
        $$->makeChild({$1,$3}); 
        sTable.ExitScope();
     
    }
    ;
var_declaration : type_specifier declaration_list SEMICOLON {

        SymbolInfo *sInfo = new SymbolInfo("", "var_declaration");
        $$=new Node(sInfo,"var_declaration : type_specifier declaration_list SEMICOLON",$1->getReturnOrDataType());
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($3->getEndLine());
        SaveData($1->getReturnOrDataType(),$2,$$->getStartLine());
        $$->makeChild({$1,$2,$3});
     
}
;

type_specifier : INT {

        SymbolInfo *sInfo = new SymbolInfo("", "type_specifier");
        $$=new Node(sInfo,"type_specifier : INT","INT");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($1->getEndLine());
        $$->makeChild({$1});
       
}
    | FLOAT {

        SymbolInfo *sInfo = new SymbolInfo("", "type_specifier");
        $$=new Node(sInfo,"type_specifier : FLOAT","FLOAT");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($1->getEndLine());
        $$->makeChild({$1});
        
}
    | VOID {

        SymbolInfo *sInfo = new SymbolInfo("", "type_specifier");
        $$=new Node(sInfo,"type_specifier : VOID","VOID");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($1->getEndLine());
        $$->makeChild({$1});
        
}
;
declaration_list : declaration_list COMMA ID {

        SymbolInfo *sInfo= new SymbolInfo("", "declaration_list");
        $$=new Node(sInfo,"declaration_list : declaration_list COMMA ID");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($3->getEndLine());
        $$->addParameter($1->getParameterList());
        $$->addParameter({{"",$3->getName()},false});
        $$->makeChild({$1,$2,$3});

      
}
    | declaration_list COMMA ID LSQUARE CONST_INT RSQUARE {

        SymbolInfo *sInfo = new SymbolInfo("", "declaration_list");
        $$=new Node(sInfo,"declaration_list : declaration_list COMMA ID LSQUARE CONST_INT RSQUARE");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($6->getEndLine());
        $$->addParameter($1->getParameterList());
        $$->addParameter({{"",$3->getName()},true});
        $$->setArrayStatus(true);
        $$->makeChild({$1,$2,$3,$4,$5,$6});
 
}
    | ID {

        SymbolInfo *sInfo = new SymbolInfo("", "declaration_list");
        $$=new Node(sInfo,"declaration_list : ID");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($1->getEndLine()); 
        $$->addParameter({{$1->getReturnOrDataType(),$1->getName()},false});
        $$->makeChild({$1}); 
 
}
    | ID LSQUARE CONST_INT RSQUARE {

        SymbolInfo *sInfo= new SymbolInfo("", "declaration_list");
        $$=new Node(sInfo,"declaration_list : ID LSQUARE CONST_INT RSQUARE");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($4->getEndLine());
        $$->addParameter({{$1->getReturnOrDataType(),$1->getName()},true});
        $$->makeChild({$1,$2,$3,$4});
 

}
;
statements : statement {

        SymbolInfo *sInfo = new SymbolInfo("", "statement");
        $$=new Node(sInfo,"statements : statement");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($1->getEndLine()); 
        $$->makeChild({$1});

  
}
    | statements statement {

        SymbolInfo *sInfo = new SymbolInfo("", "statement");
        $$=new Node(sInfo,"statements : statements statement");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($2->getEndLine());
        $$->makeChild({$1,$2});
 
}
;
statement : var_declaration {

        SymbolInfo *sInfo= new SymbolInfo("", "statement");
        $$=new Node(sInfo,"statement : var_declaration",$1->getReturnOrDataType());
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($1->getEndLine());
        $$->makeChild({$1});
      
}
    | expression_statement {

        SymbolInfo *sInfo = new SymbolInfo("", "statement");
        $$=new Node(sInfo,"statement : expression_statement",$1->getReturnOrDataType());
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($1->getEndLine());
        $$->makeChild({$1});
     
}
    | compound_statement {

        SymbolInfo *sInfo = new SymbolInfo("", "statement");
        $$=new Node(sInfo,"statement : compound_statement",$1->getReturnOrDataType());
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($1->getEndLine());
        $$->makeChild({$1});
   
        
}
    | FOR LPAREN expression_statement expression_statement expression RPAREN statement {

        SymbolInfo *sInfo = new SymbolInfo("", "statement");
        $$=new Node(sInfo,"statement : FOR LPAREN expression_statement expression_statement expression RPAREN statement");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($7->getEndLine());
        $$->makeChild({$1,$2,$3,$4,$5,$6,$7});
 
}
    | IF LPAREN expression RPAREN statement %prec THEN {

        SymbolInfo *sInfo = new SymbolInfo("", "statement");
        $$=new Node(sInfo,"statement : IF LPAREN expression RPAREN statement");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($5->getEndLine());
        $$->makeChild({$1,$2,$3,$4,$5});
     
}
    | IF LPAREN expression RPAREN statement ELSE statement {

        SymbolInfo *sInfo = new SymbolInfo("", "statement");
        $$=new Node(sInfo,"statement : IF LPAREN expression RPAREN statement ELSE statement");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($7->getEndLine());
        $$->makeChild({$1,$2,$3,$4,$5,$6,$7});
 
}
    | WHILE LPAREN expression RPAREN statement {

        SymbolInfo *sInfo= new SymbolInfo("", "statement");
        $$=new Node(sInfo,"statement : WHILE LPAREN expression RPAREN statement");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($5->getEndLine());
        $$->makeChild({$1,$2,$3,$4,$5});  
 
}
    | PRINTLN LPAREN ID RPAREN SEMICOLON {

        SymbolInfo *sInfo = new SymbolInfo("", "statement");
        $$=new Node(sInfo,"statement : PRINTLN LPAREN ID RPAREN SEMICOLON");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($5->getEndLine());
        CheckVariableDeclaredOrNot($3);
        $$->makeChild({$1,$2,$3,$4,$5});
}
    | RETURN expression SEMICOLON {

        SymbolInfo *sInfo = new SymbolInfo("", "statement");
        $$=new Node(sInfo,"statement : RETURN expression SEMICOLON");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($3->getEndLine()); 
        CheckReturnIssues($2);
        $$->makeChild({$1,$2,$3});  
 
}
;
expression_statement : SEMICOLON {

        SymbolInfo *sInfo = new SymbolInfo("", "expression_statement");
        $$=new Node(sInfo,"expression_statement : SEMICOLON");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($1->getEndLine());
        $$->makeChild({$1}); 
     
}
	| expression SEMICOLON {

        SymbolInfo *sInfo = new SymbolInfo("", "expression_statement");
        $$=new Node(sInfo,"expression_statement : expression SEMICOLON");  
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($2->getEndLine());
        $$->makeChild({$1,$2});
       
}
;
variable : ID {

        SymbolInfo *sInfo = new SymbolInfo($1->getName(), "VARIABLE");
        CheckVariableDeclaredOrNot($1);
        $$=new Node(sInfo,"variable : ID",$1->getReturnOrDataType()); 
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($1->getEndLine());
        $$->makeChild({$1});
         		
}
    | ID LSQUARE expression RSQUARE {

        SymbolInfo *sInfo = new SymbolInfo($1->getName(), "VARIABLE");
        $$=new Node(sInfo,"variable : ID LSQUARE expression RSQUARE",$1->getReturnOrDataType());
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($4->getEndLine()); 
        CheckArrayIssues($$,$1,$3);
        $$->makeChild({$1,$2,$3,$4});    
      
}
;
expression : logic_expression {
 
        SymbolInfo *sInfo = new SymbolInfo($1->getName(), "expression");
        $$=new Node(sInfo,"expression : logic_expression",$1->getReturnOrDataType());
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($1->getEndLine());
        $$->setArrayStatus($1->getArrayStatus());
        $$->makeChild({$1});
        
}
    | variable ASSIGNOP logic_expression {

        SymbolInfo *sInfo = new SymbolInfo("", "expression");
        $$=new Node(sInfo,"expression : variable ASSIGNOP logic_expression");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($3->getEndLine());
        CheckAssignmentIssues($$,$1,$3);
        $$->makeChild({$1,$2,$3});
 
}
;
logic_expression : rel_expression {

        SymbolInfo *sInfo = new SymbolInfo($1->getName(), "logic_expression");
        $$=new Node(sInfo,"logic_expression : rel_expression",$1->getReturnOrDataType());
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($1->getEndLine());
        $$->setArrayStatus($1->getArrayStatus());
        $$->makeChild({$1});
 
}
    | rel_expression LOGICOP rel_expression {

		SymbolInfo *sInfo = new SymbolInfo("", "logic_expression");
        CheckLogicalIssues($$,$1,$3);
        $$=new Node(sInfo,"logic_expression : rel_expression LOGICOP rel_expression");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($3->getEndLine());
        $$->makeChild({$1,$2,$3});

}
;
rel_expression : simple_expression {
 
        SymbolInfo *sInfo = new SymbolInfo($1->getName(), "rel_expression");
        $$=new Node(sInfo,"rel_expression : simple_expression",$1->getReturnOrDataType());
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($1->getEndLine());
        $$->setArrayStatus($1->getArrayStatus());
        $$->makeChild({$1});
       
}
    | simple_expression RELOP simple_expression {

        SymbolInfo *sInfo= new SymbolInfo("", "rel_expression");
        $$=new Node(sInfo,"rel_expression : simple_expression RELOP simple_expression");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($3->getEndLine());
        CheckRelationalIssues($$,$1,$3);
        $$->makeChild({$1,$2,$3});

}
;
simple_expression : term {

        SymbolInfo *sInfo = new SymbolInfo($1->getName(), "simple_expression");
        $$=new Node(sInfo,"simple_expression : term",$1->getReturnOrDataType());
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($1->getEndLine());
        $$->setArrayStatus($1->getArrayStatus());
        $$->makeChild({$1});
       
}
	| simple_expression ADDOP term {

        SymbolInfo *sInfo = new SymbolInfo("", "simple_expression");
        $$=new Node(sInfo,"simple_expression : simple_expression ADDOP term",TypeCasting($1->getReturnOrDataType(), $3->getReturnOrDataType()));
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($3->getEndLine());
        if ($1->getReturnOrDataType() == "VOID" || $3->getReturnOrDataType() == "VOID") {
            cout<<"Line# " << $$->getStartLine() <<": " <<"Void cannot be used in expression"<<std::endl;
		}
        $$->makeChild({$1,$2,$3});
	
}
;
term : unary_expression {

       SymbolInfo *sInfo = new SymbolInfo($1->getName(), "term");
        $$=new Node(sInfo,"term : unary_expression",$1->getReturnOrDataType());
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($1->getEndLine());
        $$->setArrayStatus($1->getArrayStatus());
        $$->makeChild({$1});
       
}
    | term MULOP unary_expression {

        SymbolInfo *sInfo = new SymbolInfo("", "term");  
        $$=new Node(sInfo,"term : term MULOP unary_expression");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($3->getEndLine());
        CheckMultiplicationIssues($$,$1,$2,$3);
        $$->makeChild({$1,$2,$3});

}
;
unary_expression : ADDOP unary_expression {

        SymbolInfo *sInfo = new SymbolInfo("", "unary_expression");
        $$=new Node(sInfo,"unary_expression : ADDOP unary_expression");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($2->getEndLine());
        if ($2->getReturnOrDataType() == "VOID") {
            cout<<"Line# " << $$->getStartLine() <<": " <<"Void cannot be used in expression"<<std::endl;
            }
        else {$$->setReturnOrDataType($2->getReturnOrDataType());}
        $$->makeChild({$1,$2});

		
}
    | NOT unary_expression {

        SymbolInfo *sInfo = new SymbolInfo("", "unary_expression");
        $$=new Node(sInfo,"unary_expression : NOT unary_expression");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($2->getEndLine());
        CheckUnaryExpression($$,$2);
        $$->makeChild({$1,$2});
 

}
    | factor {

        SymbolInfo *sInfo = new SymbolInfo($1->getName(), "unary_expression");
        $$=new Node(sInfo,"unary_expression : factor",$1->getReturnOrDataType());
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($1->getEndLine());
        $$->setArrayStatus($1->getArrayStatus());
        $$->makeChild({$1});
       
}
;


factor : variable {

        SymbolInfo *sInfo = new SymbolInfo($1->getName(), "factor");
        $$=new Node(sInfo,"factor : variable",$1->getReturnOrDataType());
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($1->getEndLine());
        $$->setArrayStatus($1->getArrayStatus());
        $$->makeChild({$1});
        
}
    | ID LPAREN argument_list RPAREN {

        SymbolInfo *sInfo = new SymbolInfo("", "factor");
        $$=new Node(sInfo,"factor : ID LPAREN argument_list RPAREN");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($4->getEndLine());
        callFunction($$,$1,$3);
        $$->makeChild({$1,$2,$3,$4});

}

    | LPAREN expression RPAREN {

        SymbolInfo *sInfo = new SymbolInfo($2->getName(), "factor");
        $$=new Node(sInfo,"factor : LPAREN expression RPAREN",$2->getReturnOrDataType());
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($2->getEndLine());
        $$->makeChild({$1,$2,$3});
        
}
    | CONST_INT {

        SymbolInfo *sInfo = new SymbolInfo($1->getName(), "factor");	
        $$=new Node(sInfo,"factor : CONST_INT","INT");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($1->getEndLine());
        $$->makeChild({$1});

}
    | CONST_FLOAT {

        SymbolInfo *sInfo = new SymbolInfo($1->getName(), "factor");
        $$=new Node(sInfo,"factor : CONST_FLOAT","FLOAT");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($1->getEndLine());
        $$->makeChild({$1});


}
    | variable INCOP {

        SymbolInfo *sInfo = new SymbolInfo("", "factor");
        $$=new Node(sInfo,"factor : variable INCOP");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($2->getEndLine());
        CheckUnaryOperandIssues($$,$1);
        $$->makeChild({$1,$2});

}
    | variable DECOP {

        SymbolInfo *sInfo= new SymbolInfo("", "factor");
        $$=new Node(sInfo,"factor : variable DECOP");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($2->getEndLine());
        CheckUnaryOperandIssues($$,$1);
        $$->makeChild({$1,$2});

}
;

argument_list : arguments {
	
        SymbolInfo *sInfo = new SymbolInfo("", "argument_list");
        $$=new Node(sInfo,"argument_list : arguments");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($1->getEndLine());
        $$->addParameter($1->getParameterList());
        $$->makeChild({$1});	

}
    | {
        SymbolInfo *sInfo= new SymbolInfo("", "argument_list");
        $$=new Node(sInfo,"argument_list : ");
}
;
arguments : arguments COMMA logic_expression {

        SymbolInfo *sInfo = new SymbolInfo("", "arguments");
        $$=new Node(sInfo,"arguments : arguments COMMA logic_expression");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($3->getEndLine());
        $$->addParameter($1->getParameterList());
        $$->addParameter({{$3->getReturnOrDataType(),$3->getName()},$3->getArrayStatus()});
        $$->makeChild({$1,$2,$3});

}
    | logic_expression {

        SymbolInfo *sInfo = new SymbolInfo("", "arguments");
        $$=new Node(sInfo,"arguments : logic_expression");
        $$->setStartLine($1->getStartLine());
        $$->setEndLine($1->getEndLine());
        $$->addParameter({{$1->getReturnOrDataType(),$1->getName()},$1->getArrayStatus()});
        $$->makeChild({$1});

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
    return EXIT_SUCCESS;
}