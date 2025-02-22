#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <glad/gl.h>

char * read_shader_file(const char * filename){
    FILE * f = fopen(filename,"r");
    if(!f){printf("ERROR: cannot find shader: %s\n", filename); return NULL;}

    fseek(f,0,SEEK_END);
    long len = ftell(f);
    rewind(f);

    char* str = (char*)malloc(len+1);
    fread(str,len,1,f);
    fclose(f);

    str[len] = 0;

    return str;
}





bool compile_shader(GLuint * shader, GLenum type, const char * source){
    *shader = glCreateShader(type);
    glShaderSource(*shader,1,&source,NULL);
    glCompileShader(*shader);
    GLuint success;

    glGetShaderiv(*shader, GL_COMPILE_STATUS, &success);
    if(!success){
        //char infoLog[512];
        //glGetShaderInfoLog(shader, 512, NULL, infoLog);
        printf("ERROR compile_shader(): shader compilation failed\n");
        return false;
    }
    return true;
}


bool create_shader_prog(GLuint* prog, GLuint vertex_shader, GLuint fragment_shader){
    //GLuint vertex_shader = compile_shader(GL_VERTEX_SHADER,vertex_source);
    //GLuint fragment_shader = compile_shader(GL_FRAGMENT_SHADER,fragment_source);

    *prog = glCreateProgram();
    glAttachShader(*prog,vertex_shader);
    glAttachShader(*prog,fragment_shader);

    glLinkProgram(*prog);
    GLint params = -1;
    glGetProgramiv(*prog, GL_LINK_STATUS, &params);
    if (GL_TRUE != params) {
        printf( "ERROR: create_shader_prog() could not link shaders.\n");
        return false;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return true;
}


bool shader_program_from_files(GLuint * prog){
    GLuint vertex_shader, fragment_shader;
    
    //reading source
    char * vertex_source = read_shader_file("shaders/vertex.glsl");
    char * fragment_source = read_shader_file("shaders/fragment.glsl");
    if(!vertex_source || !fragment_source) return false;

    //compilation
    if(!compile_shader(&vertex_shader,GL_VERTEX_SHADER,vertex_source)) return false;
    if(!compile_shader(&fragment_shader,GL_FRAGMENT_SHADER,fragment_source)) return false;

    //linking
    if(!create_shader_prog(prog,vertex_shader,fragment_shader)) return false;
    
    return true;
}