#pragma once

#include <SDL3/SDL.h>
#include <glad/gl.h>
#include <cglm/cglm.h>
#include <stdio.h>
#include "bhandler.h"
#include "utils/shader_reader.h"




void app_setup_buffers(GLuint * vao, GLuint vao_binding_point, GLuint vbo, GLuint ebo){
    GLuint attrib_pos = 0, attrib_clr = 1;
    glCreateVertexArrays(1,vao);
    glVertexArrayVertexBuffer(
        *vao,
        vao_binding_point,
        vbo,
        0, //offset is 0
        (VB_ATTRIB_POS_SIZE+VB_ATTRIB_CLR_SIZE+VB_ATTRIB_UV_SIZE)*sizeof(float) //3 for pos and 3 for color
    );
    glVertexArrayElementBuffer(*vao,ebo);

    //positions
    glEnableVertexArrayAttrib(*vao, VB_ATTRIB_POS_OFFSET);
    glVertexArrayAttribFormat(*vao, VB_ATTRIB_POS_OFFSET, VB_ATTRIB_POS_SIZE, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(*vao, VB_ATTRIB_POS_OFFSET, vao_binding_point);

    //colors
    glEnableVertexArrayAttrib(*vao, VB_ATTRIB_CLR_OFFSET);
    glVertexArrayAttribFormat(*vao, VB_ATTRIB_CLR_OFFSET, VB_ATTRIB_CLR_SIZE, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
    glVertexArrayAttribBinding(*vao, VB_ATTRIB_CLR_OFFSET, vao_binding_point);

    //uvs
    glEnableVertexArrayAttrib(*vao, VB_ATTRIB_UV_OFFSET);
    glVertexArrayAttribFormat(*vao, VB_ATTRIB_UV_OFFSET, VB_ATTRIB_UV_SIZE, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
    glVertexArrayAttribBinding(*vao, VB_ATTRIB_UV_OFFSET, vao_binding_point);
}