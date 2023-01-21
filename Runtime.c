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
	RuntimeValue* new_val = malloc(sizeof(RuntimeValue) + 1);
	if (new_val == NULL) {
		printf("System error");
		exit(0);
	}
	new_val->type = C_INT;
	new_val->i_val = i_val;
	return new_val;
}

RuntimeValue* make_runtime_decimal(double d_val) {
	RuntimeValue* new_val = (RuntimeValue*) malloc(sizeof(RuntimeValue) + 1);
	if (new_val == NULL) {
		printf("System error");
		exit(0);
	}
	new_val->type = C_DECIMAL;
	new_val->d_val = d_val;
	return new_val;
}

RuntimeValue* make_runtime_string(char* s_val) {
	RuntimeValue* new_val = (RuntimeValue*) malloc(sizeof(RuntimeValue) + 1);
	if (new_val == NULL) {
		printf("System error");
		exit(0);
	}
	new_val->type = C_STRING;
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
	new_val->type = C_NULL;
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
	if (value->type == C_INT) {
		return value->i_val;
	} else if (value->type == C_DECIMAL) {
		return (int)(value->d_val);
	}
}

double runtime_as_decimal(RuntimeValue* value) {
	if (value->type == C_INT) {
		return (double)(value->i_val);
	} else if (value->type == C_DECIMAL) {
		return value->d_val;
	}
}

VariableBlock* var_make_null(char* var_name) {
	VariableBlock* new_block = (VariableBlock*) malloc(sizeof(VariableBlock) + 1);
	new_block->node_type = NODE_TYPE_ASSIGN_VAR;
	new_block->type = "null";
	new_block->var_name = var_name;
	new_block->next_dim = NULL;
	return new_block;
}

VariableBlock* var_make_int(char* var_name, int content) {
	VariableBlock* new_block = (VariableBlock*) malloc(sizeof(VariableBlock) + 1);
	new_block->node_type = NODE_TYPE_ASSIGN_VAR;
	new_block->type = "integer";
	new_block->var_name = var_name;
	new_block->i_val = content;
	new_block->next_dim = NULL;
	return new_block;
}

VariableBlock* var_make_decimal(char* var_name, double content) {
	VariableBlock* new_block = (VariableBlock*) malloc(sizeof(VariableBlock) + 1);
	new_block->node_type = NODE_TYPE_ASSIGN_VAR;
	new_block->type = "decimal";
	new_block->var_name = var_name;
	new_block->d_val = content;
	new_block->next_dim = NULL;
	return new_block;
}

VariableBlock* var_make_string(char* var_name, char* content) {
	VariableBlock* new_block = (VariableBlock*) malloc(sizeof(VariableBlock) + 1);
	new_block->node_type = NODE_TYPE_ASSIGN_VAR;
	new_block->type = "string";
	new_block->var_name = var_name;
	new_block->s_val = malloc(sizeof(char) * strlen(content) + 1);
	strcpy(new_block->s_val, content);
	new_block->next_dim = NULL;
	return new_block;
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

RuntimeValue* execute(AST* ast) {
	if (ast->node_type == NODE_TYPE_ASSIGN_VAR) {
		VariableBlock* var_block = (VariableBlock*) ast;
		if (hash_has_key(env->vars, var_block->var_name)) {
			yyerror("Defined variable.");
			exit(0);
		}
		if (!strcmp(var_block->type, "integer")) {
			hash_put(env->vars, var_block->var_name, make_runtime_integer(var_block->i_val));
		} else if (!strcmp(var_block->type, "decimal")) {
			hash_put(env->vars, var_block->var_name, make_runtime_decimal(var_block->d_val));
		} else if (!strcmp(var_block->type, "string")) {
			hash_put(env->vars, var_block->var_name, make_runtime_string(var_block->s_val));
		} else if (!strcmp(var_block->type, "null")) {
			hash_put(env->vars, var_block->var_name, make_runtime_null());
		}
		if (var_block->next_dim != NULL) {
			execute(var_block->next_dim);
		}
	} else if (ast->node_type == NODE_TYPE_REASSIGN) {
		VariableBlock* var_block = (VariableBlock*) ast;
		if (!hash_has_key(env->vars, var_block->var_name)) {
			yyerror("Undefined variable.");
			exit(0);
		}
		hash_delete(env->vars, var_block->var_name);
		if (!strcmp(var_block->type, "integer")) {
			hash_put(env->vars, var_block->var_name, make_runtime_integer(var_block->i_val));
		} else if (!strcmp(var_block->type, "decimal")) {
			hash_put(env->vars, var_block->var_name, make_runtime_decimal(var_block->d_val));
		} else if (!strcmp(var_block->type, "string")) {
			hash_put(env->vars, var_block->var_name, make_runtime_string(var_block->s_val));
		} else if (!strcmp(var_block->type, "null")) {
			hash_put(env->vars, var_block->var_name, make_runtime_null());
		}
	} else if (ast->node_type == NODE_TYPE_CONSTANT) {
		RuntimeValue* value = malloc(sizeof(RuntimeValue) + 1);
		value->type = ((Constant *)ast)->type;
		value->i_val = ((Constant *)ast)->i_val;
		value->d_val = ((Constant *)ast)->d_val;
		value->s_val = ((Constant *)ast)->s_val;
		return value;
	} else if (ast->node_type == NODE_TYPE_IF_STATEMENT) {
		IfStatement* if_statement = (IfStatement *) ast;
		if (runtime_as_integer(execute(if_statement->condition))) {
			execute(if_statement->if_statement);
		} else {
			IfStatement* tmp_stat = if_statement->else_if_statement;
			int flag1 = 0;
			while (tmp_stat != NULL) {
				if (runtime_as_integer(execute(tmp_stat->condition))) {
					execute(tmp_stat->if_statement);
					flag1 = 1;
					break;
				}
				tmp_stat = tmp_stat->else_if_statement;
			}
			if (!flag1 && if_statement->else_statement != NULL) {
				execute(if_statement->else_statement);
			}
		}
	} else if (ast->node_type == NODE_TYPE_ELSE) {
		execute(ast->left_node);
	} else if (ast->node_type == NODE_TYPE_PRIORITY) {
		return execute(ast->left_node);
	} else if (ast->node_type == NODE_TYPE_STATEMENT) {
		execute(ast->left_node);
		if (ast->right_node != NULL) {
			execute(ast->right_node);
		}
	} else if (ast->node_type == '&') {
		RuntimeValue* l = execute(ast->left_node);
		RuntimeValue* r= execute(ast->right_node);
		if (l->type == C_INT && r->type == C_INT) {
			return make_runtime_integer(runtime_as_integer(l) && runtime_as_integer(r));
		}
	} else if (ast->node_type == '|') {
		RuntimeValue* l = execute(ast->left_node);
		RuntimeValue* r= execute(ast->right_node);
		if (l->type == C_INT && r->type == C_INT) {
			return make_runtime_integer(runtime_as_integer(l) || runtime_as_integer(r));
		}
	} else if (ast->node_type == '!') {
		RuntimeValue* l = execute(ast->left_node);
		return make_runtime_integer(!runtime_as_integer(l));
	} else if (ast->node_type == '^') {
		RuntimeValue* l = execute(ast->left_node);
		RuntimeValue* r= execute(ast->right_node);
		if (l->type == C_INT && r->type == C_INT) {
			return make_runtime_integer(runtime_as_integer(l) ^ runtime_as_integer(r));
		}
	} else if (ast->node_type == '<') {
		RuntimeValue* l = execute(ast->left_node);
		RuntimeValue* r= execute(ast->right_node);
		if (l->type == C_DECIMAL || r->type == C_DECIMAL) {
			return make_runtime_integer(runtime_as_decimal(l) < runtime_as_decimal(r));
		} else if (l->type == C_INT && r->type == C_INT) {
			return make_runtime_integer(runtime_as_integer(l) < runtime_as_integer(r));
		}
	} else if (ast->node_type == '>') {
		RuntimeValue* l = execute(ast->left_node);
		RuntimeValue* r= execute(ast->right_node);
		if (l->type == C_DECIMAL || r->type == C_DECIMAL) {
			return make_runtime_integer(runtime_as_decimal(l) > runtime_as_decimal(r));
		} else if (l->type == C_INT && r->type == C_INT) {
			return make_runtime_integer(runtime_as_integer(l) > runtime_as_integer(r));
		}
	} else if (ast->node_type == '$') {
		RuntimeValue* l = execute(ast->left_node);
		RuntimeValue* r= execute(ast->right_node);
		if (l->type == C_STRING && r->type == C_STRING) {
			return make_runtime_integer(!strcmp(l->s_val, r->s_val));
		} else if (l->type == C_DECIMAL || r->type == C_DECIMAL) {
			return make_runtime_integer(runtime_as_decimal(l) != runtime_as_decimal(r));
		} else if (l->type == C_INT && r->type == C_INT) {
			return make_runtime_integer(runtime_as_integer(l) != runtime_as_integer(r));
		}
	} else if (ast->node_type == '+') {
		RuntimeValue* l = execute(ast->left_node);
		RuntimeValue* r= execute(ast->right_node);
		if (l->type == C_STRING && r->type == C_STRING) {
			return make_runtime_string(strcat(l->s_val, r->s_val));
		} else if (l->type == C_DECIMAL || r->type == C_DECIMAL) {
			return make_runtime_decimal(runtime_as_decimal(l) + runtime_as_decimal(r));
		} else if (l->type == C_INT && r->type == C_INT) {
			return make_runtime_integer(runtime_as_integer(l) + runtime_as_integer(r));
		}
	} else if (ast->node_type == '-') {
		RuntimeValue* l = execute(ast->left_node);
		RuntimeValue* r= execute(ast->right_node);
		if (l->type == C_DECIMAL || r->type == C_DECIMAL) {
			return make_runtime_decimal(runtime_as_decimal(l) - runtime_as_decimal(r));
		} else if (l->type == C_INT && r->type == C_INT) {
			return make_runtime_integer(runtime_as_integer(l) - runtime_as_integer(r));
		}
	} else if (ast->node_type == '*') {
		RuntimeValue* l = execute(ast->left_node);
		RuntimeValue* r= execute(ast->right_node);
		if (l->type == C_DECIMAL || r->type == C_DECIMAL) {
			return make_runtime_decimal(runtime_as_decimal(l) * runtime_as_decimal(r));
		} else if (l->type == C_INT && r->type == C_INT) {
			return make_runtime_integer(runtime_as_integer(l) * runtime_as_integer(r));
		}
	} else if (ast->node_type == '/') {
		RuntimeValue* l = execute(ast->left_node);
		RuntimeValue* r= execute(ast->right_node);
		if (runtime_as_decimal(r) == 0.0) {
			yyerror("Divided by 0!");
			exit(0);
		}
		if (l->type == C_DECIMAL || r->type == C_DECIMAL) {
			return make_runtime_decimal(runtime_as_decimal(l) / runtime_as_decimal(r));
		} else if (l->type == C_INT && r->type == C_INT) {
			return make_runtime_integer(runtime_as_integer(l) / runtime_as_integer(r));
		}
	} else if (ast->node_type == NODE_TYPE_GTE) {
		RuntimeValue* l = execute(ast->left_node);
		RuntimeValue* r= execute(ast->right_node);
		if (l->type == C_DECIMAL || r->type == C_DECIMAL) {
			return make_runtime_integer(runtime_as_decimal(l) >= runtime_as_decimal(r));
		} else if (l->type == C_INT && r->type == C_INT) {
			return make_runtime_integer(runtime_as_integer(l) >= runtime_as_integer(r));
		}
	} else if (ast->node_type == NODE_TYPE_LTE) {
		RuntimeValue* l = execute(ast->left_node);
		RuntimeValue* r= execute(ast->right_node);
		if (l->type == C_DECIMAL || r->type == C_DECIMAL) {
			return make_runtime_integer(runtime_as_decimal(l) <= runtime_as_decimal(r));
		} else if (l->type == C_INT && r->type == C_INT) {
			return make_runtime_integer(runtime_as_integer(l) <= runtime_as_integer(r));
		}
	} else if (ast->node_type == NODE_TYPE_EQ) {
		RuntimeValue* l = execute(ast->left_node);
		RuntimeValue* r= execute(ast->right_node);
		if (l->type == C_DECIMAL || r->type == C_DECIMAL) {
			return make_runtime_integer(runtime_as_decimal(l) <= runtime_as_decimal(r));
		} else if (l->type == C_INT && r->type == C_INT) {
			return make_runtime_integer(runtime_as_integer(l) <= runtime_as_integer(r));
		}
	} else if (ast->node_type == NODE_TYPE_MOD) {
		RuntimeValue* l = execute(ast->left_node);
		RuntimeValue* r= execute(ast->right_node);
		if (runtime_as_decimal(r) == 0.0) {
			yyerror("Modulo by 0!");
			exit(0);
		}
		if (l->type == C_DECIMAL || r->type == C_DECIMAL) {
			return make_runtime_decimal(runtime_as_decimal(l) / runtime_as_decimal(r));
		} else {
			return make_runtime_integer(runtime_as_integer(l) / runtime_as_integer(r));
		}
	} else if (ast->node_type == NODE_TYPE_FUNC) {
		FunctionStatement* functional = (FunctionStatement *)ast;
		RuntimeValue* argument_list = execute(functional->expr_list);
		if (!strcmp(functional->name, "Print")) {
			RuntimeValue* param1 = list_buffer_get(argument_list->list, 0);
			if (param1->type == C_STRING) {
				printf("%s", param1->s_val);
			} else if (param1->type == C_INT) {
				printf("%d", param1->i_val);
			} else if (param1->type == C_DECIMAL) {
				printf("%f", param1->d_val);
			} else if (param1->type == C_NULL) {
				printf("NULL");
			}
		} else if (!strcmp(functional->name, "PrintLine")) {
			RuntimeValue* param1 = list_buffer_get(argument_list->list, 0);
			if (param1->type == C_STRING) {
				printf("%s\n", param1->s_val);
			} else if (param1->type == C_INT) {
				printf("%d\n", param1->i_val);
			} else if (param1->type == C_DECIMAL) {
				printf("%f\n", param1->d_val);
			} else if (param1->type == C_NULL) {
				printf("NULL\n");
			}
		}
	} else if (ast->node_type == NODE_TYPE_EXPR_ITEM) {
		RuntimeValue* value = malloc(sizeof(RuntimeValue) + 1);
		value->type = C_LIST_BUFFER;
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
	}
}

ListBuffer* integrate_params(AST* node) {
	ListBuffer* default_list = list_buffer_init();
	RuntimeValue* param1 = execute(node->left_node);
	list_buffer_add(default_list, param1);
	if (node->right_node != NULL) {
		ListBuffer* param_list_2 = integrate_params(node->right_node);	
		list_buffer_concat(default_list, param_list_2);
		return default_list;
	} else {
		return default_list;
	}
}

