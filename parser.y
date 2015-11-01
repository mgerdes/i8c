%{
#define YYSTYPE Node*
#include "i8c.h"

Environment* struct_env;
int yyparse(void);
int yylex();
void yyerror(const char *str) {
    fprintf(stderr, "Line %d: %s \"%s\"\n", yylineno, str, yytext);
}
%}

%token FLOAT INT CHAR VOID RETURN WHILE IF ELSE FOR IDENTIFIER NUMBER STRING STRUCT

%left '=' 
%left '<' '>' LTE GTE EQ NEQ
%right '+' '-'
%right '*' '/' '%'
%right '!' 
%precedence NEG

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
    | struct_definition program
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
            ((Symbol*) $2)->type = new_type(0);
            ((Symbol*) $2)->type->is_func = 1;
            f->identifier = (Symbol*) $2;
            f->parameter_declarations = (List*) $4;
            f->statements = $7;
            $$ = (Node*) f;
        }
    | type IDENTIFIER '(' function_definition_args ')' ';'
        {
            Function_Definition* f = new_function_definition();
            ((Symbol*) $2)->type = new_type(0);
            ((Symbol*) $2)->type->is_func = 1;
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

            ((Symbol*) $2)->type = (Type*) $1;
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

            ((Symbol*) $2)->type = (Type*) $1;
            l1->head = (Node*) $2;

            Declaration* d = new_declaration();
            d->identifiers = l1;

            List* function_parameters = new_list();
            function_parameters->head = (Node*) d;
            function_parameters->rest = (List*) $4;

            $$ = (Node*) function_parameters;
        }
    ;

struct_definition
    : STRUCT IDENTIFIER '{' list_of_declarations '}' ';'
        {
            if (!struct_env) {
                struct_env = new_environment();
            }

            Struct* s = new_struct();

            s->symbol = (Symbol*) $2;
            s->symbol->type = new_type(0);
            s->symbol->type->member_env = new_environment();
            s->symbol->type->is_struct = 1;

            List* declarations = (List*) $4;
            while (declarations) {
                List* identifiers = ((Declaration*) declarations->head)->identifiers;
                while (identifiers) {
                    put_symbol(s->symbol->type->member_env, (Symbol*) identifiers->head);
                    identifiers = identifiers->rest;
                }
                declarations = declarations->rest;
            }

            s->symbol->type->size = s->symbol->type->member_env->total_offset;

            put_symbol(struct_env, s->symbol);
            $$ = (Node*) s;
        }
    ;

list_of_declarations
    : 
        {
            $$ = 0;
        }
    | declaration ';' list_of_declarations
        {
            List* l = new_list();
            l->head = $1;
            l->rest = (List*) $3;
            $$ = (Node*) l;
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
    | RETURN ';'
         {
            Return* r = new_return();
            r->expression = 0;
            $$ = (Node*) r;
         }
    | while_loop
    | for_loop
    | if_statement 
    | declaration ';'
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

for_loop
    : FOR '(' expression ';' expression ';' expression ')' '{' block '}'
        {
            List* l1 = new_list();
            l1->head = $7;
            l1->rest = 0;

            ((List*) $10)->rest = l1;

            While_Loop* w = new_while_loop();
            w->expression = $5; 
            w->statements = $10; 

            List* l2 = new_list(); 
            l2->head = $3;
            l2->rest = (List*) w;

            $$ = (Node*) l2;
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
    | expression '*' expression
        {
            Binary_Operator* b = new_binary_operator();
            b->op = '*';
            b->left_expression = $1;
            b->right_expression = $3;
            $$ = (Node*) b;
        }
    | expression '/' expression
        {
            Binary_Operator* b = new_binary_operator();
            b->op = '/';
            b->left_expression = $1;
            b->right_expression = $3;
            $$ = (Node*) b;
        }
    | expression '%' expression
        {
            Binary_Operator* b = new_binary_operator();
            b->op = '%';
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
    | expression NEQ expression
        {
            Binary_Operator* b = new_binary_operator();
            b->op = EQ;
            b->left_expression = $1;
            b->right_expression = $3;

            Negation* n = new_negation();
            n->expression = (Node*) b;

            $$ = (Node*) n;
        }
    | '(' expression ')'
        {
            $$ = $2;
        }
    | '!' expression
        {
            Negation* n = new_negation();
            n->expression = $2;
            $$ = (Node*) n;
        }
    | IDENTIFIER '.' IDENTIFIER
        {
            Member_Lookup* m = new_member_lookup();
            m->struct_symbol = (Symbol*) $1;
            m->member_symbol = (Symbol*) $3;
            $$ = (Node*) m;
        }
    | assignment
    | unary_minus
    | reference
    | dereference
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

unary_minus
    : '-' expression %prec NEG
        {
            Binary_Operator* b = new_binary_operator();
            b->op = '-';

            Constant* zero = new_constant();           
            zero->constant_kind = INT_CONSTANT;
            zero->int_value = 0;

            b->left_expression = (Node*) zero;
            b->right_expression = $2;
            $$ = (Node*) b;
        }
    ;

function_call_args
    : 
        {
            $$ = 0;
        }
    | expression  
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
                
            List* identifiers = (List*) $2;
            while (identifiers) {
                ((Symbol*) identifiers->head)->type = (Type*) $1;
                identifiers = identifiers->rest;
            }

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
    | IDENTIFIER '.' IDENTIFIER '=' expression
        {
            Member_Lookup* m = new_member_lookup();
            m->struct_symbol = (Symbol*) $1;
            m->member_symbol = (Symbol*) $3;
            
            Assignment* a = new_assignment();
            a->l_value = (Node*) m;
            a->r_value = $5;
            $$ = (Node*) a;
        }
    ;

reference
    : '&' IDENTIFIER 
        {
            Reference* r = new_reference();
            r->symbol = (Symbol*) $2;
            $$ = (Node*) r;
        }

dereference
    : '*' expression
        {
            Dereference* d = new_dereference();
            d->expression = $2;
            $$ = (Node*) d;
        }

type
    : INT
        {
            $$ = (Node*) new_type(4);
        }
    | FLOAT
        {
            $$ = (Node*) new_type(4);
        }
    | VOID
        {
            $$ = (Node*) new_type(0);
        }
    | CHAR
        {
            $$ = (Node*) new_type(4);
        }
    | type '*'
        {
            $$ = (Node*) new_type(4);
        }
    | STRUCT IDENTIFIER
        {
            Symbol* s = get_symbol(struct_env, ((Symbol*) $2)->name);
            if (!s) {
                printf("Invalid struct name, %s\n", ((Symbol*) $2)->name);
            } 
            $$ = (Node*) s->type;            
        }
    ;

%%
