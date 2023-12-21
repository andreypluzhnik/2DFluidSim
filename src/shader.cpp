
#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "shader.h"

// adapted from https://learnopengl.com/Getting-started/Shaders


   
    unsigned int ID = 5;

    Shader::Shader(const char* path, GLenum type) : shader_name(path){
        // 1. retrieve the vertex/fragment source code from filePath
        std::string shaderCode;
        std::ifstream shaderFile;
        // ensure ifstream objects can throw exceptions:
        shaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

        try 
        {
            // open files
            shaderFile.open(path);
            std::stringstream shaderStream;
            // read file's buffer contents into streams
            shaderStream << shaderFile.rdbuf();
            // close file handlers
            shaderFile.close();
            // convert stream into string
            shaderCode = shaderStream.str();
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
        }

        std::cout<< "SHADER " << shader_name << " SUCCESSFULLY READ" << std::endl;
        const char* cShaderCode = shaderCode.c_str();



        // compute shader
        shader_ID = glCreateShader(type);
        glShaderSource(shader_ID, 1, &cShaderCode, NULL);
        glCompileShader(shader_ID);
    }


    unsigned int Shader::getID() const{
        return shader_ID;
    }

    const char* Shader::getName(){
        return shader_name;
    }


