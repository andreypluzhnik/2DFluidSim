
#include "fluid_sim.h"



FluidSim::FluidSim(){
    forceQueue = new SplatQueue(SIM_WIDTH, SIM_HEIGHT);
    colorQueue = new SplatQueue(SIM_WIDTH, SIM_HEIGHT);
    winWidthHeight[0] = SIM_WIDTH;
    winWidthHeight[1] = SIM_HEIGHT;
};

FluidSim::FluidSim(float dt, float dl, float viscosity, const int diffusion_cycles, const int pressure_cycles) : DT(dt), DL(dl), 
VISCOSITY(viscosity), DIFFUSION_CYCLES(diffusion_cycles), PRESSURE_CYCLES(pressure_cycles) {
    forceQueue = new SplatQueue(SIM_WIDTH, SIM_HEIGHT);
    colorQueue = new SplatQueue(SIM_WIDTH, SIM_HEIGHT);
}


GLFWwindow* FluidSim::glfwWindow(){
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
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return NULL;
    }
    
    // glfw: assign self to GLFW user pointer
    // --------------------------------------
    glfwSetWindowUserPointer(window, this);

    // SplateQueue: initialize SplatQueue dimensions
    // -------------------------------
    SplatQueue::resize(winWidthHeight[0], winWidthHeight[1]);

    return window;   
}

GLuint FluidSim::genTexture2D(const size_t width, const size_t height, InitialCondition ic){
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


void FluidSim::bindImageTexture(const GLuint location, const GLuint texture){
    glBindImageTexture(location, texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA16F);
}

void FluidSim::bindTexture(const GLuint location, const GLuint texture){
    glActiveTexture(GL_TEXTURE0 + location);
    glBindTexture(GL_TEXTURE_2D, texture);
}

void FluidSim::unbindTexture(const GLuint texture){
    glBindTexture(texture, 0);
}


void FluidSim:: spectral_color(float &r, float &g, float &b) // RGB <0,1> <- lambda l <400,700> [nm]
    {
    pathLength +=1 ;
    int section = pathLength / 130;
    float l = (section % 2 == 0 ? pathLength - section * 130 : (section + 1) * 130 - pathLength) + 560;
    float t;  r=0.0; g=0.0; b=0.0;
         if ((l>=400.0)&&(l<410.0)) { t=(l-400.0)/(410.0-400.0); r=    +(0.33*t)-(0.20*t*t); }
    else if ((l>=410.0)&&(l<475.0)) { t=(l-410.0)/(475.0-410.0); r=0.14         -(0.13*t*t); }
    else if ((l>=545.0)&&(l<595.0)) { t=(l-545.0)/(595.0-545.0); r=    +(1.98*t)-(     t*t); }
    else if ((l>=595.0)&&(l<650.0)) { t=(l-595.0)/(650.0-595.0); r=0.98+(0.06*t)-(0.40*t*t); }
    else if ((l>=650.0)&&(l<700.0)) { t=(l-650.0)/(700.0-650.0); r=0.65-(0.84*t)+(0.20*t*t); }
         if ((l>=415.0)&&(l<475.0)) { t=(l-415.0)/(475.0-415.0); g=             +(0.80*t*t); }
    else if ((l>=475.0)&&(l<590.0)) { t=(l-475.0)/(590.0-475.0); g=0.8 +(0.76*t)-(0.80*t*t); }
    else if ((l>=585.0)&&(l<639.0)) { t=(l-585.0)/(639.0-585.0); g=0.84-(0.84*t)           ; }
         if ((l>=400.0)&&(l<475.0)) { t=(l-400.0)/(475.0-400.0); b=    +(2.20*t)-(1.50*t*t); }
    else if ((l>=475.0)&&(l<560.0)) { t=(l-475.0)/(560.0-475.0); b=0.7 -(     t)+(0.30*t*t); }
    }

int FluidSim::runSim(){
    
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
    //****************************************************//
    
    //***************** Diffusion Shader *****************//
    ShaderProgram cDiffusionShader;
    cDiffusionShader.addShader(cDiffusionPath, GL_COMPUTE_SHADER);
    cDiffusionShader.compileShaders();
    //****************************************************//

    //***************** Divergence Shader *****************//
    ShaderProgram cDivergenceShader;
    cDivergenceShader.addShader(cDivergencePath, GL_COMPUTE_SHADER);
    cDivergenceShader.compileShaders();
    //*****************************************************//


    //***************** Curl Shader *****************//
    ShaderProgram cCurlShader;
    cCurlShader.addShader(cCurlPath, GL_COMPUTE_SHADER);
    cCurlShader.compileShaders();
    //*****************************************************//

    //***************** Vorticity Shader *****************//
    ShaderProgram cVorticityShader;
    cVorticityShader.addShader(cVorticityPath, GL_COMPUTE_SHADER);
    cVorticityShader.compileShaders();
    //****************************************************//


    //***************** Pressure Field Projection Shader *****************//
    ShaderProgram cPressureProjectShader;
    cPressureProjectShader.addShader(cPressure_fieldProjectPath, GL_COMPUTE_SHADER);
    cPressureProjectShader.compileShaders();
    //********************************************************************//

    //***************** Force Splat Shader *****************//
    ShaderProgram cForceSplatShader;
    cForceSplatShader.addShader(cForceSplatPath, GL_COMPUTE_SHADER);
    cForceSplatShader.compileShaders();
    //******************************************************//

    //***************** Color Shader *****************//
    ShaderProgram cColorSplatShader;
    cColorSplatShader.addShader(cColorSplatPath, GL_COMPUTE_SHADER);
    cColorSplatShader.compileShaders();
    //************************************************//

    //***************** Copy Shader *****************//
    ShaderProgram cCopyShader;
    cCopyShader.addShader(cCopyPath, GL_COMPUTE_SHADER);
    cCopyShader.compileShaders();
    //***********************************************//


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
    // ------------------
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture attribute
    // ------------------
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    

    GLuint color_field[2], velocity_field[3], pressure_field[2], div_velocity_field, curl_velocity_field;

    color_field[0] = genTexture2D(SIM_WIDTH, SIM_HEIGHT, VERTICAL_BARS);
    color_field[1] = genTexture2D(SIM_WIDTH, SIM_HEIGHT, VERTICAL_BARS);
    pressure_field[0] = genTexture2D(SIM_WIDTH, SIM_HEIGHT, ZERO);
    pressure_field[1] = genTexture2D(SIM_WIDTH, SIM_HEIGHT, ZERO);
    velocity_field[0] = genTexture2D(SIM_WIDTH, SIM_HEIGHT, RANDOM);
    velocity_field[1] = genTexture2D(SIM_WIDTH, SIM_HEIGHT, ZERO);
    velocity_field[2] = genTexture2D(SIM_WIDTH, SIM_HEIGHT, ZERO);
    div_velocity_field = genTexture2D(SIM_WIDTH, SIM_HEIGHT, ZERO);
    curl_velocity_field = genTexture2D(SIM_WIDTH, SIM_HEIGHT, ZERO);
    

    

    GLuint force_field = genTexture2D(SIM_WIDTH, SIM_HEIGHT, ZERO);


    
    
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        
        // input
        // ----- 
        processInput(window);
       
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // advect dye step
        //----------------
        cAdvectShader.use();
        cAdvectShader.setFloat("dt", DT);

        bindTexture(0, color_field[0]);
        bindImageTexture(1, color_field[1]);
        bindTexture(2, velocity_field[0]);
        std::swap(color_field[0], color_field[1]);
        glDispatchCompute(SIM_WIDTH, SIM_HEIGHT, 1);
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

        
        // advect step
        //------------
        cAdvectShader.use();
        cAdvectShader.setFloat("dt", DT);

        bindTexture(0, velocity_field[0]);
        bindImageTexture(1, velocity_field[2]);
        bindTexture(2, velocity_field[0]);

        glDispatchCompute(SIM_WIDTH, SIM_HEIGHT, 1);
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);


        // curl step
        //---------------

        cCurlShader.use();
        cCurlShader.setFloat("dl", DL);
        bindTexture(0, velocity_field[2]);
        bindImageTexture(1, curl_velocity_field);

        glDispatchCompute(SIM_WIDTH, SIM_HEIGHT, 1);
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

        // vorticity step
        //---------------
        
        cVorticityShader.use();
        cVorticityShader.setFloat("vorticity", VORTICITY);
        cVorticityShader.setFloat("DT", DT);
        bindTexture(0, curl_velocity_field);
        bindTexture(0, velocity_field[2]);
        

 

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
        while(!(forceQueue->isEmpty())){

            splat forceSplat = forceQueue->pop();
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

        while(!(colorQueue->isEmpty())){
            float r,g,b = 0;
            spectral_color(r, g, b);
            splat colorSplat = colorQueue->pop();

            cColorSplatShader.use();
            cColorSplatShader.setFloat("radius", 8);
            cColorSplatShader.setVec2f("center", float(colorSplat.i_pos[0]), float(colorSplat.i_pos[1]));
            cColorSplatShader.setVec3f("color", r, g, b);
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
        
        for(int cycle = 0; cycle < PRESSURE_CYCLES; ++cycle){

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


        bindTexture(0, color_field[1]);
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


