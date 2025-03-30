#pragma once

#include <SDL3/SDL.h>
#include <glad/gl.h>
#include <cglm/cglm.h>
#include <stdio.h>
#include "node.h"

#define CGLTF_IMPLEMENTATION
#include "cgltf.h"

#define VB_ATTRIB_POS_OFFSET 0
#define VB_ATTRIB_CLR_OFFSET 1
#define VB_ATTRIB_UV_OFFSET 2
#define VB_ATTRIB_NORMAL_OFFSET 3


#define VB_ATTRIB_POS_SIZE 3
#define VB_ATTRIB_CLR_SIZE 3
#define VB_ATTRIB_UV_SIZE 2
#define VB_ATTRIB_NORMAL_SIZE 3
#define VB_ATTRIB_SIZE_MAX 11

//__________________________________________________
// vmodel - unique sample of the model
//__________________________________________________

/*
The simplest primitive consisting of triangles. 
Can contain only one material (shader). 
To assemble a full model from several primitives, you will need to use nodes.
*/
typedef struct //model template
{
    bool use_vertex_colors; //takes 3 elements in buffer
    bool use_uv; //2 additional elements in the buffer
    float * vb; //local vertex buffer 
    uint32_t vb_len; //length of the original buffer (in bytes)

    uint32_t * eb; //local triangles buffer 
    uint32_t eb_len; //length of the original triangles buffer (in bytes)
} vmodel; 




void vmodel_load_primitive_cgltf(vmodel *out, const cgltf_primitive *primitive) {
    size_t num_vertices = 0;
    size_t num_indices = 0;
    size_t vertex_stride = VB_ATTRIB_SIZE_MAX;

    //check possible errors 
    if (primitive->attributes_count > 0) num_vertices = primitive->attributes[0].data->count;
    if (primitive->indices) num_indices = primitive->indices->count;

    
    out->vb_len = num_vertices * vertex_stride * sizeof(float);
    out->vb = (float*)malloc(out->vb_len);

    out->eb_len = num_indices * sizeof(uint32_t);
    out->eb = (uint32_t*)malloc(out->eb_len);



    //fill vbo with zeros
    memset(out->vb, 0, out->vb_len);

    // fill vbo with values from cgltf primitive
    for (size_t i = 0; i < num_vertices; i++) {
        float *vbo_ver = &out->vb[i * vertex_stride];

        //for each attribute in current vertex
        for (size_t j = 0; j < primitive->attributes_count; j++) {
            cgltf_attribute *attr = &primitive->attributes[j];
            cgltf_accessor *accessor = attr->data;

            int num_components = 1;
            if (accessor->type == cgltf_type_vec2) num_components = 2;
            if (accessor->type == cgltf_type_vec3) num_components = 3;
            if (accessor->type == cgltf_type_vec4) num_components = 4;

            float temp[4] = {0}; //max 4 values for attrib
            cgltf_accessor_read_float(accessor, i, temp, num_components);

            switch (attr->type) {
                case cgltf_attribute_type_position:
                    vbo_ver[0] = temp[0];
                    vbo_ver[1] = temp[1];
                    vbo_ver[2] = temp[2];
                    break;
                case cgltf_attribute_type_color:
                    vbo_ver[3] = temp[0];
                    vbo_ver[4] = temp[1];
                    vbo_ver[5] = temp[2];
                    break;
                case cgltf_attribute_type_texcoord:
                    vbo_ver[6] = temp[0];
                    vbo_ver[7] = temp[1];
                    break;
                case cgltf_attribute_type_normal:
                    vbo_ver[8] = temp[0];
                    vbo_ver[9] = temp[1];
                    vbo_ver[10] = temp[2];
                    break;
                default:
                    break;
            }
        }
    }

    // fill ebo
    if (primitive->indices) {
        for (size_t i = 0; i < num_indices; i++) {
            out->eb[i] = (uint32_t)cgltf_accessor_read_index(primitive->indices, i);
        }
    }
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

    // mat4 transform; //model matrix
    vec3 pos;
    vec3 scale;
    vec3 rot;

    // reference to the parent node. child node will inherit all the transformations.
    node * parent; 

} vmodel_inst; 


void vmodel_inst_get_transform(vmodel_inst * vmi, mat4 m){
    glm_mat4_identity(m);
    glm_euler_xyz(vmi->rot,m);
    glm_scale(m,vmi->scale); //not affected by rotation
    glm_translated(m,vmi->pos); //not affected by scale rotation

    if(vmi->parent != NULL && vmi->parent->node_state!=NODE_STATE_NONE){
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















//model used for debugging (vertices)
float _vmodel_test_rectangle_vertices[] = {
        /*pos                       colors          uv      normals */
        -0.5f, -0.5f, -0.5f,    1.2f, 0.2f, 0.2f,   0,0,    0,0,0, //0
        0.5f, -0.5f, -0.5f, 0.2f, 1.2f, 0.2f, 0,0, 0,0,0, //1
        -0.5f, 0.5f, -0.5f, 0.2f, 0.2f, 1.2f, 0,0, 0,0,0, //2
        0.5f, 0.5f, -0.5f, 1.2f, 0.2f, 0.2f, 0,0, 0,0,0, //3


        -0.5f, -0.5f, 0.5f, 0.2f, 0.2f, 0.2f, 0,0, 0,0,0, //4
        0.5f, -0.5f, 0.5f, 0.2f, 0.2f, 0.2f, 0,0, 0,0,0, //5
        -0.5f, 0.5f, 0.5f, 0.2f, 0.2f, 0.2f, 0,0, 0,0,0, //6
        0.5f, 0.5f, 0.5f, 0.2f, 0.2f, 0.2f, 0,0, 0,0,0, //7
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