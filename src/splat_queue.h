#ifndef SPLAT_QUEUE_H
#define SPLAT_QUEUE_H


#include <queue>
#include "vec2.h"




struct splat{
    vec2 i_pos, f_pos, dir; 
};


class SplatQueue{


    public: 

        SplatQueue();

        SplatQueue(int sim_width, int sim_height);


        void build_splat(double x, double y);

        void push(splat s);

        splat pop();

        bool isEmpty();

        int size();

        void clear();
        
        void end_trail();
    
    public:

        void static resize(int new_win_width, int new_win_height){
            win_width = new_win_width;
            win_height = new_win_height;
        }

 

    

    private:
        bool start_trail = false;
        splat cur_splat;
        std::queue<splat> queue; 
        



    private:
        const unsigned int SIM_WIDTH, SIM_HEIGHT;
        static inline unsigned int win_width, win_height =0 ;


};

// unsigned int SplatQueue::win_width = 0;
// unsigned int SplatQueue::win_height = 0;


#endif