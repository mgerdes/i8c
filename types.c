#include <stdlib.h>
#include "i8c.h"

Type* new_type(int size) {
    Type* t = malloc(sizeof(Type));
    t->size = size;
    t->is_struct = 0;
    return t;
}
