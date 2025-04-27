#version 460 core

uniform vec3 light_dir;


in vec3 vertex_color;  //get vertex color
in vec3 frag_normal;

out vec4 frag_color;

void main(){
    float light_power = max(dot(frag_normal,light_dir),0.1);
    frag_color = light_power * vec4(vertex_color.r,vertex_color.g,vertex_color.b, 1.0);
}
