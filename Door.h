#ifndef __DOOR_H__
#define __DOOR_H__

#include <glut.h>

class Door {
public:
    Door(float x, float y, bool p);
    ~Door();
    void draw(GLuint texture);
    void approach(float x, float y);
    void approach_notice(float x, float y, GLuint texture);

private:
    bool bOpen;
    float Dx;
    float Dy;
    bool Dp;
};

#endif // __DOOR_H__