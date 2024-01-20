/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "2005087.y" /* yacc.c:339  */

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


void declareFunction(string functionName, string returnType, vector<pair<pair<string, string>,bool>> parameterList = {})
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
			util.printError("redeclaration of " + functionName, yylineno);
			return;
		}
	}
}
void defineFunction(string functionName, string returnType, vector<pair<pair<string, string>,bool>> parameterList = {})
{
    currentFunctionParameters.clear();
    currentFunctionParameters=parameterList;
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
					util.printError("Conflicting types for \'" + functionName+"\'", yylineno);
					//return;
				}
			//    if (searched->getParameterList().size() < parameterList.size())
			// 	{
			// 		util.printError("Too many arguments to function " + functionName, yylineno);
			// 		return;
			// 	}
			// 	if (searched->getParameterList().size() > parameterList.size())
			// 	{
			// 		util.printError("Too few arguments to function " + functionName, yylineno);
			// 		return;
			// 	}
				// if (!searched->getParameterList().empty())
				// {
					// for (int i = 0; i < parameterList.size(); i++)
					// {
					// 	if (searched->getParameterList().at(i).first != parameterList[i].first)
					// 	{
					// 		util.printError("conflicting types for " + functionName, yylineno);
					// 		return;
					// 	}
					// }
                    
				// }
                if(searched->getParameterList().size() != parameterList.size()){
                   util.printError("Conflicting types for \'" + functionName+"\'", yylineno); 
                }

			searched->setFunctionInfo("FUNCTION_DEFINITION");
			searched->addParameter(parameterList);
			searched->setReturnOrDataType(returnType);
		}
		else
		{
			util.printError("\'" + functionName+"\' redeclared as different kind of symbol", yylineno);
			return;
		}
	}
    currentFunctionParameters.clear();
}
void callFunction(Node *actualNode, Node *nameNode, Node *argNode)
{
    auto parameterList=argNode->getParameterList();
	Node *searched = sTable.Lookup(nameNode);
	if (searched == NULL)
	{
		util.printError("Undeclared function \'" + nameNode->getName() + "\'", yylineno);
	}
	else if (!(searched->getFunctionInfo() == "FUNCTION_DECLARATION" ||searched->getFunctionInfo() == "FUNCTION_DEFINITION") )
	{
		util.printError("\'" + nameNode->getName() + "\' is not a function", yylineno);
	}
	else if (searched->getParameterList().size() < parameterList.size())
	{
		util.printError("Too many arguments to function \'" + nameNode->getName()+"\'", yylineno);
		return;
	}
	else if (searched->getParameterList().size() > parameterList.size())
	{
		util.printError("Too few arguments to function \'" + nameNode->getName()+"\'", yylineno);
		return;
	}
	else 
	{
        //bool everythingIsOk=true;
		for (int i = 0; i < searched->getParameterList().size(); i++)
		{
            
            cout<<searched->getName()<<searched->getParameterList().at(i).first.first<<" "<<parameterList[i].first.first<<" "<<parameterList[i].first.second<<endl;
			if ((searched->getParameterList().at(i).first.first != parameterList[i].first.first))
			{
				util.printError("Type mismatch for argument "+to_string(i+1)+" of \'" + nameNode->getName()+"\'", yylineno);
                //everythingIsOk=false;
			}
		}
            actualNode->setReturnOrDataType(searched->getReturnOrDataType());
        
	}

}
void SaveData(string dataType,Node* node ) {
   auto list=node->getParameterList();
        for (auto element : list) {
             if (dataType == "VOID") {
                util.printError("Variable or field \'"+element.first.second+"\' declared void", yylineno);
                    return;
                 } 
            //element.first.first=dataType;     
            Node *toBeSearched = new Node(new SymbolInfo(element.first.second, ""),"",dataType);
            Node *searched = sTable.LookupCurrent(toBeSearched);

            if (searched == NULL) {
                Node *toBeInserted = new Node(new SymbolInfo(element.first.second, "ID"), "", dataType);

                if (element.second) {
                    toBeInserted->setArrayStatus(true);
                }
                sTable.Insert(toBeInserted);
                element.first.first=dataType;
               // toBeInserted->setReturnOrDataType(dataType);
            } else if (searched->getReturnOrDataType() != dataType) {
                util.printError("Conflicting types for \'" + element.first.second + "\'", yylineno);
            } else {
                util.printError("Redefinition of variable \'" + element.first.second + "\'", yylineno);
            }
            //node->addParameter(list);
        }
}

	



void CheckVariableDeclaredOrNot(Node *node)
{
	string variable = node->getName();
	Node *toBeSearched = new Node(new SymbolInfo(variable, "ID"));
	Node *searched = sTable.Lookup(toBeSearched);
	if (searched == NULL)
	{
		util.printError("Undeclared variable \'" + variable + "\'", yylineno);
		return;
	}
	node->setReturnOrDataType(searched->getReturnOrDataType());
	node->setArrayStatus(searched->getArrayStatus());
}
void CheckReturnIssues(Node *node)
{
    //cout<<currentFunctionReturnType;
	if (currentFunctionReturnType == "VOID")
	{
		util.printError("Function return type void", yylineno);
	}
	if (currentFunctionReturnType == "INT" && node->getReturnOrDataType() == "FLOAT")
	{
		util.printError("Warning: possible loss of data in assignment of FLOAT to INT", yylineno);
	}
}
void CheckArrayIssues(Node *actualNode, Node *nameNode, Node *indexNode)
{
    //cout<<indexNode->getReturnOrDataType()<<endl;
	string name = nameNode->getName();
	Node *toBeSearched = new Node(new SymbolInfo(name, "ID"));
	Node *searched = sTable.Lookup(toBeSearched);
	if (searched == NULL)
	{
		util.printError("Undeclared variable \'" + name + "\'", yylineno);
		return;
	}
	if (!searched->getArrayStatus())
	{
		util.printError("\'" + name + "\' is not an array", yylineno);
		return;
	}
	if (indexNode->getReturnOrDataType() != "INT")
	{
		util.printError("Array subscript is not an integer", yylineno);
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
		util.printError("Type mismatch for \'" + leftNode->getName() + "\' , is an array", yylineno);
		return;
	}
	if (leftNode->getReturnOrDataType() == "VOID" || rightNode->getReturnOrDataType() == "VOID")
	{
		util.printError("Void cannot be used in expression", yylineno);
		//$$->setReturnOrDataType("ERROR");
		return;
	}
	if (leftNode->getReturnOrDataType() == "INT")
	{
		if (rightNode->getReturnOrDataType() == "FLOAT")
		{
			util.printError("Warning: possible loss of data in assignment of FLOAT to INT", yylineno);
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
		util.printError("Void cannot be used in expression", yylineno);
		// actualNode->setReturnOrDataType("ERROR");
		return;
	}
	if (leftNode->getReturnOrDataType() == "FLOAT" || rightNode->getReturnOrDataType() == "FLOAT")
	{
		util.printError("Warning: Void cannot be used in expression", yylineno);
		actualNode->setReturnOrDataType("INT");
	}
	actualNode->setReturnOrDataType("INT");
}
void CheckRelationalIssues(Node *actualNode, Node *leftNode, Node *rightNode)
{
	if (leftNode->getReturnOrDataType() == "VOID" || rightNode->getReturnOrDataType() == "VOID")
	{
		util.printError("Void cannot be used in expression", yylineno);
		//$$->setReturnOrgetReturnOrDataType("ERROR");
	}
	else
	{
		actualNode->setReturnOrDataType("INT");
	}
}
bool CheckingZerOperand(string str){
    bool hasNonZeroDigit = false;

    for (int i = 0; i < str.size(); ++i) {
        char c = str[i];

        if (c == 'e' || c == 'E') {
            break;
        }

        if (c != '0' && c != '.') {
            hasNonZeroDigit = true;
            break;
        }

        if (c == '.') {
            for (++i; i < str.size(); ++i) {
                if (str[i] != '0') {
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
    // if (x == "VOID" || y == "VOID" || x == "ERROR" || y == "ERROR") {
    //     return "ERROR";
    // }
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


string TypeCasting(string s1, const string s2) {
    if (s1 == "VOID" || s2 == "VOID" || s1 == "ERROR" || s2 == "ERROR") {
        return "ERROR";
    } else if (s1 == "FLOAT" || s2 == "FLOAT") {
        return "FLOAT";
    } else {
        return "INT";
    }
}

void CheckMultiplicationIssues(Node* actualNode,Node* leftNode,Node* midNode,Node* rightNode){
    if (leftNode->getReturnOrDataType() == "VOID" || rightNode->getReturnOrDataType() == "VOID") {
			util.printError("Void cannot be used in expression", yylineno);
			
		}
		
		else if (midNode->getName() == "%") {
			if (leftNode->getReturnOrDataType() == "FLOAT" || rightNode->getReturnOrDataType() == "FLOAT") {
				util.printError("Operands of modulus must be integers" , yylineno);
				
			}
			else if (CheckingZerOperand(rightNode->getName())) {
				util.printError("Warning: division by zero i=0f=1Const=0" , yylineno);	
			}
			else {
				actualNode->setReturnOrDataType("INT");
			}
		}
		else if (midNode->getName() == "/") {
			if (CheckingZerOperand(rightNode->getName())) {
				util.printError("Warning: division by zero i=0f=1Const=0" , yylineno);
			}
			else {
				actualNode->setReturnOrDataType(TypeCasting(leftNode->getReturnOrDataType(), rightNode->getReturnOrDataType()));
			}
		}
		else if (midNode->getName() == "*") {
			actualNode->setReturnOrDataType(TypeCasting(leftNode->getReturnOrDataType(), rightNode->getReturnOrDataType()));
		}
}
void CheckUnaryExpression(Node *actualNode,Node *node){
   if (node->getReturnOrDataType() == "VOID") {
			util.printError("Void cannot be used in expression", yylineno);
			return;
		}
   else if (node->getReturnOrDataType() == "FLOAT") {
			util.printError("Warning :Operands of bitwise operation should be integers", yylineno);
    }
actualNode->setReturnOrDataType("INT"); 
}
void CheckUnaryOperandIssues(Node* actualNode,Node* operand)
{
    if (operand->getReturnOrDataType() == "VOID") {
			util.printError("Void cannot be used in expression", yylineno);
			actualNode->setReturnOrDataType("ERROR");
            return;
		}
		else if (operand->getReturnOrDataType() == "ERROR") {
			actualNode->setReturnOrDataType("ERROR");
            return;
		}
			actualNode->setReturnOrDataType(operand->getReturnOrDataType());
}   


#line 461 "y.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "y.tab.h".  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    IF = 258,
    ELSE = 259,
    SWITCH = 260,
    CASE = 261,
    BREAK = 262,
    DEFAULT = 263,
    FOR = 264,
    WHILE = 265,
    DO = 266,
    CONTINUE = 267,
    MAIN = 268,
    VOID = 269,
    INT = 270,
    FLOAT = 271,
    DOUBLE = 272,
    CHAR = 273,
    ID = 274,
    RETURN = 275,
    PRINTLN = 276,
    CONST_INT = 277,
    CONST_FLOAT = 278,
    CONST_CHAR = 279,
    NOT = 280,
    LPAREN = 281,
    RPAREN = 282,
    LCURL = 283,
    RCURL = 284,
    LSQUARE = 285,
    RSQUARE = 286,
    COMMA = 287,
    SEMICOLON = 288,
    ADDOP = 289,
    MULOP = 290,
    INCOP = 291,
    DECOP = 292,
    RELOP = 293,
    LOGICOP = 294,
    BITOP = 295,
    ASSIGNOP = 296,
    THEN = 297
  };
#endif
/* Tokens.  */
#define IF 258
#define ELSE 259
#define SWITCH 260
#define CASE 261
#define BREAK 262
#define DEFAULT 263
#define FOR 264
#define WHILE 265
#define DO 266
#define CONTINUE 267
#define MAIN 268
#define VOID 269
#define INT 270
#define FLOAT 271
#define DOUBLE 272
#define CHAR 273
#define ID 274
#define RETURN 275
#define PRINTLN 276
#define CONST_INT 277
#define CONST_FLOAT 278
#define CONST_CHAR 279
#define NOT 280
#define LPAREN 281
#define RPAREN 282
#define LCURL 283
#define RCURL 284
#define LSQUARE 285
#define RSQUARE 286
#define COMMA 287
#define SEMICOLON 288
#define ADDOP 289
#define MULOP 290
#define INCOP 291
#define DECOP 292
#define RELOP 293
#define LOGICOP 294
#define BITOP 295
#define ASSIGNOP 296
#define THEN 297

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 396 "2005087.y" /* yacc.c:355  */

    Node *node;

#line 589 "y.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 606 "y.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  11
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   160

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  43
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  27
/* YYNRULES -- Number of rules.  */
#define YYNRULES  67
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  121

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   297

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   412,   412,   422,   431,   439,   446,   453,   461,   475,
     490,   490,   507,   507,   529,   539,   549,   558,   568,   580,
     592,   603,   611,   619,   628,   637,   647,   655,   664,   671,
     679,   687,   695,   703,   710,   717,   724,   731,   741,   751,
     758,   767,   777,   786,   794,   803,   811,   820,   829,   838,
     847,   858,   866,   875,   887,   896,   908,   917,   926,   934,
     941,   949,   958,   968,   976,   982,   991,  1000
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IF", "ELSE", "SWITCH", "CASE", "BREAK",
  "DEFAULT", "FOR", "WHILE", "DO", "CONTINUE", "MAIN", "VOID", "INT",
  "FLOAT", "DOUBLE", "CHAR", "ID", "RETURN", "PRINTLN", "CONST_INT",
  "CONST_FLOAT", "CONST_CHAR", "NOT", "LPAREN", "RPAREN", "LCURL", "RCURL",
  "LSQUARE", "RSQUARE", "COMMA", "SEMICOLON", "ADDOP", "MULOP", "INCOP",
  "DECOP", "RELOP", "LOGICOP", "BITOP", "ASSIGNOP", "THEN", "$accept",
  "start", "program", "unit", "func_declaration", "func_definition", "$@1",
  "$@2", "parameter_list", "compound_statement", "var_declaration",
  "type_specifier", "declaration_list", "statements", "statement",
  "expression_statement", "variable", "expression", "logic_expression",
  "rel_expression", "simple_expression", "term", "unary_expression",
  "factor", "argument_list", "arguments", "lcurls", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297
};
# endif

#define YYPACT_NINF -71

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-71)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     123,   -71,   -71,   -71,    17,   123,   -71,   -71,   -71,   -71,
      10,   -71,   -71,    28,    19,     6,    18,    13,   -71,     9,
     -23,    26,    54,    27,   -71,    62,    63,   123,   -71,   -71,
      75,   -71,   -71,    58,   -71,    62,    87,    80,    91,    92,
     103,   101,    -7,   104,   -71,   -71,    -7,    -7,   -71,   -71,
      -7,   -71,   -71,   105,    79,   -71,   -71,     0,   102,   -71,
     106,    98,   107,   -71,   -71,   -71,   -71,   -71,    -7,    37,
      -7,    -7,    -7,   108,   121,    39,   -71,   116,   -71,   114,
     -71,   -71,   -71,   -71,    -7,   -71,    -7,    -7,    -7,    -7,
     119,    37,   120,   -71,   122,   118,   117,   -71,   124,   -71,
     -71,   -71,   107,   125,   -71,   100,    -7,   100,   -71,    -7,
     -71,   127,   148,   126,   -71,   -71,   -71,   100,   100,   -71,
     -71
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    23,    21,    22,     0,     2,     4,     6,     7,     5,
       0,     1,     3,    26,     0,     0,     0,     0,    20,    12,
       0,    17,     0,    24,     9,     0,    10,     0,    16,    27,
       0,    67,    13,     0,     8,     0,    15,     0,     0,     0,
       0,    41,     0,     0,    59,    60,     0,     0,    19,    39,
       0,    32,    30,     0,     0,    28,    31,    56,     0,    43,
      45,    47,    49,    51,    55,    11,    14,    25,     0,     0,
       0,    64,     0,     0,     0,    56,    54,     0,    53,    26,
      18,    29,    61,    62,     0,    40,     0,     0,     0,     0,
       0,     0,     0,    66,     0,    63,     0,    38,     0,    58,
      44,    46,    50,    48,    52,     0,     0,     0,    57,     0,
      42,     0,    34,     0,    36,    65,    37,     0,     0,    35,
      33
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -71,   -71,   -71,   149,   -71,   -71,   -71,   -71,   -71,    -1,
      38,     8,   -71,   -71,   -52,   -66,   -40,   -42,   -70,    69,
      68,    70,   -39,   -71,   -71,   -71,   -71
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     4,     5,     6,     7,     8,    35,    25,    20,    51,
      52,    53,    14,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    94,    95,    33
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      73,    93,    81,    91,    26,    77,    75,    76,    10,    27,
      75,    78,    41,    10,   100,    44,    45,    11,    46,    47,
       1,     2,     3,    21,    32,   106,    90,    50,    92,    13,
      96,    75,    23,    19,    65,    36,    82,    83,     9,   115,
      22,    84,    24,     9,    75,    28,    75,    75,    75,    75,
     104,    17,    18,   112,    15,   114,    41,    30,    16,    44,
      45,    38,    46,    47,   113,   119,   120,    39,    40,    75,
      49,    50,     1,     2,     3,    82,    83,    41,    42,    43,
      44,    45,    38,    46,    47,    29,    31,    48,    39,    40,
      31,    49,    50,     1,     2,     3,    34,    37,    41,    42,
      43,    44,    45,    38,    46,    47,    66,    31,    80,    39,
      40,    67,    49,    50,     1,     2,     3,    68,    69,    41,
      42,    43,    44,    45,    79,    46,    47,    71,    31,    70,
      74,    72,    87,    49,    50,    85,    88,     1,     2,     3,
      98,    97,    89,    99,    16,    86,   105,   107,   110,   108,
     109,   111,   117,   118,    12,   101,   103,   102,     0,    87,
     116
};

static const yytype_int8 yycheck[] =
{
      42,    71,    54,    69,    27,    47,    46,    46,     0,    32,
      50,    50,    19,     5,    84,    22,    23,     0,    25,    26,
      14,    15,    16,    15,    25,    91,    68,    34,    70,    19,
      72,    71,    19,    27,    35,    27,    36,    37,     0,   109,
      22,    41,    33,     5,    84,    19,    86,    87,    88,    89,
      89,    32,    33,   105,    26,   107,    19,    30,    30,    22,
      23,     3,    25,    26,   106,   117,   118,     9,    10,   109,
      33,    34,    14,    15,    16,    36,    37,    19,    20,    21,
      22,    23,     3,    25,    26,    31,    28,    29,     9,    10,
      28,    33,    34,    14,    15,    16,    33,    22,    19,    20,
      21,    22,    23,     3,    25,    26,    19,    28,    29,     9,
      10,    31,    33,    34,    14,    15,    16,    26,    26,    19,
      20,    21,    22,    23,    19,    25,    26,    26,    28,    26,
      26,    30,    34,    33,    34,    33,    38,    14,    15,    16,
      19,    33,    35,    27,    30,    39,    27,    27,    31,    27,
      32,    27,     4,    27,     5,    86,    88,    87,    -1,    34,
      33
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    14,    15,    16,    44,    45,    46,    47,    48,    53,
      54,     0,    46,    19,    55,    26,    30,    32,    33,    27,
      51,    54,    22,    19,    33,    50,    27,    32,    19,    31,
      30,    28,    52,    69,    33,    49,    54,    22,     3,     9,
      10,    19,    20,    21,    22,    23,    25,    26,    29,    33,
      34,    52,    53,    54,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    52,    19,    31,    26,    26,
      26,    26,    30,    60,    26,    59,    65,    60,    65,    19,
      29,    57,    36,    37,    41,    33,    39,    34,    38,    35,
      60,    58,    60,    61,    67,    68,    60,    33,    19,    27,
      61,    62,    64,    63,    65,    27,    58,    27,    27,    32,
      31,    27,    57,    60,    57,    61,    33,     4,    27,    57,
      57
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    43,    44,    45,    45,    46,    46,    46,    47,    47,
      49,    48,    50,    48,    51,    51,    51,    51,    52,    52,
      53,    54,    54,    54,    55,    55,    55,    55,    56,    56,
      57,    57,    57,    57,    57,    57,    57,    57,    57,    58,
      58,    59,    59,    60,    60,    61,    61,    62,    62,    63,
      63,    64,    64,    65,    65,    65,    66,    66,    66,    66,
      66,    66,    66,    67,    67,    68,    68,    69
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     1,     6,     5,
       0,     7,     0,     6,     4,     3,     2,     1,     3,     2,
       3,     1,     1,     1,     3,     6,     1,     4,     1,     2,
       1,     1,     1,     7,     5,     7,     5,     5,     3,     1,
       2,     1,     4,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     3,     2,     2,     1,     1,     4,     3,     1,
       1,     2,     2,     1,     0,     3,     1,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 412 "2005087.y" /* yacc.c:1646  */
    {
    SymbolInfo *sInfo=new SymbolInfo("","start");
    (yyval.node) =new Node(sInfo,"start : program");
    pTree.setCurrentNode((yyval.node));
    (yyval.node)->makeChild({(yyvsp[0].node)});
    util.printGrammar("start : program");
    pTree.printParseTree(util.getTreeFout());
    
}
#line 1787 "y.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 422 "2005087.y" /* yacc.c:1646  */
    {
    SymbolInfo *sInfo=new SymbolInfo("","program");
    (yyval.node)=new Node(sInfo,"program : program unit");
    pTree.setCurrentNode((yyval.node));
    (yyval.node)->makeChild({(yyvsp[-1].node),(yyvsp[0].node)});
    
    util.printGrammar("program : program unit");
    
}
#line 1801 "y.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 431 "2005087.y" /* yacc.c:1646  */
    {
    SymbolInfo *sInfo=new SymbolInfo("","program");
    (yyval.node)=new Node(sInfo,"program : unit");
    pTree.setCurrentNode((yyval.node));
    (yyval.node)->makeChild({(yyvsp[0].node)});
    util.printGrammar("program : unit");
}
#line 1813 "y.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 439 "2005087.y" /* yacc.c:1646  */
    {
    SymbolInfo *sInfo=new SymbolInfo("","unit");
    (yyval.node)=new Node(sInfo,"unit : var_declaration");
    pTree.setCurrentNode((yyval.node));
    (yyval.node)->makeChild({(yyvsp[0].node)});
    util.printGrammar("unit : var_declaration");
}
#line 1825 "y.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 446 "2005087.y" /* yacc.c:1646  */
    {
    SymbolInfo *sInfo=new SymbolInfo("","unit");
    (yyval.node)=new Node(sInfo,"unit : func_declaration");
    pTree.setCurrentNode((yyval.node));
    (yyval.node)->makeChild({(yyvsp[0].node)});
    util.printGrammar("unit : func_declaration");
}
#line 1837 "y.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 453 "2005087.y" /* yacc.c:1646  */
    {
		SymbolInfo *sInfo = new SymbolInfo("", "unit");
        (yyval.node)=new Node(sInfo,"unit : func_definition");
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[0].node)});
		util.printGrammar("unit : func_definition");
}
#line 1849 "y.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 461 "2005087.y" /* yacc.c:1646  */
    {
    currentFunctionParameters.clear();
        SymbolInfo *sInfo = new SymbolInfo("", "func_declaration");
        (yyval.node)=new Node(sInfo,"func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON");
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-5].node),(yyvsp[-4].node),(yyvsp[-3].node),(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)});
        util.printGrammar("func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON");
        //Inserting the function in SymbolTable
        string functionName=(yyvsp[-4].node)->getName();
        string returnType=(yyvsp[-5].node)->getReturnOrDataType();
        vector<pair<pair<string, string>,bool>> parameterList = (yyvsp[-2].node)->getParameterList();
        declareFunction(functionName, returnType, parameterList);
        
}
#line 1868 "y.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 475 "2005087.y" /* yacc.c:1646  */
    {
        currentFunctionParameters.clear();
        SymbolInfo *sInfo = new SymbolInfo("", "func_declaration");
        (yyval.node)=new Node(sInfo,"func_declaration : type_specifier ID LPAREN RPAREN SEMICOLON");
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-4].node),(yyvsp[-3].node),(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)});
        util.printGrammar("func_declaration : type_specifier ID LPAREN RPAREN SEMICOLON");
        //Inserting the function in SymbolTable
        string functionName=(yyvsp[-3].node)->getName();
        string returnType=(yyvsp[-4].node)->getReturnOrDataType();
        declareFunction(functionName, returnType);
        
}
#line 1886 "y.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 490 "2005087.y" /* yacc.c:1646  */
    {
currentFunctionReturnType=(yyvsp[-4].node)->getReturnOrDataType();
}
#line 1894 "y.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 493 "2005087.y" /* yacc.c:1646  */
    {
    SymbolInfo *sInfo = new SymbolInfo("", "func_definition");
    (yyval.node)=new Node(sInfo,"func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement");
    pTree.setCurrentNode((yyval.node));
    (yyval.node)->makeChild({(yyvsp[-6].node),(yyvsp[-5].node),(yyvsp[-4].node),(yyvsp[-3].node),(yyvsp[-2].node),(yyvsp[0].node)});
    util.printGrammar("func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement");
    string functionName=(yyvsp[-5].node)->getName();
    string returnType=(yyvsp[-6].node)->getReturnOrDataType();
    
    vector<pair<pair<string, string>,bool>> parameterList = (yyvsp[-3].node)->getParameterList();
    defineFunction(functionName,returnType,parameterList);
    
    
    }
#line 1913 "y.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 507 "2005087.y" /* yacc.c:1646  */
    {
        currentFunctionReturnType=(yyvsp[-3].node)->getReturnOrDataType();


    }
#line 1923 "y.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 512 "2005087.y" /* yacc.c:1646  */
    {  
    SymbolInfo *sInfo = new SymbolInfo("", "func_definition");
    (yyval.node)=new Node(sInfo,"func_definition : type_specifier ID LPAREN RPAREN compound_statement");
    pTree.setCurrentNode((yyval.node));
    (yyval.node)->makeChild({(yyvsp[-5].node),(yyvsp[-4].node),(yyvsp[-3].node),(yyvsp[-2].node),(yyvsp[0].node)});
    util.printGrammar("func_definition : type_specifier ID LPAREN RPAREN compound_statement");
    string functionName=(yyvsp[-4].node)->getName();
    string returnType=(yyvsp[-5].node)->getReturnOrDataType();
    
    //cout<<functionName<<"  "<<currentFunctionReturnType<<endl;
    defineFunction(functionName,returnType);
}
#line 1940 "y.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 529 "2005087.y" /* yacc.c:1646  */
    {
       SymbolInfo *sInfo = new SymbolInfo("", "parameter_list");
       (yyval.node)=new Node(sInfo,"parameter_list : parameter_list COMMA type_specifier ID");
       (yyval.node)->addParameter((yyvsp[-3].node)->getParameterList());
        (yyval.node)->addParameter({{(yyvsp[-1].node)->getReturnOrDataType(),(yyvsp[0].node)->getName()},false});
        currentFunctionParameters=(yyval.node)->getParameterList();
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-3].node),(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)});
        util.printGrammar("parameter_list : parameter_list COMMA type_specifier ID");
}
#line 1955 "y.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 539 "2005087.y" /* yacc.c:1646  */
    {
        SymbolInfo *sInfo = new SymbolInfo("", "parameter_list");
        (yyval.node)=new Node(sInfo,"parameter_list : parameter_list COMMA type_specifier");
        (yyval.node)->addParameter((yyvsp[-2].node)->getParameterList());
        (yyval.node)->addParameter({{(yyvsp[0].node)->getReturnOrDataType(),""},false});
        currentFunctionParameters=(yyval.node)->getParameterList();
        pTree.setCurrentNode((yyval.node));
       (yyval.node)->makeChild({(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)});
        util.printGrammar("parameter_list : parameter_list COMMA type_specifier");
}
#line 1970 "y.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 549 "2005087.y" /* yacc.c:1646  */
    {
        SymbolInfo *sInfo = new SymbolInfo("", "parameter_list");
        (yyval.node)=new Node(sInfo,"parameter_list : type_specifier ID");
        (yyval.node)->addParameter({{(yyvsp[-1].node)->getReturnOrDataType(),(yyvsp[0].node)->getName()},false});
        currentFunctionParameters=(yyval.node)->getParameterList();
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-1].node),(yyvsp[0].node)});
        util.printGrammar("parameter_list : type_specifier ID");
}
#line 1984 "y.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 558 "2005087.y" /* yacc.c:1646  */
    {
        SymbolInfo *sInfo = new SymbolInfo("", "parameter_list");
        (yyval.node)=new Node(sInfo,"parameter_list : type_specifier");
        (yyval.node)->addParameter({{(yyvsp[0].node)->getReturnOrDataType(),""},false});
        currentFunctionParameters=(yyval.node)->getParameterList();
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[0].node)});
        util.printGrammar("parameter_list : type_specifier");
}
#line 1998 "y.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 568 "2005087.y" /* yacc.c:1646  */
    {
    SymbolInfo *sInfo = new SymbolInfo("", "compound_statement");
    (yyval.node)=new Node(sInfo,"compound_statement : LCURL statements RCURL");
    pTree.setCurrentNode((yyval.node));
   (yyval.node)->makeChild({(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)});
   util.printGrammar("compound_statement : LCURL statements RCURL");
   
  sTable.PrintAllScope(util.getLogFout());
  sTable.ExitScope();

    
}
#line 2015 "y.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 580 "2005087.y" /* yacc.c:1646  */
    {
    SymbolInfo *sInfo = new SymbolInfo("", "compound_statement");
    (yyval.node)=new Node(sInfo,"compound_statement : LCURL RCURL");
    pTree.setCurrentNode((yyval.node));
    (yyval.node)->makeChild({(yyvsp[-1].node),(yyvsp[0].node)});
    util.printGrammar("compound_statement : LCURL RCURL");
    
    sTable.PrintAllScope(util.getLogFout());
    sTable.ExitScope();
    
    }
#line 2031 "y.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 592 "2005087.y" /* yacc.c:1646  */
    {
   SymbolInfo *sInfo = new SymbolInfo("", "var_declaration");
   (yyval.node)=new Node(sInfo,"var_declaration : type_specifier declaration_list SEMICOLON",(yyvsp[-2].node)->getReturnOrDataType());

    SaveData((yyvsp[-2].node)->getReturnOrDataType(),(yyvsp[-1].node));
    pTree.setCurrentNode((yyval.node));
    (yyval.node)->makeChild({(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)});
    util.printGrammar("var_declaration : type_specifier declaration_list SEMICOLON");
}
#line 2045 "y.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 603 "2005087.y" /* yacc.c:1646  */
    {
    SymbolInfo *sInfo = new SymbolInfo("", "type_specifier");
    (yyval.node)=new Node(sInfo,"type_specifier : INT","INT");
    pTree.setCurrentNode((yyval.node));
    (yyval.node)->makeChild({(yyvsp[0].node)});
    util.printGrammar("type_specifier : INT");
    
}
#line 2058 "y.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 611 "2005087.y" /* yacc.c:1646  */
    {
        SymbolInfo *sInfo = new SymbolInfo("", "type_specifier");
        (yyval.node)=new Node(sInfo,"type_specifier : FLOAT","FLOAT");
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[0].node)});
        util.printGrammar("type_specifier : FLOAT");
        
}
#line 2071 "y.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 619 "2005087.y" /* yacc.c:1646  */
    {
        SymbolInfo *sInfo = new SymbolInfo("", "type_specifier");
        (yyval.node)=new Node(sInfo,"type_specifier : VOID","VOID");
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[0].node)});
        util.printGrammar("type_specifier : VOID");
        
}
#line 2084 "y.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 628 "2005087.y" /* yacc.c:1646  */
    {
        SymbolInfo *sInfo= new SymbolInfo("", "declaration_list");
        (yyval.node)=new Node(sInfo,"declaration_list : declaration_list COMMA ID");
        (yyval.node)->addParameter((yyvsp[-2].node)->getParameterList());
        (yyval.node)->addParameter({{"",(yyvsp[0].node)->getName()},false});
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)});
        util.printGrammar("declaration_list : declaration_list COMMA ID");
}
#line 2098 "y.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 637 "2005087.y" /* yacc.c:1646  */
    {
        SymbolInfo *sInfo = new SymbolInfo("", "declaration_list");
        (yyval.node)=new Node(sInfo,"declaration_list : declaration_list COMMA ID LSQUARE CONST_INT RSQUARE");
        (yyval.node)->addParameter((yyvsp[-5].node)->getParameterList());
        (yyval.node)->addParameter({{"",(yyvsp[-3].node)->getName()},true});
        (yyval.node)->setArrayStatus(true);
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-5].node),(yyvsp[-4].node),(yyvsp[-3].node),(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)});
        util.printGrammar("declaration_list : declaration_list COMMA ID LSQUARE CONST_INT RSQUARE");
}
#line 2113 "y.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 647 "2005087.y" /* yacc.c:1646  */
    {
       SymbolInfo *sInfo = new SymbolInfo("", "declaration_list");
       (yyval.node)=new Node(sInfo,"declaration_list : ID");
       (yyval.node)->addParameter({{(yyvsp[0].node)->getReturnOrDataType(),(yyvsp[0].node)->getName()},false});
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[0].node)});
        util.printGrammar("declaration_list : ID");   
}
#line 2126 "y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 655 "2005087.y" /* yacc.c:1646  */
    {
        SymbolInfo *sInfo= new SymbolInfo("", "declaration_list");
        (yyval.node)=new Node(sInfo,"declaration_list : ID LSQUARE CONST_INT RSQUARE");
        (yyval.node)->addParameter({{(yyvsp[-3].node)->getReturnOrDataType(),(yyvsp[-3].node)->getName()},true});
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-3].node),(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)});
        util.printGrammar("declaration_list : ID LSQUARE CONST_INT RSQUARE"); 
}
#line 2139 "y.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 664 "2005087.y" /* yacc.c:1646  */
    {
        SymbolInfo *sInfo = new SymbolInfo("", "statement");
        (yyval.node)=new Node(sInfo,"statements : statement");
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[0].node)});
        util.printGrammar("statements : statement"); 
}
#line 2151 "y.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 671 "2005087.y" /* yacc.c:1646  */
    {
       SymbolInfo *sInfo = new SymbolInfo("", "statement");
       (yyval.node)=new Node(sInfo,"statements : statements statement");
       pTree.setCurrentNode((yyval.node));
       (yyval.node)->makeChild({(yyvsp[-1].node),(yyvsp[0].node)});
       util.printGrammar("statements : statements statement"); 
}
#line 2163 "y.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 679 "2005087.y" /* yacc.c:1646  */
    {
       SymbolInfo *sInfo= new SymbolInfo("", "statement");
       (yyval.node)=new Node(sInfo,"statement : var_declaration",(yyvsp[0].node)->getReturnOrDataType());
       pTree.setCurrentNode((yyval.node));
       (yyval.node)->makeChild({(yyvsp[0].node)});
       util.printGrammar("statement : var_declaration");
       
}
#line 2176 "y.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 687 "2005087.y" /* yacc.c:1646  */
    {
    SymbolInfo *sInfo = new SymbolInfo("", "statement");
    (yyval.node)=new Node(sInfo,"statement : expression_statement",(yyvsp[0].node)->getReturnOrDataType());
    pTree.setCurrentNode((yyval.node));
    (yyval.node)->makeChild({(yyvsp[0].node)});
    util.printGrammar("statement : expression_statement");
   
}
#line 2189 "y.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 695 "2005087.y" /* yacc.c:1646  */
    {
        SymbolInfo *sInfo = new SymbolInfo("", "statement");
        (yyval.node)=new Node(sInfo,"statement : compound_statement",(yyvsp[0].node)->getReturnOrDataType());
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[0].node)});
        util.printGrammar("statement : compound_statement");
        
}
#line 2202 "y.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 703 "2005087.y" /* yacc.c:1646  */
    {
        SymbolInfo *sInfo = new SymbolInfo("", "statement");
        (yyval.node)=new Node(sInfo,"statement : FOR LPAREN expression_statement expression_statement expression RPAREN statement");
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-6].node),(yyvsp[-5].node),(yyvsp[-4].node),(yyvsp[-3].node),(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)});
        util.printGrammar("statement : FOR LPAREN expression_statement expression_statement expression RPAREN statement");
}
#line 2214 "y.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 710 "2005087.y" /* yacc.c:1646  */
    {
        SymbolInfo *sInfo = new SymbolInfo("", "statement");
        (yyval.node)=new Node(sInfo,"statement : IF LPAREN expression RPAREN statement");
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-4].node),(yyvsp[-3].node),(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)});
        util.printGrammar("statement : IF LPAREN expression RPAREN statement");
}
#line 2226 "y.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 717 "2005087.y" /* yacc.c:1646  */
    {
        SymbolInfo *sInfo = new SymbolInfo("", "statement");
        (yyval.node)=new Node(sInfo,"statement : IF LPAREN expression RPAREN statement ELSE statement");
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-6].node),(yyvsp[-5].node),(yyvsp[-4].node),(yyvsp[-3].node),(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)});
        util.printGrammar("statement : IF LPAREN expression RPAREN statement ELSE statement");
}
#line 2238 "y.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 724 "2005087.y" /* yacc.c:1646  */
    {
        SymbolInfo *sInfo= new SymbolInfo("", "statement");
        (yyval.node)=new Node(sInfo,"statement : WHILE LPAREN expression RPAREN statement");
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-4].node),(yyvsp[-3].node),(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)});
        util.printGrammar("statement : WHILE LPAREN expression RPAREN statement");  
}
#line 2250 "y.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 731 "2005087.y" /* yacc.c:1646  */
    {
        SymbolInfo *sInfo = new SymbolInfo("", "statement");
        (yyval.node)=new Node(sInfo,"statement : PRINTLN LPAREN ID RPAREN SEMICOLON");
        CheckVariableDeclaredOrNot((yyvsp[-2].node));
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-4].node),(yyvsp[-3].node),(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)});
        util.printGrammar("statement : PRINTLN LPAREN ID RPAREN SEMICOLON");
        

}
#line 2265 "y.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 741 "2005087.y" /* yacc.c:1646  */
    {
        SymbolInfo *sInfo = new SymbolInfo("", "statement");
        (yyval.node)=new Node(sInfo,"statement : RETURN expression SEMICOLON");
        CheckReturnIssues((yyvsp[-1].node));
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)});
        util.printGrammar("statement : RETURN expression SEMICOLON");  
        
}
#line 2279 "y.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 751 "2005087.y" /* yacc.c:1646  */
    {
        SymbolInfo *sInfo = new SymbolInfo("", "expression_statement");
        (yyval.node)=new Node(sInfo,"expression_statement : SEMICOLON");
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[0].node)});
        util.printGrammar("expression_statement : SEMICOLON"); 
}
#line 2291 "y.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 758 "2005087.y" /* yacc.c:1646  */
    {
      SymbolInfo *sInfo = new SymbolInfo("", "expression_statement");
      (yyval.node)=new Node(sInfo,"expression_statement : expression SEMICOLON");  
      pTree.setCurrentNode((yyval.node));
      (yyval.node)->makeChild({(yyvsp[-1].node),(yyvsp[0].node)});
      util.printGrammar("expression_statement : expression SEMICOLON"); 
      
}
#line 2304 "y.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 767 "2005087.y" /* yacc.c:1646  */
    {
        SymbolInfo *sInfo = new SymbolInfo((yyvsp[0].node)->getName(), "VARIABLE");
        (yyval.node)=new Node(sInfo,"variable : ID",(yyvsp[0].node)->getReturnOrDataType());
        CheckVariableDeclaredOrNot((yyvsp[0].node));
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[0].node)});
        util.printGrammar("variable : ID");
        
        		
}
#line 2319 "y.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 777 "2005087.y" /* yacc.c:1646  */
    {
        SymbolInfo *sInfo = new SymbolInfo((yyvsp[-3].node)->getName(), "VARIABLE");
        (yyval.node)=new Node(sInfo,"variable : ID LSQUARE expression RSQUARE",(yyvsp[-3].node)->getReturnOrDataType());
        CheckArrayIssues((yyval.node),(yyvsp[-3].node),(yyvsp[-1].node));
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-3].node),(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)});
        util.printGrammar("variable : ID LSQUARE expression RSQUARE");      
}
#line 2332 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 786 "2005087.y" /* yacc.c:1646  */
    {
        SymbolInfo *sInfo = new SymbolInfo((yyvsp[0].node)->getName(), "expression");
        (yyval.node)=new Node(sInfo,"expression : logic_expression",(yyvsp[0].node)->getReturnOrDataType());
        (yyval.node)->setArrayStatus((yyvsp[0].node)->getArrayStatus());
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[0].node)});
        util.printGrammar("expression : logic_expression");
}
#line 2345 "y.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 794 "2005087.y" /* yacc.c:1646  */
    {
        SymbolInfo *sInfo = new SymbolInfo("", "expression");
        (yyval.node)=new Node(sInfo,"expression : variable ASSIGNOP logic_expression");
        CheckAssignmentIssues((yyval.node),(yyvsp[-2].node),(yyvsp[0].node));
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)});
        util.printGrammar("expression : variable ASSIGNOP logic_expression");
}
#line 2358 "y.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 803 "2005087.y" /* yacc.c:1646  */
    {
        SymbolInfo *sInfo = new SymbolInfo((yyvsp[0].node)->getName(), "logic_expression");
        (yyval.node)=new Node(sInfo,"logic_expression : rel_expression",(yyvsp[0].node)->getReturnOrDataType());
        (yyval.node)->setArrayStatus((yyvsp[0].node)->getArrayStatus());
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[0].node)});
        util.printGrammar("logic_expression : rel_expression");
}
#line 2371 "y.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 811 "2005087.y" /* yacc.c:1646  */
    {
		SymbolInfo *sInfo = new SymbolInfo("", "logic_expression");
        CheckLogicalIssues((yyval.node),(yyvsp[-2].node),(yyvsp[0].node));
        (yyval.node)=new Node(sInfo,"logic_expression : rel_expression LOGICOP rel_expression");
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)});
        util.printGrammar("logic_expression : rel_expression LOGICOP rel_expression");
}
#line 2384 "y.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 820 "2005087.y" /* yacc.c:1646  */
    {
        SymbolInfo *sInfo = new SymbolInfo((yyvsp[0].node)->getName(), "rel_expression");
        (yyval.node)=new Node(sInfo,"rel_expression : simple_expression",(yyvsp[0].node)->getReturnOrDataType());
        (yyval.node)->setArrayStatus((yyvsp[0].node)->getArrayStatus());
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[0].node)});
        util.printGrammar("rel_expression : simple_expression");
       
}
#line 2398 "y.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 829 "2005087.y" /* yacc.c:1646  */
    {
        SymbolInfo *sInfo= new SymbolInfo("", "rel_expression");
        (yyval.node)=new Node(sInfo,"rel_expression : simple_expression RELOP simple_expression");
        CheckRelationalIssues((yyval.node),(yyvsp[-2].node),(yyvsp[0].node));
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)});
        util.printGrammar("rel_expression : simple_expression RELOP simple_expression");
}
#line 2411 "y.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 838 "2005087.y" /* yacc.c:1646  */
    {
       SymbolInfo *sInfo = new SymbolInfo((yyvsp[0].node)->getName(), "simple_expression");
       (yyval.node)=new Node(sInfo,"simple_expression : term",(yyvsp[0].node)->getReturnOrDataType());
       (yyval.node)->setArrayStatus((yyvsp[0].node)->getArrayStatus());
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[0].node)});
        util.printGrammar("simple_expression : term");
        
}
#line 2425 "y.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 847 "2005087.y" /* yacc.c:1646  */
    {
        SymbolInfo *sInfo = new SymbolInfo("", "simple_expression");
        (yyval.node)=new Node(sInfo,"simple_expression : simple_expression ADDOP term",TypeCasting((yyvsp[-2].node)->getReturnOrDataType(), (yyvsp[0].node)->getReturnOrDataType()));
        if ((yyvsp[-2].node)->getReturnOrDataType() == "VOID" || (yyvsp[0].node)->getReturnOrDataType() == "VOID") {
			util.printError("Void cannot be used in expression", yylineno);
		}
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)});
        util.printGrammar("simple_expression : simple_expression ADDOP term");	
}
#line 2440 "y.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 858 "2005087.y" /* yacc.c:1646  */
    {
       SymbolInfo *sInfo = new SymbolInfo((yyvsp[0].node)->getName(), "term");
        (yyval.node)=new Node(sInfo,"term : unary_expression",(yyvsp[0].node)->getReturnOrDataType());
        (yyval.node)->setArrayStatus((yyvsp[0].node)->getArrayStatus());
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[0].node)});
        util.printGrammar("term : unary_expression");
}
#line 2453 "y.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 866 "2005087.y" /* yacc.c:1646  */
    {
      SymbolInfo *sInfo = new SymbolInfo("", "term");  
      (yyval.node)=new Node(sInfo,"term : term MULOP unary_expression");
      CheckMultiplicationIssues((yyval.node),(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node));
      pTree.setCurrentNode((yyval.node));
      (yyval.node)->makeChild({(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)});
      util.printGrammar("term : term MULOP unary_expression");  
}
#line 2466 "y.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 875 "2005087.y" /* yacc.c:1646  */
    {
    SymbolInfo *sInfo = new SymbolInfo("", "unary_expression");
    (yyval.node)=new Node(sInfo,"unary_expression : ADDOP unary_expression");
    if ((yyvsp[0].node)->getReturnOrDataType() == "VOID") {
			util.printError("Void cannot be used in expression", yylineno);
		}
		else {(yyval.node)->setReturnOrDataType((yyvsp[0].node)->getReturnOrDataType());}
    pTree.setCurrentNode((yyval.node));
    (yyval.node)->makeChild({(yyvsp[-1].node),(yyvsp[0].node)});
    util.printGrammar("unary_expression : ADDOP unary_expression");
		
}
#line 2483 "y.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 887 "2005087.y" /* yacc.c:1646  */
    {
        SymbolInfo *sInfo = new SymbolInfo("", "unary_expression");
        (yyval.node)=new Node(sInfo,"unary_expression : NOT unary_expression");
        CheckUnaryExpression((yyval.node),(yyvsp[0].node));
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-1].node),(yyvsp[0].node)});
        util.printGrammar("unary_expression : NOT unary_expression");

}
#line 2497 "y.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 896 "2005087.y" /* yacc.c:1646  */
    {
        SymbolInfo *sInfo = new SymbolInfo((yyvsp[0].node)->getName(), "unary_expression");
        (yyval.node)=new Node(sInfo,"unary_expression : factor",(yyvsp[0].node)->getReturnOrDataType());
        (yyval.node)->setArrayStatus((yyvsp[0].node)->getArrayStatus());
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[0].node)});
        util.printGrammar("unary_expression : factor");
        
}
#line 2511 "y.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 908 "2005087.y" /* yacc.c:1646  */
    {
       SymbolInfo *sInfo = new SymbolInfo((yyvsp[0].node)->getName(), "factor");
       (yyval.node)=new Node(sInfo,"factor : variable",(yyvsp[0].node)->getReturnOrDataType());
       (yyval.node)->setArrayStatus((yyvsp[0].node)->getArrayStatus());
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[0].node)});
        util.printGrammar("factor : variable");
        
}
#line 2525 "y.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 917 "2005087.y" /* yacc.c:1646  */
    {
        SymbolInfo *sInfo = new SymbolInfo("", "factor");
        (yyval.node)=new Node(sInfo,"factor : ID LPAREN argument_list RPAREN");
        callFunction((yyval.node),(yyvsp[-3].node),(yyvsp[-1].node));
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-3].node),(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)});
        util.printGrammar("factor : ID LPAREN argument_list RPAREN");
}
#line 2538 "y.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 926 "2005087.y" /* yacc.c:1646  */
    {
        SymbolInfo *sInfo = new SymbolInfo((yyvsp[-1].node)->getName(), "factor");
        (yyval.node)=new Node(sInfo,"factor : LPAREN expression RPAREN",(yyvsp[-1].node)->getReturnOrDataType());
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)});
        util.printGrammar("factor : LPAREN expression RPAREN");
        
}
#line 2551 "y.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 934 "2005087.y" /* yacc.c:1646  */
    {
        SymbolInfo *sInfo = new SymbolInfo((yyvsp[0].node)->getName(), "factor");	
        (yyval.node)=new Node(sInfo,"factor : CONST_INT","INT");
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[0].node)});
        util.printGrammar("factor : CONST_INT");
}
#line 2563 "y.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 941 "2005087.y" /* yacc.c:1646  */
    {
        SymbolInfo *sInfo = new SymbolInfo((yyvsp[0].node)->getName(), "factor");
        (yyval.node)=new Node(sInfo,"factor : CONST_FLOAT","FLOAT");
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[0].node)});
        util.printGrammar("factor : CONST_FLOAT");

}
#line 2576 "y.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 949 "2005087.y" /* yacc.c:1646  */
    {
        SymbolInfo *sInfo = new SymbolInfo("", "factor");
        (yyval.node)=new Node(sInfo,"factor : variable INCOP");
        CheckUnaryOperandIssues((yyval.node),(yyvsp[-1].node));
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-1].node),(yyvsp[0].node)});
        util.printGrammar("factor : variable INCOP");
		
}
#line 2590 "y.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 958 "2005087.y" /* yacc.c:1646  */
    {
        SymbolInfo *sInfo= new SymbolInfo("", "factor");
        (yyval.node)=new Node(sInfo,"factor : variable DECOP");
        CheckUnaryOperandIssues((yyval.node),(yyvsp[-1].node));
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-1].node),(yyvsp[0].node)});
        util.printGrammar("factor : variable DECOP");
}
#line 2603 "y.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 968 "2005087.y" /* yacc.c:1646  */
    {
       SymbolInfo *sInfo = new SymbolInfo("", "argument_list");
        (yyval.node)=new Node(sInfo,"argument_list : arguments");
        (yyval.node)->addParameter((yyvsp[0].node)->getParameterList());
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[0].node)});
        util.printGrammar("argument_list : arguments");		
}
#line 2616 "y.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 976 "2005087.y" /* yacc.c:1646  */
    {
        SymbolInfo *sInfo= new SymbolInfo("", "argument_list");
        (yyval.node)=new Node(sInfo,"argument_list : ");
        util.printGrammar("argument_list : ");
}
#line 2626 "y.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 982 "2005087.y" /* yacc.c:1646  */
    {
    SymbolInfo *sInfo = new SymbolInfo("", "arguments");
    (yyval.node)=new Node(sInfo,"arguments : arguments COMMA logic_expression");
    (yyval.node)->addParameter((yyvsp[-2].node)->getParameterList());
    (yyval.node)->addParameter({{(yyvsp[0].node)->getReturnOrDataType(),(yyvsp[0].node)->getName()},(yyvsp[0].node)->getArrayStatus()});
    pTree.setCurrentNode((yyval.node));
    (yyval.node)->makeChild({(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)});
    util.printGrammar("arguments : arguments COMMA logic_expression");
}
#line 2640 "y.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 991 "2005087.y" /* yacc.c:1646  */
    {
        SymbolInfo *sInfo = new SymbolInfo("", "arguments");
        (yyval.node)=new Node(sInfo,"arguments : logic_expression");
        (yyval.node)->addParameter({{(yyvsp[0].node)->getReturnOrDataType(),(yyvsp[0].node)->getName()},(yyvsp[0].node)->getArrayStatus()});
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[0].node)});
        util.printGrammar("arguments : logic_expression");
}
#line 2653 "y.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 1000 "2005087.y" /* yacc.c:1646  */
    {
    (yyval.node)=(yyvsp[0].node);
    sTable.EnterScope();
    for (auto functionArgument : currentFunctionParameters) {
        string functionName=functionArgument.first.second;
			if (functionName == "") {
				continue;
			}
			Node* toBeInserted = new Node(new SymbolInfo(functionName, "ID"),"", functionArgument.first.first);
			toBeInserted->setArrayStatus(functionArgument.second);
			if (!sTable.Insert(toBeInserted)) {
                util.printError("Redefinition of parameter \'"+toBeInserted->getName() +"\'",yylineno );
				break;
			}
		}
        currentFunctionParameters.clear();
}
#line 2675 "y.tab.c" /* yacc.c:1646  */
    break;


#line 2679 "y.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 1018 "2005087.y" /* yacc.c:1906  */


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

    //sTable.PrintAllScope(util.getLogFout());
    util.getLogFout()<<"Total Lines: "<<yylineno<<endl;
    util.getLogFout()<<"Total Errors: "<<util.getErrorCount()<<endl;
    //util.getLogFout()<<"Total warnings: "<<util.getWarningCount()<<endl;
//cout<<currentFunctionReturnType<<endl;
    return EXIT_SUCCESS;
}
