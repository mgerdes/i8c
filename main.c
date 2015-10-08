#include <stdio.h>
#include <string.h>
#include "parser.tab.h"
#include "i8c.h"

extern FILE *yyin;

int main(int argc, char* argv[]) {
    char* file_name;
    
    if (argc > 1) {
        file_name = argv[1];
        yyin = fopen(file_name, "r");
    }

    yyparse();

    print_ast(program);

    printf("-----------------------\n");

    *(file_name + strlen(file_name) - 1) = 's';
    printf("%s\n", file_name);
    
    init_code_gen(file_name);
    write_header();
    // push the global environment
    push_new_environment();
    gen_code(program);
    write_footer();
}
