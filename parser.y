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

program:
