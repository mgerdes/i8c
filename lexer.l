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

"if"        { return IF; }
"else"      { return ELSE; }
"int"       { return INT; }
"char"      { return CHAR; }
"float"     { return FLOAT; }
"void"      { return VOID; }
"while"     { return WHILE; }
"for"       { return FOR; }
"return"    { return RETURN; }
"struct"    { return STRUCT; }

"!"         { return '!'; }
"+"         { return '+'; }
"&"         { return '&'; }
"-"         { return '-'; }
"*"         { return '*'; }
"/"         { return '/'; }
"="         { return '='; }
">"         { return '>'; }
">="        { return GTE; }
"<"         { return '<'; }
"<="        { return LTE; }
"=="        { return EQ; }

";"         { return ';'; }
","         { return ','; }
"{"         { return '{'; }
"}"         { return '}'; }
"("         { return '('; }
")"         { return ')'; }
"["         { return '['; }
"]"         { return ']'; }


\"(\\.|[^"])*\" {
                    Constant* c = new_constant();
                    c->constant_kind = STRING_CONSTANT;
                    c->string_value = strdup(yytext);
                    yylval = (Node*) c;

                    return STRING;
                };

{L}{A}*     { 
                Symbol* s = new_symbol();
                s->name = strdup(yytext);
                yylval = (Node*) s;

                return IDENTIFIER; 
            };
{D}*        { 
                Constant* n = new_constant();
                n->constant_kind = INT_CONSTANT;
                n->int_value = atoi(yytext);
                yylval = (Node*) n;

                return NUMBER; 
            };
({D}*\.{D}*)|({D}*\.)|(\.{D}*) {
                Constant* f = new_constant();
                f->constant_kind = FLOAT_CONSTANT;
                f->int_value = atof(yytext);
                yylval = (Node*) f;

                return NUMBER; 
            };

%%
