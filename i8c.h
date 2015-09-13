#define SYMBOL_TABLE_SIZE 20

enum {
    KIND_FUNC,
    KIND_CONSTANT,
    KIND_RETURN,
    KIND_ADD,
    KIND_SUB,
    KIND_SYMBOL,
    KIND_ASSIGNMENT,
    KIND_BLOCK,
    KIND_DECLARATION,
    KIND_FUNC_CALL,
};

enum {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_ARRAY,
};

typedef struct Node {
    int kind;
    long i_value;
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

Environment* new_environment();
void put_symbol(Environment* env, Node* symbol); 
Node* get_symbol(Environment* env, Node* symbol); 
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
