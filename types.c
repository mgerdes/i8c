#include <stdlib.h>
#include "i8c.h"

Type* type_int = &(Type){ TYPE_INT, 4 };
Type* type_float = &(Type){ TYPE_FLOAT, 4 };
Type* type_void = &(Type){ TYPE_VOID, 4 };

Type* make_array_type(Type* type, int array_size) {
    Type* new_type = malloc(sizeof(Type)); 
    new_type->kind = TYPE_ARRAY;
    new_type->ptr = type;
    new_type->size = array_size * type->size;
    return new_type;
}

Type* get_type(Node* e) {
    if (e->kind == KIND_BIN_OP) {
        Type* left_type = get_type(e->left_node);
        Type* right_type = get_type(e->right_node);
        if (left_type == type_float || right_type == type_float) {
            return type_float;
        } else {
            return type_int;
        }
    } else if (e->kind == KIND_FUNC_CALL) {
        return e->symbol->type;
    } else if (e->kind == KIND_SYMBOL || e->kind == KIND_CONSTANT) {
        return e->type;
    }
}
