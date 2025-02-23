#pragma once

#include <cglm/cglm.h>

typedef struct node node;
typedef struct node {
    vec3 pos;
    vec3 rot;
    vec3 scale;

    node * parent;
}node;


void node_init(node * n){
    n->pos[0]=0.0f; n->pos[1]=0.0f; n->pos[2]=0.0f;
    n->rot[0]=0.0f; n->rot[1]=0.0f; n->rot[2]=0.0f;
    n->scale[0]=1.0f; n->scale[1]=1.0f; n->scale[2]=1.0f;
    n->parent = NULL;
}



void node_get_transform(node * n, mat4 m){
    glm_mat4_identity(m);
    glm_euler_xyz(n->rot,m);
    glm_scale(m,n->scale); //not affected by rotation
    glm_translated(m,n->pos); //not affected by scale rotation
    if(n->parent != NULL){
        mat4 parent_tr;
        node_get_transform(n->parent,parent_tr);
        glm_mat4_mul(parent_tr,m,m);
    }
};


