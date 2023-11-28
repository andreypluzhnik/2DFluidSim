#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "shader.h"
#include "shader_program.h"
      
        
    ShaderProgram::ShaderProgram(std::vector<Shader> shaders){
        shader_list = shaders;
    }

    ShaderProgram::ShaderProgram(){
         ID = glCreateProgram();
    }
    // core functions
    // ------------------------------------------------------------------------
    void ShaderProgram::addShader(const char* path, GLenum type){
        Shader s(path, type);
        shader_list.push_back(s);
    }
    // ------------------------------------------------------------------------
    void ShaderProgram::use(){
        glUseProgram(ID);
    }
    // ------------------------------------------------------------------------
    void ShaderProgram::compileShaders(){
        for(Shader s : shader_list){
            unsigned int shader_ID = s.getID();
            const char* shader_name = s.getName();
            checkCompileErrors(shader_ID, shader_name); 

            glAttachShader(ID, shader_ID);

        }

        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");
        
        // just to be safe, delete after linking
        for(Shader s : this->shader_list){
            glDeleteShader(s.getID());

        }

    }


    // utility uniform functions
    // ------------------------------------------------------------------------
    void ShaderProgram::setBool(const std::string &name, bool value) const
    {         
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
    }
    // ------------------------------------------------------------------------
    void ShaderProgram::setFloat(const std::string &name, float value) const
    { 
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
    }
    // ------------------------------------------------------------------------
    void ShaderProgram::setInt(const std::string &name, int value) const
    { 
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
    }

    void ShaderProgram::setVec2i(const std::string &name, int v1, int v2) const{
        glUniform2i(glGetUniformLocation(ID, name.c_str()), v1, v2);
    }

    void ShaderProgram::setVec2f(const std::string &name, float v1, float v2) const {
        glUniform2f(glGetUniformLocation(ID, name.c_str()), v1, v2);
    }

    void ShaderProgram::setVec3f(const std::string &name, float v1, float v2, float v3) const {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), v1, v2, v3);

    }


    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void ShaderProgram::checkCompileErrors(unsigned int shader, std::string type)
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }else{
                std::cout << "SHADER COMPILATION SUCCESSFUL" << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }else{
                std::cout << "PROGRAM COMPILATION SUCCESSFUL" << std::endl<<std::endl;
            }
        }
    }

