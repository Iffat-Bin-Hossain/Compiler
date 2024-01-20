%option noyywrap 
%option yylineno
%x SLCOMMENT
%x MLCOMMENT
%x STRING
%x INDENTATION

%{
#include<bits/stdc++.h>
#include "2005087_SymbolTable.cpp"
#include "2005087_Utility.cpp"
#include "2005087_Node.h"
#include "y.tab.h"
using namespace std;
void yyerror(char *s);
extern SymbolTable sTable;
extern Utility util;
extern YYSTYPE yylval;
int startLineNo;
int tabNeeded=0;
int tabGot=0;
bool tabDeleted=false;

std::string singlelineComment = "";
std::string multilineComment = "";
std::string String="";
std::string stringToken="";

    char specialCharacter(char c){
    if (c == 'n') return '\n';
	else if (c == 't') return '\t';
	else if (c == 'a') return '\a';
	else if (c == 'f') return '\f';
	else if (c == 'b') return '\b';
	else if (c == 'v') return '\v';
	else if (c == 'r') return '\r';
	else if (c == '0') return '\0';
    else if (c=='\"') return '\"';
    else if (c=='\'') return '\'';
    else if (c == '\\') return '\\';
	else return c;
    }
  
    string currentCharacter(string s){
    string ret = "";
    
    for (int i = 0; i <s.size(); i++) {
        if (s[i] != '\\') {
            ret += s[i];
        } 
        else {
                char c = specialCharacter(s[i + 1]);
                ret += c;
                i++;
        }
    }
    return ret;
}

%}

SPACE [ ]
NEWLINE [\n]
TAB [\t]
WHITESPACE [ \t\f\r\v]+ 
SPECIAL [ntafbvr0\'\"\\]
DIGIT [0-9]
DIGITS {DIGIT}+
FLOAT {DIGITS}(\.{DIGITS})?([eE][-+]?{DIGITS})?
CHAR ((\\{SPECIAL})|([^\\\'\n\r]))
IDENTIFIER [a-zA-Z_][a-zA-Z0-9_]*
INVALID_ID [0-9][_a-zA-Z0-9]+
TOO_MANY_DECIMAL_POINTS ([-+]?({DIGIT}+\.{DIGIT}+\.{DIGIT}+)+([eE][-+]?{DIGIT}*)?)
ILLFORMED_NUMBER {DIGITS}(\.[0-9]*)?[Ee][-+\.0-9]+[-+Ee\.0-9]*



%%
{NEWLINE} {
    tabDeleted=false;
    BEGIN(INDENTATION); 
}
<INDENTATION>[ \t]*(\r)?\n {

}
<INDENTATION>{TAB} {
    tabGot++;
}
<INDENTATION>{SPACE}+ {
    util.printWarning(": Warning, tab requrired but got space.", yylineno);
}

<INDENTATION>[^\t\n] {
    int len = strlen(yytext);
    char lastChar = yytext[len - 1];
    if(lastChar=='}') {
        tabNeeded--;
        tabDeleted=true;
    }
    yyless(len - 1);
    if (tabGot != tabNeeded) {
        util.printWarning(": warning, "+std::to_string(tabNeeded) + " of tabs needed but got " + std::to_string(tabGot) + " tabs.", yylineno);
    }
    tabGot = 0;
    BEGIN(INITIAL);  
}   
"if" {
        util.printKeyword(yytext,yylineno);
        yylval.node=new Node(new SymbolInfo(yytext,"IF"));
        return IF;
     }
"else" {
        util.printKeyword(yytext,yylineno);
        yylval.node=new Node(new SymbolInfo(yytext,"ELSE"));
        return ELSE;
     }
"for" {
        util.printKeyword(yytext,yylineno);
        yylval.node=new Node(new SymbolInfo(yytext,"FOR"));
        return FOR;
     }
"while" {
        util.printKeyword(yytext,yylineno);
        yylval.node=new Node(new SymbolInfo(yytext,"WHILE"));
        return WHILE;
     }
"do" {
            util.printKeyword(yytext,yylineno);
        yylval.node=new Node(new SymbolInfo(yytext,"DO"));
        return DO;
     }
"break" {
            util.printKeyword(yytext,yylineno);
        yylval.node=new Node(new SymbolInfo(yytext,"BREAK"));
        return BREAK;
     }
"int" {
            util.printKeyword(yytext,yylineno);
        yylval.node=new Node(new SymbolInfo(yytext,"INT"));
        return INT;
     }
"char" {
            util.printKeyword(yytext,yylineno);
        yylval.node=new Node(new SymbolInfo(yytext,"CHAR"));
        return CHAR;
     }
"float" {
            util.printKeyword(yytext,yylineno);
        yylval.node=new Node(new SymbolInfo(yytext,"FLOAT"));
        return FLOAT;
     }
"double" {
            util.printKeyword(yytext,yylineno);
        yylval.node=new Node(new SymbolInfo(yytext,"DOUBLE"));
        return DOUBLE;
     }
"void" {
            util.printKeyword(yytext,yylineno);
        yylval.node=new Node(new SymbolInfo(yytext,"VOID"));
        return VOID;
     }
"return" {
            util.printKeyword(yytext,yylineno);
        yylval.node=new Node(new SymbolInfo(yytext,"RETURN"));
        return RETURN;  
     }
"switch" {
            util.printKeyword(yytext,yylineno);
        yylval.node=new Node(new SymbolInfo(yytext,"SWITCH"));
        return SWITCH;
     }
"case" {
            util.printKeyword(yytext,yylineno);
        yylval.node=new Node(new SymbolInfo(yytext,"CASE"));
        return CASE;
     }
"default" {
            util.printKeyword(yytext,yylineno);
        yylval.node=new Node(new SymbolInfo(yytext,"DEFAULT"));
        return DEFAULT;
     }
"continue" {
            util.printKeyword(yytext,yylineno);
        yylval.node=new Node(new SymbolInfo(yytext,"CONTINUE"));
        return CONTINUE;
     }
"println" {
            util.printKeyword(yytext,yylineno);
        yylval.node=new Node(new SymbolInfo(yytext,"PRINTLN"));
        return PRINTLN;
     }


{IDENTIFIER} {
util.printAnother("ID",yytext,yylineno);
yylval.node=new Node(new SymbolInfo(yytext,"ID"));
return ID;
}

{DIGITS} {
    util.printAnother("CONST_INT",yytext,yylineno); 
    yylval.node=new Node(new SymbolInfo(yytext,"CONST_INT"),"int");
    return CONST_INT;
}

{FLOAT} {
    util.printAnother("CONST_FLOAT",yytext,yylineno);
    yylval.node=new Node(new SymbolInfo(yytext,"CONST_FLOAT"),"float");
    return CONST_FLOAT;
}

\'{CHAR}*\' {
    string current=yytext;
    current.erase(current.begin());
    current.pop_back(); 
    current=currentCharacter(current);
    if(current.length()==0){
        util.printError("EMPTY_CONST_CHAR","\'"+current+"\'",yylineno);
    }
    else if(current.length()==1){
        util.printAnother("CONST_CHAR",current,yylineno);
    }
    else{
         util.printError("MULTICHAR_CONST_CHAR","\'"+ current+"\'", yylineno);
    }

}

\'{CHAR}? {
string current = yytext;
current.erase(current.begin()); 
util.printError("UNFINISHED_CONST_CHAR", "\'" + current,yylineno);
}


"+"|"-" {
util.printOperator("ADDOP",yytext,yylineno);
yylval.node=new Node(new SymbolInfo(yytext,"ADDOP")); 
return ADDOP;
}
"*"|"/"|"%"	{
util.printOperator("MULOP",yytext,yylineno);
yylval.node=new Node(new SymbolInfo(yytext,"MULOP")); 
return MULOP;
}
"++" {util.printOperator("INCOP",yytext,yylineno);
yylval.node=new Node(new SymbolInfo(yytext,"INCOP")); 
return INCOP;}
"--" {
util.printOperator("DECOP",yytext,yylineno);
yylval.node=new Node(new SymbolInfo(yytext,"DECOP")); 
return DECOP;   
}
"<"|">"|"<="|">="|"=="|"!=" {util.printOperator("RELOP",yytext,yylineno);
yylval.node=new Node(new SymbolInfo(yytext,"RELOP")); 
return RELOP;
}
"=" {util.printOperator("ASSIGNOP",yytext,yylineno);
yylval.node=new Node(new SymbolInfo(yytext,"ASSIGNOP")); 
return ASSIGNOP;}
"&&"|"||" {util.printOperator("LOGICOP",yytext,yylineno);
yylval.node=new Node(new SymbolInfo(yytext,"LOGICOP")); 
return LOGICOP;}
"&"|"|"|"^"|"<<"|">>" {
util.printOperator("BITOP",yytext,yylineno);
yylval.node=new Node(new SymbolInfo(yytext,"BITOP")); 
return BITOP;}
"!" {util.printOperator("NOT",yytext,yylineno); 
yylval.node=new Node(new SymbolInfo(yytext,"NOT")); 
return NOT;} 


    
"(" {
        util.printPunctuation(yytext,yylineno);
        yylval.node=new Node(new SymbolInfo(yytext,"LPAREN")); 
        return LPAREN;
}
")" {
        util.printPunctuation(yytext,yylineno);
        yylval.node=new Node(new SymbolInfo(yytext,"RPAREN")); 
        return RPAREN;
}
"{" {
        util.printPunctuation(yytext,yylineno);
        yylval.node=new Node(new SymbolInfo(yytext,"LCURL")); 
        tabNeeded++;
        return LCURL;
}
"}" {
        util.printPunctuation(yytext,yylineno);
        yylval.node=new Node(new SymbolInfo(yytext,"RCURL")); 
        if(!tabDeleted){
            tabNeeded--;
        }
        return RCURL;
}
"[" {
        util.printPunctuation(yytext,yylineno);
        yylval.node=new Node(new SymbolInfo(yytext,"LSQUARE")); 
        return LSQUARE;
}
"]" {
        util.printPunctuation(yytext,yylineno);
        yylval.node=new Node(new SymbolInfo(yytext,"RSQUARE")); 
        return RSQUARE;
 }
"," {
        util.printPunctuation(yytext,yylineno);
        yylval.node=new Node(new SymbolInfo(yytext,"COMMA")); 
        return COMMA;
}
";" {
        util.printPunctuation(yytext,yylineno);
        yylval.node=new Node(new SymbolInfo(yytext,"SEMICOLON")); 
        return SEMICOLON;
 }


\" {
    BEGIN(STRING);
    startLineNo = yylineno;
}

<STRING><<EOF>> {
    util.printError("UNFINISHED_STRING", "\"" + String, yylineno-1);
    String = "";
    stringToken="";
    BEGIN(INITIAL);
}

<STRING>\\\" {
String += yytext;
stringToken+=yytext;
}

<STRING>\\((\r)?\n) {
    String += yytext;
}

<STRING>((\r)?\n) {
    util.printError("UNFINISHED_STRING", "\"" + String, yylineno-1);
    String = "";
    stringToken="";
    BEGIN(INITIAL);
}

<STRING>\" {
    if (startLineNo == yylineno){
util.printAnother("SINGLE LINE STRING", "\"" + String + "\"", startLineNo);
    }
        
    else{
 util.printAnother("MULTI LINE STRING", "\"" + String + "\"", startLineNo);
    }
        
    String = "";
    stringToken="";
    BEGIN(INITIAL);
}

<STRING>. {
    String += yytext;
    stringToken+=yytext;
}

"//" {
BEGIN(SLCOMMENT);
startLineNo= yylineno;
}

<SLCOMMENT>\n {
    BEGIN(INITIAL);
    singlelineComment = singlelineComment.substr(0, singlelineComment.length());
    util.printSingleLineComment(singlelineComment, startLineNo);
    singlelineComment = "";
}

<SLCOMMENT>.*(\\(\r)?\n.*)* {
    singlelineComment += yytext;
}

"/*" {
    BEGIN(MLCOMMENT);
	startLineNo= yylineno;
}

<MLCOMMENT><<EOF>> {
    BEGIN(INITIAL);
    util.printError("UNFINISHED_COMMENT","/*"+multilineComment, yylineno);
    multilineComment="";
}

<MLCOMMENT>"*/" {
    BEGIN(INITIAL);
    util.printMultiLineComment(multilineComment, startLineNo);
    multilineComment = "";
}

<MLCOMMENT>([^*]|\n)+|. {
    multilineComment+=yytext;
}


{TOO_MANY_DECIMAL_POINTS} {
    util.printError("TOO_MANY_DECIMAL_POINTS", yytext, yylineno);
}
{INVALID_ID} {
    util.printError("INVALID_ID_SUFFIX_NUM_PREFIX", yytext, yylineno);
}
{ILLFORMED_NUMBER} {
    util.printError("ILLFORMED_NUMBER",yytext,yylineno);
}
{WHITESPACE} {
    // Skip whitespace
}
<<EOF>> {return 0;}
. {
   util.printError("UNRECOGNIZED_CHAR", yytext, yylineno);
}
%%
