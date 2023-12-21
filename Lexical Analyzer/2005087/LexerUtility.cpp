#include <bits/stdc++.h>

class LexerUtility
{
    std::ofstream logFout;
    std::ofstream tokenFout;
    int errorCount;

public:
    LexerUtility()
    {
        logFout.open("log.txt");
        tokenFout.open("token.txt");
        errorCount = 0;
    }
    ofstream& getLogFout(){
        return logFout;
    }
    ofstream& gettokenFout(){
        return tokenFout;
    }
    

    void printLog(std::string token, std::string symbol, int lineNo)
    {
        logFout << "Line# " << lineNo << ": Token <" << token << "> Lexeme " << symbol << " found" << std::endl;
    }
    void printToken(std::string token, std::string symbol)
    {
        tokenFout << "<" << token << ", " << symbol << ">" << std::endl;
    }
    void printKeyword(std::string keyword, int lineNo)
    {
        std::string type = "";
        for (int i = 0; i < keyword.length(); i++)
        {
            type += (std::toupper(keyword[i]));
        }
        printLog(type, keyword, lineNo);
        printToken(type, keyword);
    }
    void printPunctuation(std::string symbol, int lineNo)
    {
        std::string Sign[] = {"(", ")", "{", "}", "[", "]", ",", ";"};
        std::string type[] = {"LPAREN", "RPAREN", "LCURL", "RCURL", "LSQUARE", "RSQUARE", "COMMA", "SEMICOLON"};
        int length = sizeof(Sign) / sizeof(Sign[0]);
        for (int i = 0; i < length; i++)
        {
            if (symbol == Sign[i])
            {
                printLog(type[i], symbol, lineNo);
                printToken(type[i], symbol);
                break;
            }
        }
    }
    void printAnother(std::string tokenName, std::string symbol, int lineNo)
    {
        printLog(tokenName, symbol, lineNo);
        printToken(tokenName, symbol);
    }

    void printSingleLineComment(std::string commentText, int lineNo)
    {
        //int commentLine = countCommentLine(commentText);
        printLog("SINGLE LINE COMMENT", "//"+commentText, lineNo);
    }
    void printMultiLineComment(std::string commentText, int lineNo)
    {
        //int commentLine = countCommentLine(commentText);
        printLog("MULTI LINE COMMENT","/*" + commentText+"*/" , lineNo);
    }


    void printError(std::string error, std::string text, int lineNo)
    {
        errorCount++;
        logFout << "Error at line# " << lineNo << ": " << error << " " << text << std::endl;
    }
    int getErrorCount(){
        return errorCount;
    }
};