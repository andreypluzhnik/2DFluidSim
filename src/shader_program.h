#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "shader.h"

class ShaderProgram{
    public:
        
        ShaderProgram();
        ShaderProgram(std::vector<Shader> shader_list);

        // add shader program
        void addShader(const char* path, GLenum type);
        void use();
        void compileShaders();


        // utility uniform functions
        // ------------------------------------------------------------------------
        void setBool(const std::string &name, bool value) const;
        // ------------------------------------------------------------------------
        void setFloat(const std::string &name, float value) const;
        // ------------------------------------------------------------------------
        void setInt(const std::string &name, int value) const;
        // ------------------------------------------------------------------------
        void setVec2i(const std::string &name, int v1, int v2) const;
        // ------------------------------------------------------------------------
        void setVec2f(const std::string &name, float v1, float v2) const;
        // ------------------------------------------------------------------------
        void setVec3f(const std::string &name, float v1, float v2, float v3) const;


    private:
        // utility function for checking shader compilation/linking errors.
        // ------------------------------------------------------------------------

        void checkCompileErrors(unsigned int shader, std::string type);


    private:
        std::vector<Shader> shader_list;
        unsigned int ID = 5;
};



#endif