#ifndef FLUID_SIM_H
#define FLUID_SIM_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include <algorithm>
#include <memory>

#include "splat_queue.h"
#include "bitmap.h"
#include "shader_program.h"
#include "vec2.h"




enum InitialCondition {
    RANDOM, VERTICAL_BARS, MONOCHROME, VERTICAL_SPRAY, ZERO, HORIZONTAL_JET, RECTANGLE
};

class FluidSim {
    public:
        const unsigned int SIM_WIDTH = 1280;
        const unsigned int SIM_HEIGHT = 720;
        const float DT = 1;
        const float DL = 1;
        const float VISCOSITY = 0.001;
        bool ENABLE_DIFFUSION = true;
        bool ENABLE_BG_FORCE = true;
        const int DIFFUSION_CYCLES = 50;
        const int PRESSURE_CYCLES = 70;
        const float VORTICITY = 0.4;

        const uint block_size_x = 32;
        const uint block_size_y = 32;

        // shader file paths
        const char* vertexPath = "vertex_shader.glsl";
        const char* fragmentPath = "fragment_shader.glsl";
        const char* cForcePath = "force.glsl";
        const char* cAdvectPath = "advection.glsl";
        const char* cDiffusionPath = "diffusion.glsl";
        const char* cDivergencePath = "divergence.glsl";
        const char* cCurlPath = "curl.glsl";
        const char* cVorticityPath = "vorticity.glsl";
        const char* cPressure_fieldProjectPath = "pressure_projection.glsl";
        const char* cForceSplatPath = "force_splat.glsl";
        const char* cColorSplatPath = "color_splat.glsl";
        const char* cCopyPath = "copy.glsl";
        
        // mouse coordinate and window dimension information
        vec2 mouseXY; 
        point2 winWidthHeight;
                
        //splat objects
        SplatQueue* forceQueue; 
        SplatQueue* colorQueue;

        // splat coloring
        unsigned int pathLength = 0.001;

        // constants for jacobi iteration
        float alpha, beta;

    public:
        FluidSim();
        FluidSim(float dt, float dl, float viscosity, const int diffusion_cycles, const int pressure_cycles);



        // glfw window initialization
        GLFWwindow* glfwWindow();
        // generate texture ID and populate texture
        GLuint genTexture2D(const size_t width, const size_t height, InitialCondition ic); 
        // bind texture unit image2D
        void bindImageTexture(const GLuint location, const GLuint texture);
        // bind texture unit to sampler
        void bindTexture(const GLuint location, const GLuint texture);
        // unbind bound texture
        void unbindTexture(const GLuint texture);
        // runs the fluid simulation
        int runSim();
        // color update
        //https://stackoverflow.com/questions/3407942/rgb-values-of-visible-spectrum/22681410#22681410
        void spectral_color(float &r,float &g,float &b); // RGB <0,1> <- lambda l <400,700> [nm]
        //-----


    public:
        // input handling 
        static void mousePositionCallback(GLFWwindow *window, double xpos, double ypos){
            
            FluidSim* simInstance = (FluidSim*)glfwGetWindowUserPointer(window);
            if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
                
                glfwGetCursorPos(window, simInstance->mouseXY.pu(), simInstance->mouseXY.pv());
                simInstance->forceQueue->build_splat(simInstance->mouseXY.u(), simInstance->mouseXY.v());
            }else{
                simInstance->forceQueue->end_trail();
            }

            if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS){
                glfwGetCursorPos(window, simInstance->mouseXY.pu(), simInstance->mouseXY.pv());
                simInstance->colorQueue->build_splat(simInstance->mouseXY.u(), simInstance->mouseXY.v());
            }else{
                simInstance->colorQueue->end_trail();
            }

        }

        static void framebufferSizeCallback(GLFWwindow* window, int width, int height)
        {
            FluidSim* simInstance = (FluidSim*)glfwGetWindowUserPointer(window);
            simInstance->winWidthHeight[0] = width;  
            simInstance->winWidthHeight[1] = height;
            SplatQueue::resize(simInstance->winWidthHeight.u(), simInstance->winWidthHeight.v());

            glViewport(0, 0, width, height);
            
        }

        static void processInput(GLFWwindow *window){
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(window, true);

        }


};





#endif