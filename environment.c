#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "i8c.h"

Environment* cur_environment;

Environment* new_environment() {
    Environment* new_environment = malloc(sizeof(Environment));
    new_environment->total_offset = 4;
    return new_environment;
}

Environment_Element* new_element() {
    Environment_Element* element = malloc(sizeof(Environment_Element));
    return element;
}

void push_new_environment() {
    Environment* env = new_environment();
    env->parent_environment = cur_environment;
    cur_environment = env;
}

void pop_environment() {
    Environment_Element* cur_element = cur_environment->first_element;
    while (cur_element) {
        cur_element = cur_element->next_element;
    }
    cur_environment = cur_environment->parent_environment;     
}

Environment* top_environment() {
    return cur_environment;
}

void put_symbol(Environment* env, Symbol* symbol) {
    Environment_Element* element = new_element();    
    symbol->offset = env->total_offset;
    env->total_offset += 4;
    element->symbol = symbol;

    Environment_Element* cur_element = env->first_element;
    if (cur_element == 0) {
        env->first_element = element;
        return;
    }
    while (cur_element->next_element != 0) {
        cur_element = cur_element->next_element; 
    }
    cur_element->next_element = element;
}

Symbol* get_symbol(Environment* env, char* name) {
    Environment_Element* cur_element = env->first_element;     

    while (cur_element != 0) {
        if (strcmp(name, cur_element->symbol->name) == 0) {
            return cur_element->symbol;
        } 
        cur_element = cur_element->next_element;
    }

    if (env->parent_environment) {
        return get_symbol(env->parent_environment, name);
    }

    return 0;
}
