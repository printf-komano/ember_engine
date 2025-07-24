#pragma once

#include <glad/gl.h>
#include <stdio.h>
#include "utils/vector.h"
#include "model/model.h"


#define EMB_VB_PRIM_CAP 1024

typedef struct  //batch handler
{
    float * vb_data; //vertex buffer
    GLuint * vbo; //vbo reference
    uint32_t vb_len; //the actual number of bytes being used
    uint32_t vb_capacity; //all avilable bytes

    __uint32_t * eb_data; //element array
    GLuint * ebo; //ebo reference
    uint32_t eb_len; //the actual number of bytes being used
    uint32_t eb_capacity; //all avilable bytes
    vec primitives;
} emb_bhandler;

emb_bhandler emb_bhandler_init(
uint32_t vb_capacity/*in bytes*/, 
GLuint * vbo/*should be initalised by gl*/, 
uint32_t eb_capacity, 
GLuint * ebo 
){
    emb_bhandler bh; 

    bh.vbo = vbo;
    bh.vb_capacity = vb_capacity;
    bh.vb_data = (float*)malloc(vb_capacity);
    bh.vb_len = 0;

    bh.ebo = ebo;
    bh.eb_capacity = eb_capacity;
    bh.eb_data = (__uint32_t*)malloc(eb_capacity);
    bh.eb_len = 0;

    bh.primitives = vec_alloc(sizeof(emb_prim_inst),EMB_VB_PRIM_CAP);
    return bh;
}

//__________________________________________________
// direct vb managing
//__________________________________________________

//remove last n bytes
static void bhandler_vb_pop(emb_bhandler * bh, __uint32_t n){
    bh->vb_len -= n;
    if(bh->vb_len<0) bh->vb_len=0;
};
//push back n bytes
static float * bhandler_vb_push(emb_bhandler * bh, float * elem, __uint32_t n){
    if(bh->vb_len+n >= bh->vb_capacity) {printf("ERROR vb_push(): vb out of memory."); return NULL;}
    memcpy(bh->vb_data+(bh->vb_len/sizeof(float)),elem,n);
    bh->vb_len+=n;
    return (float*)(bh->vb_data+bh->vb_len);
};


/*`d bhandler_vb_remove(emb_bhandler * bh, __uint_32_t ind, __uint32_t pos){
}*/

//remove last n bytes
static void bhandler_eb_pop(emb_bhandler * bh, __uint32_t n){
    bh->eb_len -= n;
    if(bh->eb_len<0) bh->eb_len=0;
};
//push back n bytes
static __uint32_t * bhandler_eb_push(emb_bhandler * bh, __uint32_t * elem, __uint32_t n){
    if(bh->eb_len+n >= bh->eb_capacity) {printf("ERROR vb_push(): eb out of memory."); return NULL;}
    memcpy(bh->eb_data+(bh->eb_len/sizeof(__uint32_t)),elem,n);
    uint32_t * last_elem_addr = bh->eb_data + bh->eb_len; 
    bh->eb_len+=n;
    return last_elem_addr;
};


//number of elements for rendering
__uint32_t emb_bhandler_ecount_render(emb_bhandler * bh){   
    return bh->eb_len/sizeof(__uint32_t);
};




void emb_bhandler_free(emb_bhandler * bh){
    free(bh->vb_data);
    free(bh->eb_data);
    vec_free(&bh->primitives);
}


//__________________________________________________
// primitive instancing
//__________________________________________________

//creating the instance primitive
emb_prim_inst* emb_bhandler_instantiate(emb_bhandler * bh, emb_prim * primitive){
    emb_prim_inst instance;
    instance.primitive = primitive;

    instance.vb_start = bhandler_vb_push(bh,primitive->vb,primitive->vb_len);
    printf("instance vb_start: %d\n",instance.vb_start);
    instance.vb_len = primitive->vb_len;

    instance.eb_start = bhandler_eb_push(bh,primitive->eb,primitive->eb_len);
    instance.eb_len = primitive->eb_len;

    instance.parent = NULL;


    //disabled by default
    instance.shader_vertex_enabled = false;
    instance.shader_fragment_enabled = false;

    //glm_mat6_identity(instance.transform);
    prim_inst_def_trtansform(&instance);

    if(!instance.vb_start || !instance.eb_start) {printf("ERROR emb_bhandler_instantiate(): cannot place primitive instance in the buffer.\n");}
    
    emb_prim_inst* ret = (emb_prim_inst*)vec_push(&(bh->primitives),&instance);
    //printf("created instance in address: %d\n", ret);
    return ret;
}






//__________________________________________________
// primitive drawing
//__________________________________________________

void emb_bhandler_draw_all(emb_bhandler* bh){
}
