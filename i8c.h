enum {
    LIST_TYPE, SYMBOL_TYPE, FUNC_DEF_TYPE, WHILE_LOOP_TYPE, IF_ELSE_TYPE,
    DECLARATION_TYPE, ASSIGNMENT_TYPE, CONSTANT_TYPE, FUNC_CALL_TYPE,
    BINARY_OP_TYPE, RETURN_STATEMENT_TYPE, REFERENCE_TYPE, DEREFERENCE_TYPE,
    NEGATION_TYPE, STRUCT_TYPE, TYPE_TYPE, MEMBER_LOOKUP_TYPE
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
    struct List* identifiers;
} Declaration;

typedef struct Assignment {
    int kind;
    struct Node* l_value;
    struct Node* r_value;
} Assignment;

typedef struct Member_Lookup {
    int kind;
    struct Node* l_value;
    struct Symbol* member_symbol;
} Member_Lookup;

typedef struct Constant {
    int kind;
    int constant_kind;
    union {
        float float_value;
        int int_value;
        char* string_value;
    };
    struct Type* type;
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

typedef struct Negation {
    int kind;
    struct Node* expression;
} Negation;

typedef struct Struct {
    int kind;
    struct Symbol* symbol;
} Struct;

Node* new_node();
List* new_list();
Symbol* new_symbol();
Function_Definition* new_function_definition();
While_Loop* new_while_loop();
If_Else* new_if_else();
Declaration* new_declaration();
Assignment* new_assignment();
Member_Lookup* new_member_lookup();
Constant* new_constant();
Function_Call* new_function_call();
Binary_Operator* new_binary_operator();
Return* new_return();
Reference* new_reference();
Dereference* new_dereference();
Negation* new_negation();
Struct* new_struct();

typedef struct Type {
    int kind;
    int size; 
    struct Environment* member_env;

    int is_struct;
    int is_func;

    int is_pointer;
    struct Type* dereferenced_type;
} Type;

Type* new_type(int);

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
Environment* new_environment();
void push_new_environment();
void pop_environment();
Environment* top_environment();

void print_ast(Node* ast);
void gen_code(Node* ast);
void init_code_gen(char* file_name);

Node* program;
