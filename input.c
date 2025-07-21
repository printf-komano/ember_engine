#pragma once
#include <SDL3/SDL.h>
#include <cglm/cglm.h>

#define EMB_INPUT_TICKLEN 0.001f


float EMB_MOUSE_X = 0.0f;
float EMB_MOUSE_Y = 0.0f;
uint32_t EMB_MOUSE_LASTCHANGE = 0;

bool emb_mouse_delta(float * dx, float * dy){
    uint64_t current_time = SDL_GetTicks();
    float dtime = ((float)current_time-(float)EMB_MOUSE_LASTCHANGE)
        *EMB_INPUT_TICKLEN;
    

    //new mouse coordinates
    float newx, newy; 
    if(!SDL_GetGlobalMouseState(&newx,&newy)){
        return false;
    }

    //return difference
    *dx = (newx-EMB_MOUSE_X) * dtime;
    *dy = (newy-EMB_MOUSE_Y) * dtime;

    //reassign current values
    EMB_MOUSE_X = newx;
    EMB_MOUSE_Y = newy;
    EMB_MOUSE_LASTCHANGE = current_time;

    //TODO : change this check later lol
    return *dx != 0.0f && *dy != 0.0f;
}



void get_keyboard_movement (vec3 dir){
    const bool * keyboard_state = SDL_GetKeyboardState(NULL);

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
