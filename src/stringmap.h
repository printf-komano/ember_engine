/*my own stringmap*/
#pragma once 

#define key_len 32

#include "vector.h"


typedef struct
{
    char key[key_len];
    void* value;

}strmap_pair;


typedef vec strmap;

strmap strmap_alloc(size_t cap){
    return vec_alloc(sizeof(strmap_pair),cap);
}



