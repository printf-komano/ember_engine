#version 460 core

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 clr;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec3 normal;

out vec3 frag_pos;
out flat vec3 frag_normal;
out flat vec3 vertex_color;

void main() {
    vec4 worldpos = model*vec4(pos,1.0);
    gl_Position = proj*view*worldpos;

    vertex_color = clr;

    frag_normal = mat3(transpose(inverse(model))) * normal;
    frag_pos = worldpos.xyz;
};
