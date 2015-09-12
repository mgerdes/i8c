%{
int yyparse(void);

int yylex();

void yyerror(const char *str)
{
	fprintf(stderr,"error: %s\n",str);
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
        
    
