#version 430 core

layout(local_size_x = 32, local_size_y = 32, local_size_z = 1) in;

layout(location = 0) uniform float alpha;
layout(location = 1) uniform float beta;
layout(location = 2) uniform int boundary_sign;


layout(binding = 0) uniform sampler2D in_field;
layout(rgba16f, binding = 1) uniform image2D out_field;
layout(binding = 2) uniform sampler2D given_field;




void main(){
    vec2 tex_size = textureSize(in_field, 0);
    vec4 u_c, u_t, u_b, u_l, u_r;
    ivec2 pos = ivec2(gl_GlobalInvocationID.xy);
    const ivec2 dx = ivec2(1, 0);
    const ivec2 dy = ivec2(0, 1);
    vec4 result;

    u_c = texelFetch(in_field, pos,0);  
    u_r = texelFetchOffset(in_field, pos, 0, dx);
    u_l = texelFetchOffset(in_field, pos, 0, -dx);
    u_t = texelFetchOffset(in_field, pos, 0, dy);
    u_b = texelFetchOffset(in_field, pos, 0, -dy);

    result = (u_t + u_b + u_l + u_r + alpha * texelFetch(given_field, pos, 0)) / beta;

    if(pos.x <= 0){
        result = boundary_sign * u_c;
    }     
    if(pos.y <= 0){
        result = boundary_sign * u_c;
    }
    if(pos.x >= tex_size.x - 1){
        result = boundary_sign * u_c;
    } 
    if(pos.y >= tex_size.y - 1){
        result = boundary_sign * u_c;
    }
  

    imageStore(out_field, pos, result);
    

}