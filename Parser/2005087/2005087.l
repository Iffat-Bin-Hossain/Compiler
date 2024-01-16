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
#include "2005087_Node.cpp"
#include "y.tab.h"
using namespace std;

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

KEYWORD (if|else|for|while|do|break|int|char|float|double|void|return|switch|case|default|continue)
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
TOO_MANY_DECIMAL_POINTS ([-+]?({DIGIT}+\.{DIGIT}*)+([eE][-+]?{DIGIT}*)?)
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



{KEYWORD} {
    util.printKeyword(yytext,yylineno);
     if(yytext=="if"){
        yylval.node=new Node(new SymbolInfo(yytext,"IF"));
        return IF;
     }
     else if(yytext=="else"){
        yylval.node=new Node(new SymbolInfo(yytext,"ELSE"));
        return ELSE;
     }
     else if(yytext=="for"){
        yylval.node=new Node(new SymbolInfo(yytext,"FOR"));
        return FOR;
     }
     else if(yytext=="while"){
        yylval.node=new Node(new SymbolInfo(yytext,"WHILE"));
        return WHILE;
     }
     else if(yytext=="do"){
        yylval.node=new Node(new SymbolInfo(yytext,"DO"));
        return DO;
     }
     else if(yytext=="break"){
        yylval.node=new Node(new SymbolInfo(yytext,"BREAK"));
        return BREAK;
     }
     else if(yytext=="int"){
        yylval.node=new Node(new SymbolInfo(yytext,"INT"));
        return INT;
     }
     else if(yytext=="char"){
        yylval.node=new Node(new SymbolInfo(yytext,"CHAR"));
        return CHAR;
     }
     else if(yytext=="float"){
        yylval.node=new Node(new SymbolInfo(yytext,"FLOAT"));
        return FLOAT;
     }
     else if(yytext=="double"){
        yylval.node=new Node(new SymbolInfo(yytext,"DOUBLE"));
        return DOUBLE;
     }
     else if(yytext=="void"){
        yylval.node=new Node(new SymbolInfo(yytext,"VOID"));
        return VOID;
     }
     else if(yytext=="return"){
        yylval.node=new Node(new SymbolInfo(yytext,"RETURN"));
        return RETURN;  
     }
      else if(yytext=="switch"){
        yylval.node=new Node(new SymbolInfo(yytext,"SWITCH"));
        return SWITCH;
     }
      else if(yytext=="case"){
        yylval.node=new Node(new SymbolInfo(yytext,"CASE"));
        return CASE;
     }
      else if(yytext=="default"){
        yylval.node=new Node(new SymbolInfo(yytext,"DEFAULT"));
        return DEFAULT;
     }
     else if(yytext=="continue"){
        yylval.node=new Node(new SymbolInfo(yytext,"CONTINUE"));
        return CONTINUE;
     }
     else{
        yylval.node=new Node(new SymbolInfo(yytext,"PRINTLN"));
        return PRINTLN;
     }
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


"+"|"-" {util.printOperator("ADDOP",yytext,yylineno);
yylval.node=new Node(new SymbolInfo(yytext,"ADDOP")); 
return ADDOP;}
"*"|"/"|"%"	{util.printOperator("MULOP",yytext,yylineno);
yylval.node=new Node(new SymbolInfo(yytext,"MULOP")); 
return MULOP;}
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
return RELOP;}
"=" {util.printOperator("ASSIGNOP",yytext,yylineno);
yylval.node=new Node(new SymbolInfo(yytext,"ASSIGNOP")); 
return ASSIGNOP;}
"&&"|"||" {util.printOperator("LOGICOP",yytext,yylineno);
yylval.node=new Node(new SymbolInfo(yytext,"LOGICOP")); 
return LOGICOP;}
"&"|"|"|"^"|"<<"|">>" {util.printOperator("BITOP",yytext,yylineno);
yylval.node=new Node(new SymbolInfo(yytext,"BITOP")); 
return BITOP;}
"!" {util.printOperator("NOT",yytext,yylineno); 
yylval.node=new Node(new SymbolInfo(yytext,"NOT")); 
return NOT;} 


"("|")"|"{"|"}"|"["|"]"|","|";" {
    util.printPunctuation(yytext,yylineno);
    if(yytext[0]=='('){
        yylval.node=new Node(new SymbolInfo(yytext,"LPAREN")); 
        return LPAREN;
    }
    else if(yytext[0]==')'){
        yylval.node=new Node(new SymbolInfo(yytext,"RPAREN")); 
        return RPAREN;
    }
    else if(yytext[0]=='{'){
        yylval.node=new Node(new SymbolInfo(yytext,"LCURL")); 
        tabNeeded++;
        return LCURL;
    }
    else if(yytext[0]=='}'){
        yylval.node=new Node(new SymbolInfo(yytext,"RCURL")); 
        if(!tabDeleted){
            tabNeeded--;
        }
        return RCURL;
    }
    else if(yytext[0]=='['){
        yylval.node=new Node(new SymbolInfo(yytext,"LSQUARE")); 
        return LSQUARE;
    }
    else if(yytext[0]==']'){
        yylval.node=new Node(new SymbolInfo(yytext,"RSQUARE")); 
        return RSQUARE;
    }
    else if(yytext[0]==','){
        yylval.node=new Node(new SymbolInfo(yytext,"COMMA")); 
        return COMMA;
    }
    else{
        yylval.node=new Node(new SymbolInfo(yytext,"SEMICOLON")); 
        return SEMICOLON;
    }
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
