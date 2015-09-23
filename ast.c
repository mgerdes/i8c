#include <stdio.h>
#include <stdlib.h>
#include "i8c.h"
#include "parser.tab.h"

Node* new_node() {
    Node* n = malloc(sizeof(Node)); 
    return n;
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
        printf("DECLARATION %s\n", ast->symbol->symbol_name);
        if (ast->right_node) {
            print_ast_indented(ast->right_node, tabs + 1);
        }
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
    }
    else {
        printf("(UNKNOWN KIND %d)\n", ast->kind);
    }
}

void print_ast(Node* ast) {
    print_ast_indented(ast, 0);
}

