#version 430 core

layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

layout(location = 1) uniform float dl;
layout(binding = 0) uniform sampler2D unprojected_field;
layout(binding = 1) uniform sampler2D pressure_field;
layout(rgba16f, binding = 2) uniform image2D out_field;



void main(){
    
    ivec2 pos = ivec2(gl_GlobalInvocationID.xy); 
    ivec2 tex_size = textureSize(unprojected_field, 0);
    ivec2 dx = ivec2(1, 0);
    ivec2 dy = ivec2(0, 1);


    ivec2 right = clamp(pos + ivec2(1, 0), ivec2(0,0), tex_size.xy - 1);
    ivec2 left = clamp(pos - ivec2(1, 0), ivec2(0,0), tex_size.xy - 1);
    ivec2 top = clamp(pos + ivec2(0, 1), ivec2(0,0), tex_size.xy - 1);
    ivec2 bot = clamp(pos - ivec2(0, 1), ivec2(0,0), tex_size.xy - 1);

    float p_c = texelFetch(pressure_field, pos, 0).x;
    float p_t = texelFetch(pressure_field, top, 0).x;
    float p_b = texelFetch(pressure_field, bot, 0).x;
    float p_r = texelFetch(pressure_field, right, 0).x;
    float p_l = texelFetch(pressure_field, left, 0).x;
    
    vec4 unprojected_sample = texelFetch(unprojected_field, pos, 0);
    float grad_x = p_r - p_l;
    float grad_y = p_t - p_b;
    vec4 grad = vec4(grad_x, grad_y, 0.0, 0.0) / (2 * dl);
    vec4 result = unprojected_sample - grad;
    
    if(pos.x == 1 && pos.y == 1){
        imageStore(out_field, ivec2(0,0), -result);
    }else if(pos.x == tex_size.x - 2 && pos.y == tex_size.y - 2){
        imageStore(out_field, ivec2(tex_size.x - 1, tex_size.y - 1), -result);
    }else if(pos.x == 1){
        imageStore(out_field, left, -result);
    }else if(pos.y == 1){
        imageStore(out_field, bot, -result);
    }else if(pos.x == tex_size.x - 2){
        imageStore(out_field, right, -result);
    }else if(pos.y == tex_size.y - 2){
        imageStore(out_field, top, -result);
    }
  
    if(pos.y >= 1 && pos.x >= 1 && pos.x <= tex_size.x - 2 && pos.y <= tex_size.y - 2) imageStore(out_field, pos, result);




}
