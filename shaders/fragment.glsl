#version 460 core



in vec3 vertex_color;  //get vertex color
out vec4 frag_color;

void main(){
    frag_color = vec4(vertex_color.r,vertex_color.g,vertex_color.b, 1.0);
}