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
	int access_modifier;
	int is_static;
}

%token <expr> NUMBER
%token <identifier> VARIABLE_NAME
%token <expr> STRING
%token DIM
%token BITAND BITOR
%token IF ELSEIF ELSE ENDIF THEN
%token WHILE ENDWHILE DO LOOP
%token FOR TO DOWNTO STEP NEXT
%token ASSIGN
%token EQ
%token SUB ENDSUB RETURN
%token <expr> BREAK CONTINUE
%token FUNCTION ENDFUNCTION
%token LINE_BREAK
%token EXITSUB
%token STR_CONCAT
%token CLASS INHERITS IMPLEMENTS END_CLASS
%token PUBLIC PROTECTED PRIVATE STATIC PROPERTY
%token TRAIT END_TRAIT
%token NAMESPACE IMPORT
%token WITH
%token ALIAS
%token TRY CATCH FINALLY THROW END_TRY

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
%type <expr> sub_program
%type <expr> return_value
%type <expr> define_func_args
%type <expr> define_function
%type <expr> _define_func_args

%type <expr> namespace
%type <expr> import
%type <identifier> import_alias
%type <expr> location
%type <expr> _location
%type <expr> class_definition
%type <expr> class_attributes
%type <identifier> class_extends
%type <identifier> traits_impl
%type <expr> define_trait_list
%type <expr> _define_trait_list
%type <expr> class_inner_statements
%type <expr> _class_inner_statements
%type <expr> class_method
%type <access_modifier> access_modifier
%type <is_static> method_static
%type <expr> method_body
%type <expr> class_property
%type <expr> trait_definition
%type <expr> trait_inner_statements
%type <expr> _trait_inner_statements

%%
statements:	{$$ = NULL;}|_statements		{
								execute($1);
							};

_statements:	{$$ = NULL;}|statement _statements	{
								$$ = make_ast(NODE_TYPE_STATEMENT, $1, $2, @1.first_line);
							};

statement:	VARIABLE_NAME assignment		{
								$$ = make_redim($1, $2, @1.first_line);
							}
	|	expression				{
								$$ = $1;
							}
	|	class_definition			{
								$$ = $1;
							}
	|	trait_definition			{
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
	|	sub_program				{
								$$ = $1;
							}
	|	return_value				{
								$$ = $1;
							}
	|	BREAK					{
								$$ = make_break(@1.first_line);
							}
	|	CONTINUE				{
								$$ = make_continue(@1.first_line);
							}
	|	EXITSUB				{
								$$ = make_ast(NODE_TYPE_EXIT_SUB, NULL, NULL, @1.first_line);
							}
	|	define_function			{
								$$ = $1;
							}
	|	namespace				{
								$$ = $1;
							}
	|	import					{
								$$ = $1;
							}
	|	LINE_BREAK				{
								$$ = make_ast(NODE_TYPE_LINE_BREAK, NULL, NULL, @1.first_line);
							}
	;

/****************************************************
* Object-oriented
****************************************************/

namespace:
	NAMESPACE location { $$ = make_ast(NODE_TYPE_NAMESPACE, $2, NULL, @1.first_line); };
	
import:
	IMPORT location import_alias { $$ = make_import($3, $2, @1.first_line); };

import_alias:
	{ $$=NULL; }
	|	ALIAS VARIABLE_NAME
	{
		$$ = $2;
	};

location: VARIABLE_NAME _location
	{
		$$ = make_location($1, $2, @1.first_line);
	};
_location: { $$=NULL; }
	|	'.' VARIABLE_NAME _location
	{
		$$ = make_location($2, $3, @1.first_line);
	};

class_definition:
	CLASS VARIABLE_NAME class_attributes _class_inner_statements END_CLASS	
	{
		$$ = make_class($2, $3, $4, @1.first_line);
	};
	
class_attributes:
	class_extends traits_impl
	{
		$$ = make_class_attr($1, $2, @1.first_line);
	};
	
class_extends:
	{ $$ = NULL; }
	|	INHERITS VARIABLE_NAME
	{
		$$ = $2;
	};

traits_impl:
	{ $$ = NULL; }
	|	IMPLEMENTS define_trait_list
	{
		$$ = $2;
	};

define_trait_list: { $$ = NULL; } |
	VARIABLE_NAME _define_trait_list
	{
		$$ = make_trait_implement_definition($1, $2, @1.first_line);
	};
_define_trait_list: { $$ = NULL; } |
	',' VARIABLE_NAME _define_trait_list	
	{
		$$ = make_trait_implement_definition($2, $3, @1.first_line);
	};

class_inner_statements:
	class_method { $$ = $1; }
	|	class_property { $$ = $1; }
	|	LINE_BREAK;

_class_inner_statements:
	{ $$ = NULL; }
	|	class_inner_statements _class_inner_statements
	{
		$$ = make_ast(NODE_TYPE_CLASS_INNER_STATEMENT, $1, $2, @1.first_line);
	};

class_method:
	access_modifier method_static method_body
	{
		$$ = make_class_method($1, $2, $3, @1.first_line);
	};

access_modifier:
	PUBLIC { $$ = ACC_MOD_PUBLIC; }
	|	PROTECTED { $$ = ACC_MOD_PROTECTED; }
	|	PRIVATE { $$ = ACC_MOD_PRIVATE; };

method_static:
	{ $$ = 0; };
	|	STATIC { $$ = 1; };

method_body:
	sub_program { $$ = $1; }
	|	define_function { $$ = $1; };

class_property:
	access_modifier method_static PROPERTY dimension
	{
		$$ = make_class_property($1, $2, $4, @1.first_line);
	};

/****************************************************
* Object-oriented - TRAIT
****************************************************/

trait_definition:
	TRAIT VARIABLE_NAME traits_impl _trait_inner_statements END_TRAIT
	{
		$$ = make_trait($2, $3, $4, @1.first_line);
	};

trait_inner_statements:
	class_method { $$ = $1; }
	|	class_property { $$ = $1; }
	|	LINE_BREAK
	;

_trait_inner_statements:
	{ $$=NULL; }
	|	trait_inner_statements _trait_inner_statements
	{
		$$ = make_ast(NODE_TYPE_TRAIT_INNER_STATEMENT, $1, $2, @1.first_line);
	};

/****************************************************
* Object-oriented End
****************************************************/

dimension:
	VARIABLE_NAME assignment				{
									$$ = make_dim($1, $2, NULL, @1.first_line);
								}
	|	VARIABLE_NAME assignment ',' dimension	{
									AST* node = $4;
									node->node_type = NODE_TYPE_ASSIGN_VAR;
									$$ = make_dim($1, $2, node, @1.first_line);
								}
	;

sub_program:
	SUB VARIABLE_NAME '(' define_func_args ')' _statements ENDSUB
	{
		$$ = make_sub_define($2, $6, $4, @1.first_line);
	};
							
define_function:
	FUNCTION VARIABLE_NAME '(' define_func_args ')' _statements ENDFUNCTION	{
												$$ = make_function_define($2, $6, $4, @1.first_line);
											};
define_func_args: {$$=NULL;} |
	VARIABLE_NAME _define_func_args		{
								$$ = make_function_arg($1, NULL, $2, @1.first_line);
							};
_define_func_args: {$$=NULL;} |
	',' VARIABLE_NAME _define_func_args		{
								$$ = make_function_arg($2, NULL, $3, @1.first_line);
							};
return_value:
	RETURN expression				{
								$$ = make_return($2, @1.first_line);
							};
if_statement:		IF expression THEN _statements elseif_statement else_statement ENDIF	{
													$$ = make_if_expression($2, $4, $5, $6, @1.first_line);
												};
		
elseif_statement:	{$$ = NULL;}
	|		ELSEIF expression THEN _statements elseif_statement			{
													$$ = make_if_expression($2, $4, $5, NULL, @1.first_line);
												};
else_statement:	{$$ = NULL;}
	|		ELSE _statements							{
													$$ = make_ast(NODE_TYPE_ELSE, $2, NULL, @1.first_line);
												};
while_statement:	WHILE expression _statements ENDWHILE					{
													$$ = make_while_expression($2, $3, @1.first_line);
												};
do_loop_statement:	DO _statements LOOP WHILE expression		{
										$$ = make_do_loop_expression($2, $5, @1.first_line);
									};
for_statement:		FOR VARIABLE_NAME ASSIGN expression TO expression for_step _statements next_statement {
								Dimension* dim = make_dim($2, $4, NULL, @1.first_line);
								AST* step;
								if ($7 == NULL) {
									// default step
									step = make_ast_integer(1);
								} else {
									step = $7;
								}
								$$ = make_for_expression((AST*)dim, $6, step, $8, @1.first_line);
							};
next_statement:	NEXT {} | NEXT VARIABLE_NAME	{};
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
								$$ = make_ast(NODE_TYPE_EQ, $1, $3, @1.first_line);
							}
	|	expression BITAND expression		{
								$$ = make_ast(NODE_TYPE_BITAND, $1, $3, @1.first_line);
							}
	|	expression BITOR expression		{
								$$ = make_ast(NODE_TYPE_BITOR, $1, $3, @1.first_line);
							}
	|	expression '&' expression		{
								$$ = make_ast('&', $1, $3, @1.first_line);
							}
	|	expression '|' expression		{
								$$ = make_ast('|', $1, $3, @1.first_line);
							}
	|	'!' expression				{
								$$ = make_ast('!', $2, NULL, @1.first_line);;
							}
	|	expression '^' expression		{
								$$ = make_ast('^', $1, $3, @1.first_line);
							}
	|	expression '<' expression		{
								$$ = make_ast('<', $1, $3, @1.first_line);
							}
	|	expression '>' expression		{
								$$ = make_ast('>', $1, $3, @1.first_line);
							}
	|	expression '<' ASSIGN expression	{
								$$ = make_ast(NODE_TYPE_LTE, $1, $4, @1.first_line);
							}
	|	expression '>' ASSIGN expression	{
								$$ = make_ast(NODE_TYPE_GTE, $1, $4, @1.first_line);
							}
	|	expression '$' expression		{
								$$ = make_ast('$', $1, $3, @1.first_line);
							}
	|	expression STR_CONCAT expression	{
								$$ = make_ast(NODE_TYPE_STR_CONCAT, $1, $3, @1.first_line);
							}
	|	expression '+' expression		{
								$$ = make_ast('+', $1, $3, @1.first_line);
							}
	|	expression '-' expression		{
								$$ = make_ast('-', $1, $3, @1.first_line);
							}
	|	expression '*' expression		{
								$$ = make_ast('*', $1, $3, @1.first_line);
							}
	|	expression '/' expression		{
								$$ = make_ast('/', $1, $3, @1.first_line);
							}
	|	expression '%' expression		{
								$$ = make_ast(NODE_TYPE_MOD, $1, $3, @1.first_line);
							}
	|	'(' expression ')'			{
								$$ = make_ast(NODE_TYPE_PRIORITY, $2, NULL, @1.first_line);
							}
	|	location _func_or_var			{
								if ($2 == NULL) {
									$$ = (AST*) make_var($1, @1.first_line);
								} else if (((AST*)$2)->node_type == NODE_TYPE_EXPR_ITEM) {
									$$ = make_function_call($1, $2, @1.first_line);
								}
							}
	|	STRING					{
								$$ = $1;
							}
	|	LINE_BREAK				{
								$$ = make_ast(NODE_TYPE_LINE_BREAK, NULL, NULL, @1.first_line);
							}
	;

_func_or_var:						{
								$$ = NULL;
							}
	|	'(' expr_list ')'			{
								$$ = $2;
							};

expr_list:						{ 	$$ = make_ast(NODE_TYPE_EXPR_ITEM, NULL, NULL, 0);	}
	|	expression				{
								$$ = make_ast(NODE_TYPE_EXPR_ITEM, $1, NULL, @1.first_line);
							}
	|	expression ',' _expr_list		{
								$$ = make_ast(NODE_TYPE_EXPR_ITEM, $1, $3, @1.first_line);
							}
	;

_expr_list:	expression				{
								$$ = make_ast(NODE_TYPE_EXPR_ITEM, $1, NULL, @1.first_line);
							}
	|	expression ',' _expr_list		{
								$$ = make_ast(NODE_TYPE_EXPR_ITEM, $1, $3, @1.first_line);
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
	env->call_stack = stack_init();
	env->vars = hash_init(100);
	env->functions = hash_init(800);
	env->classes = hash_init(800);
	env->current_namespace = NULL;
	env->current_building_class = NULL;
	yyparse();
	return 0;
}
