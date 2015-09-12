%{
#include "parser.tab.c"    

void comment();

void yyerror();
%}

L [a-zA-Z_]
D [0-9] 
A {L}|{D}

%%

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
