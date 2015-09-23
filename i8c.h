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
};

enum {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_ARRAY,
};

typedef struct Node {
    int kind;
    int op;
    long i_value;
    double f_value;
    char* symbol_name;

    struct Type* type;

    struct Node* symbol;
    struct Node* body_node;

    struct Node* return_node;

    struct Node* left_node;
    struct Node* right_node;
} Node;

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
