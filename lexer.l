%{
#include "parser.tab.c"    

void comment();

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
";"         { return ';'; }
"{"         { return '{'; }
"}"         { return '}'; }
"("         { return '('; }
")"         { return ')'; }
{L}{A}*     { return IDENTIFIER; };
{D}*        { return NUMBER; };

%%
