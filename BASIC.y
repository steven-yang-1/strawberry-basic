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
%token BITAND BITOR
%token IF ELSEIF ELSE ENDIF THEN
%token WHILE ENDWHILE DO LOOP
%token FOR TO DOWNTO STEP ENDFOR
%token ASSIGN
%token EQ

%left '&' '|' '^' '$'
%left '<' '>'
%right '!'

%left '+' '-'
%left '*' '/'

%left ASSIGN;

%type <expr> _assignment;
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
%type <expr> dimension
%type <expr> while_statement
%type <expr> for_statement
%type <expr> for_step
%type <expr> do_loop_statement

%%
statements:	{$$ = NULL;}|_statements		{
								execute($1);
							};

_statements:	{$$ = NULL;}|statement _statements	{
								$$ = make_ast(NODE_TYPE_STATEMENT, $1, $2);
							};

statement:	VARIABLE_NAME assignment		{
								$$ = make_redim($1, $2);
							}
	|	expression				{
								$$ = $1;
							}
	|	if_statement				{
								$$ = $1;
							}
	|	while_statement			{
								$$ = $1;
							}
	|	do_loop_statement			{
								$$ = $1;
							}
	|	for_statement				{
								$$ = $1;
							}
	|	DIM dimension				{
								$$ = $2;
							}
	;
	
dimension:
	VARIABLE_NAME assignment			{
								$$ = make_dim($1, $2, NULL);
							}
	|	VARIABLE_NAME assignment ',' dimension			{
											AST* node = $4;
											node->node_type = NODE_TYPE_ASSIGN_VAR;
											$$ = make_dim($1, $2, node);
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
while_statement:	WHILE expression _statements ENDWHILE					{
													$$ = make_while_expression($2, $3);
												};
do_loop_statement:	DO _statements LOOP WHILE expression		{
										$$ = make_do_loop_expression($2, $5);
									};
for_statement:		FOR VARIABLE_NAME ASSIGN expression TO expression for_step _statements ENDFOR {
								Dimension* dim = make_dim($2, $4, NULL);
								AST* step;
								if ($7 == NULL) {
									// default step
									step = make_ast_integer(1);
								} else {
									step = $7;
								}
								$$ = make_for_expression((AST*)dim, $6, step, $8);
							};
for_step:						{
								$$ = NULL;
							}
	|	STEP expression				{
								$$ = $2;
							}
expression:	NUMBER					{
								$$ = $1;
							}
	|	'-' NUMBER				{
								if (((Constant *)$2)->type == C_INT) {
									((Constant *)$2)->i_val = -(((Constant *)$2)->i_val);
								} else if (((Constant *)$2)->type == C_DECIMAL) {
									((Constant *)$2)->d_val = -(((Constant *)$2)->d_val);
								}
								$$ = $2;
							}
	|	expression EQ expression		{
								$$ = make_ast(NODE_TYPE_EQ, $1, $3);
							}
	|	expression BITAND expression		{
								$$ = make_ast(NODE_TYPE_BITAND, $1, $3);
							}
	|	expression BITOR expression		{
								$$ = make_ast(NODE_TYPE_BITOR, $1, $3);
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
	|	expression '<' ASSIGN expression	{
								$$ = make_ast(NODE_TYPE_LTE, $1, $4);
							}
	|	expression '>' ASSIGN expression	{
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
								} else if (((AST*)$2)->node_type == NODE_TYPE_EXPR_ITEM) {
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
							};

expr_list:						{ 	$$=NULL;	}
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

assignment:						{
								$$ = NULL;
							}	
	|	ASSIGN _assignment			{
								$$ = $2;
							}
	;
	
_assignment:	expression				{
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
	hash_free(env->vars);
	stack_free(env->stack);
	free(env);
	return 0;
}

int yyerror(const char *message)
{
	fprintf(stderr, "Syntax error at line %g: %s\n", errline, message);
	return 0;
}
