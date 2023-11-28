#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;


uvec2 ssboDim = uvec2(512, 512);

out vec3 ourColor;
out uvec2 TexCoord;

layout(std430, binding = 0) buffer in_buffer{
    float inField[];
};

uint CoordToIndex(uvec2 coord){
    return uint(gl_VertexID * ssboDim.x * ssboDim.y);
    // return 3 * ((coord.y * ssboDim.x) + coord.x);
}

vec3 ColorAtCoord(uvec2 pos){
    uint r_idx = CoordToIndex(pos);
    uint g_idx = r_idx + 1;
    uint b_idx = r_idx + 2;
    return vec3(inField[r_idx], inField[g_idx], inField[b_idx]); 
}


void main()
{
    gl_Position = vec4(aPos, 1.0);
    TexCoord = uvec2(aTexCoord.xy * (ssboDim - 1));
    ourColor = ColorAtCoord(TexCoord);
    // ourColor = vec3(0.25*gl_VertexID);
    
}