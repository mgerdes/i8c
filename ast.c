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

Return* new_return() {
    Return* r = malloc(sizeof(Return));
    r->kind = RETURN_STATEMENT;
    return r;
}

Reference* new_reference() {
    Reference* r = malloc(sizeof(Reference));
    r->kind = REFERENCE;
    return r;
}

Dereference* new_dereference() {
    Dereference* d = malloc(sizeof(Dereference));
    d->kind = DEREFERENCE;
    return d;
}

Negation* new_negation() {
    Negation* n = malloc(sizeof(Negation));
    n->kind = NEGATION;
    return n;
}

Struct* new_struct() {
    Struct* s = malloc(sizeof(Struct));
    s->kind = STRUCT_TYPE;
    return s;
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
        Reference* re;
        Dereference* de;
        Negation* n;
    } u;

    if (!ast) {
        return;
    }

    if (ast->kind != LIST) {
        for (i = 0; i < tabs; i++) {
            printf("  ");
        }
    }

    switch(ast->kind) {
        case LIST:
            u.l = (List*) ast;
            print_ast_indented(u.l->head, tabs);
            print_ast_indented((Node*) (u.l->rest), tabs);
            break;
        case SYMBOL:
            u.s = (Symbol*) ast;
            printf("Symbol: %s\n", u.s->name);
            break;
        case FUNC_DEF:
            u.f = (Function_Definition*) ast;
            printf("Function Defintion: %s\n", u.f->identifier->name);
            print_ast_indented(u.f->statements, tabs+1);
            break;
        case WHILE_LOOP:
            u.w = (While_Loop*) ast;
            printf("While Loop:\n");
            print_ast_indented(u.w->expression, tabs+1);
            print_ast_indented(u.w->statements, tabs+1);
            break;
        case IF_ELSE:
            u.ie = (If_Else*) ast;
            printf("If Statement:\n");
            print_ast_indented(u.ie->expression, tabs+1);
            print_ast_indented(u.ie->statements, tabs+1);
            break;
        case DECLARATION:
            u.d = (Declaration*) ast;
            printf("Declaration:\n");
            print_ast_indented((Node*) u.d->identifiers, tabs+1);
            break;
        case ASSIGNMENT:
            u.a = (Assignment*) ast;
            printf("Assignment:\n");
            print_ast_indented(u.a->l_value, tabs+1);
            print_ast_indented(u.a->r_value, tabs+1);
            break;
        case CONSTANT:
            u.c = (Constant*) ast;
            printf("Constant: ");
            if (u.c->constant_kind == INT_CONSTANT) {
                printf("%d", u.c->int_value);
            } else if (u.c->constant_kind == FLOAT_CONSTANT) {
                printf("%f", u.c->float_value);
            } else if (u.c->constant_kind == STRING_CONSTANT) {
                printf("%s", u.c->string_value);
            }
            printf("\n");
            break;
        case FUNC_CALL:
            u.fc = (Function_Call*) ast;
            printf("Function Call: %s\n", u.fc->identifier->name);
            List* arguments = u.fc->arguments;
            while (arguments) {
                print_ast_indented(arguments->head, tabs+1);
                arguments = arguments->rest;
            }
            break;
        case BINARY_OP:
            u.b = (Binary_Operator*) ast;
            printf("Binary Op: ");
            print_binary_operator(u.b->op);
            printf("\n");
            print_ast_indented(u.b->left_expression, tabs+1);
            print_ast_indented(u.b->right_expression, tabs+1);
            break;
        case RETURN_STATEMENT:
            u.r = (Return*) ast;
            printf("Return: \n");
            print_ast_indented(u.r->expression, tabs+1);
            break;
        case REFERENCE:
            u.re = (Reference*) ast;
            printf("Reference: %s\n", u.re->symbol->name);
            break;
        case DEREFERENCE:
            u.de = (Dereference*) ast;
            printf("Dereference:\n");
            print_ast_indented(u.de->expression, tabs+1);
            break;
        case NEGATION:
            u.n = (Negation*) ast;
            printf("Negation:\n");
            print_ast_indented(u.n->expression, tabs+1);
            break;
        default:
            printf("Unknown ast kind, fix them cases\n");
    }
}

void print_ast(Node* ast) {
    print_ast_indented(ast, 0);
}

