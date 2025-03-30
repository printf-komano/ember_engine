#pragma once

#include <cglm/cglm.h>



#define NODE_STATE_NONE 0
#define NODE_STATE_ACTIVE 1
#define NODE_STATE_IGNORE 2 

typedef struct node node;

/*node allows to apply recursive transformations 
to the models on the scene or to each other.*/
typedef struct node {
    uint8_t node_state;
    vec3 pos;
    vec3 rot;
    vec3 scale;
    node* parent; //reference to the parent in node_pool
}node;


void node_init(node * n){
    n->pos[0]=0.0f; n->pos[1]=0.0f; n->pos[2]=0.0f;
    n->rot[0]=0.0f; n->rot[1]=0.0f; n->rot[2]=0.0f;
    n->scale[0]=1.0f; n->scale[1]=1.0f; n->scale[2]=1.0f;
    n->parent = NULL;
    n->node_state = NODE_STATE_ACTIVE;
}




void node_get_transform(node * n, mat4 m){
    glm_mat4_identity(m);
    glm_euler_xyz(n->rot,m);
    glm_scale(m,n->scale); //not affected by rotation
    glm_translated(m,n->pos); //not affected by scale rotation
    if(n->parent != NULL && n->parent->node_state!=NODE_STATE_NONE){
        mat4 parent_tr;
        node_get_transform(n->parent,parent_tr);
        glm_mat4_mul(parent_tr,m,m);
    }
};





//__________________________________________________

/*Pool of nodes. Each node is contained there, 
referencing to another nodes in the pool.*/
typedef struct
{
    /*Array will be allocated only once. 
    Even if realloc will be introduced, 
    it's gonna be pretty slow.*/
    node * nodes; 
    
    /*optimisation stuff. When new node added, value increases by 1.
    it will increase, until array is full. If so, it will become 0
    and start to search for a free place from start*/
    size_t last_index; 

    size_t capacity; //measured in number of elements

}node_pool;


void node_pool_init(node_pool * np, size_t capacity){
    np->nodes = malloc(capacity*sizeof(node));
    for(uint32_t i=0; i<capacity; ++i) np->nodes[i].node_state = NODE_STATE_NONE;
    np->capacity = capacity;
    np->last_index = 0;
}


void node_pool_remove_node(node_pool * np, size_t index){
    if(index>=np->capacity) return;
    np->nodes[index].node_state = NODE_STATE_NONE;
}


node * node_pool_add_node(node_pool * np){
    if(np->last_index < np->capacity && np->nodes[np->last_index].node_state == NODE_STATE_NONE){ //if has free space at back
        node_init(&np->nodes[np->last_index]); 
        ++np->last_index; //increase last index
        return &np->nodes[np->last_index-1];
    }
    else{ //if array is full, try to find saome free space from start
        np->last_index = 0;
        while (np->last_index < np->capacity)
        {
            if(np->nodes[np->last_index].node_state == NODE_STATE_NONE){
                node_init(&np->nodes[np->last_index]); 
                ++np->last_index;
                return &np->nodes[np->last_index-1];
            }
            ++np->last_index;
        }
        return NULL; //no free space, failed to add new node
    }
}

node * node_pool_get(node_pool * np, int32_t index){
    if(index<0 || index>=np->capacity || np->nodes[index].node_state==NODE_STATE_NONE) return NULL;
    return &np->nodes[index];
}


void node_pool_free(node_pool * np){
    free(np->nodes);
}









/*


//reading models from file
cgltf_data * _vmodel_load_gltf(char * path){
    cgltf_data * data = NULL; 
    cgltf_options options = {0};
    
    cgltf_result res = cgltf_parse_file(&options,path,&data);
    if(res != cgltf_result_success) return NULL;

    res = cgltf_load_buffers(&options,data,path);
    if(res != cgltf_result_success) return NULL;

    res = cgltf_validate(data);
    if(res != cgltf_result_success) return NULL;

    return data;
}


void vmodel_init_gltf(vec * out, char * path){
    cgltf_data * data = _vmodel_load_gltf(path);
    if (!data) return;

    //vec_alloc(sizeof(vmodel*),)
    
    for(cgltf_size i=0; i<data->meshes_count; ++i){
        cgltf_mesh* mesh = &data->meshes[i];
        for(cgltf_size j=0; j<mesh->primitives_count; ++j){
            cgltf_primitive * pr = &mesh->primitives[j]; vmodel *vm;
            vmodel_load_primitive_cgltf(vm,pr);
            

        }   
    }
    
}
*/