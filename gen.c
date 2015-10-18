#include <stdio.h>
#include <stdlib.h>
#include "i8c.h"
#include "parser.tab.h"

char* call_registers[] = {"edi", "esi", "edx", "ecx", "r8d", "r9d"}; 
FILE* output_file;

int unique_label() {
    static int label = 0;
    return label++;
}

void init_code_gen(char* file_name) {
    output_file = fopen(file_name, "w");
    push_new_environment();
}

void gen_code_symbol(Symbol* s) {
    Symbol* sy = get_symbol(top_environment(), s->name);
    if (!sy) {
        printf("Error: Use of undeclared variable %s\n", s->name);
        exit(1);
    }
    fprintf(output_file, "    mov    -%d(%%rbp), %%eax\n", sy->offset);
}

void gen_code_function_def(Function_Definition* f) {
    put_symbol(top_environment(), f->identifier);
    push_new_environment();

    if (f->statements) {
        fprintf(output_file, ".globl %s\n", f->identifier->name);
        fprintf(output_file, "%s:\n", f->identifier->name);
        fprintf(output_file, "    push   %%rbp\n");
        fprintf(output_file, "    mov    %%rsp, %%rbp\n");

        int i = 0;
        List* declarations = f->parameter_declarations;
        while (declarations) {
            gen_code(declarations->head);
            // This is not good. Declarations should not be a list of identifiers.
            Symbol* s = get_symbol(top_environment(), ((Symbol*) (((Declaration*) declarations->head)->identifiers)->head)->name);
            if (i < 6) {
                fprintf(output_file, "    mov    %%%s, -%d(%%rbp)\n", call_registers[i++], s->offset);
            } else {

            }
            declarations = declarations->rest; 
        }

        gen_code(f->statements);
        fprintf(output_file, "    leave\n");
        fprintf(output_file, "    ret\n");
    }
    pop_environment();
}

void gen_code_while_loop(While_Loop* w) {
    int label = unique_label();
    fprintf(output_file, ".L_BEGIN_WHILE_%d:\n", label);
    gen_code(w->expression);
    fprintf(output_file, "    cmp    $0, %%eax\n");
    fprintf(output_file, "    je     .L_END_WHILE_%d\n", label);
    gen_code(w->statements);
    fprintf(output_file, "    jmp    .L_BEGIN_WHILE_%d\n", label);
    fprintf(output_file, ".L_END_WHILE_%d:\n", label);
}

void gen_code_if_else(If_Else* i) {
    int label = unique_label();
    gen_code(i->expression);
    fprintf(output_file, "    cmp    $0, %%eax\n");
    fprintf(output_file, "    je     .L_END_IF_%d\n", label);
    gen_code(i->statements);
    fprintf(output_file, ".L_END_IF_%d:\n", label);
}

void gen_code_declaration(Declaration* d) {
    int size = 0;
    List* l = d->identifiers;
    while (l) {
        put_symbol(top_environment(), (Symbol*) l->head);
        size += 4;
        l = l->rest;
    }
    fprintf(output_file, "    sub    $%d, %%rsp\n", size+4);
}

void gen_code_assignment(Assignment* a) {
    Symbol* s = get_symbol(top_environment(), ((Symbol*) a->l_value)->name);
    if (!s) {
        printf("Error: attempting to use undeclared symbol %s\n", ((Symbol*) a->l_value)->name);
        exit(1);
    }
    gen_code(a->r_value);
    fprintf(output_file, "    mov    %%eax, -%d(%%rbp)\n", s->offset);
}

void gen_code_constant(Constant* c) {
    if (c->constant_kind == INT_CONSTANT) {
        fprintf(output_file, "    mov    $%d, %%eax\n", c->int_value);
    } else if (c->constant_kind == STRING_CONSTANT) {
        int label = unique_label();
        fprintf(output_file, ".data\n");
        fprintf(output_file, "    .L%d:\n", label);
        fprintf(output_file, "    .string %s\n", c->string_value);
        fprintf(output_file, ".text\n");
        fprintf(output_file, "    lea    .L%d(%%rip), %%eax\n", label);
    }
}

void gen_code_function_call(Function_Call* f) {
    Symbol* s = get_symbol(top_environment(), ((Symbol*) f->identifier)->name); 
    if (!s) {
        printf("Error: attempting to use undeclared symbol %s\n", ((Symbol*) f->identifier)->name);
        exit(1);
    }
    List* arguments = f->arguments;  
    int i = 0;
    while(arguments) {
        gen_code(arguments->head);
        if (i < 6) {
            fprintf(output_file, "    mov    %%eax, %%%s\n", call_registers[i++]);
        } else {
            fprintf(output_file, "    push   %%rax\n");
        }
        arguments = arguments->rest;
    }
    fprintf(output_file, "    xor    %%eax, %%eax\n");
    fprintf(output_file, "    call   %s\n", s->name);
}

void generate_comparison(int op, int label) {
    fprintf(output_file, "    cmp    %%eax, %%ebx\n");
    switch(op) {
        case '<':
            fprintf(output_file, "    jge    .L_CMP_%d\n", label);
            break;
        case '>':
            fprintf(output_file, "    jle    .L_CMP_%d\n", label);
            break;
        case EQ:
            fprintf(output_file, "    jne    .L_CMP_%d\n", label);
            break;

    }
    fprintf(output_file, "    mov    $1, %%eax\n");
    fprintf(output_file, "    jmp    .L_CMP_END_%d\n", label);
    fprintf(output_file, ".L_CMP_%d:\n", label);
    fprintf(output_file, "    mov    $0, %%eax\n");
    fprintf(output_file, ".L_CMP_END_%d:\n", label);
}

void gen_code_bin_op(Binary_Operator* b) {
    int label = unique_label();
    gen_code(b->left_expression);
    fprintf(output_file, "    push   %%rax\n");
    gen_code(b->right_expression);
    fprintf(output_file, "    pop    %%rbx\n");

    switch(b->op) {
        case '+': 
            fprintf(output_file, "    add    %%ebx, %%eax\n");
            break;
        case '-':
            fprintf(output_file, "    sub    %%eax, %%ebx\n");
            fprintf(output_file, "    mov    %%ebx, %%eax\n");
            break;
        case '<':
            generate_comparison('<', label);
            break;
        case '>':
            generate_comparison('>', label);
            break;
        case EQ:
            generate_comparison(EQ, label);
            break;
    }
}

void gen_code_return(Return* r) {
    gen_code(r->expression);
    fprintf(output_file, "    leave\n");
    fprintf(output_file, "    ret\n");
}

void gen_code(Node* ast) {
    union {
        List* l;
        Symbol* s;
        Function_Definition* f;
        While_Loop* w;
        If_Else* ie;
        Declaration* d;
        Assignment* a;
        Constant* c;
        Function_Call* fc;
        Binary_Operator* b;
        Return* r;
    } u;

    if (!ast) {
        return;
    }

    switch(ast->kind) {
        case LIST:
            u.l = (List*) ast;
            gen_code(u.l->head);
            gen_code((Node*) u.l->rest);
            break;
        case SYMBOL:
            u.s = (Symbol*) ast;
            gen_code_symbol(u.s);
            break;
        case FUNC_DEF:
            u.f = (Function_Definition*) ast;
            gen_code_function_def(u.f);
            break;
        case WHILE_LOOP:
            u.w = (While_Loop*) ast;
            gen_code_while_loop(u.w);
            break;
        case IF_ELSE:
            u.ie = (If_Else*) ast;
            gen_code_if_else(u.ie);
            break;
        case DECLARATION:
            u.d = (Declaration*) ast;
            gen_code_declaration(u.d);
            break;
        case ASSIGNMENT:
            u.a = (Assignment*) ast;
            gen_code_assignment(u.a);
            break;
        case CONSTANT:
            u.c = (Constant*) ast;
            gen_code_constant(u.c);
            break;
        case FUNC_CALL:
            u.fc = (Function_Call*) ast;
            gen_code_function_call(u.fc);
            break;
        case BINARY_OP:
            u.b = (Binary_Operator*) ast;
            gen_code_bin_op(u.b);
            break;
        case RETURN_STATEMENT:
            u.r = (Return*) ast;
            gen_code_return(u.r);
            break;
        default:
            break;
    }
}
