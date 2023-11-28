#version 430 core

// in uvec3 gl_NumWorkGroups; # num work groups dispatched
// in uvec3 gl_WorkGroupID; # id of current work group
// in uvec3 gl_LocalInvocationID; # id of work item in work group
// in uvec3 gl_GlobalInvocationID; # id of work item w.r.t. work items in all work groups
// in uint  gl_LocalInvocationIndex; # index of work item in work group




layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
 
layout(binding = 0, rgba32f) uniform image2D inImage;
layout(binding = 1, rgba32f) writeonly uniform image2D outImage;


uniform float test;

void main(){

    ivec2 pos = ivec2(gl_GlobalInvocationID.xy);
    ivec2 size = ivec2(gl_NumWorkGroups.xy);
    ivec2 ic = pos;

    if(pos.x < 1 || pos.y < 1 || pos.x >= size.x - 1 || pos.y >= size.y - 1){
    
        imageStore(outImage, pos, imageLoad(inImage, ic));
    
    }else if((pos.x >= size.x * 0.15 && pos.x <= size.x * 0.85) && (pos.y <= 0.05 * size.y || pos.y >= 0.95 * size.y)){
    
       
        vec4 color = vec4(1.0, 0.0039, 0.0039, 1.0);
        

        imageStore(inImage, pos, color);
        imageStore(outImage, pos, color);


    
    
    }else{



        ivec2 ic_px, ic_py, ic_nx, ic_ny;
        ic_py = pos + ivec2(0,1);
        ic_px = pos + ivec2(1,0);
        ic_ny = pos - ivec2(0,1);
        ic_nx = pos - ivec2(1,0);

        vec4 color = 0.25 * (imageLoad(inImage, ic_px) + imageLoad(inImage, ic_py) + imageLoad(inImage, ic_nx) + imageLoad(inImage, ic_ny));
        imageStore(outImage, ic, color);

    }

}
