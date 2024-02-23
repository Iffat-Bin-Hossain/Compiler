/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

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
#line 375 "2005087.y" /* yacc.c:1909  */

    Node *node;

#line 142 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
