#version 430 core


layout(local_size_x = 32, local_size_y = 32, local_size_z = 1) in;

layout(location = 0) uniform float scale;


layout(binding = 0) uniform sampler2D in_field;
layout(rgba16f, binding = 1) uniform image2D out_field;


void main(){
    ivec2 pos = ivec2(gl_GlobalInvocationID.xy);
    imageStore(out_field, pos, scale * texelFetch(in_field, pos, 0));

}