%{
	#include <stdio.h>
	#include <string.h>
	
	#include "HashTable.h"
	#include "Runtime.h"
	
	static double errline;
%}

%union {
	char* identifier;
	struct AST* expr;
}

%token <expr> NUMBER
%token <identifier> VARIABLE_NAME
%token <expr> STRING
%token DIM
%token IF ELSEIF ELSE ENDIF THEN

%left '&' '|' '^' '$'
%left '<' '>'
%right '!'

%left '+' '-'
%left '*' '/'

%left DEFSTR;

%type <expr> assignment;
%type <expr> expression;
%type <expr> _statements;
%type <expr> statement;
%type <expr> if_statement;
%type <expr> elseif_statement;
%type <expr> else_statement;
%type <expr> statements;
%type <expr> expr_list;
%type <expr> _expr_list;
%type <expr> _func_or_var

%%
statements:	{$$ = NULL;}|_statements		{
								execute($1);
							};

_statements:	{$$ = NULL;}|statement _statements	{
								$$ = make_ast(NODE_TYPE_STATEMENT, $1, $2);
							};

statement:	expression				{
								$$ = $1;
							}
	|	DIM VARIABLE_NAME DEFSTR assignment	{
								if (((Constant *)$4)->type == C_INT) {
									$$ = (AST*) var_make_int($2, ((Constant *)$4)->i_val);
								} else if (((Constant *)$4)->type == C_DECIMAL) {
									$$ = (AST*) var_make_decimal($2, ((Constant *)$4)->d_val);
								} else if (((Constant *)$4)->type == C_STRING){
									$$ = (AST*) var_make_string($2, ((Constant *)$4)->s_val);
								}
							}
	|	STRING					{
								$$ = $1;
							}
	|	if_statement				{
								$$ = $1;
							}
	;

if_statement:		IF expression THEN _statements elseif_statement else_statement ENDIF	{
													$$ = make_if_expression($2, $4, $5, $6);
												};
		
elseif_statement:	{$$ = NULL;}
	|		ELSEIF expression THEN _statements elseif_statement			{
													$$ = make_if_expression($2, $4, $5, NULL);
												};
else_statement:	{$$ = NULL;}
	|		ELSE _statements							{
													$$ = make_ast(NODE_TYPE_ELSE, $2, NULL);
												};

expression:	NUMBER					{
								$$ = $1;
							}
	|	expression '&' expression		{
								$$ = make_ast('&', $1, $3);
							}
	|	expression '|' expression		{
								$$ = make_ast('|', $1, $3);
							}
	|	'!' expression				{
								$$ = make_ast('!', $2, NULL);;
							}
	|	expression '^' expression		{
								$$ = make_ast('^', $1, $3);
							}
	|	expression '<' expression		{
								$$ = make_ast('<', $1, $3);
							}
	|	expression '>' expression		{
								$$ = make_ast('>', $1, $3);
							}
	|	expression '<' DEFSTR expression	{
								$$ = make_ast(NODE_TYPE_LTE, $1, $4);
							}
	|	expression '>' DEFSTR expression	{
								$$ = make_ast(NODE_TYPE_GTE, $1, $4);
							}
	|	expression '$' expression		{
								$$ = make_ast('$', $1, $3);
							}
	|	expression '+' expression		{
								$$ = make_ast('+', $1, $3);
							}
	|	expression '-' expression		{
								$$ = make_ast('-', $1, $3);
							}
	|	expression '*' expression		{
								$$ = make_ast('*', $1, $3);
							}
	|	expression '/' expression		{
								$$ = make_ast('/', $1, $3);
							}
	|	expression '%' expression		{
								$$ = make_ast(NODE_TYPE_MOD, $1, $3);
							}
	|	'(' expression ')'			{
								$$ = make_ast(NODE_TYPE_PRIORITY, $2, NULL);
							}
	|	VARIABLE_NAME _func_or_var		{
								if ($2 == NULL) {
									$$ = (AST*) make_var($1);
								} else {							
									$$ = make_function_call($1, $2);
								}
							}
	|	STRING					{
								$$ = $1;
							}
	;

_func_or_var:						{
								$$ = NULL;
							}
	|	'(' expr_list ')'			{
								$$ = $2;
							}

expr_list:						{ $$=NULL; }
	|	expression				{
								$$ = make_ast(NODE_TYPE_EXPR_ITEM, $1, NULL);
							}
	|	expression ',' _expr_list		{
								$$ = make_ast(NODE_TYPE_EXPR_ITEM, $1, $3);
							}
	;

_expr_list:	expression				{
								$$ = make_ast(NODE_TYPE_EXPR_ITEM, $1, NULL);
							}
	|	expression ',' _expr_list		{
								$$ = make_ast(NODE_TYPE_EXPR_ITEM, $1, $3);
							}
	;


assignment:	NUMBER					{
								$$ = $1;
							}
	|	STRING					{
								$$ = $1;
							}
	;
%%
int main()
{
	env = malloc(sizeof(RuntimeEnvironment) + 1);
	env->stack = stack_init();
	env->vars = hash_init(800);
	yyparse();
	return 0;
}

int yyerror(const char *message)
{
	fprintf(stderr, "Syntax error at line %g: %s\n", errline, message);
	return 0;
}
