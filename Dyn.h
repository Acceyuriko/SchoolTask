#ifndef __DYN_H__
#define __DYN_H__

#include <glut.h>

class Dyn {
public:
    Dyn(float ix, float iy, float iz);
    ~Dyn();
    void draw();
    void rotate();
    bool if_rotate();
    void angle_change();
    void hue_inc();
    void hue_dec();
    void size_inc();
    void size_dec();
    void shape_change();

public:
    static const enum SHAPE {
        CUBE = 0,
        BALL = 1,
        CYLINDER = 2,
        CONE = 3,
        PRISM = 4,
        PRISMOID = 5
    };

private:
    float l;
    float x;
    float y;
    float z;
    int hue;
    int shape;
    float angle;
    bool bRotate;
    float r;
    float g;
    float  b;
};

#endif //__DYN_H__