#define SYMBOL_TABLE_SIZE 20

enum {
    KIND_FUNC,
    KIND_INT,
    KIND_RETURN,
    KIND_ADD,
    KIND_SUB,
    KIND_SYMBOL,
    KIND_ASSIGNMENT,
    KIND_BLOCK,
};

typedef struct Node {
    int kind;
    long i_value;
    char* symbol_name;

    struct Node* symbol;
    struct Node* body_node;

    struct Node* return_node;

    struct Node* left_node;
    struct Node* right_node;
} Node;

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

Node* new_node();
void print_ast(Node* ast);
