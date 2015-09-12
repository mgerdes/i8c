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
"return"    { return RETURN; }

"+"         { return '+'; }
"-"         { return '-'; }
"*"         { return '*'; }
"/"         { return '/'; }

";"         { return ';'; }
"{"         { return '{'; }
"}"         { return '}'; }
"("         { return '('; }
")"         { return ')'; }

{L}{A}*     { return IDENTIFIER; };
{D}*        { 
                Node* number = new_node();
                number->kind = KIND_INT;
                number->i_value = atoi(yytext);
                yylval = number;

                return NUMBER; 
            };

%%
