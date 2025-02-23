#pragma once

#include <SDL3/SDL.h>
#include <glad/gl.h>
#include <cglm/cglm.h>
#include <stdio.h>
#include "node.h"

//__________________________________________________
// vmodel - unique sample of the model
//__________________________________________________
typedef struct //model template
{
    bool use_vertex_colors; //takes 3 elements in buffer
    bool use_uv; //2 additional elements in the buffer
    float * vb; //local vertex buffer 
    uint32_t vb_len; //length of the original buffer (in bytes)

    uint32_t * eb; //local triangles buffer 
    uint32_t eb_len; //length of the original triangles buffer (in bytes)
} vmodel; 


//model used for debugging (vertices)
float _vmodel_test_rectangle_vertices[] = {
        -0.5f, -0.5f, -0.5f, 1.2f, 0.2f, 0.2f, 0,0, //0
        0.5f, -0.5f, -0.5f, 0.2f, 1.2f, 0.2f, 0,0, //1
        -0.5f, 0.5f, -0.5f, 0.2f, 0.2f, 1.2f, 0,0, //2
        0.5f, 0.5f, -0.5f, 1.2f, 0.2f, 0.2f, 0,0, //3


        -0.5f, -0.5f, 0.5f, 0.2f, 0.2f, 0.2f, 0,0, //4
        0.5f, -0.5f, 0.5f, 0.2f, 0.2f, 0.2f, 0,0, //5
        -0.5f, 0.5f, 0.5f, 0.2f, 0.2f, 0.2f, 0,0, //6
        0.5f, 0.5f, 0.5f, 0.2f, 0.2f, 0.2f, 0,0, //7
};
__uint32_t _vmodel_test_rectangle_elements[] = {
    1,0,2,
    1,2,3, //fron

    0,5,4,
    0,1,5, //top

    7,3,2,
    7,2,6,

    1,3,7,
    1,7,5,

    0,4,2,
    2,4,6,

    4,5,6,
    6,5,7,
};
//model used for debugging
vmodel vmodel_test_rectangle(){
    vmodel m;
    
    m.vb = _vmodel_test_rectangle_vertices;
    m.eb = _vmodel_test_rectangle_elements;
    //m.transform   
    m.use_vertex_colors = true;
    m.vb_len = sizeof(_vmodel_test_rectangle_vertices);
    m.eb_len = sizeof(_vmodel_test_rectangle_elements);
    return m;
}


//__________________________________________________
// vmodel_inst - instance of the model
//__________________________________________________
typedef struct //vertex model (instance)
{    
    vmodel * model; //reference to the origianl model

    /*
    If the model is created successfully, 
    it gets its own vertex buffer, independent of the unique model. 
    Several instances can share one place in buffer (same vb_start), for optimisation.
    */

    float * vb_start; //reference to the vertex buffer
    uint32_t vb_len; //length in the vertex buffer (in bytes)

    uint32_t * eb_start; //reference to the element buffer
    uint32_t eb_len; //length in the element buffer (in bytes)

    //mat4 transform; //model matrix
    vec3 pos;
    vec3 scale;
    vec3 rot;

    node * parent;

} vmodel_inst; 


void vmodel_inst_get_transform(vmodel_inst * vmi, mat4 m){
    glm_mat4_identity(m);
    glm_euler_xyz(vmi->rot,m);
    glm_scale(m,vmi->scale); //not affected by rotation
    glm_translated(m,vmi->pos); //not affected by scale rotation

    if(vmi->parent != NULL){
        mat4 parent_tr;
        node_get_transform(vmi->parent,parent_tr);
        glm_mat4_mul(parent_tr,m,m);
    }
};

void vmodel_inst_def_trtansform(vmodel_inst * vmi){
    vmi->pos[0] = 0.0f; vmi->pos[1] = 0.0f; vmi->pos[2] = 0.0f;
    vmi->rot[0] = 0.0f; vmi->rot[1] = 0.0f; vmi->rot[2] = 0.0f;
    vmi->scale[0] = 1.0f; vmi->scale[1] = 1.0f; vmi->scale[2] = 1.0f;
}
