/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_BASIC_TAB_H_INCLUDED
# define YY_YY_BASIC_TAB_H_INCLUDED
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
    NUMBER = 258,
    VARIABLE_NAME = 259,
    STRING = 260,
    DIM = 261,
    BITAND = 262,
    BITOR = 263,
    IF = 264,
    ELSEIF = 265,
    ELSE = 266,
    ENDIF = 267,
    THEN = 268,
    WHILE = 269,
    ENDWHILE = 270,
    DO = 271,
    LOOP = 272,
    FOR = 273,
    TO = 274,
    DOWNTO = 275,
    STEP = 276,
    NEXT = 277,
    ASSIGN = 278,
    EQ = 279,
    SUB = 280,
    ENDSUB = 281,
    RETURN = 282,
    BREAK = 283,
    CONTINUE = 284,
    FUNCTION = 285,
    ENDFUNCTION = 286,
    LINE_BREAK = 287,
    EXITSUB = 288,
    STR_CONCAT = 289,
    CLASS = 290,
    INHERITS = 291,
    IMPLEMENTS = 292,
    END_CLASS = 293,
    PUBLIC = 294,
    PROTECTED = 295,
    PRIVATE = 296,
    SHARED = 297,
    PROPERTY = 298,
    TRAIT = 299,
    END_TRAIT = 300,
    NAMESPACE = 301,
    IMPORT = 302,
    WITH = 303,
    ALIAS = 304,
    TRY = 305,
    CATCH = 306,
    FINALLY = 307,
    THROW = 308,
    END_TRY = 309,
    FILE_END = 310,
    NEW_OBJECT = 311
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 13 "BASIC.y"

	char* identifier;
	struct AST* expr;
	int access_modifier;
	int is_static;

#line 121 "BASIC.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;
int yyparse (void);

#endif /* !YY_YY_BASIC_TAB_H_INCLUDED  */
