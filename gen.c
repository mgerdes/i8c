#include <stdio.h>
#include <stdlib.h>
#include "i8c.h"

void gen_code(Node* ast) {
    if (!ast) {
        return;
    }

    if (ast->kind == KIND_CONSTANT) {
    } else if (ast->kind == KIND_FUNC) {
        put_symbol(top_environment(), ast->symbol);

        push_new_environment();

        Node* function_definition_args = ast->left_node;
        while (function_definition_args) {
            put_symbol(top_environment(), function_definition_args);
            function_definition_args = function_definition_args->right_node;
        }

        gen_code(ast->body_node);
        pop_environment();
    } else if (ast->kind == KIND_RETURN) {
        gen_code(ast->return_node);
    } else if (ast->kind == KIND_SYMBOL) {
        Node* symbol = get_symbol(top_environment(), ast);
        if (!symbol) {
            printf("WE COULD NOT FIND THE SYMBOL %s\n", ast->symbol_name);
        }
    } else if (ast->kind == KIND_ASSIGNMENT) {
        Node* symbol = get_symbol(top_environment(), ast->symbol);
        if (!symbol) {
            printf("WE COULD NOT FIND THE SYMBOL %s\n", ast->symbol->symbol_name);
        }
        gen_code(ast->right_node);
    } else if (ast->kind == KIND_WHILE) {
        gen_code(ast->left_node);
        gen_code(ast->right_node);
    } else if (ast->kind == KIND_IF) {
        gen_code(ast->left_node);
        gen_code(ast->right_node);
    } else if (ast->kind == KIND_BIN_OP) {
        gen_code(ast->left_node);
        gen_code(ast->right_node);
    } else if (ast->kind == KIND_BLOCK) {
        if (ast->left_node->kind == KIND_BLOCK) {
            // this is an inner block, needs new environment
            push_new_environment();
            gen_code(ast->left_node);
            pop_environment();
        } else {
            gen_code(ast->left_node);
        }
        gen_code(ast->right_node);
    } else if (ast->kind == KIND_DECLARATION) {
        put_symbol(top_environment(), ast->symbol);
    } else if (ast->kind == KIND_PROGRAM) {
        gen_code(ast->left_node);
        gen_code(ast->right_node);
    } else if (ast->kind == KIND_FUNC_CALL) {
        Node* symbol = get_symbol(top_environment(), ast->symbol);
        if (!symbol) {
            printf("WE COULD NOT FIND THE SYMBOL %s\n", ast->symbol->symbol_name);
        }
        Node* function_call_args = ast->left_node;
        while (function_call_args) {
            gen_code(function_call_args->left_node);
            function_call_args = function_call_args->right_node;
        }
    }
    else {
        printf("(UNKNOWN KIND %d)\n", ast->kind);
    }
}

