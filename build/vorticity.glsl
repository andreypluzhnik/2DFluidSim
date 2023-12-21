#version 430 core

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

layout(location = 0) uniform float vorticity;
layout(location = 1) uniform float dt;
layout(binding = 0) uniform sampler2D curl;
layout(rgba16f, binding = 1) uniform image2D velocity_field;



void main(){
    ivec2 tex_size = textureSize(curl, 0);
    ivec2 pos = ivec2(gl_GlobalInvocationID.xy); 
    
    ivec2 right = clamp(pos + ivec2(1, 0), ivec2(0,0), tex_size.xy);
    ivec2 left = clamp(pos - ivec2(1, 0), ivec2(0,0), tex_size.xy);
    ivec2 top = clamp(pos + ivec2(0, 1), ivec2(0,0), tex_size.xy);
    ivec2 bot = clamp(pos - ivec2(0, 1), ivec2(0,0), tex_size.xy);

    float diff_x = abs(texelFetch(curl, right, 0).z - texelFetch(curl, left, 0).z);
    float diff_y = abs(texelFetch(curl, top, 0).z - texelFetch(curl, bot, 0).z);
    vec2 N = vec2(diff_x, diff_y) * 0.5;
    N = N / length(N);


    imageStore(velocity_field, pos, imageLoad(velocity_field, pos) + dt * vorticity * vec4(N.y, -N.x, 0, 0) * curl.x);
    


}