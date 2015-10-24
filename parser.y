%{
#define YYSTYPE Node*
#include "i8c.h"

int yyparse(void);
int yylex();
void yyerror(const char *str) {
    fprintf(stderr, "Line %d: %s \"%s\"\n", yylineno, str, yytext);
}
%}

%token FLOAT INT VOID RETURN WHILE IF ELSE
%token IDENTIFIER NUMBER STRING
%token LTE GTE

%left '='
%left '<' '>' LTE GTE EQ
%right '+' '-'

%%

start
    : program
        {
            program = $1;
        }
    ;

program
    : 
        {
            $$ = 0;
        }
    | function_definition program
        {
            List* l = new_list();
            l->head = $1;
            l->rest = (List*) $2;
            $$ = (Node*) l;
        }
    | declaration ';' program
        {
            List* l = new_list();
            l->head = $1;
            l->rest = (List*) $3;
            $$ = (Node*) l;
        }
    ;

function_definition
    : type IDENTIFIER '(' function_definition_args ')' '{' block '}' 
        {
            Function_Definition* f = new_function_definition();
            f->identifier = (Symbol*) $2;
            f->parameter_declarations = (List*) $4;
            f->statements = $7;
            $$ = (Node*) f;
        }
    | type IDENTIFIER '(' function_definition_args ')' ';'
        {
            Function_Definition* f = new_function_definition();
            f->identifier = (Symbol*) $2;
            f->parameter_declarations = (List*) $4;
            f->statements = 0;
            $$ = (Node*) f;
        }
    ;

function_definition_args
    :  
        {
            $$ = 0;
        }
    | type IDENTIFIER
        {
            List* l1 = new_list();
            l1->head = (Node*) $2;

            Declaration* d = new_declaration();
            d->identifiers = l1;

            List* function_parameters = new_list();
            function_parameters->head = (Node*) d;
            function_parameters->rest = 0;

            $$ = (Node*) function_parameters;
        }
    | type IDENTIFIER ',' function_definition_args
        {
            List* l1 = new_list();
            l1->head = (Node*) $2;

            Declaration* d = new_declaration();
            d->identifiers = l1;

            List* function_parameters = new_list();
            function_parameters->head = (Node*) d;
            function_parameters->rest = (List*) $4;

            $$ = (Node*) function_parameters;
        }
    ;

block
    : 
        {
            $$ = 0;
        }
    | statement block
        {
            List* l = new_list();
            l->head = $1;
            l->rest = (List*) $2;
            $$ = (Node*) l;
        }
    ;

statement
    : RETURN expression ';' 
        {
            Return* r = new_return();
            r->expression = $2;
            $$ = (Node*) r;
        }
    | while_loop
    | if_statement 
    | declaration ';'
    | assignment ';'
    | expression ';' 
    ;

while_loop
    : WHILE '(' expression ')' '{' block '}'
        {
            While_Loop* w = new_while_loop();
            w->expression = $3;
            w->statements = $6;
            $$ = (Node*) w;
        }
    ;

if_statement
    : IF '(' expression ')' '{' block '}'
        {
            If_Else* i = new_if_else();
            i->expression = $3;
            i->statements = $6;
            $$ = (Node*) i;
        }
    ;

expression
    : expression '+' expression
        {
            Binary_Operator* b = new_binary_operator();
            b->op = '+';
            b->left_expression = $1;
            b->right_expression = $3;
            $$ = (Node*) b;
        }
    | expression '-' expression
        {
            Binary_Operator* b = new_binary_operator();
            b->op = '-';
            b->left_expression = $1;
            b->right_expression = $3;
            $$ = (Node*) b;
        }
    | expression '>' expression
        {
            Binary_Operator* b = new_binary_operator();
            b->op = '>';
            b->left_expression = $1;
            b->right_expression = $3;
            $$ = (Node*) b;
        }
    | expression '<' expression
        {
            Binary_Operator* b = new_binary_operator();
            b->op = '<';
            b->left_expression = $1;
            b->right_expression = $3;
            $$ = (Node*) b;
        }
    | expression LTE expression
        {
            Binary_Operator* b = new_binary_operator();
            b->op = LTE;
            b->left_expression = $1;
            b->right_expression = $3;
            $$ = (Node*) b;
        }
    | expression GTE expression
        {
            Binary_Operator* b = new_binary_operator();
            b->op = GTE;
            b->left_expression = $1;
            b->right_expression = $3;
            $$ = (Node*) b;
        }
    | expression EQ expression
        {
            Binary_Operator* b = new_binary_operator();
            b->op = EQ;
            b->left_expression = $1;
            b->right_expression = $3;
            $$ = (Node*) b;
        }
    | '(' expression ')'
        {
            $$ = $2;
        }
    | NUMBER
    | STRING
    | IDENTIFIER
    | IDENTIFIER '(' function_call_args ')'
        {
            Function_Call* f = new_function_call();
            f->identifier = (Symbol*) $1; 
            f->arguments = (List*) $3;
            $$ = (Node*) f;
        }
    ;

function_call_args
    : expression  
        {
            List* l = new_list();
            l->head = $1;
            l->rest = 0;
            $$ = (Node*) l;
        }
    | function_call_args ',' expression
        {
            List* l = new_list();
            l->head = $3;
            l->rest = (List*) $1;
            $$ = (Node*) l;
        }
    ;

declaration
    : type list_of_identifiers 
        {
            Declaration* d = new_declaration();
            d->identifiers = (List*) $2;
            $$ = (Node*) d;
        }
    ;

list_of_identifiers
    : IDENTIFIER
        {
            List* l = new_list();
            l->head = $1;
            l->rest = 0;
            $$ = (Node*) l;
        }
    | IDENTIFIER ',' list_of_identifiers
        {
            List* l = new_list();
            l->head = $1;
            l->rest = (List*) $3;
            $$ = (Node*) l;
        }
    ;

assignment
    : IDENTIFIER '=' expression 
        {
            Assignment* a = new_assignment();
            a->l_value = $1;
            a->r_value = $3;
            $$ = (Node*) a;
        }
    ;

type
    : INT
    | FLOAT
    | VOID
    ;

%%
