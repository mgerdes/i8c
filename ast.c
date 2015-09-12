#include <stdio.h>
#include <stdlib.h>
#include "i8c.h"

Node* new_node() {
    Node* n = malloc(sizeof(Node)); 
    return n;
}

void print_ast_indented(Node* ast, int tabs) {
    int i;
    for (i = 0; i < tabs; i++) {
        printf("  ");
    }
    if (ast->kind == KIND_INT) {
        printf("INTEGER - %d\n", ast->i_value);
    } else if (ast->kind == KIND_FUNC) {
        printf("FUNCTION\n");
        print_ast_indented(ast->body_node, tabs + 1);
    } else if (ast->kind == KIND_RETURN) {
        printf("RETURN\n");
        print_ast_indented(ast->return_node, tabs + 1);
    } else if (ast->kind == KIND_ADD) {
        printf("+\n");
        print_ast_indented(ast->left_node, tabs + 1);
        print_ast_indented(ast->right_node, tabs + 1);
    } else if (ast->kind == KIND_SUB) {
        printf("-\n");
        print_ast_indented(ast->left_node, tabs + 1);
        print_ast_indented(ast->right_node, tabs + 1);
    } else {
        printf("UNKNOWN KIND - %d\n", ast->kind);
    }
}

void print_ast(Node* ast) {
    print_ast_indented(ast, 0);
}

