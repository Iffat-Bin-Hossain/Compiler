%option noyywrap 
%option yylineno
%x SLCOMMENT
%x MLCOMMENT
%x STRING

%{
#include<bits/stdc++.h>
#include "SymbolTable.cpp"
#include "LexerUtility.cpp"
using namespace std;

SymbolTable sTable(10);
LexerUtility lexUtil;
int startLineNo ;
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
    else if(c=='\"') return '\"';
    else if(c=='\'') return '\'';
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
  string ignoreNextLine(string s){
       string ret = ""; 
       int i=0;
       while(i<s.size()){
        if(s[i]=='\\' && s[i+1]=='\n'){
            i+=2;
        }
        else{
            ret+=s[i];
            i++;
        }
       }
       return ret;
    }
%}

KEYWORD (if|else|for|while|do|break|int|char|float|double|void|return|switch|case|default|continue)
WHITESPACE [\n\f\r\v]+ 
SPECIAL [ntafbvr0\'\"\\]
LETTER [a-zA-Z]
WORD {LETTER}+
DIGIT [0-9]
DIGITS {DIGIT}+
FLOAT {DIGITS}(\.{DIGITS})?([eE][-+]?{DIGITS})?
CHAR ((\\{SPECIAL})|([^\\\'\n\r]))
IDENTIFIER [a-zA-Z_][a-zA-Z0-9_]*
INVALID_ID [0-9][_a-zA-Z0-9]+
TOO_MANY_DECIMAL_POINTS ([-+]?({DIGIT}*\.{DIGIT}*)+([eE][-+]?{DIGIT}*)?)
ILLFORMED_NUMBER {DIGITS}(\.[0-9]*)?[Ee][-+\.0-9]+[-+Ee\.0-9]*



%%
{KEYWORD} {
    lexUtil.printKeyword(yytext,yylineno);
}

{IDENTIFIER} {
lexUtil.printAnother("ID",yytext,yylineno);
lexUtil.printToken("ID",yytext);
SymbolInfo *sInfo=new SymbolInfo(yytext,"ID");
if(sTable.Insert(sInfo)){
    sTable.PrintAllScope(lexUtil.getLogFout());
    }
else{
        lexUtil.getLogFout()<<'\t'<<yytext<<" already exists in the current ScopeTable"<<endl;
    }
}

{DIGITS} {
    lexUtil.printAnother("CONST_INT",yytext,yylineno); 
    lexUtil.printToken("CONST_INT",yytext);
}

{FLOAT} {
    lexUtil.printAnother("CONST_FLOAT",yytext,yylineno);
    lexUtil.printToken("CONST_FLOAT",yytext);
}

\'{CHAR}*\' {
    string current=yytext;
    current.erase(current.begin());
    current.pop_back(); 
    current=currentCharacter(current);
    if(current.length()==0){
        lexUtil.printError("EMPTY_CONST_CHAR","\'"+current+"\'",yylineno);
    }
    else if(current.length()==1){
        lexUtil.printAnother("CONST_CHAR",current,yylineno);
        lexUtil.printToken("CONST_CHAR",current);
    }
    else{
         lexUtil.printError("MULTICHAR_CONST_CHAR","\'"+ current+"\'", yylineno);
    }

}

\'{CHAR}? {
string current = yytext;
current.erase(current.begin()); 
//current = currentCharacter(current);
lexUtil.printError("UNFINISHED_CONST_CHAR", "\'" + current,yylineno);
}


"+"|"-" {lexUtil.printOperator("ADDOP",yytext,yylineno);}
"*"|"/"|"%"	{lexUtil.printOperator("MULOP",yytext,yylineno);}
"++"|"--" {lexUtil.printOperator("INCOP",yytext,yylineno);}
"<"|">"|"<="|">="|"=="|"!=" {lexUtil.printOperator("RELOP",yytext,yylineno);}
"=" {lexUtil.printOperator("ASSIGNOP",yytext,yylineno);}
"&&"|"||" {lexUtil.printOperator("LOGICOP",yytext,yylineno);}
"&"|"|"|"^"|"<<"|">>" {lexUtil.printOperator("BITOP",yytext,yylineno);}
"!" {lexUtil.printOperator("NOT",yytext,yylineno);}


"("|")"|"{"|"}"|"["|"]"|","|";" {
    if(yytext[0]=='{'){
        sTable.EnterScope();
    }
    if(yytext[0]=='}'){
        sTable.ExitScope();
    }
    lexUtil.printPunctuation(yytext,yylineno);
}


\" {
    BEGIN(STRING);
    startLineNo = yylineno;
}

<STRING><<EOF>> {
    lexUtil.printError("UNFINISHED_STRING", "\"" + String, yylineno-1);
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
    lexUtil.printError("UNFINISHED_STRING", "\"" + String, yylineno-1);
    String = "";
    stringToken="";
    BEGIN(INITIAL);
}

<STRING>\" {
    if (startLineNo == yylineno){
lexUtil.printAnother("SINGLE LINE STRING", "\"" + String + "\"", startLineNo);
lexUtil.printToken("SINGLE LINE STRING",currentCharacter(String));
    }
        
    else{
 lexUtil.printAnother("MULTI LINE STRING", "\"" + String + "\"", startLineNo);
 lexUtil.printToken("MULTI LINE STRING", currentCharacter(stringToken));
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
    singlelineComment = singlelineComment.substr(0, singlelineComment.length()-1);
    lexUtil.printSingleLineComment(singlelineComment, startLineNo);
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
    lexUtil.printError("UNFINISHED_COMMENT","/*"+multilineComment, yylineno);
    multilineComment="";
}

<MLCOMMENT>"*/" {
    BEGIN(INITIAL);
    lexUtil.printMultiLineComment(multilineComment, startLineNo);
    multilineComment = "";
}

<MLCOMMENT>([^*]|\n)+|. {
    multilineComment+=yytext;
}


{TOO_MANY_DECIMAL_POINTS} {
    lexUtil.printError("TOO_MANY_DECIMAL_POINTS", yytext, yylineno);
}
{INVALID_ID} {
    lexUtil.printError("INVALID_ID_SUFFIX_NUM_PREFIX", yytext, yylineno);
}
{ILLFORMED_NUMBER} {
    lexUtil.printError("ILLFORMED_NUMBER",yytext,yylineno);
}
{WHITESPACE} {
    // Skip whitespace
}
. {
   lexUtil.printError("UNRECOGNIZED_CHAR", yytext, yylineno);
}
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
    yylex();
    fclose(yyin);

    sTable.PrintAllScope(lexUtil.getLogFout());
    lexUtil.getLogFout()<<"Total lines: "<<yylineno<<endl;
    lexUtil.getLogFout()<<"Total errors: "<<lexUtil.getErrorCount()<<endl;


    return EXIT_SUCCESS;
}
