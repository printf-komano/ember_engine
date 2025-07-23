#include <SDL3/SDL.h>
#include <SDL3/SDL_time.h>
#include <glad/gl.h>
#include <cglm/cglm.h>

#include <stdio.h>


#include "utils/vector.h"
#include "utils/shader_reader.h"
#include "bhandler.h"

#include "model/camera.h"
#include "model/node.h"

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
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
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
    emb_bhandler batch = emb_bhandler_init(2000024*1024, &vbo, 2000024*1024, &ebo);
    glCreateBuffers(1,&vbo);
    glCreateBuffers(1,&ebo);

    //__________________________________________________
    // add the debug figures
    //__________________________________________________

    emb_prim color_rect = emb_debug_rainbow_cube();
    emb_prim_inst* pr0 = emb_bhandler_instantiate(&batch,&color_rect);
    emb_prim_inst* pr1 = emb_bhandler_instantiate(&batch,&color_rect);

    
    /*create new node pool - can be 1 or more*/
    emb_node_pool nodepool;
    emb_node_pool_init(&nodepool,1024);
    emb_node* n = emb_node_pool_push(&nodepool);

    /*apply some transformations to the node*/
    n->pos[2] = -0.5f;
    n->scale[0] = 0.75f;
    n->scale[1] = 0.75f;
    n->scale[2] = 0.75f;
    
   
    pr0->scale[0]=0.25f;
    pr0->scale[1]=0.25f;
    pr0->scale[2]=0.25f;
    pr0->rot[0]=0.25f;
    pr0->pos[0]=1.75f;
    pr0->parent = n;
    
    pr1->rot[1]=-0.25f;
    pr1->scale[1]=0.1f;
    pr1->parent = n;


    emb_node* multinode = emb_node_pool_push(&nodepool);
    multinode->pos[0] = 56;
    multinode->pos[1] = 56;
    multinode->pos[2] = 56;

    //adding objects in a loop (testing)
    for(uint16_t i = 0; i < 100; ++i){
        emb_prim_inst* pri = emb_bhandler_instantiate(&batch,&color_rect);
        pri->pos[0] = rand()%256;
        pri->pos[1] = rand()%256;
        pri->pos[2] = rand()%256;
        pri->parent=multinode;
    }



    glNamedBufferStorage(vbo,batch.vb_capacity,batch.vb_data,GL_DYNAMIC_STORAGE_BIT);
    glNamedBufferStorage(ebo,batch.eb_capacity,batch.eb_data,GL_DYNAMIC_STORAGE_BIT);


	

    //__________________________________________________
    // VAO
    //__________________________________________________
    emb_setup_buffers(&vao,0,vbo,ebo);
    GLuint attrib_pos = 0;
    GLuint attrib_clr = 1;
    glBindVertexArray(vao);

    //__________________________________________________
    // shaders
    //__________________________________________________
    GLuint shader_prog;

    //reading source
    char * vertex_source = read_shader_file("shaders/vertex.glsl");
    char * fragment_source = read_shader_file("shaders/fragment.glsl");
    
       

    if(!shader_program_from_source(&shader_prog, vertex_source, fragment_source)){
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
    
    vec3 light_dir; light_dir[0]=0.0f; light_dir[1]=-1.0f; light_dir[2] = 0.0f;
    GLuint light_dir_uniform_loc = glGetUniformLocation(shader_prog,"light_dir");

    //__________________________________________________
    // creating the camera
    //__________________________________________________
    camera cam = {{0.0f,0.0f,1.0f},{0.0f,0.0f,0.0f},glm_rad(90.0f)};
    float mousex, mousey, mousedx, mousedy;
    vec3 movement_dir;

    //__________________________________________________
    // main loop
    //__________________________________________________
    uint32_t prev_tick = SDL_GetTicks();
    
    glEnable(GL_CULL_FACE); glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST); 

    //SDL_SetWindowRelativeMouseMode(window,true);

    while(true){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //__________________________________________________
        // delta time
        //__________________________________________________
        uint32_t current_tick = SDL_GetTicks();
        float delta_time = ((float)current_tick-(float)prev_tick)*0.001f;
        prev_tick = current_tick;
        //printf("fps: %f\n",1.0f/delta_time);

        //__________________________________________________
        // mouse control
        //__________________________________________________
        emb_mouse_delta(&mousedx,&mousedy);
        //printf("mouse: dx=%f, dy=%f\n",mousedx, mousedy);
        if(true){
            cam.rot[0] = glm_clamp(cam.rot[0]+mousedy*0.4f,-1.4f,1.4f);
            cam.rot[1] += mousedx*0.4f;
        }
        



        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if(event.type == SDL_EVENT_QUIT) goto break_main_loop;
        }
        
       

        glm_perspective(cam.fov,(float)WIDTH/(float)HEIGHT,0.1f,10000.0f,proj);

        emb_get_keyboard_movement(movement_dir);

        movement_dir[0]*=10.0f;
        movement_dir[1]*=10.0f;
        movement_dir[2]*=10.0f;

        glm_vec3_rotate(movement_dir,cam.rot[1],(vec3){0,1,0});
        glm_vec3_scale(movement_dir,delta_time,movement_dir);

        glm_vec3_add(cam.pos,movement_dir,cam.pos);

        glClearColor(0.0f,0.0f,0.0f,0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        multinode->rot[1]+=delta_time*0.1f;
        //__________________________________________________
        // rendering each emb_prim_inst
        //__________________________________________________
        for(uint32_t i = 0; i<batch.primitives.len; ++i){
            emb_prim_inst * inst = VEC_GETPTR(&batch.primitives,emb_prim_inst,i);
            {inst->rot[0]+=delta_time*0.5f; inst->rot[2]+=delta_time;} //debug moving the model
                        
            prim_inst_get_transform(inst,model);
            camera_get_view(&cam,view);

            glUniform3f(light_dir_uniform_loc,light_dir[0],light_dir[1],light_dir[2]);
            glUniformMatrix4fv(proj_uniform_loc,1,GL_FALSE,(float*)proj);
            glUniformMatrix4fv(model_uniform_loc,1,GL_FALSE,(float*)model);
            glUniformMatrix4fv(view_uniform_loc,1,GL_FALSE,(float*)view);
            void * eoffset = (void*)( (inst->eb_start - batch.eb_data));
            

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

    emb_bhandler_free(&batch);
    emb_node_pool_free(&nodepool);

    return EXIT_SUCCESS;
}



