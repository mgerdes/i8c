#include <stdlib.h>
#include "i8c.h"

Type* new_type(int size) {
    Type* t = malloc(sizeof(Type));
    t->kind = TYPE_TYPE;
    t->size = size;
    t->is_struct = 0;
    t->is_func = 0;
    return t;
}
