%{
#define YYSTYPE Node*
#include "i8c.h"

int yyparse(void);
int yylex();
void yyerror(const char *str)
{
    fprintf(stderr, "Line %d: %s at \"%s\"\n", yylineno, str, yytext);
}
%}

%token INT RETURN
%token IDENTIFIER NUMBER

%right '+'
%right '-'

%%

program
    : function_definition program
        {
            print_ast($1);
        }
    | 
    ;

function_definition
    : INT IDENTIFIER '(' ')' '{' block '}' {
            Node* function = new_node();
            function->kind = KIND_FUNC;
            function->body_node = $6;
            $$ = function;
        }
    ;

block
    : statement block
        {
            $$ = $1;
        }
    |
    ;

statement
    : RETURN expression ';' 
        {
            Node* return_stmt = new_node();
            return_stmt->kind = KIND_RETURN;
            return_stmt->return_node = $2;
            $$ = return_stmt;
        }
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
    ;

%%
