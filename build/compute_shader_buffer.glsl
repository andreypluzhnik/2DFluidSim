#version 430 core

// in uvec3 gl_NumWorkGroups; # num work groups dispatched
// in uvec3 gl_WorkGroupID; # id of current work group
// in uvec3 gl_LocalInvocationID; # id of work item in work group
// in uvec3 gl_GlobalInvocationID; # id of work item w.r.t. work items in all work groups
// in uint  gl_LocalInvocationIndex; # index of work item in work group



layout(std430, binding = 0) buffer in_buffer {
    float inField[];
};

layout(std430, binding = 1) buffer out_buffer{
    float outField[];
};
layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
// layout(binding = 0, rgba32f) writeonly uniform image2D outImage;



// converts work group coordinate to index of r component in ssbo
uint CoordToIndex(uvec2 coord){
    return 3 * ((coord.y * gl_NumWorkGroups.x) + coord.x);
}

vec4 ColorAtCoord(uvec2 pos){
    uint r_idx = CoordToIndex(pos);
    uint g_idx = r_idx + 1;
    uint b_idx = r_idx + 2;
    return vec4(outField[r_idx], outField[g_idx], outField[b_idx], 1.0); 
}

void main(){
    

    ivec2 pos = ivec2(gl_GlobalInvocationID.xy);
    ivec2 size = ivec2(gl_NumWorkGroups.xy);

    if(pos.x < 1 || pos.y < 1 || pos.x >= size.x - 1 || pos.y >= size.y - 1){
        // 0.0 at boundary
        
        for(int comp = 0; comp < 3; ++comp){
            uint coord = CoordToIndex(pos);
            outField[coord + comp] = 0.0;
        }


        // imageStore(outImage, pos, ColorAtCoord(pos));
    
    }else if((pos.x >= size.x * 0.15 && pos.x <= size.x * 0.85) && (pos.y <= 0.05 * size.y || pos.y >= 0.95 * size.y)){
    
       
        vec4 color = vec4(1.0, 0.0039, 0.0039, 1.0);
        uint ic = CoordToIndex(pos.xy);
        inField[ic] = color[0];
        inField[ic + 1] = color[1];
        inField[ic + 2] = color[2];

        for(int comp = 0; comp < 3; ++comp){
            outField[ic + comp] = inField[ic + comp];
        }

        // imageStore(outImage, pos, ColorAtCoord(pos));


    
    
    }else{
        // Relaxation Step
        // px = x + 1
        // py = y + 1
        // nx = x - 1
        // ny = y - 1
        // ic : initial condition of field
        uint ic_px, ic_py, ic_nx, ic_ny, ic;

        for(int comp = 0; comp < 3; ++comp){
            ic_py = CoordToIndex(pos + ivec2(0,1)) + comp;
            ic_px = CoordToIndex(pos + ivec2(1,0)) + comp;
            ic_ny = CoordToIndex(pos - ivec2(0,1)) + comp;
            ic_nx = CoordToIndex(pos - ivec2(1,0)) + comp;
            ic = CoordToIndex(pos) + comp;

            outField[ic] = 0.25f * (inField[ic_px] + inField[ic_py] + inField[ic_nx] + inField[ic_ny]);
            
        }
        
        
        // vec4 color = ColorAtCoord(pos);
        // imageStore(outImage, pos, color);
        
    }


}

