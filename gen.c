#include <stdio.h>
#include <stdlib.h>
#include "i8c.h"
#include "parser.tab.h"

FILE* output_file;

void init_code_gen(char* file_name) {
    output_file = fopen(file_name, "w");
}

void write_header() {

}

void write_footer() {

}

int gen_unique_label_num() {
    static int label_num = 0;
    return label_num++;
}

void gen_code_bin_op(Node* ast) {
    gen_code(ast->left_node);
    fprintf(output_file, "\tpush\t%%rax\n");
    gen_code(ast->right_node);
    fprintf(output_file, "\tpop\t%%rbx\n");
    if (ast->op == '+') {
        fprintf(output_file, "\tadd\t%%ebx, %%eax\n");
    }
}

void gen_code_constant(Node* ast) {
    fprintf(output_file, "\tmov\t$%d, %%eax\n", ast->i_value);
}

void gen_code_assignment(Node* ast) {
    Node* symbol = get_symbol(top_environment(), ast->symbol);
    if (!symbol) {
        fprintf(output_file, "WE COULD NOT FIND THE SYMBOL %s\n", ast->symbol->symbol_name);
    }
    gen_code(ast->right_node);
    fprintf(output_file, "\tmov\t%%eax, -%d(%%rbp)\n", symbol->offset);
}

void gen_code_symbol(Node* ast) {
    Node* symbol = get_symbol(top_environment(), ast);
    if (!symbol) {
        fprintf(output_file, "WE COULD NOT FIND THE SYMBOL %s\n", ast->symbol_name);
    }
    fprintf(output_file, "\tmov\t-%d(%%rbp), %%eax\n", symbol->offset);
}

void gen_code_declaration(Node* ast) {
    Node* cur_declaration = ast->right_node;
    int stack_space = 0;
    while (cur_declaration) {
        put_symbol(top_environment(), cur_declaration->symbol);
        stack_space += cur_declaration->type->size;
        cur_declaration = cur_declaration->right_node;
    }
    fprintf(output_file, "\tsub\t$%d, %%rsp\n", stack_space+12); 
}

void gen_code_function_definition(Node* ast) {
    fprintf(output_file, ".globl %s\n", ast->symbol->symbol_name);
    fprintf(output_file, "%s:\n", ast->symbol->symbol_name);
    fprintf(output_file, "\tpush\t%%rbp\n");
    fprintf(output_file, "\tmov\t%%rsp, %%rbp\n");
    gen_code(ast->body_node);
    fprintf(output_file, "\tleave\n");
    fprintf(output_file, "\tret\n");
}

void gen_code_string(Node* ast) {
    fprintf(output_file, ".data\n");
    fprintf(output_file, "\t.L0:\n");
    fprintf(output_file, "\t.string %s\n", ast->s_value);
    fprintf(output_file, ".text\n");
    fprintf(output_file, "\tlea\t.L0(%rip), %%eax\n");
}

void gen_code_if(Node* ast) {
    int cur_label = gen_unique_label_num();
    switch (ast->left_node->op) {
        case '<':
            gen_code(ast->left_node);
            fprintf(output_file, "\tcmp\t%%eax, %%ebx\n");
            fprintf(output_file, "\tjge\t.UL%d\n", cur_label);
            gen_code(ast->right_node);
            fprintf(output_file, ".UL%d:\n", cur_label);
            break;
        case EQ:
            gen_code(ast->left_node);
            fprintf(output_file, "\tcmp\t%%eax, %%ebx\n");
            fprintf(output_file, "\tjne\t.UL%d\n", cur_label);
            gen_code(ast->right_node);
            fprintf(output_file, ".UL%d:\n", cur_label);
            break;
    }
}

void gen_code_func_call(Node* ast) {
    Node* symbol = get_symbol(top_environment(), ast->symbol);
    if (!symbol) {
        fprintf(output_file, "WE COULD NOT FIND THE SYMBOL %s\n", ast->symbol->symbol_name);
    }
    Node* function_call_args = ast->left_node;
    while (function_call_args) {
        gen_code(function_call_args->left_node);
        fprintf(output_file, "\tpush\t%%rax\n");
        function_call_args = function_call_args->right_node;
    }
    fprintf(output_file, "\tpop\t%%rax\n");
    fprintf(output_file, "\tmov\t%%rax, %%rsi\n");
    fprintf(output_file, "\tpop\t%%rax\n");
    fprintf(output_file, "\tmov\t%%rax, %%rdi\n");
    fprintf(output_file, "\txor\t%%eax, %%eax\n");
    fprintf(output_file, "\tcall\t%s\n", symbol->symbol_name);
}

void gen_code_return(Node* ast) {
    gen_code(ast->return_node);
    fprintf(output_file, "\tleave\n");
    fprintf(output_file, "\tret\n");
}

void gen_code(Node* ast) {
    if (!ast) {
        return;
    }

    if (ast->kind == KIND_CONSTANT) {
        gen_code_constant(ast);
    } else if (ast->kind == KIND_FUNC) {
        put_symbol(top_environment(), ast->symbol);

        push_new_environment();

        Node* function_definition_args = ast->left_node;
        while (function_definition_args) {
            put_symbol(top_environment(), function_definition_args);
            function_definition_args = function_definition_args->right_node;
        }

        gen_code_function_definition(ast);
        pop_environment();
    } else if (ast->kind == KIND_FUNC_DEF) {
        put_symbol(top_environment(), ast->symbol);
    } else if (ast->kind == KIND_RETURN) {
        gen_code_return(ast);
    } else if (ast->kind == KIND_SYMBOL) {
        gen_code_symbol(ast);
    } else if (ast->kind == KIND_STRING) {
        gen_code_string(ast);
    } else if (ast->kind == KIND_ASSIGNMENT) {
        gen_code_assignment(ast);
    } else if (ast->kind == KIND_WHILE) {
        gen_code(ast->left_node);
        gen_code(ast->right_node);
    } else if (ast->kind == KIND_IF) {
        gen_code_if(ast);
    } else if (ast->kind == KIND_BIN_OP) {
        gen_code_bin_op(ast);
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
        gen_code_declaration(ast);
    } else if (ast->kind == KIND_PROGRAM) {
        gen_code(ast->left_node);
        gen_code(ast->right_node);
    } else if (ast->kind == KIND_FUNC_CALL) {
        gen_code_func_call(ast);
    } else {
        fprintf(output_file, "(UNKNOWN KIND %d)\n", ast->kind);
    }
}

