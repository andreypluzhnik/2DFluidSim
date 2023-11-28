#version 430 core

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

layout(location = 0) uniform float radius;
layout(location = 1) uniform vec3 color;
layout(location = 2) uniform vec2 center;

layout(rgba16f, binding = 0) uniform image2D out_color_field;


void main(){
    ivec2 pos = ivec2(gl_GlobalInvocationID);
    float dist = length(pos - center);
    if(dist < radius) imageStore(out_color_field, pos, vec4(color, 1.0));

}