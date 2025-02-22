#pragma once

#include <SDL3/SDL.h>
#include <glad/gl.h>
#include <cglm/cglm.h>
#include <stdio.h>



typedef struct
{
    vec3 pos;
    vec3 rot;
    float fov;
}camera;



void camera_get_view(camera * cam, mat4 m){
    glm_mat4_identity(m);
    glm_euler_xyz((vec3){-cam->rot[0],-cam->rot[1],-cam->rot[2]},m);
    glm_translated(m,(vec3){-cam->pos[0],-cam->pos[1],cam->pos[2]});
} 