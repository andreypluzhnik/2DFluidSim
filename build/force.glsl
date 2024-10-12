#version 430 core


layout (local_size_x = 32, local_size_y = 32, local_size_z = 1) in;
layout(binding = 0) uniform sampler2D force_field;
layout(rgba16f, binding = 1) uniform image2D velocity_field;

uniform float dt;





void main(){
    ivec2 pos = ivec2(gl_GlobalInvocationID.xy);
    vec4 updated_velocity = imageLoad(velocity_field, pos) + texelFetch(force_field, pos, 0) * dt;
    imageStore(velocity_field, pos, updated_velocity);

}