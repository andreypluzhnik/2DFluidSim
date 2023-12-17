

#include "splat_queue.h"

SplatQueue::SplatQueue(int sim_width, int sim_height) : SIM_WIDTH(sim_width), SIM_HEIGHT(sim_height) {

    cur_splat.i_pos[0] = -1;
    cur_splat.i_pos[1] = -1;

}

SplatQueue::SplatQueue() : SIM_WIDTH(0), SIM_HEIGHT(0) {
   
    cur_splat.i_pos[0] = -1;
    cur_splat.i_pos[1] = -1;

}


void SplatQueue::build_splat(double x, double y){

    x = std::min(std::max(0.0, x), double(win_width) - 1) * float(SIM_WIDTH) / float(win_width);
    y = std::min(std::max(0.0, y), double(win_height) - 1)* float(SIM_HEIGHT) / float(win_height);
    y = SIM_HEIGHT - y;

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

void SplatQueue::push(splat s){
    queue.push(s);
    cur_splat.i_pos = cur_splat.f_pos;
}

splat SplatQueue::pop(){
    
    splat ret = queue.front();
    ret.dir = (ret.f_pos - ret.i_pos);
    ret.dir = ret.dir / ret.dir.length();
    queue.pop();
    return ret;
}

bool SplatQueue::isEmpty(){
    return queue.empty();
}

int SplatQueue::size(){
    return queue.size();
}

void SplatQueue::clear(){
    for(int i = 0; i < size(); i++){
        queue.pop();
    }
}

void SplatQueue::end_trail(){
    start_trail = false;
}

