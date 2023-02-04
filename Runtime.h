#ifndef RUNTIME_H
#define RUNTIME_H

#include "Stack.h"
#include "HashTable.h"
#include "ListBuffer.h"

#define NODE_TYPE_ASSIGN_VAR 10000
#define NODE_TYPE_CONSTANT 10001
#define NODE_TYPE_AST 10002
#define NODE_TYPE_IF_STATEMENT 10003
#define NODE_TYPE_GTE 10004
#define NODE_TYPE_LTE 10005
#define NODE_TYPE_MOD 10006
#define NODE_TYPE_PRIORITY 10007
#define NODE_TYPE_ELSE 10008
#define NODE_TYPE_STATEMENT 10009
#define NODE_TYPE_EXPR_ITEM 10010
#define NODE_TYPE_FUNC 10012
#define NODE_TYPE_VAR 10013
#define NODE_TYPE_REASSIGN 10014
#define NODE_TYPE_EQ 10015
#define NODE_TYPE_BITAND 10016
#define NODE_TYPE_BITOR 10017
#define NODE_TYPE_WHILE_STATEMENT 10018
#define NODE_TYPE_FOR_STATEMENT 10019
#define NODE_TYPE_DO_LOOP_STATEMENT 10020
#define NODE_TYPE_FUNCTION_DEFINE 10021
#define NODE_TYPE_RETURN 10022
#define NODE_TYPE_BREAK 10023
#define NODE_TYPE_CONTINUE 10024

#define C_INT 0
#define C_DECIMAL 1
#define C_STRING 2
#define C_LIST_BUFFER 3
#define C_NULL 4
#define C_FUNCTION_DEFINE 5
#define C_BREAK 6
#define C_CONTINUE 7


typedef struct Var {
	int node_type;
	char* name;
} Var;

Var* make_var(char* name);

typedef struct Constant {
	int node_type;
	int i_val;
	double d_val;
	char* s_val;
	int type;
} Constant;


typedef struct RuntimeValue {
	int runtime_type;
	int i_val;
	double d_val;
	char* s_val;
	ListBuffer* list;
} RuntimeValue;


typedef struct RuntimeEnvironment {
	Stack* call_stack;
	HashTable* vars;
	HashTable* functions;
} RuntimeEnvironment;

struct RuntimeEnvironment* env;

Constant* make_ast_integer(int i_val);

Constant* make_ast_decimal(double d_val);

Constant* make_ast_string(char* s_val);

char* new_string(char* str);

int dump_numeric_as_integer(Constant* constant);

double dump_numeric_as_decimal(Constant* constant);

int runtime_as_integer(RuntimeValue* value);

double runtime_as_decimal(RuntimeValue* value);

RuntimeValue* make_runtime_integer(int i_val);

RuntimeValue* make_runtime_decimal(double d_val);

RuntimeValue* make_runtime_string(char* s_val);

RuntimeValue* make_runtime_null();

RuntimeValue* make_runtime_break();

RuntimeValue* make_runtime_continue();

typedef struct AST {
	int node_type;
	struct AST* left_node;
	struct AST* right_node;
} AST;

typedef struct Dimension {
	int node_type;
	char* var_name;
	AST* node;
	AST* next_dim;
} Dimension;

typedef struct RuntimeFunction {
	int type;
	char* name;
	ListBuffer* arguments;
	AST* statements;
} RuntimeFunction;

Dimension* var_make_null(char* var_name);

AST* make_dim(char* var_name, AST* node, AST* next_dim);

AST* make_redim(char* var_name, AST* node);

typedef struct IfStatement {
	int node_type;
	AST* condition;
	AST* if_statement;
	AST* else_if_statement;
	AST* else_statement;
} IfStatement;

AST* make_ast(int node_type, AST* left_node, AST* right_node);

AST* make_if_expression(AST* condition, AST* if_statement, AST* else_if_statement, AST* else_statement);


typedef struct Function {
	int node_type;
	char* name;
	AST* expr_list;
} FunctionStatement;

AST* make_function_call(char* name, AST* expr_list);

ListBuffer* integrate_params(AST* node);

typedef struct WhileStatement {
	int node_type;
	AST* condition;
	AST* statements;
} WhileStatement;

AST* make_while_expression(AST* condition, AST* statements);

typedef struct DoLoopStatement {
	int node_type;
	AST* condition;
	AST* statements;
} DoLoopStatement;

AST* make_do_loop_expression(AST* statements, AST* condition);

typedef struct ForStatement {
	int node_type;
	AST* dim;
	AST* until;
	AST* step;
	AST* statements;
} ForStatement;

AST* make_for_expression(AST* dim, AST* until, AST* step, AST* statements);

typedef struct FunctionDefineStatement {
	int node_type;
	AST* statements;
	AST* arguments;
	char* name;
} FunctionDefineStatement;

AST* make_function_define(char* name, AST* statements, AST* arguments);

typedef struct FunctionStackElement {
	HashTable* local_vars;
} FunctionStackElement;

FunctionStackElement* dup_new_func_stack_element();

typedef struct ReturnStatement {
	int node_type;
	AST* return_expr;
} ReturnStatement;

AST* make_return(AST* expr);

typedef struct BreakStatement {
	int node_type;
	AST* placeholder;
	AST* placeholder2;
} BreakStatement;

typedef struct ContinueStatement {
	int node_type;
	AST* placeholder;
	AST* placeholder2;
} ContinueStatement;

AST* make_break();
AST* make_continue();

RuntimeValue* execute(AST* ast);

#endif
