%{
int yyparse(void);

int yylex();

void yyerror(const char *str)
{
    fprintf(stderr, "%d: %s at \"%s\"\n", yylineno, str, yytext);
}
%}

%token INT RETURN
%token IDENTIFIER NUMBER

%%

program
    : function_definition program
    | 
    ;

function_definition
    : INT IDENTIFIER '(' ')' '{' RETURN NUMBER ';' '}'
    ;
        
    
