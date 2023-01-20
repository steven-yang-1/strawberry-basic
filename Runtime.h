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

#define C_INT 0
#define C_DECIMAL 1
#define C_STRING 2
#define C_LIST_BUFFER 3


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
	int type;
	int i_val;
	double d_val;
	char* s_val;
	ListBuffer* list;
} RuntimeValue;


typedef struct RuntimeEnvironment {
	Stack* stack;
	HashTable* vars;
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

typedef struct VariableBlock {
	int node_type;
	char* type;
	char* var_name;
	int i_val;
	double d_val;
	char* s_val;
} VariableBlock;

VariableBlock* var_make_int(char* var_name, int content);

VariableBlock* var_make_decimal(char* var_name, double content);

VariableBlock* var_make_string(char* var_name, char* content);


typedef struct AST {
	int node_type;
	struct AST* left_node;
	struct AST* right_node;
} AST;

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

RuntimeValue* execute(AST* ast);

#endif
