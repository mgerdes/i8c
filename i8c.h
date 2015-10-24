enum {
    LIST, SYMBOL, FUNC_DEF, WHILE_LOOP, IF_ELSE,
    DECLARATION, ASSIGNMENT, CONSTANT, FUNC_CALL,
    BINARY_OP, RETURN_STATEMENT, REFERENCE, DEREFERENCE
};

enum {
    INT_CONSTANT, FLOAT_CONSTANT, STRING_CONSTANT
};

typedef struct Node {
    int kind;
} Node;

typedef struct List {
    int kind;
    struct Node* head;
    struct List* rest;
} List;

typedef struct Symbol {
    int kind;
    int offset;
    char* name;
    struct Type* type;
} Symbol;

typedef struct Function_Definition {
    int kind;
    struct Type* return_type;
    struct Symbol* identifier;
    struct List* parameter_declarations;
    struct Node* statements;
} Function_Definition;

typedef struct While_Loop {
    int kind;
    struct Node* expression;
    struct Node* statements;
} While_Loop;

typedef struct If_Else {
    int kind;
    struct Node* expression;
    struct Node* statements;
} If_Else;

typedef struct Declaration {
    int kind;
    struct Type* type;
    struct List* identifiers;
} Declaration;

typedef struct Assignment {
    int kind;
    struct Node* l_value;
    struct Node* r_value;
} Assignment;

typedef struct Constant {
    int kind;
    int constant_kind;
    union {
        float float_value;
        int int_value;
        char* string_value;
    };
} Constant;

typedef struct Function_Call {
    int kind;
    struct Symbol* identifier;
    struct List* arguments;
} Function_Call;

typedef struct Binary_Operator {
    int kind;
    int op;
    struct Node* left_expression;
    struct Node* right_expression;
} Binary_Operator;

typedef struct Return {
    int kind;
    struct Node* expression;
} Return;

typedef struct Reference {
    int kind;
    struct Symbol* symbol;
} Reference;

typedef struct Dereference {
    int kind;
    struct Node* expression;
} Dereference;

Node* new_node();
List* new_list();
Symbol* new_symbol();
Function_Definition* new_function_definition();
While_Loop* new_while_loop();
If_Else* new_if_else();
Declaration* new_declaration();
Assignment* new_assignment();
Constant* new_constant();
Function_Call* new_function_call();
Binary_Operator* new_binary_operator();
Return* new_return();
Reference* new_reference();
Dereference* new_dereference();

typedef struct Environment_Element {
    struct Symbol* symbol;
    struct Environment_Element* next_element;
} Environment_Element;

typedef struct Environment {
    int total_offset;
    struct Environment* parent_environment;        
    struct Environment_Element* first_element;
} Environment;

void put_symbol(Environment* env, Symbol* symbol); 
Symbol* get_symbol(Environment* env, char* name); 
void push_new_environment();
void pop_environment();
Environment* top_environment();

void print_ast(Node* ast);
void gen_code(Node* ast);
void init_code_gen(char* file_name);

Node* program;
