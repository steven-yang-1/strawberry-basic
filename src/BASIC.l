%{
#include "BASIC.tab.h"
#include "HashTable.h"
#include "Runtime.h"

extern YYSTYPE yylval;
extern int yylineno;

#define C_INT 0
#define C_DECIMAL 1
#define C_STRING 2

#define YY_USER_ACTION \
    yylloc.first_line = yylloc.last_line = yylineno;
%}

%option noyywrap
%option caseless
%option yylineno

numeric	[0-9]+(\.[0-9]+)?
var_name	[_A-Za-z0-9]+

%%
"rem".*	{}
"dim"		{
			return DIM;
		}
"=="		{
			return EQ;
		}
"="		{
			return ASSIGN;
		}
"+"		{
			return '+';
		}
"-"		{
			return '-';
		}
"*"		{
			return '*';
		}
"/"		{
			return '/';
		}
"%"		{
			return '%';
		}
"("		{
			return '(';
		}
")"		{
			return ')';
		}
"bitand"	{
			return BITAND;
		}
"bitor"	{
			return BITOR;
		}
"and"		{
			return '&';
		}
"&"		{
			return STR_CONCAT;
		}
"or"		{
			return '|';
		}
"xor"		{
			return '^';
		}
"not"		{
			return '!';
		}
"<>"		{
			return '$';
		}
"<"		{
			return '<';
		}
">"		{
			return '>';
		}
"if"		{
			return IF;
		}
"then"		{
			return THEN;
		}
"else"		{
			return ELSE;
		}
"elseif"	{
			return ELSEIF;
		}
"end if"	{
			return ENDIF;
		}
"while"	{
			return WHILE;
		}
"end while"	{
			return ENDWHILE;
		}
"for"		{
			return FOR;
		}
"to"		{
			return TO;
		}
"next"		{
			return NEXT;
		}
"step"		{
			return STEP;
		}
"do"		{
			return DO;
		}
"loop"		{
			return LOOP;
		}
"break"	{
			return BREAK;
		}
"continue"	{
			return CONTINUE;
		}
"."		{
			return '.';
		}
"sub"		{
			return SUB;
		}
"end sub"	{
			return ENDSUB;
		}
"function"	{
			return FUNCTION;
		}
"end function"	{
			return ENDFUNCTION;
		}
"return"	{
			return RETURN;
		}
"exit sub"	{
			return EXITSUB;
		}
"true"		{
			yylval.expr = (AST*) make_ast_integer(1);
			return NUMBER;
		}
"false"	{
			yylval.expr = (AST*) make_ast_integer(0);
			return NUMBER;
		}
{numeric}	{
			double num = atof(yytext);
			int found_decimal_dot = 0;
			for (int i = 0; i < strlen(yytext); i++) {
				if (yytext[i] == '.') {
					found_decimal_dot = 1;
					break;
				}
			}
			if (found_decimal_dot) {
				yylval.expr = (AST*) make_ast_decimal(num);
			} else {
				yylval.expr = (AST*) make_ast_integer((int)num);
			}
			return NUMBER;
		}
{var_name}	{
			yylval.identifier = new_string(yytext);
			return VARIABLE_NAME;
		}
\"[^"]*\"	{
			char* new_text = malloc(sizeof(char) * (strlen(yytext) - 2) + 1);
			for (int i = 0; i  < strlen(yytext) - 2; i++) {
				new_text[i] = yytext[i + 1];
			}
			yylval.expr = (AST*) make_ast_string(new_text);
			return STRING;
		}
"\'".*		{}
[ \t]		;
[\r]		;
[\n]		{
			return LINE_BREAK;
		};
<<EOF>>	return 0;
.		return yytext[0];
%%

int yyerror(const char *message)
{
	fprintf(stderr, "Syntax error at line %d: %s\n", yylineno, message);
	return 0;
}
