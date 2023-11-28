#version 430 core

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;



layout (location = 0) uniform float intensity;
layout(binding = 0) uniform sampler2D in_force_field;
layout(binding = 1) uniform sampler2D out_force_field;

