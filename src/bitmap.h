#ifndef BITMAP_H
#define BITMAP_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>


class bitmap {

    public:
        bitmap(const size_t w, const size_t h) : WIDTH(w), HEIGHT(h), PHYSICAL_SIZE(w * h * 4 * sizeof(float)){
            // allocate memory
            data = new float[WIDTH * HEIGHT * 4];

        }

        bitmap(const bitmap &input) : WIDTH(input.get_width()),
                                     HEIGHT(input.get_height()),
                                     PHYSICAL_SIZE(input.get_physical_size()){
                
                data = new float[WIDTH * HEIGHT * 4];

                for(size_t row = 0; row < HEIGHT; ++row){
                    for(size_t col = 0; col <  WIDTH; ++col){
                        size_t idx = 4 * (row * WIDTH + col);
                        data[idx] = get_data_at_index(idx);
                        data[idx + 1] = get_data_at_index(idx + 1);
                        data[idx + 2] = get_data_at_index(idx + 2);
                        data[idx + 3] = get_data_at_index(idx + 3);
                    } 
                }


        }

        void random_populate(){
            for(size_t row = 0; row <  HEIGHT; ++row){
                for(size_t col = 0; col <  WIDTH; ++col){
                    write_color_to_row_col(row, col, random_double(-1, 1), random_double(-1, 1), random_double(-1, 1), 1.0);
                } 
            }
        }

        void monochrome_populate(float r, float g, float b){
            for(size_t row = 0; row <  HEIGHT; ++row){
                for(size_t col = 0; col <  WIDTH; ++col){
                    if(col < WIDTH * 0.9 && row >= HEIGHT * 0.4 && row <= HEIGHT * 0.6){
                        write_color_to_row_col(row, col, r, g, b, 1.0);
                    }else{
                        write_color_to_row_col(row, col, 0, 0, 0, 1.0);
                    }
                } 
            }
        }

        void vertical_bars(float r1, float g1, float b1, float r2, float g2, float b2){
            for(size_t row = 0; row <  HEIGHT; ++row){
                for(size_t col = 0; col <  WIDTH; ++col){
                    if(row >= 0.2 * HEIGHT && row <= 0.8 * HEIGHT){
                        if(col >= 0.15 * WIDTH && col <= 0.2 * WIDTH){
                            write_color_to_row_col(row, col, r1, g1, b1, 1.0);
                        }else if(col >= 0.80 * WIDTH && col <= 0.85 * WIDTH){
                            write_color_to_row_col(row, col, r2, g2, b2, 1.0);
                        }   

                    }else{
                        write_color_to_row_col(row, col, 0.0, 0.0, 0.0, 1.0);
                    }

                } 
            }
        }

        void horizontal_jets(float r1, float g1, float b1, float r2, float g2, float b2, int y_spacing, int thickness){
            y_spacing = int(clamp(y_spacing, 0, HEIGHT));

            
            for(size_t row = 0; row <  HEIGHT; ++row){
                for(size_t col = 0; col <  WIDTH; ++col){
                    if(row >= 0.5 * (HEIGHT + y_spacing) && row <= 0.5 * (HEIGHT + y_spacing) + thickness){
                        write_color_to_row_col(row, col, r2, g2, b2, 1.0);

                    }else if(row <= 0.5 * (HEIGHT - y_spacing) && row >= 0.5 * (HEIGHT - y_spacing) - thickness){
                        write_color_to_row_col(row, col, r1, g1, b1, 1.0);
                    }else if(col <= 0.5 * WIDTH){
                        write_color_to_row_col(row, col, 0, 0.0, 0.0, 1.0);
                    }else{
                        write_color_to_row_col(row, col, 0, 0.0, 0.0, 1.0);
                    }
                }
            }

        }

        void rectangle(float r, float g, float b, int top, int left, int w, int h){
            
            
            for(size_t row = 0; row <  HEIGHT; ++row){
                for(size_t col = 0; col <  WIDTH; ++col){
                    if(row >= top && row <= top + h && col >= left && col <= left + w){
                        write_color_to_row_col(row, col, r, g , b, 1.0);
                    }else{
                        write_color_to_row_col(row, col, 0.0, 0.0, 0.0, 1.0);
                    }
                }
            }

        }
        

        // Maps the function aexp(-x sigma) in an angular spread around a specified point 
        // where x is distance from origin and spread is spread a 2d 'cone' in degrees
        // a : Amplitude of function at origin of spread
        // sigma : Distance scaling factor
        // spread : Only points within the line of angle spread will adhere to function. 
        // offset_x, offset_y : origin of 'cone' 

        void decaying_vertical_spray(float a, float sigma, float spread, int offset_x, int offset_y){
            spread = clamp(spread, 1, 180.0);
            spread = 90 - spread * 0.5;

            // convert to radians
            spread = spread * pi / 180.0;
            // get slope
            float slope = tan(spread);
            // guarantee that sigma is positive to avoid exponential growth
            sigma = abs(sigma);

            for(int row = 0; row <  HEIGHT; ++row){
                for(int col = 0; col <  WIDTH; ++col){
                    if( row > (abs(int(col - offset_x)) + offset_y) ){
                        float dist_to_func_origin = sqrt((row - offset_y) * (row - offset_y) + (col - offset_x) * (col - offset_x));
                        float func_val = a * exp(-dist_to_func_origin * sigma);  

                        write_color_to_row_col(row, col, func_val * (col - offset_x)/dist_to_func_origin, func_val * (row - offset_y)/dist_to_func_origin, 0.0, 0.0);

                    }else{
                        write_color_to_row_col(row, col, 0.0, 0.0, 0.0, 0.0);
                    }


                } 
            }
            
        }



     
        float* get_float_array(){
            return data;
        }

        float* get_float_array_copy(){
            float* data_copy= new float[WIDTH * HEIGHT * 4];
            
            for(size_t row = 0; row <  HEIGHT; ++row){
                for(size_t col = 0; col <  WIDTH; ++col){
                    data_copy[4 * (row * WIDTH + col)] = data[4 * (row * WIDTH + col)];
                    data_copy[4 * (row * WIDTH + col) + 1] = data[4 * (row * WIDTH + col) + 1];
                    data_copy[4 * (row * WIDTH + col) + 2] = data[4 * (row * WIDTH + col) + 2];
                    data_copy[4 * (row * WIDTH + col) + 2] = data[4 * (row * WIDTH + col) + 3];
                } 
            } 

            return data_copy;

        }

        
        float get_data_at_index(size_t idx){
            return data[idx];
        }

        size_t get_physical_size() const { 
            return PHYSICAL_SIZE;
        }

        size_t get_width() const {
            return WIDTH;
        }

        size_t get_height() const {
            return HEIGHT;
        }

       

        ~bitmap(){
            delete [] data;
        }




    private:
        // WIDTH and HEIGHT give dimensions in pixels
        const size_t WIDTH, HEIGHT, PHYSICAL_SIZE;
        float* data;

        // random double from 0(inclusive) to 1
        float random_double(){
            return static_cast<float>(rand() / (static_cast<float>(RAND_MAX) + 1.0));
        }

        float random_double(double min, double max){
            // return random number in [min,max)
            return min + (max - min) * random_double();
        }

        void write_color_to_row_col(size_t row, size_t col, float r, float g, float b, float a){
            data[4 * (row * WIDTH + col)] = r;
            data[4 * (row * WIDTH + col) + 1] = g;
            data[4 * (row * WIDTH + col) + 2] = b;
            data[4 * (row * WIDTH + col) + 3] = a;

        }


        float clamp(double x, double min, double max){
            if(x < min) return min;
            if(x > max) return max;
            return x;
        }

    
        const double pi = 3.1415926535897932384626433832795028841971;



};



#endif