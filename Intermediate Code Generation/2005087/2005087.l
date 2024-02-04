%option noyywrap 
%option yylineno

%{
#include <bits/stdc++.h>
#include "2005087_SymbolTable.cpp"
#include "2005087_Node.h"
#include "y.tab.h"
using namespace std;
void yyerror(char *s);
extern YYSTYPE yylval;
int startLineNo;


%}
WHITESPACE [ \n\t\f\r\v]+ 
DIGIT [0-9]
DIGITS {DIGIT}+
FLOAT {DIGITS}(\.{DIGITS})?([eE][-+]?{DIGITS})?
IDENTIFIER [a-zA-Z_][a-zA-Z0-9_]*

%%

"if" {
        yylval.node=new Node(new SymbolInfo(yytext,"IF"));
        yylval.node->setStartLine(yylineno);
        yylval.node->setEndLine(yylineno);
        return IF;
     }
"else" {
        yylval.node=new Node(new SymbolInfo(yytext,"ELSE"));
        yylval.node->setStartLine(yylineno);
        yylval.node->setEndLine(yylineno);
        return ELSE;
     }
"for" {
        yylval.node=new Node(new SymbolInfo(yytext,"FOR"));
        yylval.node->setStartLine(yylineno);
        yylval.node->setEndLine(yylineno);
        return FOR;
     }
"while" {
        yylval.node=new Node(new SymbolInfo(yytext,"WHILE"));
        yylval.node->setStartLine(yylineno);
        yylval.node->setEndLine(yylineno);
        return WHILE;
     }
"do" {
        yylval.node=new Node(new SymbolInfo(yytext,"DO"));
        yylval.node->setStartLine(yylineno);
        yylval.node->setEndLine(yylineno);
        return DO;
     }
"break" {
        yylval.node=new Node(new SymbolInfo(yytext,"BREAK"));
        yylval.node->setStartLine(yylineno);
        yylval.node->setEndLine(yylineno);
        return BREAK;
     }
"int" {
        yylval.node=new Node(new SymbolInfo(yytext,"INT"));
        yylval.node->setStartLine(yylineno);
        yylval.node->setEndLine(yylineno);
        return INT;
     }
"char" {
        yylval.node=new Node(new SymbolInfo(yytext,"CHAR"));
        yylval.node->setStartLine(yylineno);
        yylval.node->setEndLine(yylineno);
        return CHAR;
     }
"float" {
        yylval.node=new Node(new SymbolInfo(yytext,"FLOAT"));
        yylval.node->setStartLine(yylineno);
        yylval.node->setEndLine(yylineno);
        return FLOAT;
     }
"double" {
        yylval.node=new Node(new SymbolInfo(yytext,"DOUBLE"));
        yylval.node->setStartLine(yylineno);
        yylval.node->setEndLine(yylineno);
        return DOUBLE;
     }
"void" {
        yylval.node=new Node(new SymbolInfo(yytext,"VOID"));
        yylval.node->setStartLine(yylineno);
        yylval.node->setEndLine(yylineno);
        return VOID;
     }
"return" {
        yylval.node=new Node(new SymbolInfo(yytext,"RETURN"));
        yylval.node->setStartLine(yylineno);
        yylval.node->setEndLine(yylineno);
        return RETURN;  
     }
"switch" {
        yylval.node=new Node(new SymbolInfo(yytext,"SWITCH"));
        yylval.node->setStartLine(yylineno);
        yylval.node->setEndLine(yylineno);
        return SWITCH;
     }
"case" {
        yylval.node=new Node(new SymbolInfo(yytext,"CASE"));
        yylval.node->setStartLine(yylineno);
        yylval.node->setEndLine(yylineno);
        return CASE;
     }
"default" {
        yylval.node=new Node(new SymbolInfo(yytext,"DEFAULT"));
        yylval.node->setStartLine(yylineno);
        yylval.node->setEndLine(yylineno);
        return DEFAULT;
     }
"continue" {
        yylval.node=new Node(new SymbolInfo(yytext,"CONTINUE"));
        yylval.node->setStartLine(yylineno);
        yylval.node->setEndLine(yylineno);
        return CONTINUE;
     }
"println" {
        yylval.node=new Node(new SymbolInfo(yytext,"PRINTLN"));
        yylval.node->setStartLine(yylineno);
        yylval.node->setEndLine(yylineno);
        return PRINTLN;
     }


{IDENTIFIER} {
        yylval.node=new Node(new SymbolInfo(yytext,"ID"));
        yylval.node->setStartLine(yylineno);
        yylval.node->setEndLine(yylineno);
        return ID;
}

{DIGITS} {
        yylval.node=new Node(new SymbolInfo(yytext,"CONST_INT"),"","int");
        yylval.node->setStartLine(yylineno);
        yylval.node->setEndLine(yylineno);
        return CONST_INT;
}

{FLOAT} {
        yylval.node=new Node(new SymbolInfo(yytext,"CONST_FLOAT"),"","float");
        yylval.node->setStartLine(yylineno);
        yylval.node->setEndLine(yylineno);
        return CONST_FLOAT;
}

"+"|"-" {
        yylval.node=new Node(new SymbolInfo(yytext,"ADDOP")); 
        yylval.node->setStartLine(yylineno);
        yylval.node->setEndLine(yylineno);
        return ADDOP;
}
"*"|"/"|"%" {
        yylval.node=new Node(new SymbolInfo(yytext,"MULOP")); 
        yylval.node->setStartLine(yylineno);
        yylval.node->setEndLine(yylineno);
        return MULOP;
}
"++" {
        yylval.node=new Node(new SymbolInfo(yytext,"INCOP")); 
        yylval.node->setStartLine(yylineno);
        yylval.node->setEndLine(yylineno);
        return INCOP;}
"--" {
        yylval.node=new Node(new SymbolInfo(yytext,"DECOP")); 
        yylval.node->setStartLine(yylineno);
        yylval.node->setEndLine(yylineno);
        return DECOP;   
}
"<"|">"|"<="|">="|"=="|"!=" {
        yylval.node=new Node(new SymbolInfo(yytext,"RELOP")); 
        yylval.node->setStartLine(yylineno);
        yylval.node->setEndLine(yylineno);
        return RELOP;
}
"=" {
        yylval.node=new Node(new SymbolInfo(yytext,"ASSIGNOP")); 
        yylval.node->setStartLine(yylineno);
        yylval.node->setEndLine(yylineno);
        return ASSIGNOP;}
"&&"|"||" {
        yylval.node=new Node(new SymbolInfo(yytext,"LOGICOP"));
        yylval.node->setStartLine(yylineno);
        yylval.node->setEndLine(yylineno); 
        return LOGICOP;}
"&"|"|"|"^"|"<<"|">>" {
        yylval.node=new Node(new SymbolInfo(yytext,"BITOP")); 
        yylval.node->setStartLine(yylineno);
        yylval.node->setEndLine(yylineno);
        return BITOP;}
"!" {
        yylval.node=new Node(new SymbolInfo(yytext,"NOT")); 
        yylval.node->setStartLine(yylineno);
        yylval.node->setEndLine(yylineno);
        return NOT;} 


    
"(" {
        yylval.node=new Node(new SymbolInfo(yytext,"LPAREN")); 
        yylval.node->setStartLine(yylineno);
        yylval.node->setEndLine(yylineno);
        return LPAREN;
}
")" {
        yylval.node=new Node(new SymbolInfo(yytext,"RPAREN")); 
        yylval.node->setStartLine(yylineno);
        yylval.node->setEndLine(yylineno);
        return RPAREN;
}
"{" {
        yylval.node=new Node(new SymbolInfo(yytext,"LCURL")); 
        yylval.node->setStartLine(yylineno);
        yylval.node->setEndLine(yylineno);
        return LCURL;
}
"}" {
        yylval.node=new Node(new SymbolInfo(yytext,"RCURL")); 
        yylval.node->setStartLine(yylineno);
        yylval.node->setEndLine(yylineno);
        return RCURL;
}
"[" {
        yylval.node=new Node(new SymbolInfo(yytext,"LSQUARE")); 
        yylval.node->setStartLine(yylineno);
        yylval.node->setEndLine(yylineno);
        return LSQUARE;
}
"]" {
        yylval.node=new Node(new SymbolInfo(yytext,"RSQUARE"));
        yylval.node->setStartLine(yylineno);
        yylval.node->setEndLine(yylineno); 
        return RSQUARE;
 }
"," {
        yylval.node=new Node(new SymbolInfo(yytext,"COMMA")); 
        yylval.node->setStartLine(yylineno);
        yylval.node->setEndLine(yylineno);
        return COMMA;
}
";" {
        yylval.node=new Node(new SymbolInfo(yytext,"SEMICOLON")); 
        yylval.node->setStartLine(yylineno);
        yylval.node->setEndLine(yylineno);
        return SEMICOLON;
 }
{WHITESPACE} {
    // Skip whitespace
}
<<EOF>> {return 0;}
. {
        cout<<"UNRECOGNIZED_CHAR"<<endl;
}
%%
