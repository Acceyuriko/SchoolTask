#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include <cstdio>
#include <windows.h>
#include <glut.h>
#include "DrawFunction.h"
#include "Door.h"
#include "Dyn.h"
#include "Light.h"
#include "ContinuousBmp.h"
#include "ObjLoader.h"

namespace Animation {
    void Reshape(int width, int height);
    void Idle();
    void ProcessKey(unsigned char k, int x, int y);
    void Redraw();
    GLint GenList(void(*func)(void* param), void* param);
    unsigned char* LoadBitmapFile(char* filename, BITMAPINFOHEADER* bitmapInfoHeader);
    void LoadTexture(int i, char *filename);
    void InitTexture();
    void DrawScene();
    void DrawList();
    void Init(int argc, char *argv[]);

    static const GLuint BITMAP_ID = 0x4D42;

    extern GLfloat vision_angle;
    extern GLfloat fRotate;
    extern GLfloat fScale;

    extern GLfloat eye[3];
    extern GLfloat eye_radium;
    extern GLfloat eye_angle_1;
    extern GLfloat eye_angle_2;

    extern GLfloat step_radium;

    extern GLfloat center[3];

    extern bool bOrbit;

    extern float eye_temp[3];
    extern float center_temp[3];
    extern float eye_angle_temp;

    extern Door door1;
    extern Door door2;
    extern Door door3;
    extern Door door4;

    extern Dyn dyn1;

    extern Light light0;
    extern Light light1;
    extern Light light2;
    extern Light light3;
    extern Light light4;

    extern ContinuousBmp gif;

    extern ObjLoader loader;

    extern GLuint texture[4];
    extern std::vector<GLint> list;
};


#endif // __ANIMATION_H__
