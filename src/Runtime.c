#include "Runtime.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HashTable.h"
#include "ListBuffer.h"
#include "./itoa/itoa.h"

Var* make_var(AST* location, int line_no) {
	Var* new_val = malloc(sizeof(Var) + 1);
	new_val->node_type = NODE_TYPE_VAR;
	new_val->line_no = line_no;
	new_val->location = location;
	return new_val;
}

Constant* make_ast_integer(int i_val) {
	Constant* new_val = (Constant*) malloc(sizeof(struct Constant) + 1);
	if (new_val == NULL) {
		printf("System error");
		exit(0);
	}
	new_val->node_type = NODE_TYPE_CONSTANT;
	new_val->type = C_INT;
	new_val->value.i_val = i_val;
	return new_val;
}

Constant* make_ast_decimal(double d_val) {
	Constant* new_val = (Constant*) malloc(sizeof(struct Constant) + 1);
	if (new_val == NULL) {
		printf("System error");
		exit(0);
	}
	new_val->node_type = NODE_TYPE_CONSTANT;
	new_val->type = C_DECIMAL;
	new_val->value.d_val = d_val;
	return new_val;
}

Constant* make_ast_string(char* s_val) {
	Constant* new_val = (Constant*) malloc(sizeof(struct Constant) + 1);
	if (new_val == NULL) {
		printf("System error");
		exit(0);
	}
	new_val->node_type = NODE_TYPE_CONSTANT;
	new_val->type = C_STRING;
	new_val->value.s_val = malloc(sizeof(char) * strlen(s_val) + 1);
	strcpy(new_val->value.s_val, s_val);
	return new_val;
}

RuntimeValue* make_runtime_integer(int i_val) {
	RuntimeValue* new_val = (RuntimeValue*) malloc(sizeof(RuntimeValue) + 1);
	if (new_val == NULL) {
		printf("System error");
		exit(0);
	}
	new_val->runtime_type = C_INT;
	new_val->value.i_val = i_val;
	new_val->is_return = 0;
	return new_val;
}

RuntimeValue* make_runtime_decimal(double d_val) {
	RuntimeValue* new_val = (RuntimeValue*) malloc(sizeof(RuntimeValue) + 1);
	if (new_val == NULL) {
		printf("System error");
		exit(0);
	}
	new_val->runtime_type = C_DECIMAL;
	new_val->value.d_val = d_val;
	new_val->is_return = 0;
	return new_val;
}

RuntimeValue* make_runtime_string(char* s_val) {
	RuntimeValue* new_val = (RuntimeValue*) malloc(sizeof(RuntimeValue) + 1);
	if (new_val == NULL) {
		printf("System error");
		exit(0);
	}
	new_val->runtime_type = C_STRING;
	new_val->value.s_val = (char *) malloc(sizeof(char) * strlen(s_val) + 1);
	strcpy(new_val->value.s_val, s_val);
	new_val->is_return = 0;
	return new_val;
}

RuntimeValue* make_runtime_long(long l_val) {
	RuntimeValue* new_val = (RuntimeValue*) malloc(sizeof(RuntimeValue) + 1);
	if (new_val == NULL) {
		printf("System error");
		exit(0);
	}
	new_val->runtime_type = C_LONG;
	new_val->value.l_val = l_val;
	new_val->is_return = 0;
	return new_val;
}

RuntimeValue* make_runtime_null() {
	RuntimeValue* new_val = (RuntimeValue*) malloc(sizeof(RuntimeValue) + 1);
	if (new_val == NULL) {
		printf("System error");
		exit(0);
	}
	new_val->runtime_type = C_NULL;
	new_val->is_return = 0;
	return new_val;
}

RuntimeValue* make_runtime_break() {
	RuntimeValue* new_val = (RuntimeValue*) malloc(sizeof(RuntimeValue) + 1);
	if (new_val == NULL) {
		printf("System error");
		exit(0);
	}
	new_val->runtime_type = C_BREAK;
	new_val->is_return = 0;
	return new_val;
}

RuntimeValue* make_runtime_continue() {
	RuntimeValue* new_val = (RuntimeValue*) malloc(sizeof(RuntimeValue) + 1);
	if (new_val == NULL) {
		printf("System error");
		exit(0);
	}
	new_val->runtime_type = C_CONTINUE;
	new_val->is_return = 0;
	return new_val;
}

char* new_string(char* str) {
	char* new_str = (char*) malloc(strlen(str) + 1);
	strcpy(new_str, str);
	return new_str;
}

int dump_numeric_as_integer(Constant* constant) {
	if (constant->type == C_INT) {
		return constant->value.i_val;
	} else if (constant->type == C_DECIMAL) {
		return (int)(constant->value.d_val);
	}
}

double dump_numeric_as_decimal(Constant* constant) {
	if (constant->type == C_INT) {
		return (double)(constant->value.i_val);
	} else if (constant->type == C_DECIMAL) {
		return constant->value.d_val;
	}
}

int runtime_as_integer(RuntimeValue* value) {
	if (value->runtime_type == C_INT) {
		return value->value.i_val;
	} else if (value->runtime_type == C_DECIMAL) {
		return (int)(value->value.d_val);
	}
}

double runtime_as_decimal(RuntimeValue* value) {
	if (value->runtime_type == C_INT) {
		return (double)(value->value.i_val);
	} else if (value->runtime_type == C_DECIMAL) {
		return value->value.d_val;
	}
}

Dimension* var_make_null(char* var_name, int line_no) {
	Dimension* new_block = (Dimension*) malloc(sizeof(Dimension) + 1);
	new_block->node_type = NODE_TYPE_ASSIGN_VAR;
	new_block->line_no = line_no;
	//new_block->runtime_type = "null";
	new_block->var_name = var_name;
	new_block->next_dim = NULL;
	new_block->shared = 0;
	return new_block;
}

AST* make_dim(AST* location, char* var_name, AST* node, AST* next_dim, int line_no) {
	Dimension* new_block = (Dimension*) malloc(sizeof(Dimension) + 1);
	if (var_name == NULL) {
		new_block->node_type = NODE_TYPE_SET_ATTR;
	} else {
		new_block->node_type = NODE_TYPE_ASSIGN_VAR;
	}
	new_block->line_no = line_no;
	new_block->var_name = var_name;
	new_block->location = location;
	new_block->node = node;
	new_block->next_dim = next_dim;
	new_block->shared = 0;
	return (AST*) new_block;
}

AST* make_redim(char* var_name, AST* node, int line_no) {
	Dimension* new_block = (Dimension*) malloc(sizeof(Dimension) + 1);
	new_block->node_type = NODE_TYPE_REASSIGN;
	new_block->line_no = line_no;
	new_block->var_name = var_name;
	new_block->location = NULL;
	new_block->node = node;
	new_block->next_dim = NULL;
	new_block->shared = 0;
	return (AST*) new_block;
}

AST* make_ast(int node_type, AST* left_node, AST* right_node, int line_no) {
	AST* new_val = malloc(sizeof(AST) + 1);
	new_val->node_type = node_type;
	new_val->line_no = line_no;
	new_val->left_node = left_node;
	new_val->right_node = right_node;
	return new_val;
}

AST* make_if_expression(AST* condition, AST* if_statement, AST* else_if_statement, AST* else_statement, int line_no) {
	IfStatement* new_val = malloc(sizeof(IfStatement) + 1);
	new_val->node_type = NODE_TYPE_IF_STATEMENT;
	new_val->line_no = line_no;
	new_val->condition = condition;
	new_val->if_statement = if_statement;
	new_val->else_if_statement = else_if_statement;
	new_val->else_statement = else_statement;
	return (AST*) new_val;
}

AST* make_function_call(AST* location, AST* expr_list, int line_no) {
	FunctionStatement* new_val = malloc(sizeof(FunctionStatement) + 1);
	new_val->node_type = NODE_TYPE_FUNC_CALL;
	new_val->line_no = line_no;
	new_val->location = location;
	new_val->expr_list = expr_list;
	return (AST*) new_val;
}

AST* make_for_expression(AST* dim, AST* until, AST* step, AST* statements, int line_no) {
	ForStatement* new_val = malloc(sizeof(ForStatement) + 1);
	new_val->node_type = NODE_TYPE_FOR_STATEMENT;
	new_val->line_no = line_no;
	new_val->dim = dim;
	new_val->until = until;
	new_val->step = step;
	new_val->statements = statements;
	return (AST*) new_val;
}

AST* make_sub_define(char* name, AST* statements, AST* arguments, int line_no) {
	FunctionDefineStatement* new_val = malloc(sizeof(FunctionDefineStatement) + 1);
	new_val->name = malloc(sizeof(char) * strlen(name) + 1);
	strcpy(new_val->name, name);
	new_val->node_type = NODE_TYPE_SUB;
	new_val->line_no = line_no;
	new_val->statements = statements;
	new_val->arguments = arguments;
	return (AST*) new_val;
}

AST* make_class(char* class_name, AST* class_attributes, AST* class_body, int line_no) {
	ClassDefinition* new_val = malloc(sizeof(ClassDefinition) + 1);
	new_val->node_type = NODE_TYPE_CLASS;
	new_val->line_no = line_no;
	new_val->class_name = malloc(sizeof(char) * strlen(class_name) + 1);
	strcpy(new_val->class_name, class_name);
	new_val->class_attributes = class_attributes;
	new_val->class_body = class_body;
	return (AST*) new_val;
}

AST* make_trait_implement_definition(char* trait_name, AST* next_node, int line_no) {
	TraitImplementDefinition* new_val = malloc(sizeof(TraitImplementDefinition) + 1);
	new_val->node_type = NODE_TYPE_TRAIT_NAME;
	new_val->trait_name = malloc(sizeof(char) * strlen(trait_name) + 1);
	strcpy(new_val->trait_name, trait_name);
	new_val->next_node = next_node;
	new_val->line_no = line_no;
	return (AST*) new_val;
}

AST* make_new_object(AST* location, AST* arguments, int line_no) {
	NewObject* new_val = malloc(sizeof(NewObject) + 1);
	new_val->node_type = NODE_TYPE_NEW_OBJECT;
	new_val->line_no = line_no;
	new_val->class_location = location;
	new_val->arguments = arguments;
	return (AST*) new_val;
}

AST* make_class_attr(char* extend_class, AST* trait_implement, int line_no) {
	ClassAttrDefinition* new_val = malloc(sizeof(ClassAttrDefinition) + 1);
	new_val->node_type = NODE_TYPE_CLASS_ATTR;
	new_val->line_no = line_no;
	if (extend_class == NULL) {
		new_val->extend_class = NULL;
	} else {
		new_val->extend_class = malloc(sizeof(char) * strlen(extend_class) + 1);
		strcpy(new_val->extend_class, extend_class);
	}
	new_val->trait_implement = trait_implement;
	return (AST*) new_val;
}

AST* make_class_method(int access_modifier, int is_static, AST* function_define, int line_no) {
	ClassMethod* new_val = malloc(sizeof(ClassMethod) + 1);
	new_val->node_type = NODE_TYPE_METHOD;
	new_val->access_modifier = access_modifier;
	new_val->is_static = is_static;
	new_val->function_define = function_define;
	new_val->line_no = line_no;
	return (AST*) new_val;
}

AST* make_class_property(int access_modifier, int is_static, AST* dim, int line_no) {
	ClassProperty* new_val = malloc(sizeof(ClassProperty) + 1);
	new_val->node_type = NODE_TYPE_PROPERTY;
	new_val->access_modifier = access_modifier;
	new_val->is_static = is_static;
	new_val->dim = dim;
	new_val->line_no = line_no;
	return (AST*) new_val;
}

AST* make_import(char* alias, AST* location, int line_no) {
	Import* new_val = malloc(sizeof(Import) + 1);
	new_val->node_type = NODE_TYPE_IMPORT;
	new_val->line_no = line_no;
	new_val->alias = malloc(sizeof(char) * strlen(alias) + 1);
	strcpy(new_val->alias, alias);
	new_val->location = location;
	return (AST*) new_val;
}

AST* make_function_define(char* name, AST* statements, AST* arguments, int line_no) {
	FunctionDefineStatement* new_val = malloc(sizeof(FunctionDefineStatement) + 1);
	new_val->name = malloc(sizeof(char) * strlen(name) + 1);
	strcpy(new_val->name, name);
	new_val->node_type = NODE_TYPE_FUNCTION_DEFINE;
	new_val->line_no = line_no;
	new_val->statements = statements;
	new_val->arguments = arguments;
	return (AST*) new_val;
}

AST* make_trait(char* trait_name, AST* trait_implement, AST* trait_body, int line_no) {
	TraitDefinition* new_val = malloc(sizeof(TraitDefinition) + 1);
	new_val->node_type = NODE_TYPE_TRAIT;
	new_val->trait_name = malloc(sizeof(char) * strlen(trait_name) + 1);
	strcpy(new_val->trait_name, trait_name);
	new_val->trait_implement = trait_implement;
	new_val->trait_body = trait_body;
	new_val->line_no = line_no;
	return (AST*) new_val;
}

FunctionStackElement* dup_new_func_stack_element() {
	FunctionStackElement* new_val = malloc(sizeof(FunctionStackElement) + 1);
	FunctionStackElement* peak = stack_peak(env->call_stack);
	HashTable* var_env_pt;
	if (peak == NULL) {
		var_env_pt = env->vars;
	} else {
		var_env_pt = peak;
	}
	HashTable* new_table = malloc(sizeof(HashTable) + 1);
	new_table = hash_init();
	new_val->local_vars = new_table;
	new_val->invoke_method = 2;
	return new_val;
}

FunctionStackElement* dup_new_func_stack_element_object(RuntimeObject* object) {
	FunctionStackElement* new_val = malloc(sizeof(FunctionStackElement) + 1);
	FunctionStackElement* peak = stack_peak(env->call_stack);
	HashTable* var_env_pt;
	if (peak == NULL) {
		var_env_pt = env->vars;
	} else {
		var_env_pt = peak;
	}
	HashTable* new_table = malloc(sizeof(HashTable) + 1);
	new_table = hash_init();
	new_val->local_vars = new_table;
	new_val->oop_info.object = object;
	new_val->invoke_method = 0;
	return new_val;
}

FunctionStackElement* dup_new_func_stack_element_class(RuntimeClass* klass) {
	FunctionStackElement* new_val = malloc(sizeof(FunctionStackElement) + 1);
	FunctionStackElement* peak = stack_peak(env->call_stack);
	HashTable* var_env_pt;
	if (peak == NULL) {
		var_env_pt = env->vars;
	} else {
		var_env_pt = peak;
	}
	HashTable* new_table = malloc(sizeof(HashTable) + 1);
	new_table = hash_init();
	new_val->local_vars = new_table;
	new_val->oop_info.klass = klass;
	new_val->invoke_method = 1;
	return new_val;
}

AST* make_return(AST* expr, int line_no) {
	ReturnStatement* new_val = malloc(sizeof(ReturnStatement) + 1);
	new_val->node_type = NODE_TYPE_RETURN;
	new_val->line_no = line_no;
	new_val->return_expr = expr;
	return (AST*) new_val;
}

AST* make_break(int line_no) {
	BreakStatement* new_val = malloc(sizeof(BreakStatement) + 1);
	if (new_val == NULL) {
		yyerror("System error.2");
		exit(0);
	}
	new_val->node_type = NODE_TYPE_BREAK;
	new_val->line_no = line_no;
	return (AST*) new_val;
}

AST* make_continue(int line_no) {
	ContinueStatement* new_val = malloc(sizeof(ContinueStatement) + 1);
	if (new_val == NULL) {
		yyerror("System error.3");
		exit(0);
	}
	new_val->node_type = NODE_TYPE_CONTINUE;
	new_val->line_no = line_no;
	return (AST*) new_val;
}

RuntimeValue* make_new_runtime_list_buffer() {
	RuntimeValue* new_val = (RuntimeValue*) malloc(sizeof(RuntimeValue) + 1);
	new_val->runtime_type = C_LIST_BUFFER;
	new_val->value.list = list_buffer_init();
	return new_val;
}

AST* make_location(char* name, AST* next_node, int line_no) {
	Location* new_val = malloc(sizeof(Location) + 1);
	new_val->node_type = NODE_TYPE_LOCATE;
	new_val->line_no = line_no;
	new_val->name = malloc(sizeof(char) * strlen(name) + 1);
	strcpy(new_val->name, name);
	new_val->next_node = next_node;
	return (AST*) new_val;
}

AST* make_accessor(AST* left_node, char* attribute_name, AST* arguments, int line_no) {
	Accessor* new_val = (Accessor*) malloc(sizeof(Accessor) + 1);
	new_val->node_type = NODE_TYPE_ACCESSOR;
	new_val->line_no = line_no;
	new_val->left_node = left_node;
	new_val->attribute_name = malloc(sizeof(char) * strlen(attribute_name) + 1);
	strcpy(new_val->attribute_name, attribute_name);
	new_val->arguments = arguments;
	return (AST*) new_val;
}

void init_current_building_runtime_class() {
	if (env->current_building_class->properties == NULL) {
		env->current_building_class->properties = hash_init();
	}
	if (env->current_building_class->shared_properties == NULL) {
		env->current_building_class->shared_properties = hash_init();
	}
	if (env->current_building_class->methods == NULL) {
		env->current_building_class->methods = hash_init();
	}
	if (env->current_building_class->shared_methods == NULL) {
		env->current_building_class->shared_methods = hash_init();
	}
}

int check_priviledge_method(Location class_location, char* class_method) {
	return 1;

}

int check_priviledge_property(Location class_location, char* class_property) {
	return 1;
}

RuntimeClass* find_as_runtime_class(ListBuffer* location, AST* ast) {
	RuntimeNamespace* current_namespace = env->current_namespace;
	
	for (int i = 0; i < location->count; i++) {
		char* node_name = list_buffer_get(location, i);
		if (hash_has_key(current_namespace->next_level, node_name)) {
			RuntimeNamespace* name_space_or_class = (RuntimeNamespace*) hash_get(current_namespace->next_level, node_name);
			if (name_space_or_class->runtime_type == C_NAMESPACE && i < location->count - 1) {
			 	current_namespace = name_space_or_class;
			} else if (name_space_or_class->runtime_type == C_CLASS_DEFINE && i == location->count - 1) {
				return (RuntimeClass*) name_space_or_class;
			} else {
				raise_error("System error.4", ast);
				exit(0);
			}
		} else {
			break;
		}
	}
	
	current_namespace = env->root_namespace;
	
	for (int i = 0; i < location->count; i++) {
		char* node_name = list_buffer_get(location, i);
		if (hash_has_key(current_namespace->next_level, node_name)) {
			RuntimeNamespace* name_space_or_class = (RuntimeNamespace*) hash_get(current_namespace->next_level, node_name);
			if (name_space_or_class->runtime_type == C_NAMESPACE && i < location->count - 1) {
			 	current_namespace = name_space_or_class;
			} else if (name_space_or_class->runtime_type == C_CLASS_DEFINE && i == location->count - 1) {
				return (RuntimeClass*) name_space_or_class;
			} else {
				raise_error("System error.5", ast);
				exit(0);
			}
		} else {
			break;
		}
	}
	/*
	char* node_name = list_buffer_get(location, 0);
	
	if (hash_has_key(env->classes, node_name)) {
		return (RuntimeClass*) hash_get(env->classes, node_name);
	}
	*/
	
	raise_error("Cannot find the specified class.", ast);
	exit(0);
}

RuntimeValue* execute(AST* ast) {
	if (ast->node_type == NODE_TYPE_ASSIGN_VAR) {
		Dimension* dim = (Dimension*) ast;
		FunctionStackElement* peak = (FunctionStackElement*) stack_peak(env->call_stack);
		if (peak == NULL && hash_has_key(env->vars, dim->var_name)) {
			raise_error("Defined variable.", ast);
			exit(0);
		}
		if (peak != NULL && hash_has_key(peak->local_vars, dim->var_name)) {
			raise_error("Defined variable.", ast);
			exit(0);
		}
		RuntimeValue* value = NULL;
		if (peak == NULL) {
			if (dim->node != NULL) {
				value = execute(dim->node);
				hash_put(env->vars, dim->var_name, value);
			} else {
				hash_put(env->vars, dim->var_name, make_runtime_null());
			}
			if (dim->next_dim != NULL) {
				execute(dim->next_dim);
			}
			if (value != NULL) {
				return value;
			}
		} else {
			if (dim->node != NULL) {
				value = execute(dim->node);
				hash_put(peak->local_vars, dim->var_name, value);
			} else {
				hash_put(peak->local_vars, dim->var_name, make_runtime_null());
			}
			if (dim->next_dim != NULL) {
				execute(dim->next_dim);
			}
			if (value != NULL) {
				return value;
			}
		}
	} else if (ast->node_type == NODE_TYPE_FUNCTION_DEFINE || ast->node_type == NODE_TYPE_SUB) {
		// Function definition
		FunctionDefineStatement* function_statement = (FunctionDefineStatement*) ast;
		RuntimeFunction* func = malloc(sizeof(RuntimeFunction) + 1);
		if (ast->node_type == NODE_TYPE_FUNCTION_DEFINE) {
			func->runtime_type = C_FUNCTION_DEFINE;
		} else {
			func->runtime_type = C_SUB;
		}
		func->name = malloc(sizeof(char) * strlen(function_statement->name) + 1);
		strcpy(func->name, function_statement->name);
		func->statements = function_statement->statements;
		RuntimeValue* args_list;
		func->arguments = NULL;
		if (function_statement->arguments != NULL) {
			args_list = execute(function_statement->arguments);
			if (args_list != NULL) {
				func->arguments = args_list->value.list;
			}
		} else {
			func->arguments = list_buffer_init();
		}
		if (hash_has_key(env->functions, func->name)) {
			raise_error("You cannot redefine a function or subroutine.", ast);
			exit(0);
		}
		if (env->current_building_class == NULL) {
			hash_put(env->functions, func->name, (void*) func);
		} else {
			RuntimeValue* return_value_with_function = malloc(sizeof(RuntimeValue) + 1);
			return_value_with_function->runtime_type = func->runtime_type;
			return_value_with_function->value.function = func;
			
			return return_value_with_function;
		}
	} else if (ast->node_type == NODE_TYPE_REASSIGN) {
		Dimension* dim = (Dimension*) ast;
		FunctionStackElement* peak = (FunctionStackElement*) stack_peak(env->call_stack);
		if (peak == NULL && !hash_has_key(env->vars, dim->var_name)) {
			raise_error("Undefined variable.", ast);
			exit(0);
		}
		if (peak != NULL && !hash_has_key(peak->local_vars, dim->var_name)) {
			raise_error("Undefined variable.", ast);
			exit(0);
		}
		if (peak == NULL) {
			if (dim->node != NULL) {
				RuntimeValue* value = execute(dim->node);
				hash_put(env->vars, dim->var_name, value);
				return value;
			} else {
				RuntimeValue* null_value = make_runtime_null();
				hash_put(env->vars, dim->var_name, null_value);
				return null_value;
			}
		} else {
			if (dim->node != NULL) {
				RuntimeValue* value = execute(dim->node);
				hash_put(peak->local_vars, dim->var_name, value);
				return value;
			} else {
				RuntimeValue* null_value = make_runtime_null();
				hash_put(peak->local_vars, dim->var_name, null_value);
				return null_value;
			}
		}
	} else if (ast->node_type == NODE_TYPE_SET_ATTR) {
		Dimension* dimension = (Dimension*) ast;
		if (dimension->location == NULL) {
			raise_error("System error.6.", ast);
			exit(0);
		}
		RuntimeValue* location_rt = execute(dimension->location);
		ListBuffer* location = location_rt->value.list;
		RuntimeClass* current_object = NULL;
		RuntimeValue* set_object_rt_value = NULL;
		FunctionStackElement* peak = (FunctionStackElement*) stack_peak(env->call_stack);
		for (int i = 0; i < location->count;) {
			char* current_node_name = (char*) list_buffer_get(location, i);
			if (current_object == NULL) {
				if (hash_has_key(env->current_namespace->next_level, current_node_name)) {
					current_object = env->current_namespace; // RuntimeNamespace/RuntimeClass
				} else if (hash_has_key(env->root_namespace->next_level, current_node_name)) {
					current_object = env->root_namespace; // RuntimeNamespace/RuntimeClass
				} /*else if (hash_has_key(env->classes, current_node_name)) {
					current_object = hash_get(env->classes, current_node_name); // RuntimeClass
				}*/ 
				else if (hash_has_key(env->vars, current_node_name)) {
					current_object = (RuntimeValue*) hash_get(env->vars, current_node_name);
					i++;
				} else if (peak != NULL && peak->invoke_method == 0) {
					// set properties from object
					current_object = (RuntimeObject*)(peak->oop_info.object);	// RuntimeObject
					i++;
				} else {
					raise_error("Cannot find specified namespace or class.3", ast);
					exit(0);
				}
			} else if (current_object->runtime_type == C_NAMESPACE) {
				if (hash_has_key(((RuntimeNamespace*)current_object)->next_level, current_node_name)) {
					RuntimeClass* compute_next_obj = hash_get(((RuntimeNamespace*)current_object)->next_level, current_node_name);
					if (compute_next_obj == C_NAMESPACE) {
						current_object = (RuntimeNamespace*) hash_get(((RuntimeNamespace*)current_object)->next_level, current_node_name);
					} else {
						current_object = (RuntimeClass*) hash_get(((RuntimeNamespace*)current_object)->next_level, current_node_name);
					}
					i++;
				} else {
					raise_error("Cannot find specified namespace.", ast);
					exit(0);
				}
			} else if (current_object->runtime_type == C_CLASS_DEFINE) {
				if (hash_has_key(((RuntimeClass*)current_object)->shared_properties, current_node_name)) {
					set_object_rt_value = (RuntimeObject*) hash_get(((RuntimeClass*)current_object)->shared_properties, current_node_name);
					break;
				} else {
					raise_error("Cannot find specified property.", ast);
					exit(0);
				}
			} else if (current_object->runtime_type == C_NEW_OBJECT) {
				if (hash_has_key(((RuntimeObject*)(((RuntimeValue*)(current_object))->value.object))->data, current_node_name)) {
					RuntimeValue* property = hash_get(((RuntimeObject*)(((RuntimeValue*)(current_object))->value.object))->data, current_node_name);
					if (property->runtime_type == C_INT) {
						set_object_rt_value = property;
					} else if (property->runtime_type == C_DECIMAL) {
						set_object_rt_value = property;
					} else if (property->runtime_type == C_STRING) {
						set_object_rt_value = property;
					} else if (property->runtime_type == C_LONG) {
						set_object_rt_value = property;
					} else if (property->runtime_type == C_NEW_OBJECT) {
						current_object = property;
						set_object_rt_value = property;
						i++;
						continue;
					} else if (property->runtime_type == C_NULL) {
						set_object_rt_value = property;
					} else {
						raise_error("System error!", ast);
						exit(0);
					}
					i++;
					break;
				} else {
					raise_error("Undefined element of an object.", ast);
					exit(0);
				}
			} else {
				raise_error("error", ast);
				exit(0);
			}
		}
		
		if (set_object_rt_value == NULL) {
			raise_error("Cannot locate given path.", ast);
			exit(0);
		}
		
		RuntimeValue* eval_value = execute(dimension->node);
		
		// clone
		if (eval_value->runtime_type == C_INT) {
			RuntimeValue* p = set_object_rt_value;
			p->runtime_type = eval_value->runtime_type;
			p->value.i_val = eval_value->value.i_val;
		} else if (eval_value->runtime_type == C_DECIMAL) {
			RuntimeValue* p = set_object_rt_value;
			p->runtime_type = eval_value->runtime_type;
			p->value.d_val = eval_value->value.d_val;
		} else if (eval_value->runtime_type == C_STRING) {
			RuntimeValue* p = set_object_rt_value;
			p->runtime_type = eval_value->runtime_type;
			p->value.s_val = malloc(sizeof(char) * strlen(eval_value->value.s_val) + 1);
			strcpy(p->value.s_val, eval_value->value.s_val);
		} else if (eval_value->runtime_type == C_LONG) {
			RuntimeValue* p = set_object_rt_value;
			p->runtime_type = eval_value->runtime_type;
			p->value.l_val = eval_value->value.l_val;
		} else if (eval_value->runtime_type == C_NEW_OBJECT) {
			RuntimeValue* p = set_object_rt_value;
			p->runtime_type = eval_value->runtime_type;
			p->value.object = eval_value->value.object;
		}
	} else if (ast->node_type == NODE_TYPE_CONSTANT) {
		RuntimeValue* value = malloc(sizeof(RuntimeValue) + 1);
		value->runtime_type = ((Constant *)ast)->type;
		if (value->runtime_type == C_INT) {
			value->value.i_val = ((Constant *)ast)->value.i_val;
		} else if (value->runtime_type == C_DECIMAL) {
			value->value.d_val = ((Constant *)ast)->value.d_val;
		} else if (value->runtime_type == C_STRING) {
			value->value.s_val = ((Constant *)ast)->value.s_val;
		} else if (value->runtime_type == C_LONG) {
			value->value.l_val = ((Constant *)ast)->value.l_val;
		} else {
			raise_error("System error. 11", ast);
		}
		value->is_return = 0;
		return value;
	} else if (ast->node_type == NODE_TYPE_IF_STATEMENT) {
		IfStatement* if_statement = (IfStatement *) ast;
		RuntimeValue* eval_if = execute(if_statement->condition);
		if (runtime_as_integer(eval_if)) {
			RuntimeValue* eval_if_statement = execute(if_statement->if_statement);
			return eval_if_statement;
		} else {
			IfStatement* tmp_stat = if_statement->else_if_statement;
			if (if_statement->else_if_statement != NULL) {
				while (tmp_stat != NULL) {
					if (runtime_as_integer(execute(tmp_stat->condition))) {
						return execute(tmp_stat->if_statement);
					}
					tmp_stat = tmp_stat->else_if_statement;
				}
			}
			if (if_statement->else_statement != NULL) {
				return execute(if_statement->else_statement);
			}
		}
	} else if (ast->node_type == NODE_TYPE_ELSE) {
		return execute(ast->left_node);
	} else if (ast->node_type == NODE_TYPE_PRIORITY) {
		return execute(ast->left_node);
	} else if (ast->node_type == NODE_TYPE_STATEMENT) {
		if (ast->left_node == NULL) {
			return NULL;
		}
		AST* current_node = ast;
		while (current_node != NULL) {
			RuntimeValue* interrupter = execute(current_node->left_node);
			if (interrupter != NULL) {
				if (interrupter->runtime_type == C_BREAK || interrupter->runtime_type == C_CONTINUE || interrupter->is_return) {
					return interrupter;
				}
			}
			current_node = current_node->right_node;
		}
		return NULL;
	} else if (ast->node_type == '&') {
		RuntimeValue* l = execute(ast->left_node);
		RuntimeValue* r= execute(ast->right_node);
		if (l->runtime_type == C_INT && r->runtime_type == C_INT) {
			return make_runtime_integer(runtime_as_integer(l) && runtime_as_integer(r));
		} else {
			raise_error("Type error.", ast);
			exit(0);
		}
	} else if (ast->node_type == NODE_TYPE_BITAND) {
		RuntimeValue* l = execute(ast->left_node);
		RuntimeValue* r= execute(ast->right_node);
		if (l->runtime_type == C_INT && r->runtime_type == C_INT) {
			return make_runtime_integer(runtime_as_integer(l) & runtime_as_integer(r));
		} else {
			raise_error("Type error.", ast);
			exit(0);
		}
	} else if (ast->node_type == NODE_TYPE_BITOR) {
		RuntimeValue* l = execute(ast->left_node);
		RuntimeValue* r= execute(ast->right_node);
		if (l->runtime_type == C_INT && r->runtime_type == C_INT) {
			return make_runtime_integer(runtime_as_integer(l) | runtime_as_integer(r));
		} else {
			raise_error("Type error.", ast);
			exit(0);
		}
	} else if (ast->node_type == '|') {
		RuntimeValue* l = execute(ast->left_node);
		RuntimeValue* r= execute(ast->right_node);
		if (l->runtime_type == C_INT && r->runtime_type == C_INT) {
			return make_runtime_integer(runtime_as_integer(l) || runtime_as_integer(r));
		} else {
			raise_error("Type error.", ast);
			exit(0);
		}
	} else if (ast->node_type == '!') {
		RuntimeValue* l = execute(ast->left_node);
		return make_runtime_integer(!runtime_as_integer(l));
	} else if (ast->node_type == '^') {
		RuntimeValue* l = execute(ast->left_node);
		RuntimeValue* r= execute(ast->right_node);
		if (l->runtime_type == C_INT && r->runtime_type == C_INT) {
			return make_runtime_integer(runtime_as_integer(l) ^ runtime_as_integer(r));
		} else {
			raise_error("Type error.", ast);
			exit(0);
		}
	} else if (ast->node_type == '<') {
		RuntimeValue* l = execute(ast->left_node);
		RuntimeValue* r= execute(ast->right_node);
		if (l->runtime_type == C_DECIMAL || r->runtime_type == C_DECIMAL) {
			return make_runtime_integer(runtime_as_decimal(l) < runtime_as_decimal(r));
		} else if (l->runtime_type == C_INT && r->runtime_type == C_INT) {
			return make_runtime_integer(runtime_as_integer(l) < runtime_as_integer(r));
		} else {
			raise_error("Type error.", ast);
			exit(0);
		}
	} else if (ast->node_type == '>') {
		RuntimeValue* l = execute(ast->left_node);
		RuntimeValue* r= execute(ast->right_node);
		if (l->runtime_type == C_DECIMAL || r->runtime_type == C_DECIMAL) {
			return make_runtime_integer(runtime_as_decimal(l) > runtime_as_decimal(r));
		} else if (l->runtime_type == C_INT && r->runtime_type == C_INT) {
			return make_runtime_integer(runtime_as_integer(l) > runtime_as_integer(r));
		} else {
			raise_error("Type error.", ast);
			exit(0);
		}
	} else if (ast->node_type == '$') {
		RuntimeValue* l = execute(ast->left_node);
		RuntimeValue* r= execute(ast->right_node);
		if (l->runtime_type == C_STRING && r->runtime_type == C_STRING) {
			return make_runtime_integer(!strcmp(l->value.s_val, r->value.s_val));
		} else if (l->runtime_type == C_DECIMAL || r->runtime_type == C_DECIMAL) {
			return make_runtime_integer(runtime_as_decimal(l) != runtime_as_decimal(r));
		} else if (l->runtime_type == C_INT && r->runtime_type == C_INT) {
			return make_runtime_integer(runtime_as_integer(l) != runtime_as_integer(r));
		} else {
			raise_error("Type error.", ast);
			exit(0);
		}
	} else if (ast->node_type == '+') {
		RuntimeValue* l = execute(ast->left_node);
		RuntimeValue* r= execute(ast->right_node);
		if (l->runtime_type == C_DECIMAL || r->runtime_type == C_DECIMAL) {
			return make_runtime_decimal(runtime_as_decimal(l) + runtime_as_decimal(r));
		} else if (l->runtime_type == C_INT && r->runtime_type == C_INT) {
			return make_runtime_integer(runtime_as_integer(l) + runtime_as_integer(r));
		} else {
			raise_error("Type error.", ast);
			exit(0);
		}
	} else if (ast->node_type == NODE_TYPE_STR_CONCAT) {
		RuntimeValue* l = execute(ast->left_node);
		RuntimeValue* r= execute(ast->right_node);
		if (l->runtime_type == C_STRING && r->runtime_type == C_STRING) {
			return make_runtime_string(strcat(l->value.s_val, r->value.s_val));
		} else {
			raise_error("\'&\' cannot concat two expressions which one of them is not String.", ast);
			exit(0);
		}
	} else if (ast->node_type == '-') {
		RuntimeValue* l = execute(ast->left_node);
		RuntimeValue* r= execute(ast->right_node);
		if (l->runtime_type == C_DECIMAL || r->runtime_type == C_DECIMAL) {
			return make_runtime_decimal(runtime_as_decimal(l) - runtime_as_decimal(r));
		} else if (l->runtime_type == C_INT && r->runtime_type == C_INT) {
			return make_runtime_integer(runtime_as_integer(l) - runtime_as_integer(r));
		} else {
			raise_error("Type error.", ast);
			exit(0);
		}
	} else if (ast->node_type == '*') {
		RuntimeValue* l = execute(ast->left_node);
		RuntimeValue* r= execute(ast->right_node);
		if (l->runtime_type == C_DECIMAL || r->runtime_type == C_DECIMAL) {
			return make_runtime_decimal(runtime_as_decimal(l) * runtime_as_decimal(r));
		} else if (l->runtime_type == C_INT && r->runtime_type == C_INT) {
			return make_runtime_integer(runtime_as_integer(l) * runtime_as_integer(r));
		} else {
			raise_error("Type error.", ast);
			exit(0);
		}
	} else if (ast->node_type == '/') {
		RuntimeValue* l = execute(ast->left_node);
		RuntimeValue* r= execute(ast->right_node);
		if (runtime_as_decimal(r) == 0.0) {
			raise_error("Divided by 0!", ast);
			exit(0);
		}
		if (l->runtime_type == C_DECIMAL || r->runtime_type == C_DECIMAL) {
			return make_runtime_decimal(runtime_as_decimal(l) / runtime_as_decimal(r));
		} else if (l->runtime_type == C_INT && r->runtime_type == C_INT) {
			return make_runtime_integer(runtime_as_integer(l) / runtime_as_integer(r));
		} else {
			raise_error("Type error.", ast);
			exit(0);
		}
	} else if (ast->node_type == NODE_TYPE_GTE) {
		RuntimeValue* l = execute(ast->left_node);
		RuntimeValue* r= execute(ast->right_node);
		if (l->runtime_type == C_DECIMAL || r->runtime_type == C_DECIMAL) {
			return make_runtime_integer(runtime_as_decimal(l) >= runtime_as_decimal(r));
		} else if (l->runtime_type == C_INT && r->runtime_type == C_INT) {
			return make_runtime_integer(runtime_as_integer(l) >= runtime_as_integer(r));
		} else {
			raise_error("Type error.", ast);
			exit(0);
		}
	} else if (ast->node_type == NODE_TYPE_LTE) {
		RuntimeValue* l = execute(ast->left_node);
		RuntimeValue* r= execute(ast->right_node);
		if (l->runtime_type == C_DECIMAL || r->runtime_type == C_DECIMAL) {
			return make_runtime_integer(runtime_as_decimal(l) <= runtime_as_decimal(r));
		} else if (l->runtime_type == C_INT && r->runtime_type == C_INT) {
			return make_runtime_integer(runtime_as_integer(l) <= runtime_as_integer(r));
		} else {
			raise_error("Type error.", ast);
			exit(0);
		}
	} else if (ast->node_type == NODE_TYPE_EQ) {
		RuntimeValue* l = execute(ast->left_node);
		RuntimeValue* r= execute(ast->right_node);
		if (l->runtime_type == C_DECIMAL || r->runtime_type == C_DECIMAL) {
			return make_runtime_integer(runtime_as_decimal(l) == runtime_as_decimal(r));
		} else if (l->runtime_type == C_INT && r->runtime_type == C_INT) {
			return make_runtime_integer(runtime_as_integer(l) == runtime_as_integer(r));
		} else {
			raise_error("Type error.", ast);
			exit(0);
		}
	} else if (ast->node_type == NODE_TYPE_MOD) {
		RuntimeValue* l = execute(ast->left_node);
		RuntimeValue* r= execute(ast->right_node);
		if (runtime_as_decimal(r) == 0.0) {
			raise_error("Modulo by 0!", ast);
			exit(0);
		}
		if (l->runtime_type == C_DECIMAL || r->runtime_type == C_DECIMAL) {
			return make_runtime_decimal(runtime_as_decimal(l) / runtime_as_decimal(r));
		} else {
			return make_runtime_integer(runtime_as_integer(l) / runtime_as_integer(r));
		}
	} else if (ast->node_type == NODE_TYPE_FUNC_CALL) {
		// Function call
		FunctionStatement* function_statement = (FunctionStatement *)ast;
		
		RuntimeValue* location_rt = execute(function_statement->location);
		ListBuffer* location = location_rt->value.list;
		RuntimeValue* argument_list = execute(function_statement->expr_list);
		
		char* first_function_name = list_buffer_get(location, 0);
		
		RuntimeClass* set_object_rt_value = NULL;
		
		RuntimeValue* param1 = list_buffer_get(argument_list->value.list, 0);
		
		if (!strcmp(first_function_name, "Print")) {
			RuntimeValue* param1 = list_buffer_get(argument_list->value.list, 0);
			if (param1->runtime_type == C_STRING) {
				printf("%s", param1->value.s_val);
			} else if (param1->runtime_type == C_INT) {
				printf("%d", param1->value.i_val);
			} else if (param1->runtime_type == C_DECIMAL) {
				printf("%f", param1->value.d_val);
			} else if (param1->runtime_type == C_NULL) {
				printf("NULL");
			} else if (param1->runtime_type == C_LONG) {
				printf("%d", param1->value.l_val);
			} else if (param1->runtime_type == C_NEW_OBJECT) {
				printf("(OBJECT)");
			} else {
				printf("System error.7\n");
			}
			return NULL;
		} else if (!strcmp(first_function_name, "PrintLine")) {
			if (param1->runtime_type == C_STRING) {
				printf("%s\n", param1->value.s_val);
			} else if (param1->runtime_type == C_INT) {
				printf("%d\n", param1->value.i_val);
			} else if (param1->runtime_type == C_DECIMAL) {
				printf("%f\n", param1->value.d_val);
			} else if (param1->runtime_type == C_NULL) {
				printf("Null\n");
			} else if (param1->runtime_type == C_LONG) {
				printf("%d", param1->value.l_val);
			} else if (param1->runtime_type == C_NEW_OBJECT) {
				printf("(OBJECT)");
			} else {
				raise_error("System error.8", ast);
				exit(0);
			}
			return NULL;
		} else if (!strcmp(first_function_name, "CStr")) {
			RuntimeValue* result = make_runtime_string("");
			if (param1->runtime_type == C_STRING) {
				result->value.s_val = malloc(sizeof(char) * strlen(param1->value.s_val) + 1);
				strcpy(result->value.s_val, param1->value.s_val);
			} else if (param1->runtime_type == C_INT) {
				result->value.s_val = malloc(sizeof(char) * 16 + 1);
				ultoa(result->value.s_val, param1->value.i_val);
			} else if (param1->runtime_type == C_NULL) {
			} else {
				raise_error("System error.9", ast);
				exit(0);
			}
			return result;
		} else if (!strcmp(first_function_name, "Chr")) {
			if (param1->runtime_type == C_INT) {
				char ch[1] = {(char)(param1->value.i_val)};
				RuntimeValue* result = make_runtime_string(ch);
				return result;
			} else {
				raise_error("Arguments type error.", ast);
				exit(0);
			}
		} else {
			RuntimeClass* current_object = NULL;
			FunctionStackElement* peak = (FunctionStackElement*) stack_peak(env->call_stack);
			for (int i = 0; i < location->count;) {
				char* current_node_name = (char*) list_buffer_get(location, i);
				if (current_object == NULL) {
					if (env->current_namespace != NULL && hash_has_key(env->current_namespace->next_level, current_node_name)) {
						current_object = env->current_namespace; // RuntimeNamespace/RuntimeClass
					} else if (hash_has_key(env->root_namespace->next_level, current_node_name)) {
						current_object = env->root_namespace; // RuntimeNamespace/RuntimeClass
					} /*else if (hash_has_key(env->classes, current_node_name)) {
						current_object = hash_get(env->classes, current_node_name); // RuntimeClass
					}*/ else if (peak != NULL && peak->invoke_method == 0) {
						// preprare for using methods in the object.
						current_object = peak->oop_info.object;	// RuntimeObject
					} else if (peak == NULL && hash_has_key(env->vars, current_node_name)) {
						current_object = (RuntimeValue*) hash_get(env->vars, current_node_name);	// RuntimeValue
						i++;
					} else if (hash_has_key(env->functions, current_node_name)) { // call a function
						set_object_rt_value = hash_get(env->functions, current_node_name);
						break;
					} else {
						raise_error("Cannot find specified namespace or class.1", ast);
						exit(0);
					}
				} else if (current_object->runtime_type == C_NAMESPACE) {
					if (hash_has_key(((RuntimeNamespace*)current_object)->next_level, current_node_name)) {
						RuntimeClass* compute_next_obj = hash_get(((RuntimeNamespace*)current_object)->next_level, current_node_name);
						if (compute_next_obj == C_NAMESPACE) {
							current_object = (RuntimeNamespace*) hash_get(((RuntimeNamespace*)current_object)->next_level, current_node_name);
						} else {
							current_object = (RuntimeClass*) hash_get(((RuntimeNamespace*)current_object)->next_level, current_node_name);
						}
						i++;
					} else {
						raise_error("Cannot find specified namespace.", ast);
						exit(0);
					}
				} else if (current_object->runtime_type == C_CLASS_DEFINE) {
					if (hash_has_key(((RuntimeClass*)current_object)->shared_methods, current_node_name)) {
						set_object_rt_value = (RuntimeFunction*) hash_get(((RuntimeClass*)current_object)->shared_methods, current_node_name);
						break;
					} else {
						raise_error("Cannot find specified shared method.", ast);
						exit(0);
					}
				} else if (current_object->runtime_type == C_NEW_OBJECT) {
					if (hash_has_key(((RuntimeObject*)(((RuntimeValue*)(current_object))->value.object))->p_runtime_class->methods, current_node_name)) {
						set_object_rt_value = (RuntimeFunction*) hash_get(((RuntimeObject*)(((RuntimeValue*)(current_object))->value.object))->p_runtime_class->methods, current_node_name);
						break;
					} else if (hash_has_key(((RuntimeObject*)(((RuntimeValue*)(current_object))->value.object))->data, current_node_name)) {
						current_object = hash_get(((RuntimeObject*)(((RuntimeValue*)(current_object))->value.object))->data, current_node_name);
					} else {
						raise_error("Cannot find specified shared method.", ast);
						exit(0);
					}
				} else {
					raise_error("error", ast);
					exit(0);
				}
			}
			
			if (set_object_rt_value == NULL) {
				raise_error("Cannot find specified method or shared method.", ast);
				exit(0);
			}
			
			if (current_object != NULL && current_object->runtime_type == C_NEW_OBJECT) {
				stack_push(env->call_stack, dup_new_func_stack_element_object(current_object));
			} else {
				stack_push(env->call_stack, dup_new_func_stack_element());
			}
			RuntimeFunction* current_runtime_function =
				(RuntimeFunction*) set_object_rt_value;
			
			if (current_runtime_function == NULL) {
				raise_error("The function/subroutine/method which you're finding does not defined.", ast);
				exit(0);
			}
			if (current_runtime_function->arguments->count > 0) {
				execute_function_header(current_runtime_function->arguments, argument_list->value.list, ast);
			}
			
			RuntimeValue* return_value = execute(current_runtime_function->statements);
			FunctionStackElement* popped_env = (FunctionStackElement*) stack_pop(env->call_stack);
			hash_free(popped_env->local_vars);
			free(popped_env);
			if (current_runtime_function->runtime_type == C_FUNCTION_DEFINE) {
				if (return_value == NULL) {
					raise_error("Function must return a value.", ast);
					exit(0);
				}
				return_value->is_return = 0;
				return return_value;
			} else if (return_value != NULL && return_value->runtime_type != C_EXITSUB) {
				raise_error("Subroutine cannot return a value.", ast);
				exit(0);
			}
		}
		
	} else if (ast->node_type == NODE_TYPE_EXPR_ITEM) {
		RuntimeValue* value = malloc(sizeof(RuntimeValue) + 1);
		value->runtime_type = C_LIST_BUFFER;
		value->value.list = integrate_params(ast);
		return value;
	} else if (ast->node_type == NODE_TYPE_VAR) {
		Var* var = (Var*) ast;
		
		RuntimeValue* location_rt = execute(var->location);
		ListBuffer* location = location_rt->value.list;
		
		if (location->count == 1) {
		
			char* var_name = (char*) list_buffer_get(location, 0);
			
			FunctionStackElement* peak = (FunctionStackElement*) stack_peak(env->call_stack);
			HashTable* from_env;
			if (peak == NULL) {
				from_env = env->vars;
			} else {
				from_env = peak->local_vars;
			}
			
			RuntimeValue* fetched_var = (RuntimeValue*) hash_get(from_env, var_name);
			if (peak != NULL && fetched_var == NULL) {
				fetched_var = (RuntimeValue*) hash_get(env->vars, var_name);
			}
			if (fetched_var != NULL) {
				return fetched_var;
			} else {
				raise_error("Undefined variable.", ast);
				exit(0);
			}
		} else {
			RuntimeClass* current_object = NULL;
			RuntimeValue* ret_rt_val = NULL;
			FunctionStackElement* peak = (FunctionStackElement*) stack_peak(env->call_stack);
			for (int i = 0; i < location->count;) {
				char* current_node_name = (char*) list_buffer_get(location, i);
				if (current_object == NULL) {
					if (hash_has_key(env->current_namespace->next_level, current_node_name)) {
						current_object = env->current_namespace; // RuntimeNamespace/RuntimeClass
					} else if (hash_has_key(env->root_namespace->next_level, current_node_name)) {
						current_object = env->root_namespace; // RuntimeNamespace/RuntimeClass
					} /*else if (hash_has_key(env->classes, current_node_name)) {
						current_object = hash_get(env->classes, current_node_name); // RuntimeClass
					} */else if (peak != NULL && peak->invoke_method == 0) {
						// set properties from object
						current_object = peak->oop_info.object; // RuntimeObject
						i++;
					} else if (hash_has_key(env->vars, current_node_name)) {
						current_object = (RuntimeValue*) hash_get(env->vars, current_node_name);
						i++;
					} else {
						raise_error("Cannot find specified namespace or class.2", ast);
						exit(0);
					}
				} else if (current_object->runtime_type == C_NAMESPACE) {
					if (hash_has_key(((RuntimeNamespace*)current_object)->next_level, current_node_name)) {
						RuntimeClass* compute_next_obj = hash_get(((RuntimeNamespace*)current_object)->next_level, current_node_name);
						if (compute_next_obj == C_NAMESPACE) {
							current_object = (RuntimeNamespace*) hash_get(((RuntimeNamespace*)current_object)->next_level, current_node_name);
						} else {
							current_object = (RuntimeClass*) hash_get(((RuntimeNamespace*)current_object)->next_level, current_node_name);
						}
						i++;
					} else {
						raise_error("Cannot find specified namespace.", ast);
						exit(0);
					}
				} else if (current_object->runtime_type == C_CLASS_DEFINE) {
					if (hash_has_key(((RuntimeClass*)current_object)->shared_properties, current_node_name)) {
						ret_rt_val = (RuntimeObject*) hash_get(((RuntimeClass*)current_object)->shared_properties, current_node_name);
						break;
					} else {
						raise_error("Cannot find specified property.", ast);
						exit(0);
					}
				} else if (current_object->runtime_type == C_NEW_OBJECT) {
					if (hash_has_key(((RuntimeObject*)(((RuntimeValue*)(current_object))->value.object))->data, current_node_name)) {
						RuntimeValue* property = hash_get(((RuntimeObject*)(((RuntimeValue*)(current_object))->value.object))->data, current_node_name);
						if (property->runtime_type == C_INT) {
							ret_rt_val = property;
						} else if (property->runtime_type == C_DECIMAL) {
							ret_rt_val = property;
						} else if (property->runtime_type == C_STRING) {
							ret_rt_val = property;
						} else if (property->runtime_type == C_LONG) {
							ret_rt_val = property;
						} else if (property->runtime_type == C_NEW_OBJECT) {
							current_object = property;
							ret_rt_val = property;
							i++;
							continue;
						} else {
							raise_error("System error!", ast);
							exit(0);
						}
						i++;
						break;
					} else {
						raise_error("Cannot accese an undefined element.", ast);
						exit(0);
					}
				} else {
					raise_error("error", ast);
					exit(0);
				}
			}
			
			if (ret_rt_val == NULL) {
				raise_error("Cannot locate given path.", ast);
				exit(0);
			}
			
			return ret_rt_val;
		}
	} else if (ast->node_type == NODE_TYPE_WHILE_STATEMENT) {
		WhileStatement* while_statement = (WhileStatement *)ast;
		while (runtime_as_integer(execute(while_statement->condition))) {
			RuntimeValue* loop_val = execute(while_statement->statements);
			if (loop_val != NULL) {
				if (loop_val->runtime_type == C_BREAK) {
					break;
				} else if (loop_val->runtime_type == C_CONTINUE) {
					continue;
				}
			}
		}
	} else if (ast->node_type == NODE_TYPE_DO_LOOP_STATEMENT) {
		DoLoopStatement* do_loop_statement = (DoLoopStatement *)ast;
		do {
			RuntimeValue* fin_val = execute(do_loop_statement->statements);
			if (fin_val != NULL) {
				if (fin_val->runtime_type == C_BREAK) {
					break;
				} else if (fin_val->runtime_type == C_CONTINUE) {
					continue;
				}
			}
		} while (runtime_as_integer(execute(do_loop_statement->condition)));
	} else if (ast->node_type == NODE_TYPE_FOR_STATEMENT) {
		ForStatement* for_statement = (ForStatement *) ast;
		if (hash_has_key(env->vars, ((Dimension*)(for_statement->dim))->var_name)) {
			for_statement->dim->node_type = NODE_TYPE_REASSIGN;
		}
		FunctionStackElement* peak = (FunctionStackElement*) stack_peak(env->call_stack);
		if (peak != NULL && hash_has_key(peak->local_vars, ((Dimension*)(for_statement->dim))->var_name)) {
			for_statement->dim->node_type = NODE_TYPE_REASSIGN;
		}
		RuntimeValue* start = execute(for_statement->dim);
		RuntimeValue* until = execute(for_statement->until);
		RuntimeValue* step = execute(for_statement->step);
		int is_decimal_compute;
		int i_val;
		double d_val;
		if (start->runtime_type == C_DECIMAL || until->runtime_type == C_DECIMAL || (step != NULL && step->runtime_type == C_DECIMAL)) {
			is_decimal_compute = 1;
			d_val = runtime_as_decimal(start);
			start->runtime_type = C_DECIMAL;
			start->value.d_val = d_val;
		} else if (start->runtime_type == C_INT && until->runtime_type == C_INT && (step != NULL &&step->runtime_type == C_INT)) {
			is_decimal_compute = 0;
			i_val = runtime_as_integer(start);
			start->runtime_type = C_INT;
			start->value.i_val = i_val;
		}
		while (is_decimal_compute ? (
				runtime_as_decimal(step) > 0 ? 
					d_val <= runtime_as_decimal(until) : d_val >= runtime_as_decimal(until)
			) :
			(
				runtime_as_integer(step) > 0 ?
					i_val <= runtime_as_integer(until) : i_val >= runtime_as_integer(until)
			)
		) {
			RuntimeValue* finally_return = execute(for_statement->statements);
			if (finally_return != NULL) {
				if (finally_return->runtime_type == C_BREAK) {
					break;
				} else if (finally_return->runtime_type == C_CONTINUE) {
					continue;
				}
			}
			if (is_decimal_compute) {
				d_val = d_val + runtime_as_decimal(step);
				start->value.d_val = d_val;
			} else {
				i_val = i_val + runtime_as_integer(step);
				start->value.i_val = i_val;
			}
		}
		//hash_delete(env->vars, ((Dimension*)(for_statement->dim))->var_name);
	} else if (ast->node_type == NODE_TYPE_RETURN) {
		if (stack_peak(env->call_stack) == NULL) {
			raise_error("Cannot return value on the top level.", ast);
			exit(0);
		}
		ReturnStatement* return_statement = (ReturnStatement*) ast;
		RuntimeValue* returned_value = execute(return_statement->return_expr);
		returned_value->is_return = 1;
		return returned_value;
	} else if (ast->node_type == NODE_TYPE_BREAK) {
		return make_runtime_break();
	} else if (ast->node_type == NODE_TYPE_CONTINUE) {
		return make_runtime_continue();
	} else if (ast->node_type == NODE_TYPE_EXIT_SUB) {
		if (stack_peak(env->call_stack) == NULL) {
			raise_error("Cannot exit subroutine with no subroutine running.", ast);
			exit(0);
		}
		RuntimeValue* returned_value = make_runtime_null();
		returned_value->runtime_type = C_EXITSUB;
		returned_value->is_return = 2;
		return returned_value;
	} else if (ast->node_type == NODE_TYPE_FUNCTION_ARG_DEFINE) {
		RuntimeValue* value = malloc(sizeof(RuntimeValue) + 1);
		value->runtime_type = C_LIST_BUFFER;
		// ListBuffer<RuntimeFunctionArg>
		value->value.list = flatten_function_args(ast);
		return value;
	} else if (ast->node_type == NODE_TYPE_LOCATE) {
		Location* location = (Location*) ast;
		char* node_name = location->name;
		RuntimeValue* rt_location = make_new_runtime_list_buffer();
		list_buffer_add(rt_location->value.list, node_name);
		if (location->next_node != NULL) {
			RuntimeValue* next_node_rt_value = execute(location->next_node);
			list_buffer_concat(rt_location->value.list, next_node_rt_value->value.list);
			return rt_location;
		} else {
			return rt_location;
		}
	} else if (ast->node_type == NODE_TYPE_NAMESPACE) {
		// RuntimeValue<ListBuffer>
		RuntimeValue* rt_location = execute(ast->left_node/* location */);
		for (int i = 0; i < rt_location->value.list->count; i++) {
			char* node_name = list_buffer_get(rt_location->value.list, i);
			if (env->current_namespace == NULL) {	// first loop, at the top level
				if (hash_has_key(env->root_namespace->next_level, node_name)) {
					RuntimeNamespace* name_space = (RuntimeNamespace*) hash_get(env->root_namespace->next_level, node_name);
					if (name_space->runtime_type == C_NAMESPACE) {
						env->current_namespace = name_space;
					} else {
						raise_error("A class has been defined and conflict with this domain, please rename the namespace.", ast);
						exit(0);
					}
				} else {
					RuntimeNamespace* new_namespace = malloc(sizeof(RuntimeNamespace) + 1);
					new_namespace->runtime_type = C_NAMESPACE;
					new_namespace->name = malloc(sizeof(char) * strlen(node_name) + 1);
					strcpy(new_namespace->name, node_name);
					new_namespace->next_level = hash_init();
					hash_put(env->root_namespace->next_level, node_name, new_namespace);
					env->current_namespace = new_namespace;
				}
			} else {
				if (!hash_has_key(env->current_namespace->next_level, node_name)) {
					RuntimeNamespace* new_namespace = malloc(sizeof(RuntimeNamespace) + 1);
					new_namespace->runtime_type = C_NAMESPACE;
					new_namespace->name = malloc(sizeof(char) * strlen(node_name) + 1);
					strcpy(new_namespace->name, node_name);
					new_namespace->next_level = hash_init();
					
					hash_put(env->current_namespace->next_level, node_name, new_namespace);
					env->current_namespace = new_namespace;
				} else {
					RuntimeNamespace* name_space = (RuntimeNamespace*) hash_get(env->current_namespace->next_level, node_name);
					if (name_space->runtime_type == C_NAMESPACE) {
					 	env->current_namespace = name_space;
					} else {
						raise_error("A class has been defined and conflict with this domain, please rename the namespace.", ast);
						exit(0);
					}
				}
			}
		}
	} else if (ast->node_type == NODE_TYPE_IMPORT) {
		
	} else if (ast->node_type == NODE_TYPE_CLASS) {
		ClassDefinition* class_definition = (ClassDefinition*) ast;
		
		RuntimeClass* runtime_class = (RuntimeClass*) malloc(sizeof(RuntimeClass) + 1);
		runtime_class->runtime_type = C_CLASS_DEFINE;
		runtime_class->name = malloc(sizeof(char) * strlen(class_definition->class_name) + 1);
		strcpy(runtime_class->name, class_definition->class_name);
		
		runtime_class->properties = NULL;
		runtime_class->shared_properties = NULL;
		runtime_class->methods = NULL;
		runtime_class->shared_methods = NULL;
		
		env->current_building_class = runtime_class;
		
		init_current_building_runtime_class();
		
		execute(class_definition->class_attributes);
		execute(class_definition->class_body);
		
		hash_put(env->current_namespace->next_level, class_definition->class_name, runtime_class);
		
		env->current_building_class = NULL;
	} else if (ast->node_type == NODE_TYPE_CLASS_ATTR) {
		if (env->current_building_class == NULL) {
			raise_error("System error.10", ast);
			exit(0);
		}
		ClassAttrDefinition* cls_attr_definition = (ClassAttrDefinition*) ast;
		if (cls_attr_definition->extend_class != NULL) {
			env->current_building_class->super_class = malloc(sizeof(char) * strlen(cls_attr_definition->extend_class) + 1);
			strcpy(env->current_building_class->super_class, cls_attr_definition->extend_class);
		}
	} else if (ast->node_type == NODE_TYPE_METHOD) {
		if (env->current_building_class == NULL) {
			raise_error("Syntax error when defining method of class.", ast);
			exit(0);
		}
		ClassMethod* class_method = (ClassMethod*) ast;
		RuntimeValue* function = execute(class_method->function_define);
		if (function->runtime_type != C_FUNCTION_DEFINE && function->runtime_type != C_SUB) {
			raise_error("Syntax error when defining method of class.", ast);
			exit(0);
		}
		function->value.function->access_modifier = class_method->access_modifier;
		function->value.function->is_static = class_method->is_static;
		if (class_method->is_static == 0) {
			if (env->current_building_class->runtime_type == C_TRAIT) {
				hash_put(env->current_building_class->methods, function->value.function->name, function->value.function);
			} else if (env->current_building_class->runtime_type == C_CLASS_DEFINE) {
				hash_put(env->current_building_class->methods, function->value.function->name, function->value.function);
			}
		} else {
			if (env->current_building_class->runtime_type == C_TRAIT) {
				hash_put(env->current_building_class->shared_methods, function->value.function->name, function->value.function);
			} else if (env->current_building_class->runtime_type == C_CLASS_DEFINE) {
				hash_put(env->current_building_class->shared_methods, function->value.function->name, function->value.function);
			}
		}
	} else if (ast->node_type == NODE_TYPE_PROPERTY) {
		if (env->current_building_class == NULL) {
			raise_error("Syntax error when defining method of class.", ast);
			exit(0);
		}
		ClassProperty* class_property = (ClassProperty*) ast;
		Dimension* dimension = (Dimension*)class_property->dim;
		dimension->node_type = NODE_TYPE_PROPERTY_DIM;
		dimension->shared = class_property->is_static;
		RuntimeValue* result_property = execute(class_property->dim);
		class_property->access_modifier = class_property->access_modifier;
		class_property->is_static = class_property->is_static;
		/*if (class_property->is_static == 1) {
			hash_put(env->current_building_class->shared_properties, dimension->var_name, result_property);
		} else {
			hash_put(env->current_building_class->properties, dimension->var_name, result_property);
		}*/
	} else if (ast->node_type == NODE_TYPE_PROPERTY_DIM) {
		Dimension* dim = (Dimension*) ast;
		if (hash_has_key(env->current_building_class->properties, dim->var_name) && dim->shared == 0) {
			raise_error("The property of the class has been defined. Please rename it!", ast);
			exit(0);
		}
		if (hash_has_key(env->current_building_class->shared_properties, dim->var_name) && dim->shared == 1) {
			raise_error("The shared property of the class has been defined. Please rename it!", ast);
			exit(0);
		}
		if (dim->node != NULL) {
			RuntimeValue* value = execute(dim->node);
			if (dim->shared == 1) {
				hash_put(env->current_building_class->shared_properties, dim->var_name, value);
			} else {
				hash_put(env->current_building_class->properties, dim->var_name, value);
			}
		} else {
			if (dim->shared == 1) {
				hash_put(env->current_building_class->shared_properties, dim->var_name, make_runtime_null());
			} else {
				hash_put(env->current_building_class->properties, dim->var_name, make_runtime_null());
			}
		}
		if (dim->next_dim != NULL) {
			dim->next_dim->node_type = NODE_TYPE_PROPERTY_DIM;
			execute(dim->next_dim);
		}
	} else if (ast->node_type == NODE_TYPE_TRAIT_NAME) {
		// trait use
		TraitImplementDefinition* trait_use = (TraitImplementDefinition*) ast;
		LinkedList* list_traits = list_init();
		TraitImplementDefinition* tmp_p = trait_use;
		do {
			list_add(list_traits, tmp_p->trait_name);
			tmp_p = tmp_p->next_node;
		} while (tmp_p != NULL);
		env->current_building_class->traits = list_traits;
	} else if (ast->node_type == NODE_TYPE_CLASS_INNER_STATEMENT) {
		if (ast->left_node == NULL) {
			return NULL;
		}
		AST* current_node = ast;
		while (current_node != NULL) {
			execute(current_node->left_node);
			current_node = current_node->right_node;
		}
		return NULL;
	} else if (ast->node_type == NODE_TYPE_TRAIT) {
		TraitDefinition* trait_definition = (TraitDefinition*) ast;
		
		RuntimeClass* runtime_trait = (RuntimeClass*) malloc(sizeof(RuntimeClass) + 1);
		runtime_trait->runtime_type = C_TRAIT;
		runtime_trait->name = malloc(sizeof(char) * strlen(trait_definition->trait_name) + 1);
		strcpy(runtime_trait->name, trait_definition->trait_name);
		
		env->current_building_class = (RuntimeClass*)runtime_trait;
		init_current_building_runtime_class();
		
		if (trait_definition->trait_implement != NULL) {
			execute(trait_definition->trait_implement);
		}
		if (trait_definition->trait_body != NULL) {
			execute(trait_definition->trait_body);
		}
		
		hash_put(env->current_namespace->next_level, trait_definition->trait_name, runtime_trait);
	} else if (ast->node_type == NODE_TYPE_TRAIT_INNER_STATEMENT) {
		if (ast->left_node == NULL) {
			return NULL;
		}
		AST* current_node = ast;
		while (current_node != NULL) {
			execute(current_node->left_node);
			current_node = current_node->right_node;
		}
		return NULL;
	} else if (ast->node_type == NODE_TYPE_NEW_OBJECT) {
		NewObject* object = (NewObject*) ast;
	
		RuntimeValue* new_val = malloc(sizeof(RuntimeValue) + 1);
		new_val->runtime_type = C_NEW_OBJECT;
		
		RuntimeValue* location_list = execute(object->class_location);
		ListBuffer* loc_list = location_list->value.list;
		
		RuntimeObject* new_object = malloc(sizeof(RuntimeObject) + 1);
		new_object->runtime_type = C_NEW_OBJECT;
		
		char* class_name = list_buffer_get(loc_list, loc_list->count - 1);
		
		new_object->class_name = malloc(sizeof(char) * strlen(class_name) + 1);
		strcpy(new_object->class_name, class_name);
		
		new_object->path = loc_list;
		new_object->in_namespace = env->current_namespace;
		new_object->p_runtime_class = find_as_runtime_class(loc_list, ast);
		if (new_object->p_runtime_class->properties == NULL) {
			raise_error("System error of properties table didn't correctly initialized.", ast);
			exit(0);
		}
		
		RuntimeValue* me_wrapper = (RuntimeValue*) malloc(sizeof(RuntimeValue) + 1);
		me_wrapper->runtime_type = C_NEW_OBJECT;
		me_wrapper->value.object = (RuntimeValue*) new_object;
		me_wrapper->is_return = 0;
		
		new_object->data = (HashTable*) hash_init();
		
		hash_put(new_object->data, "Me", me_wrapper);
		
		// instantiate a new object.
		
		HashTable* properties_table = new_object->p_runtime_class->properties;
		
		/*
		for (int j = 0; j < properties_table->pool_size; j++) {
			LinkedList* list = properties_table->container[j];
			if (list->count > 0) {
				LinkedListNode* pointer_node = list->head;
				for (int i = 0; i <= list->count; i++) {
					if (i > 0) {
						char* key = (char*)(((HashTableNode *) (pointer_node->value))->key);
						hash_put(new_object->data, key, (RuntimeValue*)((HashTableNode *) (pointer_node->value))->value);
					}
					pointer_node = pointer_node->next_node;
				}
			}
		}
		*/
		
		unsigned int property_table_keys_count;
		void** property_table_keys = hash_keys(properties_table, &property_table_keys_count);
		
		for (int i = 0; i < property_table_keys_count; i++) {
			char* key = (char*)property_table_keys[i];
			hash_put(new_object->data, key, (RuntimeValue*)hash_get(properties_table, key));
		}
		
		
		// instantiate the properties of parent class and traits.
		
		// to do
		
		new_val->value.object = new_object;
		
		// Execute constructor
		
		stack_push(env->call_stack, dup_new_func_stack_element_object(new_val));
		
		if (hash_has_key(new_object->p_runtime_class->methods, "New")) {
		
			RuntimeFunction* init_method = (RuntimeFunction*) hash_get(new_object->p_runtime_class->methods, "New");
			RuntimeValue* argument_list = execute(object->arguments);
			
			if (init_method->arguments->count > 0) {
				execute_function_header(init_method->arguments, argument_list->value.list, ast);
			}
			RuntimeValue* return_value = execute(init_method->statements);
			FunctionStackElement* popped_env = (FunctionStackElement*) stack_pop(env->call_stack);
			hash_free(popped_env->local_vars);
			free(popped_env);
			if (init_method->runtime_type == C_FUNCTION_DEFINE) {
				raise_error("Constructor must be a subroutine.", ast);
				exit(0);
			} else if (return_value != NULL && return_value->runtime_type != C_EXITSUB) {
				raise_error("Subroutine cannot return a value.", ast);
				exit(0);
			}
		
		}
		
		// *************************
		
		new_val->is_return = 0;
		
		return new_val;
	} else if (ast->node_type == NODE_TYPE_ACCESSOR) {
		Accessor* accessor = (Accessor*) ast;
		RuntimeValue* rt_left = execute(accessor->left_node);
		
		if (rt_left->runtime_type == C_NEW_OBJECT) {
			if (hash_has_key(rt_left->value.object->p_runtime_class->properties, accessor->attribute_name)) {
				if (hash_has_key(rt_left->value.object->data, accessor->attribute_name)) {
					return (RuntimeValue*) hash_get(rt_left->value.object->data, accessor->attribute_name);
				} else {
					return make_runtime_null();
				}
			} else if (hash_has_key(rt_left->value.object->p_runtime_class->methods, accessor->attribute_name)) {
				stack_push(env->call_stack, dup_new_func_stack_element_object(rt_left)); // to do  ->value.object
				
				RuntimeFunction* method = (RuntimeFunction*) hash_get(rt_left->value.object->p_runtime_class->methods, accessor->attribute_name);
				RuntimeValue* argument_list = execute(accessor->arguments);
				
				if (method->arguments->count > 0) {
					execute_function_header(method->arguments, argument_list->value.list, ast);
				}
				RuntimeValue* return_value = execute(method->statements);
				FunctionStackElement* popped_env = (FunctionStackElement*) stack_pop(env->call_stack);
				hash_free(popped_env->local_vars);
				free(popped_env);
				if (method->runtime_type == C_FUNCTION_DEFINE) {
					if (return_value == NULL) {
						raise_error("Function must return a value.", ast);
						exit(0);
					}
					return return_value;
				} else if (return_value != NULL && return_value->runtime_type != C_EXITSUB) {
					raise_error("Subroutine cannot return a value.", ast);
					exit(0);
				}
			} else {
				raise_error("Undefined element of object.", ast);
				exit(0);
			}
		} else if (rt_left->runtime_type == C_CLASS_DEFINE) {
			char* right_node_name = accessor->attribute_name;
			RuntimeClass* this_class = rt_left->value.klass;
			if (hash_has_key(this_class->shared_properties, right_node_name)) {
				RuntimeValue* shared_property = hash_get(this_class->shared_properties, right_node_name);
				return shared_property;
			} else if (hash_has_key(this_class->shared_methods, right_node_name)) {
				stack_push(env->call_stack, dup_new_func_stack_element_class(this_class));
				
				RuntimeFunction* method = (RuntimeFunction*) hash_get(this_class->shared_methods, accessor->attribute_name);
				RuntimeValue* argument_list = execute(accessor->arguments);
				
				if (method->arguments->count > 0) {
					execute_function_header(method->arguments, argument_list->value.list, ast);
				}
				RuntimeValue* return_value = execute(method->statements);
				FunctionStackElement* popped_env = (FunctionStackElement*) stack_pop(env->call_stack);
				hash_free(popped_env->local_vars);
				free(popped_env);
				if (method->runtime_type == C_FUNCTION_DEFINE) {
					if (return_value == NULL) {
						raise_error("Function must return a value.", ast);
						exit(0);
					}
					return return_value;
				} else if (return_value != NULL && return_value->runtime_type != C_EXITSUB) {
					raise_error("Subroutine cannot return a value.", ast);
					exit(0);
				}
			} else {
				raise_error("Cannot find the property of the namespace.", ast);
				exit(0);
			}
		} else if (rt_left->runtime_type == C_NAMESPACE) {
			char* right_node_name = accessor->attribute_name;
			RuntimeNamespace* this_namespace = rt_left->value.domain_namespace;
			if (hash_has_key(this_namespace->next_level, right_node_name)) {
				RuntimeClass* rt = hash_get(this_namespace->next_level, right_node_name);
				if (rt->runtime_type == C_NAMESPACE) {
					RuntimeValue* rt_value = malloc(sizeof(RuntimeValue) + 1);
					rt_value->runtime_type = C_NAMESPACE;
					rt_value->value.domain_namespace = (RuntimeNamespace*) rt;
					rt_value->is_return = 0;
					return rt_value;
				} else if (rt->runtime_type == C_CLASS_DEFINE) {
					RuntimeValue* rt_value = malloc(sizeof(RuntimeValue) + 1);
					rt_value->runtime_type = C_CLASS_DEFINE;
					rt_value->value.klass = (RuntimeClass*) rt;
					rt_value->is_return = 0;
					return rt;
				} else if (rt->runtime_type == C_NEW_OBJECT) {
					RuntimeValue* rt_value = malloc(sizeof(RuntimeValue) + 1);
					rt_value->runtime_type = C_NEW_OBJECT;
					rt_value->value.object = (RuntimeObject*) rt;
					rt_value->is_return = 0;
					return rt;
				} else {
					return (RuntimeValue*) rt;
				}
			} else {
				raise_error("Cannot find the property of the namespace.", ast);
				exit(0);
			}
		} else if (rt_left->runtime_type == C_STRING) {
			raise_error("String not implemented.", ast);
			exit(0);
		} else {
			raise_error("The dot symbol must be put on the right of an instantiated object, namespace or class.", ast);
			exit(0);
		}
	}
	return NULL;
}

AST* make_function_arg(char* name, AST* default_value, AST* next_node, int line_no) {
	FunctionArgumentDefinition* new_block = (FunctionArgumentDefinition*) malloc(sizeof(FunctionArgumentDefinition) + 1);
	new_block->node_type = NODE_TYPE_FUNCTION_ARG_DEFINE;
	new_block->arg_name = (char*)malloc(sizeof(char)*strlen(name) + 1);
	new_block->line_no = line_no;
	strcpy(new_block->arg_name, name);
	new_block->default_value = default_value;
	new_block->next_node = next_node;
	return (AST*) new_block;
}

void execute_function_header(ListBuffer* definitions, ListBuffer* values, AST* ast) {
	FunctionStackElement* peak = (FunctionStackElement*) stack_peak(env->call_stack);
	if (peak == NULL) {
		raise_error("System error.1", ast);
		exit(0);
	}
	if (definitions->count != values->count) {
		raise_error("The arguments are not matched in the function.", ast);
		exit(0);
	}
	for (int i = 0; i < definitions->count; i++) {
		RuntimeFunctionArg* arg = (RuntimeFunctionArg*) list_buffer_get(definitions, i);
		if (arg->default_value == NULL) {
			hash_put(peak->local_vars, arg->name, list_buffer_get(values, i));
		} else {
			hash_put(peak->local_vars, arg->name, arg->default_value);
		}
	}
}

// ListBuffer<RuntimeValue>
ListBuffer* integrate_params(AST* node) {
	ListBuffer* result_list = list_buffer_init();
	AST* current_node = node;
	while (current_node != NULL) {
		if (current_node->left_node != NULL) {
			list_buffer_add(result_list, execute(current_node->left_node));
		}
		current_node = current_node->right_node;
	}
	return result_list;
}

// ListBuffer<RuntimeFunctionArg>
ListBuffer* flatten_function_args(AST* node) {
	FunctionArgumentDefinition* arg = (FunctionArgumentDefinition*) node;
	ListBuffer* result_list = list_buffer_init();
	FunctionArgumentDefinition* tmp_arg = arg;
	RuntimeFunctionArg* runtime_arg;
	while (tmp_arg != NULL) {
		runtime_arg = (RuntimeFunctionArg*) malloc(sizeof(RuntimeFunctionArg) + 1);
		runtime_arg->name = (char*)malloc(sizeof(char)*strlen(tmp_arg->arg_name) + 1);
		strcpy(runtime_arg->name, tmp_arg->arg_name);
		runtime_arg->default_value = NULL;
		list_buffer_add(result_list, (void *)runtime_arg);
		tmp_arg = tmp_arg->next_node;
	}
	
	return result_list;
}

AST* make_while_expression(AST* condition, AST* statements, int line_no) {
	WhileStatement* new_val = malloc(sizeof(WhileStatement) + 1);
	new_val->node_type = NODE_TYPE_WHILE_STATEMENT;
	new_val->line_no = line_no;
	new_val->condition = condition;
	new_val->statements = statements;
	return (AST*) new_val;
}

AST* make_do_loop_expression(AST* statements, AST* condition, int line_no) {
	DoLoopStatement* new_val = malloc(sizeof(DoLoopStatement) + 1);
	new_val->node_type = NODE_TYPE_DO_LOOP_STATEMENT;
	new_val->line_no = line_no;
	new_val->condition = condition;
	new_val->statements = statements;
	return (AST*) new_val;
}

void raise_error(const char *message, const AST* ast) {
	fprintf(stderr, "Syntax error at line %d: %s\n", ast->line_no, message);
	fflush(stderr);
	return 0;
}
