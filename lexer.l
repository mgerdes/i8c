%{
#define YYSTYPE Node*
#include "parser.tab.c"    

void yyerror();
%}
%option yylineno

L [a-zA-Z_]
D [0-9] 
A {L}|{D}

%%

[ \t\n]+ { /* Ignore whitespace */ }

"int"       { return INT; }
"float"     { return FLOAT; }
"return"    { return RETURN; }

"+"         { return '+'; }
"-"         { return '-'; }
"*"         { return '*'; }
"/"         { return '/'; }
"="         { return '='; }

";"         { return ';'; }
"{"         { return '{'; }
"}"         { return '}'; }
"("         { return '('; }
")"         { return ')'; }
"["         { return '['; }
"]"         { return ']'; }

{L}{A}*     { 
                Node* symbol = new_node();
                symbol->kind = KIND_SYMBOL;
                symbol->symbol_name = strdup(yytext);
                yylval = symbol;

                return IDENTIFIER; 
            };
{D}*        { 
                Node* number = new_node();
                number->kind = KIND_CONSTANT;
                number->type = type_int;
                number->i_value = atoi(yytext);
                yylval = number;

                return NUMBER; 
            };
({D}*\.{D}*)|({D}*\.)|(\.{D}*) {
                Node* number = new_node();
                number->kind = KIND_CONSTANT;
                number->type = type_float;
                number->f_value = atof(yytext);
                yylval = number;

                return NUMBER; 
            };

%%
