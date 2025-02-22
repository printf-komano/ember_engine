#include <SDL3/SDL.h>
#include <SDL3/SDL_time.h>
#include <glad/gl.h>
#include <cglm/cglm.h>

#include <stdio.h>

#include "utils/vector.h"
#include "utils/shader_reader.h"
#include "bhandler.h"
#include "model/camera.h"
#include "input.c"
#include "app.c"


#define WIDTH 1024
#define HEIGHT 1024

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    //__________________________________________________
    // CREATE WINDOW
    //__________________________________________________
    SDL_Window* window = SDL_CreateWindow("ember", WIDTH, HEIGHT,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_TRANSPARENT
    );
    if(window==NULL) return EXIT_FAILURE;

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    if(!gl_context) {printf("ERROR: no gl_context\n");}

    if(!gladLoadGL((GLADloadfunc) SDL_GL_GetProcAddress)) {printf("ERROR: no gladLoadGL\n");}

    printf("opengl version: %s\n",glGetString(GL_VERSION));


    //__________________________________________________
    // vertex buffer and element buffer
    //__________________________________________________
    GLuint vbo, ebo, vao;
    bhandler batch = bhandler_init(1024, &vbo, 1024, &ebo);
    glCreateBuffers(1,&vbo);
    glCreateBuffers(1,&ebo);

    //__________________________________________________
    // add the debug rectangle
    //__________________________________________________
    vmodel color_rect = vmodel_test_rectangle();
    bhandler_vmodel_instance(&batch,&color_rect);
    bhandler_vmodel_instance(&batch,&color_rect);
    //bhandler_vmodel_instance(&batch,&color_rect);

    vmodel_inst* vmi0 = VEC_GETPTR(&batch.models,vmodel_inst,0);
    vmi0->scale[0]=0.25f;
    vmi0->scale[1]=0.25f;
    vmi0->scale[2]=0.25f;
    vmi0->rot[0]=0.25f;
    vmi0->pos[0]=1.75f;
    
    vmodel_inst* vmi1 = VEC_GETPTR(&batch.models,vmodel_inst,0);
    vmi1->rot[1]=-0.25f;

    glNamedBufferStorage(vbo,batch.vb_capacity,batch.vb_data,GL_DYNAMIC_STORAGE_BIT);
    glNamedBufferStorage(ebo,batch.eb_capacity,batch.eb_data,GL_DYNAMIC_STORAGE_BIT);



    //__________________________________________________
    // VAO
    //__________________________________________________
    app_setup_buffers(&vao,0,vbo,ebo);
    GLuint attrib_pos = 0;
    GLuint attrib_clr = 1;
    glBindVertexArray(vao);

    //__________________________________________________
    // shaders
    //__________________________________________________
    GLuint shader_prog;
    if(!shader_program_from_files(&shader_prog)){
        printf("ERROR: no shader program.\n");
        return EXIT_FAILURE;
    }
    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
    glUseProgram(shader_prog);

    mat4 model;
    GLuint model_uniform_loc = glGetUniformLocation(shader_prog,"model");
    
    mat4 view;
    GLuint view_uniform_loc = glGetUniformLocation(shader_prog,"view");

    mat4 proj;
    GLuint proj_uniform_loc = glGetUniformLocation(shader_prog,"proj");


    //__________________________________________________
    // creating the camera
    //__________________________________________________
    camera cam = {{0.0f,0.0f,-1.0f},{0.0f,0.0f,0.0f},glm_rad(90.0f)};
    vec3 movement_dir;
    vec2 mouse_motion; vec2 mouse_dxy;

    //__________________________________________________
    // main loop
    //__________________________________________________
    uint64_t prev_tick = SDL_GetTicks();
    
    glEnable(GL_CULL_FACE); glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST); 

    //SDL_SetWindowRelativeMouseMode(window,true);

    while(true){
        mouse_dxy[0] = 0.0f; mouse_dxy[1] = 0.0f;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //__________________________________________________
        // delta time
        //__________________________________________________
        uint64_t current_tick = SDL_GetTicks();
        float delta_time = ((float)current_tick-(float)prev_tick)*0.001f;
        prev_tick = current_tick;

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if(event.type == SDL_EVENT_QUIT) goto break_main_loop;
            if(event.type == SDL_EVENT_MOUSE_MOTION){
                mouse_dxy[0] = (event.motion.x-mouse_motion[0])*delta_time; mouse_dxy[1] = (event.motion.y-mouse_motion[1])*delta_time;//calculate mouse motion changes
                mouse_motion[0] = event.motion.x; mouse_motion[1] = event.motion.y;   
                printf("mdx:%f, mdy:%f\n",mouse_dxy[0],mouse_dxy[1]);

            }

        }
        cam.rot[0] = glm_clamp(cam.rot[0]+mouse_dxy[1],-1.4f,1.4f);
        cam.rot[1] += mouse_dxy[0];

        glm_perspective(cam.fov,(float)WIDTH/(float)HEIGHT,0.1f,100.0f,proj);

        get_keyboard_movement(movement_dir);
        glm_vec3_scale(movement_dir,delta_time,movement_dir);
        glm_vec3_add(cam.pos,movement_dir,cam.pos);

        glClearColor(0.0f,0.0f,0.0f,0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //__________________________________________________
        // rendering each vmodel_inst
        //__________________________________________________
        for(uint32_t i = 0; i<batch.models.len; ++i){
            vmodel_inst * inst = VEC_GETPTR(&batch.models,vmodel_inst,i);
            if(i==1){inst->rot[0]+=delta_time*0.5f; inst->rot[2]+=delta_time;} //debug moving the model
            //if(i==0){inst->rot[1]+=delta_time*0.2f;} //debug moving the model
            //printf("i:%i, x:%f, y:%f, z:%f\n",i,inst->scale[0],inst->scale[1],inst->scale[2]);

            vmodel_inst_get_transform(inst,model);
            camera_get_view(&cam,view);
            glUniformMatrix4fv(proj_uniform_loc,1,GL_FALSE,(float*)proj);
            glUniformMatrix4fv(model_uniform_loc,1,GL_FALSE,(float*)model);
            glUniformMatrix4fv(view_uniform_loc,1,GL_FALSE,(float*)view);
            void * eoffset = (void*)( (inst->eb_start - batch.eb_data)/**sizeof(uint32_t)*/ );
            

            glDrawElements(GL_TRIANGLES,inst->eb_len/sizeof(uint32_t),GL_UNSIGNED_INT,eoffset);
        }

        SDL_GL_SwapWindow(window);

    }


    break_main_loop:
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteProgram(shader_prog);

    SDL_DestroyWindow(window);
    SDL_Quit();

    bhandler_free(&batch);

    return EXIT_SUCCESS;
}
