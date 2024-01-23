#include <bits/stdc++.h>

class Utility
{
    std::ofstream treeFout;
    std::ofstream logFout;
    std::ofstream errorFout;
    int errorCount;
    int warnCount;

public:
    Utility()
    {
        treeFout.open("2005087_parsetree.txt");
        logFout.open("2005087_log.txt");
        errorFout.open("2005087_error.txt");
        errorCount = 0;
        warnCount = 0;
    }
    ofstream &getTreeFout()
    {
        return treeFout;
    }
    ofstream &getLogFout()
    {
        return logFout;
    }
    ofstream &getErrorFout()
    {
        return errorFout;
    }

    void printLog(std::string token, std::string symbol, int lineNo)
    {
        logFout << "Line# " << lineNo << ": Token <" << token << "> Lexeme " << symbol << " found" << std::endl;
    }
    void printError(std::string message, int lineNo)
    {
        errorCount++;
        errorFout <<"Line# " << lineNo <<": " << message << std::endl;
    }
    void printGrammar(std::string grammar)
    {
        logFout << grammar << std::endl;
    }
    void printKeyword(std::string keyword, int lineNo)
    {
        std::string type = "";
        for (int i = 0; i < keyword.length(); i++)
        {
            type += (std::toupper(keyword[i]));
        }
        printLog(type, keyword, lineNo);
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
                break;
            }
        }
    }
    void printOperator(std::string tokenName, std::string symbol, int lineNo)
    {
        printLog(tokenName, symbol, lineNo);
    }
    void printAnother(std::string tokenName, std::string symbol, int lineNo)
    {
        printLog(tokenName, symbol, lineNo);
    }

    void printSingleLineComment(std::string commentText, int lineNo)
    {
        printLog("SINGLE LINE COMMENT", "//" + commentText, lineNo);
    }
    void printMultiLineComment(std::string commentText, int lineNo)
    {
        printLog("MULTI LINE COMMENT", "/*" + commentText + "*/", lineNo);
    }

    void printError(std::string error, std::string text, int lineNo)
    {
        errorCount++;
        cout << error << " " << lineNo << " " << text << endl;
        logFout << "Error at line# " << lineNo << ": " << error << " " << text << std::endl;
    }
    void printWarning(std::string warning, int lineNo)
    {
        warnCount++;
        logFout << "Line# " << lineNo << warning << std::endl;
    }
    int getErrorCount()
    {
        return errorCount;
    }
    int getWarningCount()
    {
        return warnCount;
    }
};