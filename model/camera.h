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



void camera_get_view(camera *cam, mat4 m) {
    glm_mat4_identity(m);

    /*translation for camera is negative
    cuz it's calculating the offset 
    of the scene relative to camra position
    not the camera transform*/
    glm_translate(m, (vec3){-cam->pos[0], -cam->pos[1], -cam->pos[2]});

    mat4 rotation;
    glm_mat4_identity(rotation);
    glm_euler_xyz((vec3){-cam->rot[0], -cam->rot[1], -cam->rot[2]}, rotation);

    glm_mat4_mul(rotation, m, m);
}