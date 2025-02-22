/*my own stringmap*/
#pragma once 

#define key_len 256

#include "vector.h"


struct stringmap
{
    char key[key_len];
    void* value;

};
