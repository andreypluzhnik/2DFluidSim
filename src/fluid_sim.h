#ifndef FLUID_SIM
#define FLUID_SIM






class FluidSim {
    public:
        static const unsigned int SIM_WIDTH = 256;
        const unsigned int SIM_HEIGHT = 256;
        const float DT = 1;
        const float DL = 1;
        const float VISCOSITY = 0.008;
        const int DIFFUSION_CYCLES = 50;
        const int POISSON_CYCLES = 20;
};




#endif