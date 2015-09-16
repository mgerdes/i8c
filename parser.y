%{
#define YYSTYPE Node*
#include "i8c.h"

Environment* current_environment;

int yyparse(void);
int yylex();
void yyerror(const char *str) {
    fprintf(stderr, "Line %d: %s \"%s\"\n", yylineno, str, yytext);
}
%}

%token INT RETURN
%token IDENTIFIER NUMBER

%left '='
%right '+'
%right '-'

%%

start
    :   { 
            current_environment = new_environment(); 
        } 
       program
    ;

program
    : function_definition 
        {
            print_ast($1);
        }
      program
    | declaration ';'
        {
            print_ast($1);
        }
      program
    | 
    ;

function_definition
    : INT IDENTIFIER '(' ')' 
        {
            put_symbol(current_environment, $2);

            Environment* new_env = new_environment();
            new_env->parent_environment = current_environment;
            current_environment = new_env;
        }
      '{' block '}' 
        {
            Node* function = new_node();
            function->kind = KIND_FUNC;
            function->body_node = $7;
            function->symbol = $2;
            $$ = function;

            current_environment = current_environment->parent_environment;
        }
    ;

block
    : statement block
        {
            Node* block = new_node();
            block->kind = KIND_BLOCK;
            block->left_node = $1;
            block->right_node = $2;
            $$ = block;
        }
    | 
        {
            $$ = 0;
        }
    ;

statement
    : RETURN expression ';' 
        {
            Node* return_stmt = new_node();
            return_stmt->kind = KIND_RETURN;
            return_stmt->return_node = $2;
            $$ = return_stmt;
        }
    | declaration ';'
        {
            $$ = $1;
        }
    | assignment ';'
    | expression ';' 
        {
            $$ = $1;
        }
    ;

expression
    : expression '+' expression
        {
            Node* add_node = new_node();
            add_node->kind = KIND_ADD;
            add_node->left_node = $1;
            add_node->right_node = $3;
            $$ = add_node;
        }
    | expression '-' expression
        {
            Node* add_node = new_node();
            add_node->kind = KIND_SUB;
            add_node->left_node = $1;
            add_node->right_node = $3;
            $$ = add_node;
        }
    | NUMBER
        {
            $$ = $1;
        }
    | IDENTIFIER
        {
            Node* symbol = get_symbol(current_environment, $1);
            if (!symbol) {
                yyerror("Could not find symbol");
                YYABORT;
            }
            $$ = symbol;
        }
    | IDENTIFIER '(' ')'
        {
            Node* symbol = get_symbol(current_environment, $1); 
            if (!symbol) {
                yyerror("Could not find symbol");
                YYABORT;
            }

            Node* fn_call = new_node();
            fn_call->kind = KIND_FUNC_CALL;
            fn_call->symbol = symbol;

            $$ = fn_call;
        }
    ;

declaration
    : INT IDENTIFIER '=' expression 
        {
            $2->type = type_int;
            
            if (get_type($4) != type_int) {
                yyerror("Type Error");
            } 

            put_symbol(current_environment, $2);
            
            Node* declaration_stmt = new_node();
            declaration_stmt->kind = KIND_DECLARATION;
            declaration_stmt->symbol = $2; 
            declaration_stmt->right_node = $4; 
            $$ = declaration_stmt;
        }
    | INT IDENTIFIER 
        {
            $2->type = type_int;

            put_symbol(current_environment, $2);
            
            Node* declaration_stmt = new_node();
            declaration_stmt->kind = KIND_DECLARATION;
            declaration_stmt->symbol = $2; 
            declaration_stmt->right_node = 0; 
            $$ = declaration_stmt;
        }
    | INT IDENTIFIER '[' NUMBER ']'
        {
            $2->type = make_array_type(type_int, $4->i_value);
            put_symbol(current_environment, $2);

            Node* array_declaration = new_node();
            array_declaration->kind = KIND_DECLARATION;
            array_declaration->symbol = $2;
            $$ = array_declaration; 
        }
    ;

assignment
    : IDENTIFIER '=' expression 
        {
            Node* symbol = get_symbol(current_environment, $1);

            if (!symbol) {
                yyerror("Could not find symbol");
                YYABORT;
            }
            
            if (get_type($3) != symbol->type) {
                yyerror("Type Error");
            } 

            Node* assignment_stmt = new_node();
            assignment_stmt->kind = KIND_ASSIGNMENT;
            assignment_stmt->symbol = symbol; 
            assignment_stmt->right_node = $3; 
            $$ = assignment_stmt;
        }
    ;

%%
