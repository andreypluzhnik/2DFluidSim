#ifndef COMPUTE_SHADER_H
#define COMPUTE_SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

// adapted from https://learnopengl.com/Getting-started/Shaders

class Shader{
    public:

        // 'type' refers to shader type defined in OpenGL, primarily GL_COMPUTE_SHADER, GL_FRAGMENT_SHADER,
        // and GL_VERTEX_SHADER are used 
        Shader(const char* computePath, GLenum type);
        
        unsigned int getID() const;
        const char* getName();


    private:

        unsigned int shader_ID;
        const char* shader_name;
        
};





#endif