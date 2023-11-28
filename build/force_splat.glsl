#version 430 core

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;


layout(location = 0) uniform float dt;
layout(location = 1) uniform float intensity;
layout(location = 2) uniform vec2 center;
layout(location = 3) uniform vec2 direction;
layout(location = 4) uniform float spatial_decay;

layout(binding = 0) uniform sampler2D in_velocity_field;
layout(rgba16f, binding = 1) uniform image2D out_velocity_field;





void main(){
    ivec2 pos = ivec2(gl_GlobalInvocationID);
    float dist = length(pos - center);
    
    if(dist < 50 && dist > 0.01){
   
        vec4 force = vec4(intensity * direction * exp(-dist * spatial_decay), 0, 0);    
        vec4 out_velocity = texelFetch(in_velocity_field, pos, 0) + dt * force;
        imageStore(out_velocity_field, pos, out_velocity);
    
    }



}


