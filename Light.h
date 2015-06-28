#ifndef __LIGHT_H__
#define __LIGHT_H__

#include <glut.h>
#include <cmath>

class Light {
public:
    Light(float x, float y, float z, int l, int r);
    ~Light();
    void enable();
    void luminance_add(float x, float y);
    void luminance_sub(float x, float y);
private:
    float Lx;
    float Ly;
    float Lz;
    int Ll;
    int Lr;
};

#endif //__LIGHT_H__