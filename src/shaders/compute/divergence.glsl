#version 430 core

layout(local_size_x = 32, local_size_y = 32, local_size_z = 1) in;

layout(location = 1) uniform float dl;
layout(binding = 0) uniform sampler2D in_field;
layout(rgba16f, binding = 1) uniform image2D out_field;



void main(){
    ivec2 tex_size = textureSize(in_field, 0);
    ivec2 pos = ivec2(gl_GlobalInvocationID.xy); 
    
    ivec2 right = clamp(pos + ivec2(1, 0), ivec2(0,0), tex_size.xy);
    ivec2 left = clamp(pos - ivec2(1, 0), ivec2(0,0), tex_size.xy);
    ivec2 top = clamp(pos + ivec2(0, 1), ivec2(0,0), tex_size.xy);
    ivec2 bot = clamp(pos - ivec2(0, 1), ivec2(0,0), tex_size.xy);


    float diff_x = texelFetch(in_field, right, 0).x - texelFetch(in_field, left, 0).x;
    float diff_y = texelFetch(in_field, top, 0).y - texelFetch(in_field, bot, 0).y;
    
    
    float div = (diff_x + diff_y) / (2 * dl);

    imageStore(out_field, pos, vec4(div, 0, 0, 0));



}