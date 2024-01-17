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
SymbolTable sTable(10);
Utility util;


%}

%union {
    Node *node;
}

%token<node> IF ELSE SWITCH CASE BREAK DEFAULT FOR WHILE DO CONTINUE     
%token<node> MAIN VOID INT FLOAT DOUBLE CHAR ID RETURN PRINTLN
%token<node> CONST_INT CONST_FLOAT CONST_CHAR 
%token<node> NOT LPAREN RPAREN LCURL RCURL LSQUARE RSQUARE COMMA SEMICOLON
%token<node> ADDOP MULOP INCOP DECOP RELOP LOGICOP BITOP ASSIGNOP 
%type<node> start program unit var_declaration func_declaration func_definition type_specifier parameter_list compound_statement statements declaration_list statement expression expression_statement logic_expression rel_expression simple_expression term unary_expression factor variable argument_list arguments lcurls
%nonassoc THEN
%nonassoc ELSE


%%

start : program {
    SymbolInfo *sInfo=new SymbolInfo("","start");
    $$ =new Node(sInfo);
    pTree.setCurrentNode($$);
    pTree.addChildren({$1});
    util.printGrammar("start : program");
    
}
;   
program : program unit{
    SymbolInfo *sInfo=new SymbolInfo("","program");
    $$=new Node(sInfo);
    pTree.setCurrentNode($$);
    pTree.addChildren({$1,$2});
    util.printGrammar("program : program unit");
    
}
    | unit {
    SymbolInfo *sInfo=new SymbolInfo("","program");
    $$=new Node(sInfo);
    pTree.setCurrentNode($$);
    pTree.addChildren({$1});
    util.printGrammar("program : unit");
}
;
unit : var_declaration {
    SymbolInfo *sInfo=new SymbolInfo("","unit");
    $$=new Node(sInfo);
    pTree.setCurrentNode($$);
    pTree.addChildren({$1});
    util.printGrammar("unit : var_declaration");
}
    | func_declaration {
    SymbolInfo *sInfo=new SymbolInfo("","unit");
    $$=new Node(sInfo);
    pTree.setCurrentNode($$);
    pTree.addChildren({$1});
    util.printGrammar("unit : func_declaration");
}
    | func_definition {
		SymbolInfo *sInfo = new SymbolInfo("", "unit");
        $$=new Node(sInfo);
        pTree.setCurrentNode($$);
        pTree.addChildren({$1});
		util.printGrammar("unit : func_definition");
}
;
func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON {
        SymbolInfo *sInfo = new SymbolInfo("", "func_declaration");
        $$=new Node(sInfo);
        pTree.setCurrentNode($$);
        pTree.addChildren({$1,$2,$3,$4,$5,$6});
        util.printGrammar("func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON");
}
    | type_specifier ID LPAREN RPAREN SEMICOLON {
        SymbolInfo *sInfo = new SymbolInfo("", "func_declaration");
        $$=new Node(sInfo);
        pTree.setCurrentNode($$);
        pTree.addChildren({$1,$2,$3,$4,$5});
        util.printGrammar("func_declaration : type_specifier ID LPAREN RPAREN SEMICOLON");
}
;
func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement {
    SymbolInfo *sInfo = new SymbolInfo("", "func_definition");
    $$=new Node(sInfo);
    pTree.setCurrentNode($$);
    pTree.addChildren({$1,$2,$3,$4,$5,$6});
    util.printGrammar("func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement");
    }
       

    | type_specifier ID LPAREN RPAREN compound_statement {
    SymbolInfo *sInfo = new SymbolInfo("", "func_definition");
    $$=new Node(sInfo);
    pTree.setCurrentNode($$);
    pTree.addChildren({$1,$2,$3,$4,$5});
    util.printGrammar("func_definition : type_specifier ID LPAREN RPAREN compound_statement");
}
;
parameter_list : parameter_list COMMA type_specifier ID {
       SymbolInfo *sInfo = new SymbolInfo("", "parameter_list");
       $$=new Node(sInfo);
        pTree.setCurrentNode($$);
        pTree.addChildren({$1,$2,$3,$4});
        util.printGrammar("parameter_list : parameter_list COMMA type_specifier ID");
}
    | parameter_list COMMA type_specifier {
        SymbolInfo *sInfo = new SymbolInfo("", "parameter_list");
        $$=new Node(sInfo);
        pTree.setCurrentNode($$);
        pTree.addChildren({$1,$2,$3});
        util.printGrammar("parameter_list : parameter_list COMMA type_specifier");
}
    | type_specifier ID{
        SymbolInfo *sInfo = new SymbolInfo("", "parameter_list");
        $$=new Node(sInfo);
        pTree.setCurrentNode($$);
        pTree.addChildren({$1,$2});
        util.printGrammar("parameter_list : type_specifier ID");
}
    | type_specifier {
        SymbolInfo *sInfo = new SymbolInfo("", "parameter_list");
        $$=new Node(sInfo);
        pTree.setCurrentNode($$);
        pTree.addChildren({$1});
        util.printGrammar("parameter_list : type_specifier");
}
;
compound_statement : lcurls statements RCURL {
    SymbolInfo *sInfo = new SymbolInfo("", "compound_statement");
    $$=new Node(sInfo);
    pTree.setCurrentNode($$);
    pTree.addChildren({$1,$2,$3});
    util.printGrammar("compound_statement : lcurls statements RCURL");
}
    | lcurls RCURL {
    SymbolInfo *sInfo = new SymbolInfo("", "compound_statement");
    $$=new Node(sInfo);
    pTree.setCurrentNode($$);
    pTree.addChildren({$1,$2});
    util.printGrammar("compound_statement : lcurls RCURL");
    }
    ;
var_declaration : type_specifier declaration_list SEMICOLON {
   SymbolInfo *sInfo = new SymbolInfo("", "var_declaration");
   $$=new Node(sInfo,$1->getDataType());
    pTree.setCurrentNode($$);
    pTree.addChildren({$1,$2,$3});
    util.printGrammar("var_declaration : type_specifier declaration_list SEMICOLON");
}
;
type_specifier : INT {
    SymbolInfo *sInfo = new SymbolInfo("", "type_specifier");
    $$=new Node(sInfo,"int");
    pTree.setCurrentNode($$);
    pTree.addChildren({$1});
    
    util.printGrammar("type_specifier : INT");
    
}
    | FLOAT {
        SymbolInfo *sInfo = new SymbolInfo("", "type_specifier");
        $$=new Node(sInfo,"float");
        pTree.setCurrentNode($$);
        pTree.addChildren({$1});
        util.printGrammar("type_specifier : FLOAT");
        
}
    | VOID {
        SymbolInfo *sInfo = new SymbolInfo("", "type_specifier");
        $$=new Node(sInfo,"void");
        pTree.setCurrentNode($$);
        pTree.addChildren({$1});
        util.printGrammar("type_specifier : VOID");
        
}
;
declaration_list : declaration_list COMMA ID {
        SymbolInfo *sInfo= new SymbolInfo("", "declaration_list");
        $$=new Node(sInfo);
        pTree.setCurrentNode($$);
        pTree.addChildren({$1,$2,$3});
        util.printGrammar("declaration_list : declaration_list COMMA ID");
}
    | declaration_list COMMA ID LSQUARE CONST_INT RSQUARE {
        SymbolInfo *sInfo = new SymbolInfo("", "declaration_list");
        $$=new Node(sInfo);
        pTree.setCurrentNode($$);
        pTree.addChildren({$1,$2,$3,$4,$5,$6});
        util.printGrammar("declaration_list : declaration_list COMMA ID LSQUARE CONST_INT RSQUARE");
}
    | ID {
       SymbolInfo *sInfo = new SymbolInfo("", "declaration_list");
       $$=new Node(sInfo);
        pTree.setCurrentNode($$);
        pTree.addChildren({$1});
        util.printGrammar("declaration_list : ID");   
}
    | ID LSQUARE CONST_INT RSQUARE {
        SymbolInfo *sInfo= new SymbolInfo("", "declaration_list");
        $$=new Node(sInfo);
        pTree.setCurrentNode($$);
        pTree.addChildren({$1,$2,$3,$4});
        util.printGrammar("declaration_list : ID LSQUARE CONST_INT RSQUARE"); 
}
;
statements : statement {
        SymbolInfo *sInfo = new SymbolInfo("", "statement");
        $$=new Node(sInfo);
        pTree.setCurrentNode($$);
        pTree.addChildren({$1});
        util.printGrammar("statements : statement"); 
}
    | statements statement {
       SymbolInfo *sInfo = new SymbolInfo("", "statement");
       $$=new Node(sInfo);
       pTree.setCurrentNode($$);
       pTree.addChildren({$1,$2});
       util.printGrammar("statements : statements statement"); 
}
;
statement : var_declaration {
       SymbolInfo *sInfo= new SymbolInfo("", "statement");
       $$=new Node(sInfo,$1->getDataType());
       pTree.setCurrentNode($$);
       pTree.addChildren({$1});
       util.printGrammar("statement : var_declaration");
       
}
    | expression_statement {
    SymbolInfo *sInfo = new SymbolInfo("", "statement");
    $$=new Node(sInfo,$1->getDataType());
    pTree.setCurrentNode($$);
    pTree.addChildren({$1});
    util.printGrammar("statement : expression_statement");
   
}
    | compound_statement {
        SymbolInfo *sInfo = new SymbolInfo("", "statement");
        $$=new Node(sInfo,$1->getDataType());
        pTree.setCurrentNode($$);
        pTree.addChildren({$1});
        util.printGrammar("statement : compound_statement");
        
}
    | FOR LPAREN expression_statement expression_statement expression RPAREN statement {
        SymbolInfo *sInfo = new SymbolInfo("", "statement");
        $$=new Node(sInfo);
        pTree.setCurrentNode($$);
        pTree.addChildren({$1,$2,$3,$4,$5,$6,$7});
        util.printGrammar("statement : FOR LPAREN expression_statement expression_statement expression RPAREN statement");
}
    | IF LPAREN expression RPAREN statement %prec THEN {
        SymbolInfo *sInfo = new SymbolInfo("", "statement");
        $$=new Node(sInfo);
        pTree.setCurrentNode($$);
        pTree.addChildren({$1,$2,$3,$4,$5});
        util.printGrammar("statement : IF LPAREN expression RPAREN statement");
}
    | IF LPAREN expression RPAREN statement ELSE statement {
        SymbolInfo *sInfo = new SymbolInfo("", "statement");
        $$=new Node(sInfo);
        pTree.setCurrentNode($$);
        pTree.addChildren({$1,$2,$3,$4,$5,$6,$7});
        util.printGrammar("statement : IF LPAREN expression RPAREN statement ELSE statement");
}
    | WHILE LPAREN expression RPAREN statement {
        SymbolInfo *sInfo= new SymbolInfo("", "statement");
        $$=new Node(sInfo);
        pTree.setCurrentNode($$);
        pTree.addChildren({$1,$2,$3,$4,$5});
        util.printGrammar("statement : WHILE LPAREN expression RPAREN statement");  
}
    | PRINTLN LPAREN ID RPAREN SEMICOLON {
        SymbolInfo *sInfo = new SymbolInfo("", "statement");
        $$=new Node(sInfo);
        pTree.setCurrentNode($$);
        pTree.addChildren({$1,$2,$3,$4,$5});
        util.printGrammar("statement : PRINTLN LPAREN ID RPAREN SEMICOLON");  
}
    | RETURN expression SEMICOLON {
        SymbolInfo *sInfo = new SymbolInfo("", "statement");
        $$=new Node(sInfo);
        pTree.setCurrentNode($$);
        pTree.addChildren({$1,$2,$3});
        util.printGrammar("statement : RETURN expression SEMICOLON");  
}
;
expression_statement : SEMICOLON {
        SymbolInfo *sInfo = new SymbolInfo("", "expression_statement");
        $$=new Node(sInfo);
        pTree.setCurrentNode($$);
        pTree.addChildren({$1});
        util.printGrammar("expression_statement : SEMICOLON"); 
}
	| expression SEMICOLON {
      SymbolInfo *sInfo = new SymbolInfo("", "expression_statement");
      $$=new Node(sInfo);  
      pTree.setCurrentNode($$);
      pTree.addChildren({$1,$2});
      util.printGrammar("expression_statement : expression SEMICOLON"); 
      
}
;
variable : ID {
        SymbolInfo *sInfo = new SymbolInfo($1->getName(), "VARIABLE");
        $$=new Node(sInfo,$1->getDataType());
        pTree.setCurrentNode($$);
        pTree.addChildren({$1});
        util.printGrammar("variable : ID");
        		
}
    | ID LSQUARE expression RSQUARE {
        SymbolInfo *sInfo = new SymbolInfo($1->getName(), "VARIABLE");
        $$=new Node(sInfo,$1->getDataType());
        pTree.setCurrentNode($$);
        pTree.addChildren({$1,$2,$3,$4});
        util.printGrammar("variable : ID LSQUARE expression RSQUARE");
}
;
expression : logic_expression {
        SymbolInfo *sInfo = new SymbolInfo($1->getName(), "expression");
        $$=new Node(sInfo,$1->getDataType());
        pTree.setCurrentNode($$);
        pTree.addChildren({$1});
        util.printGrammar("expression : logic_expression");
}
    | variable ASSIGNOP logic_expression {
        SymbolInfo *sInfo = new SymbolInfo("", "expression");
        $$=new Node(sInfo);
        pTree.setCurrentNode($$);
        pTree.addChildren({$1,$2,$3});
        util.printGrammar("expression : variable ASSIGNOP logic_expression");
}
;
logic_expression : rel_expression {
        SymbolInfo *sInfo = new SymbolInfo($1->getName(), "logic_expression");
        $$=new Node(sInfo);
        pTree.setCurrentNode($$);
        pTree.addChildren({$1});
        util.printGrammar("logic_expression : rel_expression");
}
    | rel_expression LOGICOP rel_expression {
		SymbolInfo *sInfo = new SymbolInfo("", "logic_expression");
        $$=new Node(sInfo);
        pTree.setCurrentNode($$);
        pTree.addChildren({$1,$2,$3});
        util.printGrammar("logic_expression : rel_expression LOGICOP rel_expression");
}
;
rel_expression : simple_expression {
        SymbolInfo *sInfo = new SymbolInfo($1->getName(), "rel_expression");
         $$=new Node(sInfo);
         pTree.setCurrentNode($$);
        pTree.addChildren({$1});
        util.printGrammar("rel_expression : simple_expression");
       
}
    | simple_expression RELOP simple_expression {
        SymbolInfo *sInfo= new SymbolInfo("", "rel_expression");
         $$=new Node(sInfo);
         pTree.setCurrentNode($$);
        pTree.addChildren({$1,$2,$3});
        util.printGrammar("rel_expression : simple_expression RELOP simple_expression");
}
;
simple_expression : term {
       SymbolInfo *sInfo = new SymbolInfo($1->getName(), "simple_expression");
       $$=new Node(sInfo,$1->getDataType());
        pTree.setCurrentNode($$);
        pTree.addChildren({$1});
        util.printGrammar("simple_expression : term");
        
}
	| simple_expression ADDOP term {
        SymbolInfo *sInfo = new SymbolInfo("", "simple_expression");
        $$=new Node(sInfo);
        pTree.setCurrentNode($$);
        pTree.addChildren({$1,$2,$3});
        util.printGrammar("simple_expression : simple_expression ADDOP term");
		
}
;
term : unary_expression {
       SymbolInfo *sInfo = new SymbolInfo($1->getName(), "term");
        $$=new Node(sInfo,$1->getDataType());
        pTree.setCurrentNode($$);
        pTree.addChildren({$1});
        util.printGrammar("term : unary_expression");
		
}
    | term MULOP unary_expression {
      SymbolInfo *sInfo = new SymbolInfo("", "term");  
      $$=new Node(sInfo);
      pTree.setCurrentNode($$);
      pTree.addChildren({$1,$2,$3});
      util.printGrammar("term : term MULOP unary_expression");  
}
;
unary_expression : ADDOP unary_expression {
    SymbolInfo *sInfo = new SymbolInfo("", "unary_expression");
    $$=new Node(sInfo);
    pTree.setCurrentNode($$);
    pTree.addChildren({$1,$2});
    util.printGrammar("unary_expression : ADDOP unary_expression");
		
}
    | NOT unary_expression {
        SymbolInfo *sInfo = new SymbolInfo("", "unary_expression");
        $$=new Node(sInfo);
        pTree.setCurrentNode($$);
        pTree.addChildren({$1,$2});
        util.printGrammar("unary_expression : NOT unary_expression");

}
    | factor {
        SymbolInfo *sInfo = new SymbolInfo($1->getName(), "unary_expression");
        $$=new Node(sInfo,$1->getDataType());
        pTree.setCurrentNode($$);
        pTree.addChildren({$1});
        util.printGrammar("unary_expression : factor");
        
}
;
factor : variable {
       SymbolInfo *sInfo = new SymbolInfo($1->getName(), "factor");
       $$=new Node(sInfo,$1->getDataType());
        pTree.setCurrentNode($$);
        pTree.addChildren({$1});
        util.printGrammar("factor : variable");
        
}
    | ID LPAREN argument_list RPAREN {
        SymbolInfo *sInfo = new SymbolInfo("", "factor");
        $$=new Node(sInfo);
        pTree.setCurrentNode($$);
        pTree.addChildren({$1,$2,$3,$4});
        util.printGrammar("factor : ID LPAREN argument_list RPAREN");
}
    | LPAREN expression RPAREN {
        SymbolInfo *sInfo = new SymbolInfo($2->getName(), "factor");
        $$=new Node(sInfo,$2->getDataType());
        pTree.setCurrentNode($$);
        pTree.addChildren({$1,$2,$3});
        util.printGrammar("factor : LPAREN expression RPAREN");
        
}
    | CONST_INT {
        SymbolInfo *sInfo = new SymbolInfo($1->getName(), "factor");
        util.printGrammar("factor : CONST_INT");	
        $$=new Node(sInfo,"INT");
        pTree.setCurrentNode($$);
        pTree.addChildren({$1});
}
    | CONST_FLOAT {
        SymbolInfo *sInfo = new SymbolInfo($1->getName(), "factor");
        util.printGrammar("factor : CONST_FLOAT");	
        $$=new Node(sInfo,"FLOAT");
        pTree.setCurrentNode($$);
        pTree.addChildren({$1});

}
    | variable INCOP {
        SymbolInfo *sInfo = new SymbolInfo("", "factor");
        $$=new Node(sInfo);
        pTree.setCurrentNode($$);
        pTree.addChildren({$1,$2});
        util.printGrammar("factor : variable INCOP");
		
}
    | variable DECOP {
        SymbolInfo *sInfo= new SymbolInfo("", "factor");
        $$=new Node(sInfo);
        pTree.setCurrentNode($$);
        pTree.addChildren({$1,$2});
        util.printGrammar("factor : variable DECOP");
}
;
argument_list : arguments {
       SymbolInfo *sInfo = new SymbolInfo("", "argument_list");
        $$=new Node(sInfo);
        pTree.setCurrentNode($$);
        pTree.addChildren({$1});
        util.printGrammar("argument_list : arguments");
    
		
}
    | {
        SymbolInfo *sInfo= new SymbolInfo("", "argument_list");
        $$=new Node(sInfo);
        util.printGrammar("argument_list : ");
}
;
arguments : arguments COMMA logic_expression {
    SymbolInfo *sInfo = new SymbolInfo("", "arguments");
    $$=new Node(sInfo);
    pTree.setCurrentNode($$);
    pTree.addChildren({$1,$2,$3});
    util.printGrammar("arguments : arguments COMMA logic_expression");
}
    | logic_expression {
        SymbolInfo *sInfo = new SymbolInfo("", "arguments");
        $$=new Node(sInfo);
        pTree.setCurrentNode($$);
        pTree.addChildren({$1});
        util.printGrammar("arguments : logic_expression");
}
;
lcurls : LCURL {
    $$=$1;
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

    sTable.PrintAllScope(util.getLogFout());
    util.getLogFout()<<"Total lines: "<<yylineno<<endl;
    util.getLogFout()<<"Total errors: "<<util.getErrorCount()<<endl;
    util.getLogFout()<<"Total warnings: "<<util.getWarningCount()<<endl;

    return EXIT_SUCCESS;
}