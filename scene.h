/*
This gonna be a scene which contains all the objects.
objects are storagen in one array.
Each scene can have a references to a children scenes.
*/
#pragma once
#include "utils/vector.h"
#include "model/model.h"


#define SCENE_MODELS_INITAL_CAPACITY 1024

typedef struct 
{
    const char * name; //name og the scene
    vec scene_children; //another scenes inside this one (recursion)
    vec models; //array of models in this scene
} scene;


void init_scene(scene * sc, char * name){
    sc->name = name;
    sc->scene_children = vec_alloc(sizeof(scene),16);
    sc->models = vec_alloc(sizeof(vmodel_inst),SCENE_MODELS_INITAL_CAPACITY);
}


