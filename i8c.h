enum {
    KIND_FUNC,
    KIND_INT,
    KIND_RETURN,
    KIND_ADD,
    KIND_SUB,
};

typedef struct Node {
    int kind;
    long i_value;
    struct Node* body_node;
    struct Node* return_node;
    struct Node* left_node;
    struct Node* right_node;
} Node;

Node* new_node();
void print_ast(Node* ast);
