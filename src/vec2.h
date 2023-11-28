#ifndef VEC2_H
#define VEC2_H


#include <cmath>
#include <iostream>

using std::sqrt;

class vec2 {
    public:
        double e[2];
        vec2() : e{0,0} {}
        vec2(double u, double v) : e{u, v} {};

        double u() const {return e[0];}
        double v() const {return e[1];}

        vec2 operator-() const {return vec2(-e[0], -e[1]); }
        double operator[](int i) const {return e[i];}
        double& operator[](int i) {return e[i];}

        vec2& operator+=(const vec2 &v){
            e[0] += v.e[0];
            e[1] += v.e[1];
            return *this;
        }

        vec2& operator*=(const double t){
            e[0] *= t;
            e[1] *= t;
            return *this;
        }

        vec2& operator/=(const double t){
            return *this *= 1/t;
        }

        double length() const {
            return sqrt(length_squared());
        }

        double length_squared() const {
            return e[0]*e[0] + e[1]*e[1];
        }

        bool near_zero() const {
            const auto s = 1e-8;
            // return true if vector small in all dimensions 
            return (fabs(e[0] < s) && fabs(e[1] < s));
        }
    
        
};

using point2 = vec2;

// vec2 utility functions

inline std::ostream& operator<<(std::ostream &out, const vec2 &v){
    return out << v.e[0] << ' ' << v.e[1];
}

inline vec2 operator+(const vec2 &a, const vec2 &b){
    return vec2(a.e[0] + b.e[0], a.e[1] + b.e[1]);
}

inline vec2 operator-(const vec2&a, const vec2&b){
    return vec2(a.e[0] - b.e[0], a.e[1] - b.e[1]);
}

inline vec2 operator*(const vec2 &a, const vec2 &b){
    return vec2(a.e[0] * b.e[0], a.e[1] * b.e[1]);
}

inline vec2 operator*(double t, const vec2& p){
    return vec2(t * p.e[0], t * p.e[1]);
}

inline vec2 operator* (const vec2 &v, double t){
    return t * v;
}

inline vec2 operator/(vec2 v, double t){
    return (1/t) * v;
}

inline double dot(const vec2 &a, const vec2 &b){
    return a.e[0] * b.e[0]
    + a.e[1] * b.e[1];
}

inline vec2 unit_vector(vec2 p){
    return p / p.length();
}





#endif


