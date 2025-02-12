#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

#include <stdio.h>
#include "utils/vector.h"

int main() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_CORE);

    //__________________________________________________
    // CREATE WINDOW
    //__________________________________________________
    SDL_Window* window = SDL_CreateWindow("SDL Test", 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);


    SDL_Delay(2000);
    SDL_DestroyWindow(window);
    SDL_Quit();

    vec v = vec_alloc(sizeof(int16_t),2);
    int16_t a=1, b=2, c=3;

    vec_push(&v,&a); vec_push(&v,&b); vec_push(&v,&b); vec_push(&v,&c);


    printf("m v[0] = %d\n", VEC_AT(&v, int16_t, 0));
    printf("f v[2] = %d\n", VEC_GET(v, uint16_t, 2));

    vec_free(&v);

    return 0;
}

