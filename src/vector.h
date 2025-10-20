#pragma once


#include  <stdlib.h>
#include <string.h>


typedef struct  {
    void * data;
    size_t elem_size; //element size
    size_t len; //number of elements
    size_t cap;
}vec;

/// @brief create a new vector
/// @param elem_size number of elements 
/// @param cap inital memory allocated (doesn't affect len)
/// @return empty vector
vec vec_alloc(size_t elem_size, size_t cap){
    vec v; 
    v.data = malloc(cap*elem_size);
    v.elem_size = elem_size;
    v.len = 0;
    v.cap = cap;
    return v;
}

//push value in the vector 
void* vec_push(vec *v, void *value){
    if(v->len >= v->cap){
        v->cap *= 2;
        v->data = realloc(v->data,v->cap*v->elem_size);
    }
    void * ptr = memcpy((char*)v->data + v->len*v->elem_size,value,v->elem_size);
    ++v->len;
    return ptr;
}

void vec_pop(vec * v){ v->len -= v->len>0; }

void vec_remove(vec * v, size_t index){ 
    if(index >= v->len) return;
    memmove(
        (char*)v->data + index*v->elem_size, //element at index will be removed
        (char*)v->data + (index+1)*v->elem_size, //next elementwill replace current
        (v->len-index)*v->elem_size //shift all next elements to left
    );
    --v->len;
}

void vec_clear(vec * v){v->len=0;}

void vec_free(vec * v){
    free(v->data);
    v->data = NULL;
    v->len = 0;
    v->cap = 0;
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










//specific vector type for float
typedef struct{
    float * data; //pointer to array
    size_t len; //length in elements 
    size_t cap; //cap in elements
} vec_float;


//allocationg a new float vec
vec_float vec_float_alloc(size_t cap){
    vec_float v;
    v.len = 0;
    v.cap = cap;
    v.data = malloc(cap *sizeof(float));
    return v;
};

float vec_float_get(vec_float * v, size_t index){
    return (index < v->len) ? v->data[index] : 0.0f;
}

void vec_float_clear(vec_float * v) {v->len = 0;}

void vec_float_free(vec_float * v){
    free(v->data);
    v->data = NULL;
    v->len = 0;
    v->cap = 0;
}

// get the value of float vector or zero ()if out of bounds
// vec_float * v - vector
#define VEC_FLOAT_GETVAL(v,index) ( (index<v->len && index>=0)? ((float)(v->data[index])) : 0.0f )

#define VEC_FLOAT_GETPTR(v,index) ( (index<v->len && index>=0)& ((float*)&v->data[index]) : NULL )


float* vec_float_push(vec_float *v, float value){
    if(v->len >= v->cap){
        v->cap *= 2;
        v->data = realloc(v->data,v->cap*sizeof(float));
    }
    v->data[v->len] = value;
    ++v->len;
    return &v->data[v->len-1];
}

float vec_float_pop(vec_float * v){
    v->len -= (v->len>0);
    return v->data[v->len];
}
