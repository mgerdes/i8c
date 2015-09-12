#include <stdio.h>
#include "parser.tab.h"
#include "i8c.h"

extern FILE *yyin;

int main(int argc, char* argv[]) {
    char *file_name;
    
    if (argc > 1) {
        file_name = argv[1];
        yyin = fopen(file_name, "r");
    }

    yyparse();
}
