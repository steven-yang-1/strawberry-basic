#include "Runtime.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HashTable.h"
#include "ListBuffer.h"

Var* make_var(char* name) {
	Var* new_val = malloc(sizeof(Var) + 1);
	new_val->node_type = NODE_TYPE_VAR;
	new_val->name = (char*) malloc(sizeof(char) * strlen(name) + 1);
	strcpy(new_val->name, name);
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
	return new_val;
}

RuntimeValue* make_runtime_null() {
	RuntimeValue* new_val = (RuntimeValue*) malloc(sizeof(RuntimeValue) + 1);
	if (new_val == NULL) {
		printf("System error");
		exit(0);
	}
	new_val->runtime_type = C_NULL;
	return new_val;
}

RuntimeValue* make_runtime_break() {
	RuntimeValue* new_val = (RuntimeValue*) malloc(sizeof(RuntimeValue) + 1);
	if (new_val == NULL) {
		printf("System error");
		exit(0);
	}
	new_val->runtime_type = C_BREAK;
	return new_val;
}

RuntimeValue* make_runtime_continue() {
	RuntimeValue* new_val = (RuntimeValue*) malloc(sizeof(RuntimeValue) + 1);
	if (new_val == NULL) {
		printf("System error");
		exit(0);
	}
	new_val->runtime_type = C_CONTINUE;
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

Dimension* var_make_null(char* var_name) {
	Dimension* new_block = (Dimension*) malloc(sizeof(Dimension) + 1);
	new_block->node_type = NODE_TYPE_ASSIGN_VAR;
	//new_block->runtime_type = "null";
	new_block->var_name = var_name;
	new_block->next_dim = NULL;
	return new_block;
}

AST* make_dim(char* var_name, AST* node, AST* next_dim) {
	Dimension* new_block = (Dimension*) malloc(sizeof(Dimension) + 1);
	new_block->node_type = NODE_TYPE_ASSIGN_VAR;
	new_block->var_name = var_name;
	new_block->node = node;
	new_block->next_dim = next_dim;
	return (AST*) new_block;
}

AST* make_redim(char* var_name, AST* node) {
	Dimension* new_block = (Dimension*) malloc(sizeof(Dimension) + 1);
	new_block->node_type = NODE_TYPE_REASSIGN;
	new_block->var_name = var_name;
	new_block->node = node;
	return (AST*) new_block;
}

AST* make_ast(int node_type, AST* left_node, AST* right_node) {
	AST* new_val = malloc(sizeof(AST) + 1);
	new_val->node_type = node_type;
	new_val->left_node = left_node;
	new_val->right_node = right_node;
	return new_val;
}

AST* make_if_expression(AST* condition, AST* if_statement, AST* else_if_statement, AST* else_statement) {
	IfStatement* new_val = malloc(sizeof(IfStatement) + 1);
	new_val->node_type = NODE_TYPE_IF_STATEMENT;
	new_val->condition = condition;
	new_val->if_statement = if_statement;
	new_val->else_if_statement = else_if_statement;
	new_val->else_statement = else_statement;
	return (AST*) new_val;
}

AST* make_function_call(char* name, AST* expr_list) {
	FunctionStatement* new_val = malloc(sizeof(FunctionStatement) + 1);
	new_val->node_type = NODE_TYPE_FUNC;
	new_val->name = malloc(sizeof(char) * strlen(name) + 1);
	strcpy(new_val->name, name);
	new_val->expr_list = expr_list;
	return (AST*) new_val;
}

AST* make_for_expression(AST* dim, AST* until, AST* step, AST* statements) {
	ForStatement* new_val = malloc(sizeof(ForStatement) + 1);
	new_val->node_type = NODE_TYPE_FOR_STATEMENT;
	new_val->dim = dim;
	new_val->until = until;
	new_val->step = step;
	new_val->statements = statements;
	return (AST*) new_val;
}

AST* make_function_define(char* name, AST* statements, AST* arguments) {
	FunctionDefineStatement* new_val = malloc(sizeof(FunctionDefineStatement) + 1);
	new_val->name = malloc(sizeof(char) * strlen(name) + 1);
	strcpy(new_val->name, name);
	new_val->node_type = NODE_TYPE_FUNCTION_DEFINE;
	new_val->statements = statements;
	new_val->arguments = arguments;
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
	for (int i = 0; i < var_env_pt->count; i++) {
		hash_put(
			new_table,
			(char*)list_buffer_get(var_env_pt->name_list, i),
			hash_get(var_env_pt, (char*)list_buffer_get(var_env_pt->name_list, i))
		);
	}
	new_val->local_vars = new_table;
	return new_val;
}

AST* make_return(AST* expr) {
	ReturnStatement* new_val = malloc(sizeof(ReturnStatement) + 1);
	new_val->node_type = NODE_TYPE_RETURN;
	new_val->return_expr = expr;
	return (AST*) new_val;
}

AST* make_break() {
	BreakStatement* new_val = malloc(sizeof(BreakStatement) + 1);
	if (new_val == NULL) {
		yyerror("System error.");
		exit(0);
	}
	new_val->node_type = NODE_TYPE_BREAK;
	return (AST*) new_val;
}

AST* make_continue() {
	ContinueStatement* new_val = malloc(sizeof(ContinueStatement) + 1);
	if (new_val == NULL) {
		yyerror("System error.");
		exit(0);
	}
	new_val->node_type = NODE_TYPE_CONTINUE;
	return (AST*) new_val;
}

RuntimeValue* execute(AST* ast) {
	if (ast->node_type == NODE_TYPE_ASSIGN_VAR) {
		Dimension* dim = (Dimension*) ast;
		if (hash_has_key(env->vars, dim->var_name)) {
			yyerror("Defined variable.");
			exit(0);
		}
		RuntimeValue* value = NULL;
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
	} else if (ast->node_type == NODE_TYPE_FUNCTION_DEFINE) {
		FunctionDefineStatement* function_statement = (FunctionDefineStatement*) ast;
		RuntimeFunction* func = malloc(sizeof(RuntimeFunction) + 1);
		func->type = C_FUNCTION_DEFINE;
		func->name = malloc(sizeof(char) * strlen(function_statement->name) + 1);
		strcpy(func->name, function_statement->name);
		func->statements = function_statement->statements;
		func->arguments = list_buffer_init();
		if (hash_has_key(env->functions, func->name)) {
			yyerror("You cannot redefine a function or sub-program.");
			exit(0);
		}
		hash_put(env->functions, func->name, (void*) func);
	} else if (ast->node_type == NODE_TYPE_REASSIGN) {
		Dimension* dim = (Dimension*) ast;
		if (!hash_has_key(env->vars, dim->var_name)) {
			yyerror("Undefined variable.");
			exit(0);
		}
		if (dim->node != NULL) {
			hash_put(env->vars, dim->var_name, execute(dim->node));
		} else {
			hash_put(env->vars, dim->var_name, make_runtime_null());
		}
	} else if (ast->node_type == NODE_TYPE_CONSTANT) {
		RuntimeValue* value = malloc(sizeof(RuntimeValue) + 1);
		value->runtime_type = ((Constant *)ast)->type;
		value->i_val = ((Constant *)ast)->i_val;
		value->d_val = ((Constant *)ast)->d_val;
		value->s_val = ((Constant *)ast)->s_val;
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
		if (ast->left_node == NULL && ast->right_node == NULL) {
		} else {
			RuntimeValue* left_result = execute(ast->left_node);
			if (left_result != NULL) {
				if (left_result->runtime_type == C_BREAK || left_result->runtime_type == C_CONTINUE) {
					return left_result;
				}
			}
			if (ast->right_node != NULL) {
				RuntimeValue* right_result = execute(ast->right_node);
				return right_result;
			}
			return left_result;
		}
	} else if (ast->node_type == '&') {
		RuntimeValue* l = execute(ast->left_node);
		RuntimeValue* r= execute(ast->right_node);
		if (l->runtime_type == C_INT && r->runtime_type == C_INT) {
			return make_runtime_integer(runtime_as_integer(l) && runtime_as_integer(r));
		}
	} else if (ast->node_type == NODE_TYPE_BITAND) {
		RuntimeValue* l = execute(ast->left_node);
		RuntimeValue* r= execute(ast->right_node);
		if (l->runtime_type == C_INT && r->runtime_type == C_INT) {
			return make_runtime_integer(runtime_as_integer(l) & runtime_as_integer(r));
		}
	} else if (ast->node_type == NODE_TYPE_BITOR) {
		RuntimeValue* l = execute(ast->left_node);
		RuntimeValue* r= execute(ast->right_node);
		if (l->runtime_type == C_INT && r->runtime_type == C_INT) {
			return make_runtime_integer(runtime_as_integer(l) | runtime_as_integer(r));
		}
	} else if (ast->node_type == '|') {
		RuntimeValue* l = execute(ast->left_node);
		RuntimeValue* r= execute(ast->right_node);
		if (l->runtime_type == C_INT && r->runtime_type == C_INT) {
			return make_runtime_integer(runtime_as_integer(l) || runtime_as_integer(r));
		}
	} else if (ast->node_type == '!') {
		RuntimeValue* l = execute(ast->left_node);
		return make_runtime_integer(!runtime_as_integer(l));
	} else if (ast->node_type == '^') {
		RuntimeValue* l = execute(ast->left_node);
		RuntimeValue* r= execute(ast->right_node);
		if (l->runtime_type == C_INT && r->runtime_type == C_INT) {
			return make_runtime_integer(runtime_as_integer(l) ^ runtime_as_integer(r));
		}
	} else if (ast->node_type == '<') {
		RuntimeValue* l = execute(ast->left_node);
		RuntimeValue* r= execute(ast->right_node);
		if (l->runtime_type == C_DECIMAL || r->runtime_type == C_DECIMAL) {
			return make_runtime_integer(runtime_as_decimal(l) < runtime_as_decimal(r));
		} else if (l->runtime_type == C_INT && r->runtime_type == C_INT) {
			return make_runtime_integer(runtime_as_integer(l) < runtime_as_integer(r));
		}
	} else if (ast->node_type == '>') {
		RuntimeValue* l = execute(ast->left_node);
		RuntimeValue* r= execute(ast->right_node);
		if (l->runtime_type == C_DECIMAL || r->runtime_type == C_DECIMAL) {
			return make_runtime_integer(runtime_as_decimal(l) > runtime_as_decimal(r));
		} else if (l->runtime_type == C_INT && r->runtime_type == C_INT) {
			return make_runtime_integer(runtime_as_integer(l) > runtime_as_integer(r));
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
		}
	} else if (ast->node_type == '+') {
		RuntimeValue* l = execute(ast->left_node);
		RuntimeValue* r= execute(ast->right_node);
		if (l->runtime_type == C_STRING && r->runtime_type == C_STRING) {
			return make_runtime_string(strcat(l->s_val, r->s_val));
		} else if (l->runtime_type == C_DECIMAL || r->runtime_type == C_DECIMAL) {
			return make_runtime_decimal(runtime_as_decimal(l) + runtime_as_decimal(r));
		} else if (l->runtime_type == C_INT && r->runtime_type == C_INT) {
			return make_runtime_integer(runtime_as_integer(l) + runtime_as_integer(r));
		}
	} else if (ast->node_type == '-') {
		RuntimeValue* l = execute(ast->left_node);
		RuntimeValue* r= execute(ast->right_node);
		if (l->runtime_type == C_DECIMAL || r->runtime_type == C_DECIMAL) {
			return make_runtime_decimal(runtime_as_decimal(l) - runtime_as_decimal(r));
		} else if (l->runtime_type == C_INT && r->runtime_type == C_INT) {
			return make_runtime_integer(runtime_as_integer(l) - runtime_as_integer(r));
		}
	} else if (ast->node_type == '*') {
		RuntimeValue* l = execute(ast->left_node);
		RuntimeValue* r= execute(ast->right_node);
		if (l->runtime_type == C_DECIMAL || r->runtime_type == C_DECIMAL) {
			return make_runtime_decimal(runtime_as_decimal(l) * runtime_as_decimal(r));
		} else if (l->runtime_type == C_INT && r->runtime_type == C_INT) {
			return make_runtime_integer(runtime_as_integer(l) * runtime_as_integer(r));
		}
	} else if (ast->node_type == '/') {
		RuntimeValue* l = execute(ast->left_node);
		RuntimeValue* r= execute(ast->right_node);
		if (runtime_as_decimal(r) == 0.0) {
			yyerror("Divided by 0!");
			exit(0);
		}
		if (l->runtime_type == C_DECIMAL || r->runtime_type == C_DECIMAL) {
			return make_runtime_decimal(runtime_as_decimal(l) / runtime_as_decimal(r));
		} else if (l->runtime_type == C_INT && r->runtime_type == C_INT) {
			return make_runtime_integer(runtime_as_integer(l) / runtime_as_integer(r));
		}
	} else if (ast->node_type == NODE_TYPE_GTE) {
		RuntimeValue* l = execute(ast->left_node);
		RuntimeValue* r= execute(ast->right_node);
		if (l->runtime_type == C_DECIMAL || r->runtime_type == C_DECIMAL) {
			return make_runtime_integer(runtime_as_decimal(l) >= runtime_as_decimal(r));
		} else if (l->runtime_type == C_INT && r->runtime_type == C_INT) {
			return make_runtime_integer(runtime_as_integer(l) >= runtime_as_integer(r));
		}
	} else if (ast->node_type == NODE_TYPE_LTE) {
		RuntimeValue* l = execute(ast->left_node);
		RuntimeValue* r= execute(ast->right_node);
		if (l->runtime_type == C_DECIMAL || r->runtime_type == C_DECIMAL) {
			return make_runtime_integer(runtime_as_decimal(l) <= runtime_as_decimal(r));
		} else if (l->runtime_type == C_INT && r->runtime_type == C_INT) {
			return make_runtime_integer(runtime_as_integer(l) <= runtime_as_integer(r));
		}
	} else if (ast->node_type == NODE_TYPE_EQ) {
		RuntimeValue* l = execute(ast->left_node);
		RuntimeValue* r= execute(ast->right_node);
		if (l->runtime_type == C_DECIMAL || r->runtime_type == C_DECIMAL) {
			return make_runtime_integer(runtime_as_decimal(l) == runtime_as_decimal(r));
		} else if (l->runtime_type == C_INT && r->runtime_type == C_INT) {
			return make_runtime_integer(runtime_as_integer(l) == runtime_as_integer(r));
		}
	} else if (ast->node_type == NODE_TYPE_MOD) {
		RuntimeValue* l = execute(ast->left_node);
		RuntimeValue* r= execute(ast->right_node);
		if (runtime_as_decimal(r) == 0.0) {
			yyerror("Modulo by 0!");
			exit(0);
		}
		if (l->runtime_type == C_DECIMAL || r->runtime_type == C_DECIMAL) {
			return make_runtime_decimal(runtime_as_decimal(l) / runtime_as_decimal(r));
		} else {
			return make_runtime_integer(runtime_as_integer(l) / runtime_as_integer(r));
		}
	} else if (ast->node_type == NODE_TYPE_FUNC) {
		FunctionStatement* functional = (FunctionStatement *)ast;
		RuntimeValue* argument_list = execute(functional->expr_list);
		if (!strcmp(functional->name, "Print")) {
			RuntimeValue* param1 = list_buffer_get(argument_list->list, 0);
			if (param1->runtime_type == C_STRING) {
				printf("%s", param1->s_val);
			} else if (param1->runtime_type == C_INT) {
				printf("%d", param1->i_val);
			} else if (param1->runtime_type == C_DECIMAL) {
				printf("%f", param1->d_val);
			} else if (param1->runtime_type == C_NULL) {
				printf("NULL");
			}
			return NULL;
		} else if (!strcmp(functional->name, "PrintLine")) {
			RuntimeValue* param1 = list_buffer_get(argument_list->list, 0);
			if (param1->runtime_type == C_STRING) {
				printf("%s\n", param1->s_val);
			} else if (param1->runtime_type == C_INT) {
				printf("%d\n", param1->i_val);
			} else if (param1->runtime_type == C_DECIMAL) {
				printf("%f\n", param1->d_val);
			} else if (param1->runtime_type == C_NULL) {
				printf("NULL\n");
			}
			return NULL;
		} else {
			stack_push(env->call_stack, dup_new_func_stack_element());
			RuntimeFunction* runtime_function = (RuntimeFunction*) hash_get(env->functions, functional->name);
			if (runtime_function == NULL) {
				yyerror("The function or sub-program which you're finding does not defined.");
				exit(0);
			}
			RuntimeValue* return_value = execute(runtime_function->statements);
			FunctionStackElement* popped_env = (FunctionStackElement*) stack_pop(env->call_stack);
			FunctionStackElement* up_env = (FunctionStackElement*) stack_peak(env->call_stack);
			HashTable* up_env_hash;
			if (up_env == NULL) {
				up_env_hash = env->vars;
			} else {
				up_env_hash = up_env;
			}
			for (int i = 0; i < popped_env->local_vars->name_list->count; i++) {
				char* var_name = (char*) list_buffer_get(popped_env->local_vars->name_list, i);
				RuntimeValue* value = hash_get(popped_env, var_name);
				hash_put(up_env_hash, var_name, value);
			}
			return return_value;
		}
	} else if (ast->node_type == NODE_TYPE_EXPR_ITEM) {
		RuntimeValue* value = malloc(sizeof(RuntimeValue) + 1);
		value->runtime_type = C_LIST_BUFFER;
		value->list = integrate_params(ast);
		return value;
	} else if (ast->node_type == NODE_TYPE_VAR) {
		Var* var = (Var*)ast;
		RuntimeValue* fetched_var = hash_get(env->vars, var->name);
		if (fetched_var == NULL) {
			yyerror("Undefined variable.");
			exit(0);
		}
		return fetched_var;
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
			yyerror("Cannot return value on the top level.");
			exit(0);
		}
		ReturnStatement* return_statement = (ReturnStatement*) ast;
		RuntimeValue* returned_value = execute(return_statement->return_expr);
		return returned_value;
	} else if (ast->node_type == NODE_TYPE_BREAK) {
		return make_runtime_break();
	} else if (ast->node_type == NODE_TYPE_CONTINUE) {
		return make_runtime_continue();
	}
	return NULL;
}

ListBuffer* integrate_params(AST* node) {
	ListBuffer* default_list = list_buffer_init();
	if (node->left_node != NULL) {
		RuntimeValue* param1 = execute(node->left_node);
		list_buffer_add(default_list, param1);
	} else {
		return default_list;
	}
	if (node->right_node != NULL) {
		ListBuffer* param_list_2 = integrate_params(node->right_node);	
		list_buffer_concat(default_list, param_list_2);
		return default_list;
	} else {
		return default_list;
	}
}

AST* make_while_expression(AST* condition, AST* statements) {
	WhileStatement* new_val = malloc(sizeof(WhileStatement) + 1);
	new_val->node_type = NODE_TYPE_WHILE_STATEMENT;
	new_val->condition = condition;
	new_val->statements = statements;
	return (AST*) new_val;
}

AST* make_do_loop_expression(AST* statements, AST* condition) {
	DoLoopStatement* new_val = malloc(sizeof(DoLoopStatement) + 1);
	new_val->node_type = NODE_TYPE_DO_LOOP_STATEMENT;
	new_val->condition = condition;
	new_val->statements = statements;
	return (AST*) new_val;
}

