#include "Runtime.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HashTable.h"
#include "ListBuffer.h"

Var* make_var(AST* location, int line_no) {
	Var* new_val = malloc(sizeof(Var) + 1);
	new_val->node_type = NODE_TYPE_VAR;
	new_val->line_no = line_no;
	/*
	new_val->name = (char*) malloc(sizeof(char) * strlen(name) + 1);
	strcpy(new_val->name, name);
	*/
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
	new_val->i_val = i_val;
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
	new_val->d_val = d_val;
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
	new_val->s_val = malloc(sizeof(char) * strlen(s_val) + 1);
	strcpy(new_val->s_val, s_val);
	return new_val;
}

RuntimeValue* make_runtime_integer(int i_val) {
	RuntimeValue* new_val = (RuntimeValue*) malloc(sizeof(RuntimeValue) + 1);
	if (new_val == NULL) {
		printf("System error");
		exit(0);
	}
	new_val->runtime_type = C_INT;
	new_val->i_val = i_val;
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
	new_val->d_val = d_val;
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
	new_val->s_val = (char *) malloc(sizeof(char) * strlen(s_val) + 1);
	strcpy(new_val->s_val, s_val);
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
		return constant->i_val;
	} else if (constant->type == C_DECIMAL) {
		return (int)(constant->d_val);
	}
}

double dump_numeric_as_decimal(Constant* constant) {
	if (constant->type == C_INT) {
		return (double)(constant->i_val);
	} else if (constant->type == C_DECIMAL) {
		return constant->d_val;
	}
}

int runtime_as_integer(RuntimeValue* value) {
	if (value->runtime_type == C_INT) {
		return value->i_val;
	} else if (value->runtime_type == C_DECIMAL) {
		return (int)(value->d_val);
	}
}

double runtime_as_decimal(RuntimeValue* value) {
	if (value->runtime_type == C_INT) {
		return (double)(value->i_val);
	} else if (value->runtime_type == C_DECIMAL) {
		return value->d_val;
	}
}

Dimension* var_make_null(char* var_name, int line_no) {
	Dimension* new_block = (Dimension*) malloc(sizeof(Dimension) + 1);
	new_block->node_type = NODE_TYPE_ASSIGN_VAR;
	new_block->line_no = line_no;
	//new_block->runtime_type = "null";
	new_block->var_name = var_name;
	new_block->next_dim = NULL;
	return new_block;
}

AST* make_dim(char* var_name, AST* node, AST* next_dim, int line_no) {
	Dimension* new_block = (Dimension*) malloc(sizeof(Dimension) + 1);
	new_block->node_type = NODE_TYPE_ASSIGN_VAR;
	new_block->line_no = line_no;
	new_block->var_name = var_name;
	new_block->node = node;
	new_block->next_dim = next_dim;
	return (AST*) new_block;
}

AST* make_redim(char* var_name, AST* node, int line_no) {
	Dimension* new_block = (Dimension*) malloc(sizeof(Dimension) + 1);
	new_block->node_type = NODE_TYPE_REASSIGN;
	new_block->line_no = line_no;
	new_block->var_name = var_name;
	new_block->node = node;
	new_block->next_dim = NULL;
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
	new_val->node_type = NODE_TYPE_FUNC;
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

AST* make_class_attr(char* extend_class, AST* trait_implement, int line_no) {
	ClassAttrDefinition* new_val = malloc(sizeof(ClassAttrDefinition) + 1);
	new_val->node_type = NODE_TYPE_CLASS_ATTR;
	new_val->line_no = line_no;
	new_val->extend_class = malloc(sizeof(char) * strlen(extend_class) + 1);
	strcpy(new_val->extend_class, extend_class);
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
	new_table = hash_init(var_env_pt->pool_size);
	new_val->local_vars = new_table;
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
		yyerror("System error.");
		exit(0);
	}
	new_val->node_type = NODE_TYPE_BREAK;
	new_val->line_no = line_no;
	return (AST*) new_val;
}

AST* make_continue(int line_no) {
	ContinueStatement* new_val = malloc(sizeof(ContinueStatement) + 1);
	if (new_val == NULL) {
		yyerror("System error.");
		exit(0);
	}
	new_val->node_type = NODE_TYPE_CONTINUE;
	new_val->line_no = line_no;
	return (AST*) new_val;
}

RuntimeValue* make_new_runtime_list_buffer() {
	RuntimeValue* new_val = (RuntimeValue*) malloc(sizeof(RuntimeValue) + 1);
	new_val->runtime_type = C_LIST_BUFFER;
	new_val->list = list_buffer_init();
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
				func->arguments = args_list->list;
			}
		} else {
			func->arguments = list_buffer_init();
		}
		if (hash_has_key(env->functions, func->name)) {
			raise_error("You cannot redefine a function or subroutine.", ast);
			exit(0);
		}
		hash_put(env->functions, func->name, (void*) func);
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
	} else if (ast->node_type == NODE_TYPE_CONSTANT) {
		RuntimeValue* value = malloc(sizeof(RuntimeValue) + 1);
		value->runtime_type = ((Constant *)ast)->type;
		value->i_val = ((Constant *)ast)->i_val;
		value->d_val = ((Constant *)ast)->d_val;
		value->s_val = ((Constant *)ast)->s_val;
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
			return make_runtime_integer(!strcmp(l->s_val, r->s_val));
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
			return make_runtime_string(strcat(l->s_val, r->s_val));
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
	} else if (ast->node_type == NODE_TYPE_FUNC) {
		// Function call
		FunctionStatement* functional = (FunctionStatement *)ast;
		RuntimeValue* argument_list = execute(functional->expr_list);
		RuntimeValue* location_rt_value = execute(functional->location);
//		list_buffer_dump(location_rt_value->list);
		char* first_function_name = (char*) list_buffer_get(location_rt_value->list, 0);
		if (!strcmp(first_function_name, "Print")) {
			RuntimeValue* param1 = list_buffer_get(argument_list->list, 0);
			if (param1->runtime_type == C_STRING) {
				printf("%s", param1->s_val);
			} else if (param1->runtime_type == C_INT) {
				printf("%d", param1->i_val);
			} else if (param1->runtime_type == C_DECIMAL) {
				printf("%f", param1->d_val);
			} else if (param1->runtime_type == C_NULL) {
				printf("NULL");
			} else {
				printf("System error.\n");
			}
			return NULL;
		} else if (!strcmp(first_function_name, "PrintLine")) {
			RuntimeValue* param1 = list_buffer_get(argument_list->list, 0);
			if (param1->runtime_type == C_STRING) {
				printf("%s\n", param1->s_val);
			} else if (param1->runtime_type == C_INT) {
				printf("%d\n", param1->i_val);
			} else if (param1->runtime_type == C_DECIMAL) {
				printf("%f\n", param1->d_val);
			} else if (param1->runtime_type == C_NULL) {
				printf("NULL\n");
			} else {
				printf("System error.\n");
			}
			return NULL;
		} else {
		
			stack_push(env->call_stack, dup_new_func_stack_element());
			RuntimeFunction* current_runtime_function =
				(RuntimeFunction*) hash_get(env->functions, (char*)list_buffer_get(location_rt_value->list, 0));
			
			if (current_runtime_function == NULL) {
				raise_error("The function/subroutine/method which you're finding does not defined.", ast);
				exit(0);
			}
			if (current_runtime_function->arguments->count > 0) {
				execute_function_header(current_runtime_function->arguments, argument_list->list, ast);
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
				return return_value;
			} else if (return_value != NULL && return_value->runtime_type != C_EXITSUB) {
				raise_error("Subroutine cannot return a value.", ast);
				exit(0);
			}
		}
	} else if (ast->node_type == NODE_TYPE_EXPR_ITEM) {
		RuntimeValue* value = malloc(sizeof(RuntimeValue) + 1);
		value->runtime_type = C_LIST_BUFFER;
		value->list = integrate_params(ast);
		return value;
	} else if (ast->node_type == NODE_TYPE_VAR) {
		Var* var = (Var*)ast;
		
		FunctionStackElement* peak = (FunctionStackElement*) stack_peak(env->call_stack);
		HashTable* from_env;
		if (peak == NULL) {
			from_env = env->vars;
		} else {
			from_env = peak->local_vars;
		}
		
		RuntimeValue* rt_location = execute(var->location);
		
		if (rt_location->list->count == 1) {
			RuntimeValue* fetched_var = (RuntimeValue*) hash_get(from_env, (char*)list_buffer_get(rt_location->list, 0));
			if (peak != NULL && fetched_var == NULL) {
				fetched_var = (RuntimeValue*) hash_get(env->vars, (char*)list_buffer_get(rt_location->list, 0));
			}
			if (fetched_var != NULL) {
				return fetched_var;
			} else {
				raise_error("Undefined variable.", ast);
				exit(0);
			}
		} else {
			// for static property in class
			raise_error("Not implemented.", ast);
			exit(0);
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
			start->d_val = d_val;
		} else if (start->runtime_type == C_INT && until->runtime_type == C_INT && (step != NULL &&step->runtime_type == C_INT)) {
			is_decimal_compute = 0;
			i_val = runtime_as_integer(start);
			start->runtime_type = C_INT;
			start->i_val = i_val;
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
				start->d_val = d_val;
			} else {
				i_val = i_val + runtime_as_integer(step);
				start->i_val = i_val;
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
		value->list = flatten_function_args(ast);
		return value;
	} else if (ast->node_type == NODE_TYPE_LOCATE) {
		Location* location = (Location*) ast;
		char* node_name = location->name;
		RuntimeValue* rt_location = make_new_runtime_list_buffer();
		list_buffer_add(rt_location->list, node_name);
		if (location->next_node != NULL) {
			RuntimeValue* next_node_rt_value = execute(location->next_node);
			list_buffer_concat(rt_location->list, next_node_rt_value->list);
			return rt_location;
		} else {
			return rt_location;
		}
	} else if (ast->node_type == NODE_TYPE_NAMESPACE) {
		// RuntimeValue<ListBuffer>
		RuntimeValue* rt_location = execute(ast->left_node/* location */);
		for (int i = 0; i < rt_location->list->count; i++) {
			char* node_name = list_buffer_get(rt_location->list, i);
			if (env->current_namespace == NULL) {	// first loop, at the top level
				if (!hash_has_key(env->classes, node_name)) {
					RuntimeNamespace* new_namespace = malloc(sizeof(RuntimeNamespace) + 1);
					new_namespace->runtime_type = C_NAMESPACE;
					new_namespace->name = malloc(sizeof(char) * strlen(node_name) + 1);
					strcpy(new_namespace->name, node_name);
					new_namespace->next_level = hash_init(120);
					hash_put(env->classes, node_name, (void*) new_namespace);
					env->current_namespace = new_namespace;
				} else {
					env->current_namespace = (RuntimeNamespace*) hash_get(env->classes, node_name);
				}
			} else {
				if (!hash_has_key(env->current_namespace->next_level, node_name)) {
					RuntimeNamespace* new_namespace = malloc(sizeof(RuntimeNamespace) + 1);
					new_namespace->runtime_type = C_NAMESPACE;
					new_namespace->name = malloc(sizeof(char) * strlen(node_name) + 1);
					strcpy(new_namespace->name, node_name);
					new_namespace->next_level = hash_init(120);
					
					hash_put(env->current_namespace->next_level, node_name, (void*) new_namespace);
					env->current_namespace = new_namespace;
				} else {
					env->current_namespace = (RuntimeNamespace*) hash_get(env->current_namespace->next_level, node_name);
				}
			}
		}
	} else if (ast->node_type == NODE_TYPE_CLASS) {
		ClassDefinition* class_definition = (ClassDefinition*) ast;
		
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
		raise_error("System error.", ast);
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
