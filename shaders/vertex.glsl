#version 460 core

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 clr; 


out vec3 vertex_color;

void main() {
    gl_Position = proj*view*model*vec4(pos, 1.0);
    vertex_color = clr;
};
