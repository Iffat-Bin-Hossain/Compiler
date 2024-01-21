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
			util.printError("redeclaration of " + functionName, startLine);
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
				util.printError("Conflicting types for \'" + functionName + "\'", startLine);
				// return;
			}

			if (searched->getParameterList().size() != parameterList.size())
			{
				util.printError("Conflicting types for \'" + functionName + "\'", startLine);
			}

			searched->setFunctionInfo("FUNCTION_DEFINITION");
			searched->addParameter(parameterList);
			searched->setReturnOrDataType(returnType);
		}
		else
		{
			util.printError("\'" + functionName + "\' redeclared as different kind of symbol", startLine);
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
		util.printError("Undeclared function \'" + nameNode->getName() + "\'", actualNode->getStartLine());
	}
	else if (!(searched->getFunctionInfo() == "FUNCTION_DECLARATION" || searched->getFunctionInfo() == "FUNCTION_DEFINITION"))
	{
		util.printError("\'" + nameNode->getName() + "\' is not a function", actualNode->getStartLine());
	}
	else if (searched->getParameterList().size() < parameterList.size())
	{
		util.printError("Too many arguments to function \'" + nameNode->getName() + "\'", actualNode->getStartLine());
		return;
	}
	else if (searched->getParameterList().size() > parameterList.size())
	{
		util.printError("Too few arguments to function \'" + nameNode->getName() + "\'", actualNode->getStartLine());
		return;
	}
	else
	{
		for (int i = 0; i < searched->getParameterList().size(); i++)
		{

			
			if ((searched->getParameterList().at(i).first.first != parameterList[i].first.first))
			{
				util.printError("Type mismatch for argument " + to_string(i + 1) + " of \'" + nameNode->getName() + "\'", actualNode->getStartLine());
				
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
			util.printError("Variable or field \'" + element.first.second + "\' declared void", startLine);
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
			util.printError("Conflicting types for \'" + element.first.second + "\'", startLine);
		}
		else
		{
			util.printError("Redefinition of variable \'" + element.first.second + "\'", startLine);
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
		util.printError("Undeclared variable \'" + variable + "\'", node->getStartLine());
		return;
	}
	node->setReturnOrDataType(searched->getReturnOrDataType());
	node->setArrayStatus(searched->getArrayStatus());
}
void CheckReturnIssues(Node *node)
{
	// cout<<currentFunctionReturnType;
	//  if (currentFunctionReturnType == "VOID")
	//  {
	//  	util.printError("Function return type void", node->getStartLine());
	//  }
	if (currentFunctionReturnType == "INT" && node->getReturnOrDataType() == "FLOAT")
	{
		util.printError("Warning: possible loss of data in assignment of FLOAT to INT", node->getStartLine());
	}
}
void CheckArrayIssues(Node *actualNode, Node *nameNode, Node *indexNode)
{
	string name = nameNode->getName();
	Node *toBeSearched = new Node(new SymbolInfo(name, "ID"));
	Node *searched = sTable.Lookup(toBeSearched);
	if (searched == NULL)
	{
		util.printError("Undeclared variable \'" + name + "\'", actualNode->getStartLine());
		return;
	}
	if (!searched->getArrayStatus())
	{
		util.printError("\'" + name + "\' is not an array", actualNode->getStartLine());
		return;
	}
	if (indexNode->getReturnOrDataType() != "INT")
	{
		util.printError("Array subscript is not an integer", actualNode->getStartLine());
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
		util.printError("Type mismatch for \'" + leftNode->getName() + "\' , is an array", actualNode->getStartLine());
		return;
	}
	if (leftNode->getReturnOrDataType() == "VOID" || rightNode->getReturnOrDataType() == "VOID")
	{
		util.printError("Void cannot be used in expression", actualNode->getStartLine());
		return;
	}
	if (leftNode->getReturnOrDataType() == "INT")
	{
		if (rightNode->getReturnOrDataType() == "FLOAT")
		{
			util.printError("Warning: possible loss of data in assignment of FLOAT to INT", actualNode->getStartLine());
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
		util.printError("Void cannot be used in expression", actualNode->getStartLine());
		return;
	}
	if (leftNode->getReturnOrDataType() == "FLOAT" || rightNode->getReturnOrDataType() == "FLOAT")
	{
		util.printError("Warning: Void cannot be used in expression", actualNode->getStartLine());
		actualNode->setReturnOrDataType("INT");
	}
	actualNode->setReturnOrDataType("INT");
}
void CheckRelationalIssues(Node *actualNode, Node *leftNode, Node *rightNode)
{
	if (leftNode->getReturnOrDataType() == "VOID" || rightNode->getReturnOrDataType() == "VOID")
	{
		util.printError("Void cannot be used in expression", actualNode->getStartLine());
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
// string TypeCasting(string  x, string y){
//     if (x == "VOID" || y == "VOID" || x == "ERROR" || y == "ERROR") {
//         return "ERROR";
//     }
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

string TypeCasting(string s1, const string s2)
{
	// if (s1 == "VOID" || s2 == "VOID" || s1 == "ERROR" || s2 == "ERROR")
	// {
	// 	return "ERROR";
	// }
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
		util.printError("Void cannot be used in expression", actualNode->getStartLine());
	}

	else if (midNode->getName() == "%")
	{
		if (leftNode->getReturnOrDataType() == "FLOAT" || rightNode->getReturnOrDataType() == "FLOAT")
		{
			util.printError("Operands of modulus must be integers", actualNode->getStartLine());
		}
		else if (CheckingZerOperand(rightNode->getName()))
		{
			util.printError("Warning: division by zero i=0f=1Const=0", actualNode->getStartLine());
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
			util.printError("Warning: division by zero i=0f=1Const=0", actualNode->getStartLine());
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
		util.printError("Void cannot be used in expression", actualNode->getStartLine());
		return;
	}
	else if (node->getReturnOrDataType() == "FLOAT")
	{
		util.printError("Warning :Operands of bitwise operation should be integers", actualNode->getStartLine());
	}
	actualNode->setReturnOrDataType("INT");
}
void CheckUnaryOperandIssues(Node *actualNode, Node *operand)
{
	if (operand->getReturnOrDataType() == "VOID")
	{
		util.printError("Void cannot be used in expression", actualNode->getStartLine());
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
#define YYLAST   146

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  43
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  28
/* YYNRULES -- Number of rules.  */
#define YYNRULES  68
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  122

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
       0,   411,   411,   424,   436,   448,   459,   470,   482,   500,
     519,   519,   537,   537,   559,   573,   587,   600,   614,   614,
     643,   643,   673,   687,   698,   709,   721,   734,   748,   760,
     773,   784,   796,   807,   818,   830,   841,   852,   863,   873,
     883,   896,   907,   919,   932,   945,   957,   970,   982,   995,
    1007,  1020,  1032,  1047,  1059,  1072,  1088,  1101,  1116,  1129,
    1142,  1153,  1164,  1176,  1188,  1202,  1214,  1220,  1233
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
  "$@2", "parameter_list", "compound_statement", "$@3", "$@4",
  "var_declaration", "type_specifier", "declaration_list", "statements",
  "statement", "expression_statement", "variable", "expression",
  "logic_expression", "rel_expression", "simple_expression", "term",
  "unary_expression", "factor", "argument_list", "arguments", YY_NULLPTR
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

#define YYPACT_NINF -67

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-67)))

#define YYTABLE_NINF -21

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      40,   -67,   -67,   -67,    22,    40,   -67,   -67,   -67,   -67,
      18,   -67,   -67,     8,    25,    -1,    20,    31,   -67,    19,
      -9,    48,    -7,    47,   -67,    54,    62,    40,   -67,   -67,
      76,    72,   -67,   -67,    54,    81,    82,    71,    73,   -67,
     -67,   -67,    90,    93,    94,     9,    89,    95,   -67,   -67,
      89,    89,   -67,    89,   -67,   -67,   103,    50,   -67,   -67,
     -20,    91,   -67,    86,     2,    92,   -67,   -67,   -67,    89,
      84,    89,    89,    89,    96,   107,    52,   -67,   101,   -67,
     100,   -67,   -67,   -67,   -67,    89,   -67,    89,    89,    89,
      89,   104,    84,   105,   -67,   106,   102,   108,   -67,   109,
     -67,   -67,   -67,    92,   110,   -67,    71,    89,    71,   -67,
      89,   -67,   112,   131,   111,   -67,   -67,   -67,    71,    71,
     -67,   -67
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    25,    23,    24,     0,     2,     4,     6,     7,     5,
       0,     1,     3,    28,     0,     0,     0,     0,    22,    12,
       0,    17,     0,    26,     9,     0,    10,     0,    16,    29,
       0,    18,    13,     8,     0,    15,     0,     0,     0,    11,
      14,    27,     0,     0,     0,    43,     0,     0,    61,    62,
       0,     0,    41,     0,    34,    32,     0,     0,    30,    33,
      58,     0,    45,    47,    49,    51,    53,    57,    21,     0,
       0,     0,    66,     0,     0,     0,    58,    56,     0,    55,
      28,    19,    31,    63,    64,     0,    42,     0,     0,     0,
       0,     0,     0,     0,    68,     0,    65,     0,    40,     0,
      60,    46,    48,    52,    50,    54,     0,     0,     0,    59,
       0,    44,     0,    36,     0,    38,    67,    39,     0,     0,
      37,    35
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -67,   -67,   -67,   132,   -67,   -67,   -67,   -67,   -67,   -22,
     -67,   -67,    28,     5,   -67,   -67,   -57,   -61,   -42,   -44,
     -66,    53,    57,    55,   -49,   -67,   -67,   -67
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     4,     5,     6,     7,     8,    34,    25,    20,    54,
      37,    38,    55,    56,    14,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    95,    96
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      82,    77,    74,    32,    79,    10,    94,    78,    76,    92,
      10,    76,    39,     1,     2,     3,    83,    84,    26,   101,
      21,    85,    11,    27,    29,    91,    19,    93,     9,    97,
      76,   107,    35,     9,    15,    72,    88,    13,    16,    73,
      89,   105,    22,    76,   116,    76,    76,    76,    76,   113,
      23,   115,    24,    42,     1,     2,     3,    17,    18,    43,
      44,   120,   121,   114,     1,     2,     3,    28,    76,    45,
      46,    47,    48,    49,    42,    50,    51,    30,    31,    81,
      43,    44,    31,    52,    53,     1,     2,     3,    83,    84,
      45,    46,    47,    48,    49,    33,    50,    51,    36,    31,
      40,   -20,    68,    45,    52,    53,    48,    49,    45,    50,
      51,    48,    49,    41,    50,    51,    69,    52,    53,    70,
      71,    75,    80,    53,    86,    87,    99,    90,   100,    98,
      16,   106,   108,   109,   110,   118,   112,    12,   119,   111,
     102,     0,     0,   103,    88,   117,   104
};

static const yytype_int8 yycheck[] =
{
      57,    50,    46,    25,    53,     0,    72,    51,    50,    70,
       5,    53,    34,    14,    15,    16,    36,    37,    27,    85,
      15,    41,     0,    32,    31,    69,    27,    71,     0,    73,
      72,    92,    27,     5,    26,    26,    34,    19,    30,    30,
      38,    90,    22,    85,   110,    87,    88,    89,    90,   106,
      19,   108,    33,     3,    14,    15,    16,    32,    33,     9,
      10,   118,   119,   107,    14,    15,    16,    19,   110,    19,
      20,    21,    22,    23,     3,    25,    26,    30,    28,    29,
       9,    10,    28,    33,    34,    14,    15,    16,    36,    37,
      19,    20,    21,    22,    23,    33,    25,    26,    22,    28,
      19,    29,    29,    19,    33,    34,    22,    23,    19,    25,
      26,    22,    23,    31,    25,    26,    26,    33,    34,    26,
      26,    26,    19,    34,    33,    39,    19,    35,    27,    33,
      30,    27,    27,    27,    32,     4,    27,     5,    27,    31,
      87,    -1,    -1,    88,    34,    33,    89
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    14,    15,    16,    44,    45,    46,    47,    48,    55,
      56,     0,    46,    19,    57,    26,    30,    32,    33,    27,
      51,    56,    22,    19,    33,    50,    27,    32,    19,    31,
      30,    28,    52,    33,    49,    56,    22,    53,    54,    52,
      19,    31,     3,     9,    10,    19,    20,    21,    22,    23,
      25,    26,    33,    34,    52,    55,    56,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    29,    26,
      26,    26,    26,    30,    62,    26,    61,    67,    62,    67,
      19,    29,    59,    36,    37,    41,    33,    39,    34,    38,
      35,    62,    60,    62,    63,    69,    70,    62,    33,    19,
      27,    63,    64,    66,    65,    67,    27,    60,    27,    27,
      32,    31,    27,    59,    62,    59,    63,    33,     4,    27,
      59,    59
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    43,    44,    45,    45,    46,    46,    46,    47,    47,
      49,    48,    50,    48,    51,    51,    51,    51,    53,    52,
      54,    52,    55,    56,    56,    56,    57,    57,    57,    57,
      58,    58,    59,    59,    59,    59,    59,    59,    59,    59,
      59,    60,    60,    61,    61,    62,    62,    63,    63,    64,
      64,    65,    65,    66,    66,    67,    67,    67,    68,    68,
      68,    68,    68,    68,    68,    69,    69,    70,    70
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     1,     6,     5,
       0,     7,     0,     6,     4,     3,     2,     1,     0,     4,
       0,     3,     3,     1,     1,     1,     3,     6,     1,     4,
       1,     2,     1,     1,     1,     7,     5,     7,     5,     5,
       3,     1,     2,     1,     4,     1,     3,     1,     3,     1,
       3,     1,     3,     1,     3,     2,     2,     1,     1,     4,
       3,     1,     1,     2,     2,     1,     0,     3,     1
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
#line 411 "2005087.y" /* yacc.c:1646  */
    {
    
        SymbolInfo *sInfo=new SymbolInfo("","start");
        (yyval.node) =new Node(sInfo,"start : program");
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->setStartLine((yyvsp[0].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine());
        (yyval.node)->makeChild({(yyvsp[0].node)});
        util.printGrammar("start : program");
        pTree.printParseTree(util.getTreeFout());
    
}
#line 1786 "y.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 424 "2005087.y" /* yacc.c:1646  */
    {
    
        SymbolInfo *sInfo=new SymbolInfo("","program");
        (yyval.node)=new Node(sInfo,"program : program unit");
        (yyval.node)->setStartLine((yyvsp[-1].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine());
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-1].node),(yyvsp[0].node)});
        util.printGrammar("program : program unit");
     
    
}
#line 1803 "y.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 436 "2005087.y" /* yacc.c:1646  */
    {
   
        SymbolInfo *sInfo=new SymbolInfo("","program");
        (yyval.node)=new Node(sInfo,"program : unit");
        (yyval.node)->setStartLine((yyvsp[0].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine());
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[0].node)});
        util.printGrammar("program : unit");
   
}
#line 1819 "y.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 448 "2005087.y" /* yacc.c:1646  */
    {
   
        SymbolInfo *sInfo=new SymbolInfo("","unit");
        (yyval.node)=new Node(sInfo,"unit : var_declaration");
        (yyval.node)->setStartLine((yyvsp[0].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine());
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[0].node)});
        util.printGrammar("unit : var_declaration");
    
}
#line 1835 "y.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 459 "2005087.y" /* yacc.c:1646  */
    {
    
        SymbolInfo *sInfo=new SymbolInfo("","unit");
        (yyval.node)=new Node(sInfo,"unit : func_declaration");
        (yyval.node)->setStartLine((yyvsp[0].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine());
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[0].node)});
        util.printGrammar("unit : func_declaration");
   
}
#line 1851 "y.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 470 "2005087.y" /* yacc.c:1646  */
    {
        
		SymbolInfo *sInfo = new SymbolInfo("", "unit");
        (yyval.node)=new Node(sInfo,"unit : func_definition");
        (yyval.node)->setStartLine((yyvsp[0].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine());
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[0].node)});
		util.printGrammar("unit : func_definition");
       
}
#line 1867 "y.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 482 "2005087.y" /* yacc.c:1646  */
    {
     
    currentFunctionParameters.clear();
        SymbolInfo *sInfo = new SymbolInfo("", "func_declaration");
        (yyval.node)=new Node(sInfo,"func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON");
        (yyval.node)->setStartLine((yyvsp[-5].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine());
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-5].node),(yyvsp[-4].node),(yyvsp[-3].node),(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)});
        util.printGrammar("func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON");
        //Inserting the function in SymbolTable
        string functionName=(yyvsp[-4].node)->getName();
        string returnType=(yyvsp[-5].node)->getReturnOrDataType();
        vector<pair<pair<string, string>,bool>> parameterList = (yyvsp[-2].node)->getParameterList();
        declareFunction(functionName, returnType,(yyval.node)->getStartLine(), parameterList);
       
        
}
#line 1890 "y.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 500 "2005087.y" /* yacc.c:1646  */
    {
        
        currentFunctionParameters.clear();
        SymbolInfo *sInfo = new SymbolInfo("", "func_declaration");
        (yyval.node)=new Node(sInfo,"func_declaration : type_specifier ID LPAREN RPAREN SEMICOLON");
        (yyval.node)->setStartLine((yyvsp[-4].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine());
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-4].node),(yyvsp[-3].node),(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)});
        util.printGrammar("func_declaration : type_specifier ID LPAREN RPAREN SEMICOLON");
        //Inserting the function in SymbolTable
        string functionName=(yyvsp[-3].node)->getName();
        string returnType=(yyvsp[-4].node)->getReturnOrDataType();
        declareFunction(functionName, returnType,(yyval.node)->getStartLine());
         
        
}
#line 1912 "y.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 519 "2005087.y" /* yacc.c:1646  */
    {
        currentFunctionReturnType=(yyvsp[-4].node)->getReturnOrDataType();
}
#line 1920 "y.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 522 "2005087.y" /* yacc.c:1646  */
    {
     
        SymbolInfo *sInfo = new SymbolInfo("", "func_definition");
        (yyval.node)=new Node(sInfo,"func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement");
        (yyval.node)->setStartLine((yyvsp[-6].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine());
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-6].node),(yyvsp[-5].node),(yyvsp[-4].node),(yyvsp[-3].node),(yyvsp[-2].node),(yyvsp[0].node)});
        util.printGrammar("func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement");
        string functionName=(yyvsp[-5].node)->getName();
        string returnType=(yyvsp[-6].node)->getReturnOrDataType();  
        vector<pair<pair<string, string>,bool>> parameterList = (yyvsp[-3].node)->getParameterList();
        defineFunction(functionName,returnType,(yyval.node)->getStartLine(),parameterList);
    
    }
#line 1940 "y.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 537 "2005087.y" /* yacc.c:1646  */
    {
        currentFunctionReturnType=(yyvsp[-3].node)->getReturnOrDataType();
    }
#line 1948 "y.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 540 "2005087.y" /* yacc.c:1646  */
    { 
    
        SymbolInfo *sInfo = new SymbolInfo("", "func_definition");
        (yyval.node)=new Node(sInfo,"func_definition : type_specifier ID LPAREN RPAREN compound_statement");
        (yyval.node)->setStartLine((yyvsp[-5].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine()); 
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-5].node),(yyvsp[-4].node),(yyvsp[-3].node),(yyvsp[-2].node),(yyvsp[0].node)});
        util.printGrammar("func_definition : type_specifier ID LPAREN RPAREN compound_statement");
        string functionName=(yyvsp[-4].node)->getName();
        string returnType=(yyvsp[-5].node)->getReturnOrDataType();
        defineFunction(functionName,returnType,(yyval.node)->getStartLine());
    
}
#line 1967 "y.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 559 "2005087.y" /* yacc.c:1646  */
    {

        SymbolInfo *sInfo = new SymbolInfo("", "parameter_list");
        (yyval.node)=new Node(sInfo,"parameter_list : parameter_list COMMA type_specifier ID");
        (yyval.node)->setStartLine((yyvsp[-3].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine());
        (yyval.node)->addParameter((yyvsp[-3].node)->getParameterList());
        (yyval.node)->addParameter({{(yyvsp[-1].node)->getReturnOrDataType(),(yyvsp[0].node)->getName()},false});
        currentFunctionParameters=(yyval.node)->getParameterList();
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-3].node),(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)});
        util.printGrammar("parameter_list : parameter_list COMMA type_specifier ID");
   
}
#line 1986 "y.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 573 "2005087.y" /* yacc.c:1646  */
    {

        SymbolInfo *sInfo = new SymbolInfo("", "parameter_list");
        (yyval.node)=new Node(sInfo,"parameter_list : parameter_list COMMA type_specifier");
        (yyval.node)->setStartLine((yyvsp[-2].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine());
        (yyval.node)->addParameter((yyvsp[-2].node)->getParameterList());
        (yyval.node)->addParameter({{(yyvsp[0].node)->getReturnOrDataType(),""},false});
        currentFunctionParameters=(yyval.node)->getParameterList();
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)});
        util.printGrammar("parameter_list : parameter_list COMMA type_specifier");
       
}
#line 2005 "y.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 587 "2005087.y" /* yacc.c:1646  */
    {

        SymbolInfo *sInfo = new SymbolInfo("", "parameter_list");
        (yyval.node)=new Node(sInfo,"parameter_list : type_specifier ID");
        (yyval.node)->addParameter({{(yyvsp[-1].node)->getReturnOrDataType(),(yyvsp[0].node)->getName()},false});
        (yyval.node)->setStartLine((yyvsp[-1].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine());
        currentFunctionParameters=(yyval.node)->getParameterList();
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-1].node),(yyvsp[0].node)});
        util.printGrammar("parameter_list : type_specifier ID");
  
}
#line 2023 "y.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 600 "2005087.y" /* yacc.c:1646  */
    {

        SymbolInfo *sInfo = new SymbolInfo("", "parameter_list");
        (yyval.node)=new Node(sInfo,"parameter_list : type_specifier");
        (yyval.node)->addParameter({{(yyvsp[0].node)->getReturnOrDataType(),""},false});
        (yyval.node)->setStartLine((yyvsp[0].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine());
        currentFunctionParameters=(yyval.node)->getParameterList();
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[0].node)});
        util.printGrammar("parameter_list : type_specifier");
    
}
#line 2041 "y.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 614 "2005087.y" /* yacc.c:1646  */
    {
        sTable.EnterScope();
        for (auto functionArgument : currentFunctionParameters) {
        string argumentName=functionArgument.first.second;
			if (argumentName == "") {
				continue;
			}
			Node* toBeInserted = new Node(new SymbolInfo(argumentName, "ID"),"", functionArgument.first.first);
			toBeInserted->setArrayStatus(functionArgument.second);
			if (!sTable.Insert(toBeInserted)) {
                util.printError("Redefinition of parameter \'"+toBeInserted->getName() +"\'",(yyvsp[0].node)->getStartLine() );
				break;
			}
		}
        currentFunctionParameters.clear();
    }
#line 2062 "y.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 630 "2005087.y" /* yacc.c:1646  */
    {

        SymbolInfo *sInfo = new SymbolInfo("", "compound_statement");
        (yyval.node)=new Node(sInfo,"compound_statement : LCURL statements RCURL");
        (yyval.node)->setStartLine((yyvsp[-3].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine());
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-3].node),(yyvsp[-1].node),(yyvsp[0].node)});
        util.printGrammar("compound_statement : LCURL statements RCURL");  
        sTable.PrintAllScope(util.getLogFout());
        sTable.ExitScope();
     
}
#line 2080 "y.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 643 "2005087.y" /* yacc.c:1646  */
    {
        sTable.EnterScope();
        for (auto functionArgument : currentFunctionParameters) {
        string argumentName=functionArgument.first.second;
            if (argumentName == "") {
                continue;
            }
            Node* toBeInserted = new Node(new SymbolInfo(argumentName, "ID"),"", functionArgument.first.first);
            toBeInserted->setArrayStatus(functionArgument.second);
            if (!sTable.Insert(toBeInserted)) {
                util.printError("Redefinition of parameter \'"+toBeInserted->getName() +"\'",(yyvsp[0].node)->getStartLine() );
                break;
            }
        }
        currentFunctionParameters.clear();
    }
#line 2101 "y.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 659 "2005087.y" /* yacc.c:1646  */
    {

        SymbolInfo *sInfo = new SymbolInfo("", "compound_statement");
        (yyval.node)=new Node(sInfo,"compound_statement : LCURL RCURL");
        (yyval.node)->setStartLine((yyvsp[-2].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine());
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-2].node),(yyvsp[0].node)});
        util.printGrammar("compound_statement : LCURL RCURL"); 
        sTable.PrintAllScope(util.getLogFout());
        sTable.ExitScope();
     
    }
#line 2119 "y.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 673 "2005087.y" /* yacc.c:1646  */
    {

        SymbolInfo *sInfo = new SymbolInfo("", "var_declaration");
        (yyval.node)=new Node(sInfo,"var_declaration : type_specifier declaration_list SEMICOLON",(yyvsp[-2].node)->getReturnOrDataType());
        (yyval.node)->setStartLine((yyvsp[-2].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine());
        SaveData((yyvsp[-2].node)->getReturnOrDataType(),(yyvsp[-1].node),(yyval.node)->getStartLine());
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)});
        util.printGrammar("var_declaration : type_specifier declaration_list SEMICOLON");
     
}
#line 2136 "y.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 687 "2005087.y" /* yacc.c:1646  */
    {

        SymbolInfo *sInfo = new SymbolInfo("", "type_specifier");
        (yyval.node)=new Node(sInfo,"type_specifier : INT","INT");
        (yyval.node)->setStartLine((yyvsp[0].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine());
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[0].node)});
        util.printGrammar("type_specifier : INT");
       
}
#line 2152 "y.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 698 "2005087.y" /* yacc.c:1646  */
    {

        SymbolInfo *sInfo = new SymbolInfo("", "type_specifier");
        (yyval.node)=new Node(sInfo,"type_specifier : FLOAT","FLOAT");
        (yyval.node)->setStartLine((yyvsp[0].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine());
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[0].node)});
        util.printGrammar("type_specifier : FLOAT");
        
}
#line 2168 "y.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 709 "2005087.y" /* yacc.c:1646  */
    {

        SymbolInfo *sInfo = new SymbolInfo("", "type_specifier");
        (yyval.node)=new Node(sInfo,"type_specifier : VOID","VOID");
        (yyval.node)->setStartLine((yyvsp[0].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine());
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[0].node)});
        util.printGrammar("type_specifier : VOID");
        
}
#line 2184 "y.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 721 "2005087.y" /* yacc.c:1646  */
    {

        SymbolInfo *sInfo= new SymbolInfo("", "declaration_list");
        (yyval.node)=new Node(sInfo,"declaration_list : declaration_list COMMA ID");
        (yyval.node)->setStartLine((yyvsp[-2].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine());
        (yyval.node)->addParameter((yyvsp[-2].node)->getParameterList());
        (yyval.node)->addParameter({{"",(yyvsp[0].node)->getName()},false});
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)});
        util.printGrammar("declaration_list : declaration_list COMMA ID");
      
}
#line 2202 "y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 734 "2005087.y" /* yacc.c:1646  */
    {

        SymbolInfo *sInfo = new SymbolInfo("", "declaration_list");
        (yyval.node)=new Node(sInfo,"declaration_list : declaration_list COMMA ID LSQUARE CONST_INT RSQUARE");
        (yyval.node)->setStartLine((yyvsp[-5].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine());
        (yyval.node)->addParameter((yyvsp[-5].node)->getParameterList());
        (yyval.node)->addParameter({{"",(yyvsp[-3].node)->getName()},true});
        (yyval.node)->setArrayStatus(true);
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-5].node),(yyvsp[-4].node),(yyvsp[-3].node),(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)});
        util.printGrammar("declaration_list : declaration_list COMMA ID LSQUARE CONST_INT RSQUARE");
 
}
#line 2221 "y.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 748 "2005087.y" /* yacc.c:1646  */
    {

        SymbolInfo *sInfo = new SymbolInfo("", "declaration_list");
        (yyval.node)=new Node(sInfo,"declaration_list : ID");
        (yyval.node)->setStartLine((yyvsp[0].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine()); 
        (yyval.node)->addParameter({{(yyvsp[0].node)->getReturnOrDataType(),(yyvsp[0].node)->getName()},false});
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[0].node)});
        util.printGrammar("declaration_list : ID");  
 
}
#line 2238 "y.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 760 "2005087.y" /* yacc.c:1646  */
    {

        SymbolInfo *sInfo= new SymbolInfo("", "declaration_list");
        (yyval.node)=new Node(sInfo,"declaration_list : ID LSQUARE CONST_INT RSQUARE");
        (yyval.node)->setStartLine((yyvsp[-3].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine());
        (yyval.node)->addParameter({{(yyvsp[-3].node)->getReturnOrDataType(),(yyvsp[-3].node)->getName()},true});
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-3].node),(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)});
        util.printGrammar("declaration_list : ID LSQUARE CONST_INT RSQUARE"); 

}
#line 2255 "y.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 773 "2005087.y" /* yacc.c:1646  */
    {

        SymbolInfo *sInfo = new SymbolInfo("", "statement");
        (yyval.node)=new Node(sInfo,"statements : statement");
        (yyval.node)->setStartLine((yyvsp[0].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine()); 
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[0].node)});
        util.printGrammar("statements : statement");
  
}
#line 2271 "y.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 784 "2005087.y" /* yacc.c:1646  */
    {

        SymbolInfo *sInfo = new SymbolInfo("", "statement");
        (yyval.node)=new Node(sInfo,"statements : statements statement");
        (yyval.node)->setStartLine((yyvsp[-1].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine());
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-1].node),(yyvsp[0].node)});
        util.printGrammar("statements : statements statement"); 
 
}
#line 2287 "y.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 796 "2005087.y" /* yacc.c:1646  */
    {

        SymbolInfo *sInfo= new SymbolInfo("", "statement");
        (yyval.node)=new Node(sInfo,"statement : var_declaration",(yyvsp[0].node)->getReturnOrDataType());
        (yyval.node)->setStartLine((yyvsp[0].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine());
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[0].node)});
        util.printGrammar("statement : var_declaration");
      
}
#line 2303 "y.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 807 "2005087.y" /* yacc.c:1646  */
    {

        SymbolInfo *sInfo = new SymbolInfo("", "statement");
        (yyval.node)=new Node(sInfo,"statement : expression_statement",(yyvsp[0].node)->getReturnOrDataType());
        (yyval.node)->setStartLine((yyvsp[0].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine());
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[0].node)});
        util.printGrammar("statement : expression_statement");
     
}
#line 2319 "y.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 818 "2005087.y" /* yacc.c:1646  */
    {

        SymbolInfo *sInfo = new SymbolInfo("", "statement");
        (yyval.node)=new Node(sInfo,"statement : compound_statement",(yyvsp[0].node)->getReturnOrDataType());
        (yyval.node)->setStartLine((yyvsp[0].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine());
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[0].node)});
        util.printGrammar("statement : compound_statement");
   
        
}
#line 2336 "y.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 830 "2005087.y" /* yacc.c:1646  */
    {

        SymbolInfo *sInfo = new SymbolInfo("", "statement");
        (yyval.node)=new Node(sInfo,"statement : FOR LPAREN expression_statement expression_statement expression RPAREN statement");
        (yyval.node)->setStartLine((yyvsp[-6].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine());
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-6].node),(yyvsp[-5].node),(yyvsp[-4].node),(yyvsp[-3].node),(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)});
        util.printGrammar("statement : FOR LPAREN expression_statement expression_statement expression RPAREN statement");
 
}
#line 2352 "y.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 841 "2005087.y" /* yacc.c:1646  */
    {

        SymbolInfo *sInfo = new SymbolInfo("", "statement");
        (yyval.node)=new Node(sInfo,"statement : IF LPAREN expression RPAREN statement");
        (yyval.node)->setStartLine((yyvsp[-4].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine());
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-4].node),(yyvsp[-3].node),(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)});
        util.printGrammar("statement : IF LPAREN expression RPAREN statement");
     
}
#line 2368 "y.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 852 "2005087.y" /* yacc.c:1646  */
    {

        SymbolInfo *sInfo = new SymbolInfo("", "statement");
        (yyval.node)=new Node(sInfo,"statement : IF LPAREN expression RPAREN statement ELSE statement");
        (yyval.node)->setStartLine((yyvsp[-6].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine());
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-6].node),(yyvsp[-5].node),(yyvsp[-4].node),(yyvsp[-3].node),(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)});
        util.printGrammar("statement : IF LPAREN expression RPAREN statement ELSE statement");
 
}
#line 2384 "y.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 863 "2005087.y" /* yacc.c:1646  */
    {
        SymbolInfo *sInfo= new SymbolInfo("", "statement");
        (yyval.node)=new Node(sInfo,"statement : WHILE LPAREN expression RPAREN statement");
        (yyval.node)->setStartLine((yyvsp[-4].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine());
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-4].node),(yyvsp[-3].node),(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)});
        util.printGrammar("statement : WHILE LPAREN expression RPAREN statement");  
 
}
#line 2399 "y.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 873 "2005087.y" /* yacc.c:1646  */
    {
        SymbolInfo *sInfo = new SymbolInfo("", "statement");
        (yyval.node)=new Node(sInfo,"statement : PRINTLN LPAREN ID RPAREN SEMICOLON");
        (yyval.node)->setStartLine((yyvsp[-4].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine());
        CheckVariableDeclaredOrNot((yyvsp[-2].node));
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-4].node),(yyvsp[-3].node),(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)});
        util.printGrammar("statement : PRINTLN LPAREN ID RPAREN SEMICOLON");
}
#line 2414 "y.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 883 "2005087.y" /* yacc.c:1646  */
    {

        SymbolInfo *sInfo = new SymbolInfo("", "statement");
        (yyval.node)=new Node(sInfo,"statement : RETURN expression SEMICOLON");
        (yyval.node)->setStartLine((yyvsp[-2].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine()); 
        CheckReturnIssues((yyvsp[-1].node));
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)});
        util.printGrammar("statement : RETURN expression SEMICOLON");  
 
}
#line 2431 "y.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 896 "2005087.y" /* yacc.c:1646  */
    {

        SymbolInfo *sInfo = new SymbolInfo("", "expression_statement");
        (yyval.node)=new Node(sInfo,"expression_statement : SEMICOLON");
        (yyval.node)->setStartLine((yyvsp[0].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine());
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[0].node)});
        util.printGrammar("expression_statement : SEMICOLON"); 
     
}
#line 2447 "y.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 907 "2005087.y" /* yacc.c:1646  */
    {

        SymbolInfo *sInfo = new SymbolInfo("", "expression_statement");
        (yyval.node)=new Node(sInfo,"expression_statement : expression SEMICOLON");  
        (yyval.node)->setStartLine((yyvsp[-1].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine());
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-1].node),(yyvsp[0].node)});
        util.printGrammar("expression_statement : expression SEMICOLON"); 
       
}
#line 2463 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 919 "2005087.y" /* yacc.c:1646  */
    {

        SymbolInfo *sInfo = new SymbolInfo((yyvsp[0].node)->getName(), "VARIABLE");
        CheckVariableDeclaredOrNot((yyvsp[0].node));
        (yyval.node)=new Node(sInfo,"variable : ID",(yyvsp[0].node)->getReturnOrDataType()); 
        (yyval.node)->setStartLine((yyvsp[0].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine());
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[0].node)});
        util.printGrammar("variable : ID");
  
        		
}
#line 2481 "y.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 932 "2005087.y" /* yacc.c:1646  */
    {

        SymbolInfo *sInfo = new SymbolInfo((yyvsp[-3].node)->getName(), "VARIABLE");
        (yyval.node)=new Node(sInfo,"variable : ID LSQUARE expression RSQUARE",(yyvsp[-3].node)->getReturnOrDataType());
        (yyval.node)->setStartLine((yyvsp[-3].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine()); 
        CheckArrayIssues((yyval.node),(yyvsp[-3].node),(yyvsp[-1].node));
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-3].node),(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)});
        util.printGrammar("variable : ID LSQUARE expression RSQUARE");     
      
}
#line 2498 "y.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 945 "2005087.y" /* yacc.c:1646  */
    {
 
        SymbolInfo *sInfo = new SymbolInfo((yyvsp[0].node)->getName(), "expression");
        (yyval.node)=new Node(sInfo,"expression : logic_expression",(yyvsp[0].node)->getReturnOrDataType());
        (yyval.node)->setStartLine((yyvsp[0].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine());
        (yyval.node)->setArrayStatus((yyvsp[0].node)->getArrayStatus());
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[0].node)});
        util.printGrammar("expression : logic_expression");
        
}
#line 2515 "y.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 957 "2005087.y" /* yacc.c:1646  */
    {

        SymbolInfo *sInfo = new SymbolInfo("", "expression");
        (yyval.node)=new Node(sInfo,"expression : variable ASSIGNOP logic_expression");
        (yyval.node)->setStartLine((yyvsp[-2].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine());
        CheckAssignmentIssues((yyval.node),(yyvsp[-2].node),(yyvsp[0].node));
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)});
        util.printGrammar("expression : variable ASSIGNOP logic_expression");
 
}
#line 2532 "y.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 970 "2005087.y" /* yacc.c:1646  */
    {

        SymbolInfo *sInfo = new SymbolInfo((yyvsp[0].node)->getName(), "logic_expression");
        (yyval.node)=new Node(sInfo,"logic_expression : rel_expression",(yyvsp[0].node)->getReturnOrDataType());
        (yyval.node)->setStartLine((yyvsp[0].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine());
        (yyval.node)->setArrayStatus((yyvsp[0].node)->getArrayStatus());
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[0].node)});
        util.printGrammar("logic_expression : rel_expression");
 
}
#line 2549 "y.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 982 "2005087.y" /* yacc.c:1646  */
    {

		SymbolInfo *sInfo = new SymbolInfo("", "logic_expression");
        CheckLogicalIssues((yyval.node),(yyvsp[-2].node),(yyvsp[0].node));
        (yyval.node)=new Node(sInfo,"logic_expression : rel_expression LOGICOP rel_expression");
        (yyval.node)->setStartLine((yyvsp[-2].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine());
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)});
        util.printGrammar("logic_expression : rel_expression LOGICOP rel_expression");

}
#line 2566 "y.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 995 "2005087.y" /* yacc.c:1646  */
    {
 
        SymbolInfo *sInfo = new SymbolInfo((yyvsp[0].node)->getName(), "rel_expression");
        (yyval.node)=new Node(sInfo,"rel_expression : simple_expression",(yyvsp[0].node)->getReturnOrDataType());
        (yyval.node)->setStartLine((yyvsp[0].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine());
        (yyval.node)->setArrayStatus((yyvsp[0].node)->getArrayStatus());
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[0].node)});
        util.printGrammar("rel_expression : simple_expression");
       
}
#line 2583 "y.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 1007 "2005087.y" /* yacc.c:1646  */
    {

        SymbolInfo *sInfo= new SymbolInfo("", "rel_expression");
        (yyval.node)=new Node(sInfo,"rel_expression : simple_expression RELOP simple_expression");
        (yyval.node)->setStartLine((yyvsp[-2].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine());
        CheckRelationalIssues((yyval.node),(yyvsp[-2].node),(yyvsp[0].node));
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)});
        util.printGrammar("rel_expression : simple_expression RELOP simple_expression");

}
#line 2600 "y.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 1020 "2005087.y" /* yacc.c:1646  */
    {

        SymbolInfo *sInfo = new SymbolInfo((yyvsp[0].node)->getName(), "simple_expression");
        (yyval.node)=new Node(sInfo,"simple_expression : term",(yyvsp[0].node)->getReturnOrDataType());
        (yyval.node)->setStartLine((yyvsp[0].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine());
        (yyval.node)->setArrayStatus((yyvsp[0].node)->getArrayStatus());
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[0].node)});
        util.printGrammar("simple_expression : term");
       
}
#line 2617 "y.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 1032 "2005087.y" /* yacc.c:1646  */
    {

        SymbolInfo *sInfo = new SymbolInfo("", "simple_expression");
        (yyval.node)=new Node(sInfo,"simple_expression : simple_expression ADDOP term",TypeCasting((yyvsp[-2].node)->getReturnOrDataType(), (yyvsp[0].node)->getReturnOrDataType()));
        (yyval.node)->setStartLine((yyvsp[-2].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine());
        if ((yyvsp[-2].node)->getReturnOrDataType() == "VOID" || (yyvsp[0].node)->getReturnOrDataType() == "VOID") {
			util.printError("Void cannot be used in expression", (yyval.node)->getStartLine());
		}
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)});
        util.printGrammar("simple_expression : simple_expression ADDOP term");
	
}
#line 2636 "y.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 1047 "2005087.y" /* yacc.c:1646  */
    {

       SymbolInfo *sInfo = new SymbolInfo((yyvsp[0].node)->getName(), "term");
        (yyval.node)=new Node(sInfo,"term : unary_expression",(yyvsp[0].node)->getReturnOrDataType());
        (yyval.node)->setStartLine((yyvsp[0].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine());
        (yyval.node)->setArrayStatus((yyvsp[0].node)->getArrayStatus());
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[0].node)});
        util.printGrammar("term : unary_expression");
       
}
#line 2653 "y.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 1059 "2005087.y" /* yacc.c:1646  */
    {

        SymbolInfo *sInfo = new SymbolInfo("", "term");  
        (yyval.node)=new Node(sInfo,"term : term MULOP unary_expression");
        (yyval.node)->setStartLine((yyvsp[-2].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine());
        CheckMultiplicationIssues((yyval.node),(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node));
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)});
        util.printGrammar("term : term MULOP unary_expression");  

}
#line 2670 "y.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 1072 "2005087.y" /* yacc.c:1646  */
    {

        SymbolInfo *sInfo = new SymbolInfo("", "unary_expression");
        (yyval.node)=new Node(sInfo,"unary_expression : ADDOP unary_expression");
        (yyval.node)->setStartLine((yyvsp[-1].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine());
        if ((yyvsp[0].node)->getReturnOrDataType() == "VOID") {
                util.printError("Void cannot be used in expression", (yyval.node)->getStartLine());
            }
        else {(yyval.node)->setReturnOrDataType((yyvsp[0].node)->getReturnOrDataType());}
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-1].node),(yyvsp[0].node)});
        util.printGrammar("unary_expression : ADDOP unary_expression");

		
}
#line 2691 "y.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 1088 "2005087.y" /* yacc.c:1646  */
    {

        SymbolInfo *sInfo = new SymbolInfo("", "unary_expression");
        (yyval.node)=new Node(sInfo,"unary_expression : NOT unary_expression");
        (yyval.node)->setStartLine((yyvsp[-1].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine());
        CheckUnaryExpression((yyval.node),(yyvsp[0].node));
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-1].node),(yyvsp[0].node)});
        util.printGrammar("unary_expression : NOT unary_expression");
 

}
#line 2709 "y.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 1101 "2005087.y" /* yacc.c:1646  */
    {

        SymbolInfo *sInfo = new SymbolInfo((yyvsp[0].node)->getName(), "unary_expression");
        (yyval.node)=new Node(sInfo,"unary_expression : factor",(yyvsp[0].node)->getReturnOrDataType());
        (yyval.node)->setStartLine((yyvsp[0].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine());
        (yyval.node)->setArrayStatus((yyvsp[0].node)->getArrayStatus());
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[0].node)});
        util.printGrammar("unary_expression : factor");
       
}
#line 2726 "y.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 1116 "2005087.y" /* yacc.c:1646  */
    {

        SymbolInfo *sInfo = new SymbolInfo((yyvsp[0].node)->getName(), "factor");
        (yyval.node)=new Node(sInfo,"factor : variable",(yyvsp[0].node)->getReturnOrDataType());
        (yyval.node)->setStartLine((yyvsp[0].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine());
        (yyval.node)->setArrayStatus((yyvsp[0].node)->getArrayStatus());
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[0].node)});
        util.printGrammar("factor : variable");

        
}
#line 2744 "y.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 1129 "2005087.y" /* yacc.c:1646  */
    {

        SymbolInfo *sInfo = new SymbolInfo("", "factor");
        (yyval.node)=new Node(sInfo,"factor : ID LPAREN argument_list RPAREN");
        (yyval.node)->setStartLine((yyvsp[-3].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine());
        callFunction((yyval.node),(yyvsp[-3].node),(yyvsp[-1].node));
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-3].node),(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)});
        util.printGrammar("factor : ID LPAREN argument_list RPAREN");

}
#line 2761 "y.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 1142 "2005087.y" /* yacc.c:1646  */
    {

        SymbolInfo *sInfo = new SymbolInfo((yyvsp[-1].node)->getName(), "factor");
        (yyval.node)=new Node(sInfo,"factor : LPAREN expression RPAREN",(yyvsp[-1].node)->getReturnOrDataType());
        (yyval.node)->setStartLine((yyvsp[-2].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[-1].node)->getEndLine());
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)});
        util.printGrammar("factor : LPAREN expression RPAREN");
        
}
#line 2777 "y.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 1153 "2005087.y" /* yacc.c:1646  */
    {

        SymbolInfo *sInfo = new SymbolInfo((yyvsp[0].node)->getName(), "factor");	
        (yyval.node)=new Node(sInfo,"factor : CONST_INT","INT");
        (yyval.node)->setStartLine((yyvsp[0].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine());
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[0].node)});
        util.printGrammar("factor : CONST_INT");

}
#line 2793 "y.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 1164 "2005087.y" /* yacc.c:1646  */
    {

        SymbolInfo *sInfo = new SymbolInfo((yyvsp[0].node)->getName(), "factor");
        (yyval.node)=new Node(sInfo,"factor : CONST_FLOAT","FLOAT");
        (yyval.node)->setStartLine((yyvsp[0].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine());
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[0].node)});
        util.printGrammar("factor : CONST_FLOAT");


}
#line 2810 "y.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 1176 "2005087.y" /* yacc.c:1646  */
    {

        SymbolInfo *sInfo = new SymbolInfo("", "factor");
        (yyval.node)=new Node(sInfo,"factor : variable INCOP");
        (yyval.node)->setStartLine((yyvsp[-1].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine());
        CheckUnaryOperandIssues((yyval.node),(yyvsp[-1].node));
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-1].node),(yyvsp[0].node)});
        util.printGrammar("factor : variable INCOP");

}
#line 2827 "y.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 1188 "2005087.y" /* yacc.c:1646  */
    {

        SymbolInfo *sInfo= new SymbolInfo("", "factor");
        (yyval.node)=new Node(sInfo,"factor : variable DECOP");
        (yyval.node)->setStartLine((yyvsp[-1].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine());
        CheckUnaryOperandIssues((yyval.node),(yyvsp[-1].node));
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-1].node),(yyvsp[0].node)});
        util.printGrammar("factor : variable DECOP");

}
#line 2844 "y.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 1202 "2005087.y" /* yacc.c:1646  */
    {
	
        SymbolInfo *sInfo = new SymbolInfo("", "argument_list");
        (yyval.node)=new Node(sInfo,"argument_list : arguments");
        (yyval.node)->setStartLine((yyvsp[0].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine());
        (yyval.node)->addParameter((yyvsp[0].node)->getParameterList());
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[0].node)});
        util.printGrammar("argument_list : arguments");	

}
#line 2861 "y.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 1214 "2005087.y" /* yacc.c:1646  */
    {
        SymbolInfo *sInfo= new SymbolInfo("", "argument_list");
        (yyval.node)=new Node(sInfo,"argument_list : ");
        util.printGrammar("argument_list : ");
}
#line 2871 "y.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 1220 "2005087.y" /* yacc.c:1646  */
    {

        SymbolInfo *sInfo = new SymbolInfo("", "arguments");
        (yyval.node)=new Node(sInfo,"arguments : arguments COMMA logic_expression");
        (yyval.node)->setStartLine((yyvsp[-2].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine());
        (yyval.node)->addParameter((yyvsp[-2].node)->getParameterList());
        (yyval.node)->addParameter({{(yyvsp[0].node)->getReturnOrDataType(),(yyvsp[0].node)->getName()},(yyvsp[0].node)->getArrayStatus()});
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[-2].node),(yyvsp[-1].node),(yyvsp[0].node)});
        util.printGrammar("arguments : arguments COMMA logic_expression");

}
#line 2889 "y.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 1233 "2005087.y" /* yacc.c:1646  */
    {

        SymbolInfo *sInfo = new SymbolInfo("", "arguments");
        (yyval.node)=new Node(sInfo,"arguments : logic_expression");
        (yyval.node)->setStartLine((yyvsp[0].node)->getStartLine());
        (yyval.node)->setEndLine((yyvsp[0].node)->getEndLine());
        (yyval.node)->addParameter({{(yyvsp[0].node)->getReturnOrDataType(),(yyvsp[0].node)->getName()},(yyvsp[0].node)->getArrayStatus()});
        pTree.setCurrentNode((yyval.node));
        (yyval.node)->makeChild({(yyvsp[0].node)});
        util.printGrammar("arguments : logic_expression");

}
#line 2906 "y.tab.c" /* yacc.c:1646  */
    break;


#line 2910 "y.tab.c" /* yacc.c:1646  */
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
#line 1247 "2005087.y" /* yacc.c:1906  */


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

    util.getLogFout()<<"Total Lines: "<<yylineno<<endl;
    util.getLogFout()<<"Total Errors: "<<util.getErrorCount()<<endl;
    return EXIT_SUCCESS;
}
