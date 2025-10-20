#pragma once

#include <glad/gl.h>
#include <stdio.h>
#include "vector.h"
#include "model.h"


#define EMB_VB_PRIM_CAP 1024

typedef struct  //eb/vb handler
{
    float * vb_data; //vertex buffer
    GLuint * vbo; //vbo reference
    uint32_t vb_len; //the actual number of ELEMENTS being used
    uint32_t vb_capacity; //all avilable ELEMENTS

    __uint32_t * eb_data; //element array
    GLuint * ebo; //ebo reference
    uint32_t eb_len; //the actual number of ELEMENTS being used
    uint32_t eb_capacity; //all avilable ELEMENTS
    vec primitives;
} emb_ebvb_handler;

emb_ebvb_handler emb_ebvb_handler_init(
uint32_t vb_capacity/*in ELEMENTS*/, 
GLuint * vbo/*should be initalised by gl*/, 
uint32_t eb_capacity/*in ELEMENTS*/, 
GLuint * ebo 
){
    emb_ebvb_handler bh; 

    bh.vbo = vbo;
    bh.vb_capacity = vb_capacity;
    bh.vb_data = (float*)malloc(vb_capacity*sizeof(float));
    bh.vb_len = 0;

    bh.ebo = ebo;
    bh.eb_capacity = eb_capacity;
    bh.eb_data = (__uint32_t*)malloc(eb_capacity*sizeof(__uint32_t));
    bh.eb_len = 0;

    bh.primitives = vec_alloc(sizeof(emb_primitive),EMB_VB_PRIM_CAP);
    return bh;
}





//__________________________________________________
// direct vb managing
//__________________________________________________

//remove last n elements
static void ebvb_handler_vb_pop(emb_ebvb_handler * bh, __uint32_t n){
    bh->vb_len -= n;
    if(bh->vb_len<0) bh->vb_len=0;
};

//push back n elements
static float * ebvb_handler_vb_push(emb_ebvb_handler * bh, float * elem, __uint32_t n){
    if(bh->vb_len+n >= bh->vb_capacity) {
        printf("ERROR vb_push(): vb out of memory.");
        return NULL;
    }

    //position of the new element
    __uint32_t offset = bh->vb_len;

    memcpy(
        bh->vb_data + offset,
        elem,
        n * sizeof(float)
    );

    //upd length
    bh->vb_len+=n;

    //return element pointer
    return (float*)(bh->vb_data+offset);
};


/*`d bhandler_vb_remove(emb_ebvb_handler * bh, __uint_32_t ind, __uint32_t pos){
}*/

//remove last n elements
static void ebvb_handler_eb_pop(emb_ebvb_handler * bh, __uint32_t n){
    bh->eb_len -= n;
    if(bh->eb_len<0) bh->eb_len=0;
};

//push back n elements
static __uint32_t * ebvb_handler_eb_push(emb_ebvb_handler * bh, __uint32_t * elem, __uint32_t n){
    if(bh->eb_len+n >= bh->eb_capacity) {
        printf("ERROR eb_push(): eb out of memory.");
        return NULL;
    }

    //position of the new element
    __uint32_t offset = bh->eb_len;

    memcpy(
        bh->eb_data + offset,
        elem,
        n * sizeof(__uint32_t)
    );

    //upd length
    bh->eb_len+=n;

    //return element pointer
    return bh->eb_data + offset; 
};


//number of elements for rendering
__uint32_t emb_ebvb_handler_ecount_render(emb_ebvb_handler * bh){   
    return bh->eb_len;
};




void emb_ebvb_handler_free(emb_ebvb_handler * bh){
    free(bh->vb_data);
    free(bh->eb_data);
    vec_free(&bh->primitives);
}


//__________________________________________________
// primitive instancing
//__________________________________________________

//creating the instance primitive
emb_primitive* emb_ebvb_handler_instantiate(emb_ebvb_handler * bh, emb_primitive_origin * primitive){
    emb_primitive instance;
    instance.primitive = primitive;

    instance.vb_start = ebvb_handler_vb_push(bh,primitive->vb,primitive->vb_len);
    //printf("instance vb_start: %d\n",instance.vb_start);
    instance.vb_len = primitive->vb_len;

    //get vertex index
    __uint32_t vertex_offset = (instance.vb_start - bh->vb_data)
        / (VB_ATTRIB_SIZE_MAX);
    //printf("VB elem offst: %d\n",vertex_offset);
    


    instance.eb_start = ebvb_handler_eb_push(bh,primitive->eb,primitive->eb_len);
    instance.eb_len = primitive->eb_len;

    for(size_t i=0; i<primitive->eb_len; ++i){
        instance.eb_start[i] += vertex_offset;
        //printf("\tElem %d\n",instance.eb_start[i]);
    }




    instance.parent = NULL;


    //disabled by default
    instance.shader_program_override = false;

    //glm_mat6_identity(instance.transform);
    prim_inst_def_trtansform(&instance);

    if(!instance.vb_start || !instance.eb_start) {printf("ERROR emb_ebvb_handler_instantiate(): cannot place primitive instance in the buffer.\n");}
    
    emb_primitive* ret = (emb_primitive*)vec_push(&(bh->primitives),&instance);
    //printf("created instance in address: %d\n", ret);
    return ret;
}






//__________________________________________________
// primitive drawing
//__________________________________________________

GLuint EMB_DEFAUL_SHADER = 1.0f;

void emb_ebvb_handler_draw_all(emb_ebvb_handler* bh){
}
