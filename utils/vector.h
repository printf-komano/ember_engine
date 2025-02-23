#pragma once


#include  <stdlib.h>
#include <string.h>


typedef struct  {
    void * data;
    size_t elem_size; //element size
    size_t len; //number of elements
    size_t capacity;
}vec;

/// @brief create a new vector
/// @param elem_size number of elements 
/// @param capacity inital memory allocated (doesn't affect len)
/// @return empty vector
vec vec_alloc(size_t elem_size, size_t capacity){
    vec v; 
    v.data = malloc(capacity*elem_size);
    v.elem_size = elem_size;
    v.len = 0;
    v.capacity = capacity;
    return v;
}

//push value in the vector 
void* vec_push(vec *v, void *value){
    if(v->len >= v->capacity){
        v->capacity *= 2;
        v->data = realloc(v->data,v->capacity*v->elem_size);
    }
    void * ptr = memcpy((char*)v->data + v->len*v->elem_size,value,v->elem_size);
    ++v->len;
    return ptr;
}

void vec_pop(vec * v){ v->len -= v->len>0; }


void vec_clear(vec * v){v->len=0;}

void vec_free(vec * v){
    free(v->data);
    v->data = NULL;
    v->len = 0;
    v->capacity = 0;
}

/// @brief a safe getter (slower)
void * vec_get(vec *v, size_t index) {
    if (index >= v->len) return NULL;
    return (char*)v->data + index * v->elem_size;
}

//macro for safe getter
#define VEC_GET(vec, type, index) (*(type*)vec_get(&(vec), (index)))

//macro to get value directly (without length check)
#define VEC_AT(v, type, index) (*(type*) ((char*)(v)->data + (index) * (v)->elem_size) )

//macro to get value directly (without length check)
#define VEC_GETPTR(v, type, index) ((type*) ((char*)(v)->data + (index) * (v)->elem_size) )