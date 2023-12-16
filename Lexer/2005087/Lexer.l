%option noyywrap 
%option yylineno
%x SLCOMMENT
%x MLCOMMENT

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
int countCommentLine(std::string text)
    {
        int commentLine = 0;
        for (int i = 0; i < text.length(); i++)
        {
            if (text[i] == '\n')
                commentLine++;
        }
        return commentLine;
    }


%}



KEYWORD (if|else|for|while|do|break|int|char|float|double|void|return|switch|case|default|continue)
WHITESPACE [ \n\t\f\r\v]+ 
LETTER [a-zA-Z]
WORD {LETTER}+
DIGIT [0-9]
DIGITS {DIGIT}+
FLOAT {DIGITS}(\.{DIGITS})?([eE][-+]?{DIGITS})?
IDENTIFIER [a-zA-Z_][a-zA-Z0-9_]*

%%
{KEYWORD} {
    lexUtil.printKeyword(yytext,yylineno);
    
}

{IDENTIFIER} {
lexUtil.printAnother("ID",yytext,yylineno);
    
    SymbolInfo *sInfo=new SymbolInfo(yytext,"ID");
    if(sTable.Insert(sInfo)){

    }
}


{DIGITS} {
    lexUtil.printAnother("CONST_INT",yytext,yylineno); 
}

{FLOAT} {
    lexUtil.printAnother("CONST_FLOAT",yytext,yylineno);
}

"+"|"-" {lexUtil.printAnother("ADDOP",yytext,yylineno);}
"*"|"/"|"%"	{lexUtil.printAnother("MULOP",yytext,yylineno);}
"++"|"--" {lexUtil.printAnother("INCOP",yytext,yylineno);}
"<"|">"|"<="|">="|"=="|"!=" {lexUtil.printAnother("RELOP",yytext,yylineno);}
"=" {lexUtil.printAnother("ASSIGNOP",yytext,yylineno);}
"&&"|"||" {lexUtil.printAnother("LOGICOP",yytext,yylineno);}
"&"|"|"|"^"|"<<"|">>" {lexUtil.printAnother("BITOP",yytext,yylineno);}
"!" {lexUtil.printAnother("NOT",yytext,yylineno);}



"("|")"|"{"|"}"|"["|"]"|","|";" {
    if(yytext[0]=='{'){
        sTable.EnterScope();
    }
    if(yytext[0]=='}'){
        sTable.ExitScope();
    }
    lexUtil.printPunctuation(yytext,yylineno);

}


\"([^\"\\\n]|(\\[nrt\"\\]))*\" {
lexUtil.printAnother("SINGLE LINE STRING",yytext,yylineno);
}
\"([^\"]|[\\\n]|(\\[nrt\"\\]))*\" {
    int commentLine=countCommentLine(yytext);
lexUtil.printAnother("MULTI LINE STRING",yytext,yylineno-commentLine);
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
<MLCOMMENT>"*/" {
    BEGIN(INITIAL);
    lexUtil.printMultiLineComment(multilineComment, startLineNo);
    multilineComment = "";
}
<MLCOMMENT>([^*]|\n)+|. {
    multilineComment+=yytext;
}


{WHITESPACE} {
    // Skip whitespace
}






. {
    printf("%d ----> Unrecognized character: %s\n", yylineno, yytext);
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

    return EXIT_SUCCESS;
}
