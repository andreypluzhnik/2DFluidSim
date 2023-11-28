#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include <algorithm>

#include "stbi_include.h"
#include "shader.h"
#include "bitmap.h"
#include "shader_program.h"
#include "splat_queue.h"
#include "vec2.h"


// void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
void mousePositionCallback(GLFWwindow *window, double xpos, double ypos);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);


// settings
const unsigned int SIM_WIDTH = 256;
const unsigned int SIM_HEIGHT = 256;

const unsigned int SplatQueue::SIM_WIDTH = 256;
const unsigned int SplatQueue::SIM_HEIGHT = 256;



int win_width = SIM_WIDTH;
int win_height = SIM_HEIGHT;


const char* vertexPath = "vertex_shader.glsl";
const char* fragmentPath = "fragment_shader.glsl";
const char* cForcePath = "force.glsl";
const char* cAdvectPath = "advection.glsl";
const char* cDiffusionPath = "diffusion.glsl";
const char* cDivergencePath = "divergence.glsl";
const char* cPressure_fieldProjectPath = "pressure_projection.glsl";
const char* cForceSplatPath = "force_splat.glsl";
const char* cColorSplatPath = "color_splat.glsl";
const char* cCopyPath = "copy.glsl";


const float DT = 1;
const float DL = 1;
const float VISCOSITY = 0.008;
const int DIFFUSION_CYCLES = 50;
const int POISSON_CYCLES = 20;

SplatQueue forceQueue, colorQueue;
double mouseX, mouseY;

enum InitialCondition {
    RANDOM, VERTICAL_BARS, MONOCHROME, VERTICAL_SPRAY, ZERO
};



GLFWwindow* glfwWindow(){

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SIM_WIDTH, SIM_HEIGHT, "FluidSim", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mousePositionCallback);
    // glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return NULL;
    }
    
    //initialize SplatQueue dimensions
    SplatQueue::resize(win_width, win_height);

    return window;
}

// for image2D's
void bindImageTexture(const GLuint location, const GLuint texture){
    glBindImageTexture(location, texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA16F);
}

// for samplers
void bindTexture(const GLuint location, const GLuint texture){
    glActiveTexture(GL_TEXTURE0 + location);
    glBindTexture(GL_TEXTURE_2D, texture);
}

void unbindTexture(const GLuint texture){
        glBindTexture(texture, 0);

}





GLuint genTexture2D(const size_t width, const size_t height, InitialCondition ic){
    GLuint texture;
    // generate and bind texture   
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // set texture magnification parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load and create texture data
    bitmap* color_map = new bitmap(width, height);
    switch(ic){
        case(RANDOM):
            color_map->random_populate();
            break;
        case(VERTICAL_BARS):
                                       // red        cyan
            color_map->vertical_bars(1.0, 0.0, 0.0, 0.0, 1.0, 1.0);
            break;
        case(MONOCHROME):
            color_map->monochrome_populate(0.05, 0.0, 0.0);
            break;
        case(ZERO):
            color_map->monochrome_populate(0.0, 0.0, 0.0);
            break;
        case(VERTICAL_SPRAY):
            color_map->decaying_vertical_spray(0.001 , 4 / color_map->get_height() , 90, int(color_map->get_width() * 0.5), -30);
            break;

    }
    float* data = color_map->get_float_array();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SIM_WIDTH, SIM_HEIGHT, 0, GL_RGBA, GL_FLOAT, data);

    delete color_map;
    // unbind texture
    unbindTexture(texture);
    
    return texture;
}




int main(){


    GLFWwindow* window = glfwWindow();
    if(window == NULL){
        return -1;
    }

    // init fragment and vertex shaders
    ShaderProgram vfShader;
    vfShader.addShader(vertexPath, GL_VERTEX_SHADER);
    vfShader.addShader(fragmentPath, GL_FRAGMENT_SHADER);
    vfShader.compileShaders();


    //***************** Force Shader *****************//
    ShaderProgram cForceShader;
    cForceShader.addShader(cForcePath, GL_COMPUTE_SHADER);
    cForceShader.compileShaders();
    //************************************************//

    //***************** Advection Shader *****************//
    ShaderProgram cAdvectShader;
    cAdvectShader.addShader(cAdvectPath, GL_COMPUTE_SHADER);
    cAdvectShader.compileShaders();
    //************************************************//
    
    //***************** Diffusion Shader *****************//
    ShaderProgram cDiffusionShader;
    cDiffusionShader.addShader(cDiffusionPath, GL_COMPUTE_SHADER);
    cDiffusionShader.compileShaders();
    //************************************************//

    //***************** Divergence Shader *****************//
    ShaderProgram cDivergenceShader;
    cDivergenceShader.addShader(cDivergencePath, GL_COMPUTE_SHADER);
    cDivergenceShader.compileShaders();
    //************************************************//

    //***************** Pressure Field Projection Shader *****************//
    ShaderProgram cPressureProjectShader;
    cPressureProjectShader.addShader(cPressure_fieldProjectPath, GL_COMPUTE_SHADER);
    cPressureProjectShader.compileShaders();
    //************************************************//

    //***************** Force Splat Shader *****************//
    ShaderProgram cForceSplatShader;
    cForceSplatShader.addShader(cForceSplatPath, GL_COMPUTE_SHADER);
    cForceSplatShader.compileShaders();
    //************************************************//

    //***************** Color Shader *****************//
    ShaderProgram cColorSplatShader;
    cColorSplatShader.addShader(cColorSplatPath, GL_COMPUTE_SHADER);
    cColorSplatShader.compileShaders();
    //************************************************//

    //***************** Color Shader *****************//
    ShaderProgram cCopyShader;
    cCopyShader.addShader(cCopyPath, GL_COMPUTE_SHADER);
    cCopyShader.compileShaders();
    //************************************************//    



    float vertices[] = {
        // positions         // texture coords
         1.0f,  1.0f,   1.0f, 1.0f, // top right
         1.0f, -1.0f,   1.0f, 0.0f, // bottom right
        -1.0f, -1.0f,   0.0f, 0.0f, // bottom left
        -1.0f,  1.0f,   0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    // vertex arrays and buffers setup
    // -------------------------
    unsigned int VBO, EBO, VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);


    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    

    GLuint color_field[2], velocity_field[3], pressure_field[2], div_velocity_field;

    color_field[0] = genTexture2D(SIM_WIDTH, SIM_HEIGHT, VERTICAL_BARS);
    color_field[1] = genTexture2D(SIM_WIDTH, SIM_HEIGHT, VERTICAL_BARS);
    pressure_field[0] = genTexture2D(SIM_WIDTH, SIM_HEIGHT, ZERO);
    pressure_field[1] = genTexture2D(SIM_WIDTH, SIM_HEIGHT, ZERO);
    velocity_field[0] = genTexture2D(SIM_WIDTH, SIM_HEIGHT, RANDOM);
    velocity_field[1] = genTexture2D(SIM_WIDTH, SIM_HEIGHT, ZERO);
    velocity_field[2] = genTexture2D(SIM_WIDTH, SIM_HEIGHT, ZERO);
    div_velocity_field = genTexture2D(SIM_WIDTH, SIM_HEIGHT, ZERO);

    

    

    GLuint force_field = genTexture2D(SIM_WIDTH, SIM_HEIGHT, ZERO);


    // bindTexture(0, tex);



    
    
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        
        // input
        // ----- 
        processInput(window);

        // if(add_splat == true){
                
        // }
        
       
       
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

  
        cAdvectShader.use();
        cAdvectShader.setFloat("dt", DT);

        bindTexture(0, color_field[0]);
        bindImageTexture(1, color_field[1]);
        bindTexture(2, velocity_field[0]);
        std::swap(color_field[0], color_field[1]);
        glDispatchCompute(SIM_WIDTH, SIM_HEIGHT, 1);
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

        cAdvectShader.use();
        cAdvectShader.setFloat("dt", DT);

        bindTexture(0, velocity_field[0]);
        bindImageTexture(1, velocity_field[2]);
        bindTexture(2, velocity_field[0]);

        glDispatchCompute(SIM_WIDTH, SIM_HEIGHT, 1);
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);



        float alpha = DL * DL / (VISCOSITY * DT);
        float beta = 4 + alpha;
        
        
        for(int cycle = 0; cycle < DIFFUSION_CYCLES; ++cycle){
            cDiffusionShader.use();
            cDiffusionShader.setFloat("alpha", alpha);
            cDiffusionShader.setFloat("beta", beta);
            cDiffusionShader.setInt("boundary_sign", -1);
            bindTexture(0, velocity_field[0]);
            bindImageTexture(1, velocity_field[1]);
            bindTexture(2, velocity_field[2]);
            glDispatchCompute(SIM_WIDTH/32, SIM_HEIGHT/32, 1);
            glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
            std::swap(velocity_field[0], velocity_field[1]);

        }


        // force splat
        while(!forceQueue.isEmpty()){

            splat forceSplat = forceQueue.pop();
            cForceSplatShader.use();
            cForceSplatShader.setFloat("dt", DT);
            cForceSplatShader.setFloat("intensity", 4);
            cForceSplatShader.setVec2f("center", float(forceSplat.f_pos[0]), float(forceSplat.f_pos[1]));
            cForceSplatShader.setVec2f("direction", float(forceSplat.dir[0]), float(forceSplat.dir[1]));
            cForceSplatShader.setFloat("spatial_decay", 0.5);
            bindTexture(0, velocity_field[0]);
            bindImageTexture(1, velocity_field[1]);
            glDispatchCompute(SIM_WIDTH, SIM_HEIGHT, 1);
            glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
            std::swap(velocity_field[0], velocity_field[1]);
        }

        while(!colorQueue.isEmpty()){
           
            splat colorSplat = colorQueue.pop();

            cColorSplatShader.use();
            cColorSplatShader.setFloat("radius", 8);
            cColorSplatShader.setVec2f("center", float(colorSplat.i_pos[0]), float(colorSplat.i_pos[1]));
            cColorSplatShader.setVec3f("color", 1.0, 1.0, 0.0);
            bindImageTexture(0, color_field[0]);
            glDispatchCompute(SIM_WIDTH, SIM_HEIGHT, 1);
            glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

        }
        
        // force field
        cForceShader.use();
        cForceShader.setFloat("dt", DT);
        bindTexture(0, force_field);
        bindImageTexture(1, velocity_field[0]);
        glDispatchCompute(SIM_WIDTH, SIM_HEIGHT, 1);
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

        

        cDivergenceShader.use();
        cDivergenceShader.setFloat("dl", DL);
        bindTexture(0, velocity_field[0]);
        bindImageTexture(1, div_velocity_field);
        glDispatchCompute(SIM_WIDTH, SIM_HEIGHT, 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);


        cCopyShader.use();
        cCopyShader.setFloat("scale", 1);
        bindTexture(0, pressure_field[0]);
        bindImageTexture(1, pressure_field[1]);
        std::swap(pressure_field[0], pressure_field[1]);


        alpha = - DL * DL;
        beta = 4;
        cDiffusionShader.use();
        cDiffusionShader.setFloat("alpha", alpha);
        cDiffusionShader.setFloat("beta", beta);
        cDiffusionShader.setInt("boundary_sign", 1);
        
        for(int cycle = 0; cycle < POISSON_CYCLES; ++cycle){

            bindTexture(0, pressure_field[0]);
            bindImageTexture(1, pressure_field[1]);
            bindTexture(2, div_velocity_field);
            glDispatchCompute(SIM_WIDTH/32, SIM_HEIGHT/32, 1);
            glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

            std::swap(pressure_field[0], pressure_field[1]);

        }


        cPressureProjectShader.use();
        cPressureProjectShader.setFloat("dl", DL);
        bindTexture(0, velocity_field[0]);
        bindTexture(1, pressure_field[0]);
        bindImageTexture(2, velocity_field[1]);
        glDispatchCompute(SIM_WIDTH, SIM_HEIGHT, 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
        std::swap(velocity_field[0], velocity_field[1]);


        bindTexture(0, color_field[0]);
        vfShader.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
       
    
    }


    // glfw: terminate, clearing all previously allocated GLFW resources.
    //------------------------------------------------------------------
    glfwTerminate();
    return 0;
}



// process all keyboard input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);


}


// mouse callback: track left and right button presses during mouse motion to generate splats
// ---------------------------------------------------------------------------------------------------------
void mousePositionCallback(GLFWwindow *window, double xpos, double ypos){
    
    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){

        glfwGetCursorPos(window, &mouseX, &mouseY);
        forceQueue.build_splat(mouseX, mouseY);
    }else{
        forceQueue.end_trail();
    }

    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS){
        glfwGetCursorPos(window, &mouseX, &mouseY);
        colorQueue.build_splat(mouseX, mouseY);
    }else{
        colorQueue.end_trail();
    }

    // std::cout << xpos << "|"<< ypos << std::endl;

}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    win_width = width;  
    win_height = height;
    SplatQueue::resize(win_width, win_height);

    glViewport(0, 0, width, height);
    
}