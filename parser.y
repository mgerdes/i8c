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

%token FLOAT INT RETURN WHILE IF ELSE
%token IDENTIFIER NUMBER
%token LTE GTE

%left '='
%left '<' '>' LTE GTE
%right '+' '-'

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
    : type IDENTIFIER '(' function_definition_args ')' 
        {
            Node* function_definition_args = $4; 
            while (function_definition_args) {
                put_symbol(current_environment, function_definition_args); 
                function_definition_args = function_definition_args->right_node;
            }
            $2->left_node = $4;
            $2->type = $1->type;

            put_symbol(current_environment, $2);

            Environment* new_env = new_environment();
            new_env->parent_environment = current_environment;
            current_environment = new_env;
        }
      '{' block '}' 
        {
            Node* function = new_node();
            function->kind = KIND_FUNC;
            function->left_node = $4;
            function->body_node = $8;
            function->symbol = $2;
            $$ = function;

            current_environment = current_environment->parent_environment;
        }
    ;

function_definition_args
    :  
        {
            $$ = 0;
        }
    | type IDENTIFIER
        {
            Node* function_definition_args = new_node();
            function_definition_args->kind = KIND_SYMBOL;
            function_definition_args->type = $1->type; 
            function_definition_args->symbol_name = $2->symbol_name;
            function_definition_args->right_node = 0;
            $$ = function_definition_args;
        }
    | type IDENTIFIER ',' function_definition_args
        {
            Node* function_definition_args = new_node();
            function_definition_args->kind = KIND_SYMBOL;
            function_definition_args->type = $1->type; 
            function_definition_args->symbol_name = $2->symbol_name;
            function_definition_args->right_node = $4;
            $$ = function_definition_args;
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
    | '{' 
        {
            // create new environment for inner block
            Environment* new_env = new_environment();
            new_env->parent_environment = current_environment;
            current_environment = new_env;
        }
      block '}' 
        {
            // restore old environment on leaving inner block 
            current_environment = current_environment->parent_environment;
        }
      block
        {
            Node* block = new_node();
            block->kind = KIND_BLOCK;
            block->left_node = $3;
            block->right_node = $6;
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
    | while_loop
    | if_statement 
    | declaration ';'
    | assignment ';'
    | expression ';' 
    ;

while_loop
    : WHILE '(' expression ')' '{' 
        {
            Environment* new_env = new_environment();
            new_env->parent_environment = current_environment;
            current_environment = new_env;
        }
      block '}'
        {
            Node* while_loop = new_node();
            while_loop->kind = KIND_WHILE;
            while_loop->left_node = $3;
            while_loop->right_node = $7;

            current_environment = current_environment->parent_environment;

            $$ = while_loop;
        }
    ;

if_statement
    : IF '(' expression ')' '{' 
        {
            Environment* new_env = new_environment();
            new_env->parent_environment = current_environment;
            current_environment = new_env;
        }
      block '}'
        {
            Node* if_statement = new_node();
            if_statement->kind = KIND_IF;
            if_statement->left_node = $3;
            if_statement->right_node = $7;
            
            current_environment = current_environment->parent_environment;

            $$ = if_statement;
        }
    ;

expression
    : expression '+' expression
        {
            Node* add_node = new_node();
            add_node->kind = KIND_BIN_OP;
            add_node->op = '+';
            add_node->left_node = $1;
            add_node->right_node = $3;
            $$ = add_node;
        }
    | expression '-' expression
        {
            Node* sub_node = new_node();
            sub_node->kind = KIND_BIN_OP;
            sub_node->op = '-';
            sub_node->left_node = $1;
            sub_node->right_node = $3;
            $$ = sub_node;
        }
    | boolean_expression
    | NUMBER
    | IDENTIFIER
        {
            Node* symbol = get_symbol(current_environment, $1);
            if (!symbol) {
                yyerror("Could not find symbol");
                YYABORT;
            }
            $$ = symbol;
        }
    | IDENTIFIER '(' function_call_args ')'
        {
            Node* symbol = get_symbol(current_environment, $1); 
            if (!symbol) {
                yyerror("Could not find symbol");
                YYABORT;
            }

            Node* function_definition_args = symbol->left_node;
            Node* function_call_args = $3;
            while (function_definition_args && function_call_args) {
                function_definition_args = function_definition_args->right_node;
                function_call_args = function_call_args->right_node;
            }
            if (function_call_args || function_definition_args) {
                yyerror("Wrong number of arguments passed to function");
                YYABORT;
            }

            Node* fn_call = new_node();
            fn_call->kind = KIND_FUNC_CALL;
            fn_call->symbol = symbol;

            $$ = fn_call;
        }
    ;

function_call_args
    :   
        {
            $$ = 0;
        }
    | expression ',' function_call_args
        {
            Node* function_call_args = new_node();
            function_call_args->kind = KIND_SYMBOL;
            function_call_args->left_node = $1;
            function_call_args->right_node = $3;
            $$ = function_call_args;
        }
    | expression
        {
            Node* function_call_args = new_node();
            function_call_args->kind = KIND_SYMBOL;
            function_call_args->left_node = $1;
            function_call_args->right_node = 0;
            $$ = function_call_args;
        }
    ;

boolean_expression
    : expression '>' expression
        {
            Node* bin_op = new_node();
            bin_op->kind = KIND_BIN_OP;
            bin_op->left_node = $1;
            bin_op->right_node = $3;
            bin_op->op = '>';
            $$ = bin_op;
        }
    | expression '<' expression
        {
            Node* bin_op = new_node();
            bin_op->kind = KIND_BIN_OP;
            bin_op->left_node = $1;
            bin_op->right_node = $3;
            bin_op->op = '<';
            $$ = bin_op;
        }
    | expression LTE expression
        {
            Node* bin_op = new_node();
            bin_op->kind = KIND_BIN_OP;
            bin_op->left_node = $1;
            bin_op->right_node = $3;
            bin_op->op = LTE;
            $$ = bin_op;
        }
    | expression GTE expression
        {
            Node* bin_op = new_node();
            bin_op->kind = KIND_BIN_OP;
            bin_op->left_node = $1;
            bin_op->right_node = $3;
            bin_op->op = GTE;
            $$ = bin_op;
        }
    ;

declaration
    : type IDENTIFIER '=' expression 
        {
            $2->type = $1->type;
            
            put_symbol(current_environment, $2);
            
            Node* declaration_stmt = new_node();
            declaration_stmt->kind = KIND_DECLARATION;
            declaration_stmt->symbol = $2; 
            declaration_stmt->right_node = $4; 
            $$ = declaration_stmt;
        }
    | type IDENTIFIER 
        {
            $2->type = $1->type;

            put_symbol(current_environment, $2);
            
            Node* declaration_stmt = new_node();
            declaration_stmt->kind = KIND_DECLARATION;
            declaration_stmt->symbol = $2; 
            declaration_stmt->right_node = 0; 
            $$ = declaration_stmt;
        }
    | type IDENTIFIER '[' NUMBER ']'
        {
            $2->type = make_array_type($1->type, $4->i_value);
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

            Node* assignment_stmt = new_node();
            assignment_stmt->kind = KIND_ASSIGNMENT;
            assignment_stmt->symbol = symbol; 
            assignment_stmt->right_node = $3; 
            $$ = assignment_stmt;
        }
    ;

type
    : INT
        {
            Node* type = new_node();
            type->type = type_int;
            $$ = type;
        }
    | FLOAT
        {
            Node* type = new_node();
            type->type = type_float;
            $$ = type;
        }
    ;

%%
