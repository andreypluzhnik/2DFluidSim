#ifndef SPLAT_HANDLER_H
#define SPLAT_HANDLER_H



#include <queue>
#include "vec2.h"



struct splat{
    vec2 i_pos, f_pos, dir; 
};

class SplatQueue{


public: 

    SplatQueue() {

        cur_splat.i_pos[0] = -1;
        cur_splat.i_pos[1] = -1;

    }

    void build_splat(double x, double y){

        x = std::min(std::max(0.0, x), double(win_width) - 1) * float(SIM_WIDTH) / float(win_width);
        y = std::min(std::max(0.0, y), double(win_height) - 1)* float(SIM_HEIGHT) / float(win_height);
        y = win_height - y;

        if(!start_trail){
            start_trail = true;
            cur_splat.i_pos[0] = x;
            cur_splat.i_pos[1] = y;
        }else if(!(cur_splat.i_pos[0] == x && cur_splat.i_pos[1] == y)){
            cur_splat.f_pos[0] = x;
            cur_splat.f_pos[1] = y;
            push(cur_splat);
        }  
    }

    void push(splat s){
        queue.push(s);
        cur_splat.i_pos = cur_splat.f_pos;
    }

    splat pop(){
        
        splat ret = queue.front();
        ret.dir = (ret.f_pos - ret.i_pos);
        ret.dir = ret.dir / ret.dir.length();
        queue.pop();
        return ret;
    }

    bool isEmpty(){
        return queue.empty();
    }

    int size(){
        return queue.size();
    }

    void clear(){
        for(int i = 0; i < size(); i++){
            queue.pop();
        }
    }

    void end_trail(){
        start_trail = false;
    }
    
   
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
    static const unsigned int SIM_WIDTH, SIM_HEIGHT;
    static unsigned int win_width, win_height;


};


unsigned int SplatQueue::win_width = 0;
unsigned int SplatQueue::win_height = 0;



#endif