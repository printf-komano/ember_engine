#pragma once
#include <SDL3/SDL.h>
#include <cglm/cglm.h>




void get_keyboard_movement (vec3 dir){
    const uint8_t * keyboard_state = SDL_GetKeyboardState(NULL);

    //Z
    if(keyboard_state[SDL_SCANCODE_UP]){
        dir[2]-=1.0f;
    }
    if(keyboard_state[SDL_SCANCODE_DOWN]){
        dir[2]+=1.0f;
    }

    //X
    if(keyboard_state[SDL_SCANCODE_RIGHT]){
        dir[0]+=1.0f;
    }
    if(keyboard_state[SDL_SCANCODE_LEFT]){
        dir[0]-=1.0f;
    }

    //Y
    if(keyboard_state[SDL_SCANCODE_SPACE]){
        dir[1]+=1.0f;
    }
    if(keyboard_state[SDL_SCANCODE_LSHIFT]){
        dir[1]-=1.0f;
    }

    glm_vec3_norm(dir);
}
