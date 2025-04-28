#version 460 core

uniform vec3 light_dir;




in vec3 vertex_color;  //get vertex color
in vec3 frag_normal;
in vec3 frag_pos;

out vec4 frag_color;


void main(){
    vec3 flat_normal = normalize(cross(dFdx(frag_pos),dFdy(frag_pos)));
    float light_power = clamp(dot(flat_normal,-light_dir),0.1,1.0);
    frag_color = light_power * vec4(vertex_color.r,vertex_color.g,vertex_color.b, 1.0);
}
