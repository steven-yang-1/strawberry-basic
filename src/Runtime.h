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
#define NODE_TYPE_FUNC_CALL 10012
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
#define NODE_TYPE_LINE_BREAK 10025
#define NODE_TYPE_FUNCTION_ARG_DEFINE 10026
#define NODE_TYPE_SUB 10027
#define NODE_TYPE_EXIT_SUB 10028
#define NODE_TYPE_STR_CONCAT 10029
#define NODE_TYPE_CLASS 10030
#define NODE_TYPE_CLASS_ATTR 10031
#define NODE_TYPE_TRAIT_NAME 10032
#define NODE_TYPE_CLASS_INNER_STATEMENT 10033
#define NODE_TYPE_TRAIT_INNER_STATEMENT 10034
#define NODE_TYPE_TRAIT 10035
#define NODE_TYPE_LOCATE 10036
#define NODE_TYPE_IMPORT 10037
#define NODE_TYPE_NAMESPACE 10038
#define NODE_TYPE_METHOD 10039
#define NODE_TYPE_PROPERTY 10040
#define NODE_TYPE_PROPERTY_DIM 10041
#define NODE_TYPE_NEW_OBJECT 10042
#define NODE_TYPE_ACCESSOR 10043
#define NODE_TYPE_SET_ATTR 10044

#define C_INT 0
#define C_DECIMAL 1
#define C_STRING 2
#define C_LIST_BUFFER 3
#define C_NULL 4
#define C_FUNCTION_DEFINE 5
#define C_BREAK 6
#define C_CONTINUE 7
#define C_SUB 8
#define C_EXITSUB 9
#define C_CLASS_DEFINE 10
#define C_NAMESPACE 12
#define C_TRAIT 13
#define C_LONG 14
#define C_NEW_OBJECT 15

#define ACC_MOD_PUBLIC 1
#define ACC_MOD_PROTECTED 2
#define ACC_MOD_PRIVATE 3

typedef struct AST {
	int node_type;
	int line_no;
	struct AST* left_node;
	struct AST* right_node;
} AST;

typedef struct Var {
	int node_type;
	int line_no;
	AST* location;
} Var;

Var* make_var(AST* location, int line_no);

typedef struct Constant {
	int node_type;
	int line_no;
	int type;
	union {
		int i_val;
		double d_val;
		char* s_val;
		long l_val;
	} value;
} Constant;

typedef struct RuntimeFunction {
	int runtime_type;
	char* name;
	ListBuffer* arguments;
	AST* statements;
	int access_modifier;
	int is_static;
} RuntimeFunction;

typedef struct RuntimeClass {
	int runtime_type;
	char* name;
	// HashTable<RuntimeValue>
	HashTable* properties;
	// HashTable<RuntimeFunction>
	HashTable* methods;
	// HashTable<RuntimeValue>
	HashTable* shared_properties;
	// HashTable<RuntimeFunction>
	HashTable* shared_methods;
	char* super_class;
	// LinkedList<String>
	LinkedList* traits;
} RuntimeClass;

typedef struct RuntimeNamespace {
	int runtime_type;
	char* name;
	HashTable* next_level;
	int is_root;
} RuntimeNamespace;

typedef struct RuntimeObject {
	int runtime_type;
	char* class_name;
	RuntimeClass* p_runtime_class;
	RuntimeNamespace* in_namespace;
	ListBuffer* path;
	// HashTable<RuntimeValue>
	HashTable* data;
} RuntimeObject;

typedef struct RuntimeValue {
	int runtime_type;
	union {
		int i_val;
		double d_val;
		char* s_val;
		long l_val;
		ListBuffer* list;
		RuntimeFunction* function;
		RuntimeClass* klass;
		RuntimeNamespace* domain_namespace;
		RuntimeObject* object;
	} value;
	int is_return;
} RuntimeValue;

RuntimeValue* make_new_runtime_list_buffer();

typedef struct RuntimeEnvironment {
	// Stack<RuntimeFunction>
	Stack* call_stack;
	// Stack<RuntimeValue>
	HashTable* vars;
	// Stack<RuntimeFunction>
	HashTable* functions;
	// HashTable<RuntimeClass>
	// HashTable* classes;
	RuntimeClass* current_building_class;
	RuntimeNamespace* current_namespace;
	RuntimeNamespace* root_namespace;
	Stack* namespace_stack;
	HashTable* imported_files;
} RuntimeEnvironment;

RuntimeEnvironment* env;

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

RuntimeValue* make_runtime_long(long l_val);

RuntimeValue* make_runtime_null();

RuntimeValue* make_runtime_break();

RuntimeValue* make_runtime_continue();

typedef struct Dimension {
	int node_type;
	int line_no;
	char* var_name;
	AST* location;
	AST* node;
	AST* next_dim;
	int shared;
} Dimension;

typedef struct RuntimeFunctionArg {
	char* name;
	RuntimeValue* default_value;
} RuntimeFunctionArg;

Dimension* var_make_null(char* var_name, int line_no);

AST* make_dim(AST* location, char* var_name, AST* node, AST* next_dim, int line_no);

AST* make_redim(char* var_name, AST* node, int line_no);

typedef struct IfStatement {
	int node_type;
	int line_no;
	AST* condition;
	AST* if_statement;
	AST* else_if_statement;
	AST* else_statement;
} IfStatement;

AST* make_ast(int node_type, AST* left_node, AST* right_node, int line_no);

AST* make_if_expression(AST* condition, AST* if_statement, AST* else_if_statement, AST* else_statement, int line_no);

typedef struct Function {
	int node_type;
	int line_no;
	AST* location;
	AST* expr_list;
} FunctionStatement;

AST* make_function_call(AST* location, AST* expr_list, int line_no);

AST* make_accessor(AST* left_node, char* attribute_name, AST* arguments, int line_no);

ListBuffer* integrate_params(AST* node);

typedef struct WhileStatement {
	int node_type;
	int line_no;
	AST* condition;
	AST* statements;
} WhileStatement;

AST* make_while_expression(AST* condition, AST* statements, int line_no);

typedef struct DoLoopStatement {
	int node_type;
	int line_no;
	AST* condition;
	AST* statements;
} DoLoopStatement;

AST* make_do_loop_expression(AST* statements, AST* condition, int line_no);

typedef struct ForStatement {
	int node_type;
	int line_no;
	AST* dim;
	AST* until;
	AST* step;
	AST* statements;
} ForStatement;

AST* make_for_expression(AST* dim, AST* until, AST* step, AST* statements, int line_no);

typedef struct FunctionDefineStatement {
	int node_type;
	int line_no;
	AST* statements;
	AST* arguments;
	char* name;
} FunctionDefineStatement;

AST* make_sub_define(char* name, AST* statements, AST* arguments, int line_no);

AST* make_function_define(char* name, AST* statements, AST* arguments, int line_no);

typedef struct FunctionStackElement {
	// HashTable<RuntimeValue>
	HashTable* local_vars;
	union {
		RuntimeObject* object;
		RuntimeClass* klass;
	} oop_info;
	int invoke_method; // 0 is invoke from object, 1 is invoke from class. 2 is neither.
} FunctionStackElement;

FunctionStackElement* dup_new_func_stack_element();

FunctionStackElement* dup_new_func_stack_element_object(RuntimeObject* object);

FunctionStackElement* dup_new_func_stack_element_class(RuntimeClass* klass);

typedef struct ReturnStatement {
	int node_type;
	int line_no;
	AST* return_expr;
} ReturnStatement;

AST* make_return(AST* expr, int line_no);

typedef struct BreakStatement {
	int node_type;
	int line_no;
	AST* placeholder;
	AST* placeholder2;
} BreakStatement;

typedef struct ContinueStatement {
	int node_type;
	int line_no;
	AST* placeholder;
	AST* placeholder2;
} ContinueStatement;

typedef struct FunctionArgumentDefinition {
	int node_type;
	int line_no;
	AST* default_value;
	AST* next_node;
	char* arg_name;
} FunctionArgumentDefinition;

typedef struct Accessor {
	int node_type;
	int line_no;
	AST* left_node;
	char* attribute_name;
	AST* arguments;
} Accessor;

AST* make_function_arg(char* name, AST* default_value, AST* next_node, int line_no);

AST* make_break(int line_no);
AST* make_continue(int line_no);

typedef struct ClassDefinition {
	int node_type;
	int line_no;
	char* class_name;
	AST* class_attributes;
	AST* class_body;
} ClassDefinition;

typedef struct ClassMethod {
	int node_type;
	int line_no;
	int access_modifier;
	int is_static;
	AST* function_define;
} ClassMethod;

typedef struct ClassProperty {
	int node_type;
	int line_no;
	int access_modifier;
	int is_static;
	AST* dim;
} ClassProperty;

typedef struct TraitImplementDefinition {
	int node_type;
	int line_no;
	AST* next_node;
	char* trait_name;
} TraitImplementDefinition;

typedef struct ClassAttrDefinition {
	int node_type;
	int line_no;
	char* extend_class;
	AST* trait_implement;
} ClassAttrDefinition;

typedef struct TraitDefinition {
	int node_type;
	int line_no;
	AST* trait_implement;
	AST* trait_body;
	char* trait_name;
} TraitDefinition;

typedef struct Import {
	int node_type;
	int line_no;
	char* alias;
	AST* location;
} Import;

typedef struct Location {
	int node_type;
	int line_no;
	AST* next_node;
	char* name;
} Location;

typedef struct NewObject {
	int node_type;
	int line_no;
	AST* class_location;
	AST* arguments;
} NewObject;

AST* make_location(char* name, AST* next_node, int line_no);

AST* make_class(char* class_name, AST* class_attributes, AST* class_body, int line_no);

AST* make_class(char* class_name, AST* trait_implement, AST* class_body, int line_no);

void init_current_building_runtime_class();

AST* make_trait_implement_definition(char* trait_name, AST* next_node, int line_no);

AST* make_new_object(AST* location, AST* arguments, int line_no);

AST* make_trait(char* trait_name, AST* trait_implement, AST* trait_body, int line_no);

AST* make_class_attr(char* extend_class, AST* trait_implement, int line_no);

AST* make_class_method(int access_modifier, int is_static, AST* function_define, int line_no);

AST* make_class_property(int access_modifier, int is_static, AST* dim, int line_no);

AST* make_import(char* alias, AST* location, int line_no);

ListBuffer* flatten_function_args(AST* node);

void execute_function_header(ListBuffer* definitions, ListBuffer* values, AST* ast);

RuntimeValue* execute(AST* ast);

void raise_error(const char* msg, const AST* ast);

#endif
