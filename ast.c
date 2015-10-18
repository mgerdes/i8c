#include <stdio.h>
#include <stdlib.h>
#include "i8c.h"
#include "parser.tab.h"

Node* new_node() {
    Node* n = malloc(sizeof(Node)); 
    return n;
}

List* new_list() {
    List* l = malloc(sizeof(List));
    l->kind = LIST;
    return l;
}

Symbol* new_symbol() {
    Symbol* s = malloc(sizeof(Symbol)); 
    s->kind = SYMBOL;
    return s;
}

Function_Definition* new_function_definition() {
    Function_Definition* f = malloc(sizeof(Function_Definition)); 
    f->kind = FUNC_DEF;
    return f;
}

While_Loop* new_while_loop() {
    While_Loop* w = malloc(sizeof(While_Loop));
    w->kind = WHILE_LOOP;
    return w;
}

If_Else* new_if_else() {
    If_Else* i = malloc(sizeof(If_Else));
    i->kind = IF_ELSE;
    return i;
}

Declaration* new_declaration() {
    Declaration* d = malloc(sizeof(Declaration));
    d->kind = DECLARATION;
    return d;
}

Assignment* new_assignment() {
    Assignment* a = malloc(sizeof(Assignment));
    a->kind = ASSIGNMENT;
    return a;
}

Constant* new_constant() {
    Constant* c = malloc(sizeof(Constant));
    c->kind = CONSTANT;
    return c;
}

Function_Call* new_function_call() {
    Function_Call* f = malloc(sizeof(Function_Call));
    f->kind = FUNC_CALL;
    return f;
}

Binary_Operator* new_binary_operator() {
    Binary_Operator* b = malloc(sizeof(Binary_Operator));
    b->kind = BINARY_OP;
    return b;
}

void print_binary_operator(int bin_op) {
    switch(bin_op){
        case '+':
        case '-':
        case '>':
        case '<':
            printf("%c", bin_op);
            break;
        case LTE:
            printf("<=");
            break;
        case GTE:
            printf(">=");
            break;
        case EQ:
            printf("==");
            break;
        default : 
            printf("Unknown operator");
    }
}

void print_ast_indented(Node* ast, int tabs) {
    int i;
    if (!ast) {
        return;
    }
    if (ast->kind != KIND_BLOCK) {
        for (i = 0; i < tabs; i++) {
            printf("  ");
        }
    }
    if (ast->kind == KIND_CONSTANT) {
        if (ast->type == type_float) {
            printf("CONSTANT %f\n", ast->f_value);
        } else if (ast->type == type_int) {
            printf("CONSTANT %d\n", ast->i_value);
        }
    } else if (ast->kind == KIND_STRING) {
        printf("STRING %s\n", ast->s_value);
    } else if (ast->kind == KIND_FUNC) {
        printf("FUNCTION %s (", ast->symbol->symbol_name);
        Node* function_definition_args = ast->left_node;
        while (function_definition_args) {
            printf("%s", function_definition_args->symbol_name);
            function_definition_args = function_definition_args->right_node;
            if (function_definition_args) {
                printf(", ");
            }
        }
        printf(")\n");
        print_ast_indented(ast->body_node, tabs + 1);
    } else if (ast->kind == KIND_RETURN) {
        printf("RETURN\n");
        print_ast_indented(ast->return_node, tabs + 1);
    } else if (ast->kind == KIND_SYMBOL) {
        printf("SYMBOL %s\n", ast->symbol_name);
    } else if (ast->kind == KIND_ASSIGNMENT) {
        printf("ASSIGNMENT %s\n", ast->symbol->symbol_name);
        print_ast_indented(ast->right_node, tabs + 1);
    } else if (ast->kind == KIND_WHILE) {
        printf("WHILE\n");
        print_ast_indented(ast->left_node, tabs + 1);
        print_ast_indented(ast->right_node, tabs + 1);
    } else if (ast->kind == KIND_IF) {
        printf("IF\n");
        print_ast_indented(ast->left_node, tabs + 1);
        print_ast_indented(ast->right_node, tabs + 1);
    } else if (ast->kind == KIND_BIN_OP) {
        printf("BIN OP ");
        print_binary_operator(ast->op);
        printf("\n");
        print_ast_indented(ast->left_node, tabs + 1);
        print_ast_indented(ast->right_node, tabs + 1);
    } else if (ast->kind == KIND_BLOCK) {
        if (ast->left_node->kind == KIND_BLOCK) {
            // an inner block should be indented in more as it has a different scope.
            printf("  INNER BLOCK\n");
            print_ast_indented(ast->left_node, tabs + 1);
        } else {
            print_ast_indented(ast->left_node, tabs);
        }
        print_ast_indented(ast->right_node, tabs);
    } else if (ast->kind == KIND_DECLARATION) {
        Node* cur_declaration = ast->right_node;
        printf("DECLARATION ");
        while (cur_declaration) {
            printf("%s", cur_declaration->symbol->symbol_name);
            cur_declaration = cur_declaration->right_node;
            if (cur_declaration) {
                printf(", ");
            }
        }
        printf("\n");
    } else if (ast->kind == KIND_PROGRAM) {
        print_ast_indented(ast->left_node, tabs);
        print_ast_indented(ast->right_node, tabs);
    } else if (ast->kind == KIND_FUNC_CALL) {
        printf("FUNCTION_CALL %s\n", ast->symbol->symbol_name);
        Node* function_call_args = ast->left_node;
        while (function_call_args) {
            print_ast_indented(function_call_args->left_node, tabs + 1);
            function_call_args = function_call_args->right_node;
        }
    } else if (ast->kind == KIND_FUNC_DEF) {
        printf("FUNCTION_DEF %s\n", ast->symbol->symbol_name);
    } else {
        printf("(UNKNOWN KIND %d)\n", ast->kind);
    }
}

void print_ast(Node* ast) {
    print_ast_indented(ast, 0);
}

