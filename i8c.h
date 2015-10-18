enum {
    KIND_FUNC,
    KIND_CONSTANT,
    KIND_RETURN,
    KIND_SYMBOL,
    KIND_ASSIGNMENT,
    KIND_BLOCK,
    KIND_DECLARATION,
    KIND_FUNC_CALL,
    KIND_WHILE,
    KIND_FOR,
    KIND_IF,
    KIND_BIN_OP,
    KIND_FUNC_CALL_ARGS,
    KIND_PROGRAM,
    KIND_IDENTIFIER_LIST,
    KIND_STRING,
    KIND_FUNC_DEF
};

enum {
    LIST, SYMBOL, FUNC_DEF, WHILE_LOOP, IF_ELSE,
    DECLARATION, ASSIGNMENT, CONSTANT, FUNC_CALL,
    BINARY_OP
};

enum {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_ARRAY,
    TYPE_VOID,
};

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
    struct Symbol* name;
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
    struct Node* statements_if_true;
    struct Node* statements_if_false;
} If_Else;

typedef struct Declaration {
    int kind;
    struct Type* type;
    struct List* variables;
} Declaration;

typedef struct Assignment {
    int kind;
    struct Node* l_value;
    struct Node* r_value;
} Assignment;

typedef struct Constant {
    int kind;
    union {
        float float_value;
        int int_value;
        char* string_value;
    };
} Constant;

typedef struct Function_Call {
    int kind;
    struct Symbol* symbol;
    struct List* function_arguments;
} Function_Call;

typedef struct Binary_Operator {
    int kind;
    struct Node* left_expression;
    struct Node* right_expression;
} Binary_Operator;

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

typedef struct Node {
    int kind;
    int op;
    int offset;
    long i_value;
    double f_value;
    char* s_value;
    char* symbol_name;

    struct Type* type;

    struct Node* symbol;
    struct Node* body_node;

    struct Node* return_node;

    struct Node* left_node;
    struct Node* right_node;
} Node;

Node* program;
Node* new_node();
void print_ast(Node* ast);

typedef struct Environment_Element {
    Node* symbol;
    char* symbol_name;
    struct Environment_Element* next_element;
} Environment_Element;

typedef struct Environment {
    struct Environment* parent_environment;        
    Environment_Element* first_element;
    int total_offset;
} Environment;

void put_symbol(Environment* env, Node* symbol); 
Node* get_symbol(Environment* env, Node* symbol); 
void push_new_environment();
void pop_environment();
Environment* top_environment();
void print_environment(Environment* env);

typedef struct Type {
    int kind;
    int size;

    struct Type* ptr; 
} Type;

Type* make_array_type(Type* type, int array_size);
Type* get_type(Node* e);

extern Type *type_int;
extern Type *type_float;
extern Type *type_void;

void init_code_gen(char*);
void write_header();
void write_footer();
void gen_code(Node* ast); 
