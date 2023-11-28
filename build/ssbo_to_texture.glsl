#version 430 core

// in uvec3 gl_NumWorkGroups; # num work groups dispatched
// in uvec3 gl_WorkGroupID; # id of current work group
// in uvec3 gl_LocalInvocationID; # id of work item in work group
// in uvec3 gl_GlobalInvocationID; # id of work item w.r.t. work items in all work groups
// in uint  gl_LocalInvocationIndex; # index of work item in work group


layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

layout(std430, binding = 0) buffer in_buffer {
    float inField[];
};

layout(binding = 0, rgba32f) writeonly uniform image2D outImage;



// converts work group coordinate to index of r component in ssbo
uint CoordToIndex(uvec2 coord){
    return 3 * ((coord.y * gl_NumWorkGroups.x) + coord.x);
}

vec4 ColorAtCoord(uvec2 pos){
    uint r_idx = CoordToIndex(pos);
    uint g_idx = r_idx + 1;
    uint b_idx = r_idx + 2;
    return vec4(inField[r_idx], inField[g_idx], inField[b_idx], 1.0); 
}

void main(){
    

    ivec2 pos = ivec2(gl_GlobalInvocationID.xy);
    ivec2 size = ivec2(gl_NumWorkGroups.xy);
    
    imageStore(outImage, pos, ColorAtCoord(pos));


}