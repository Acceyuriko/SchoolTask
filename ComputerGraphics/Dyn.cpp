#include <string>
#include "Dyn.h"
#include "DrawFunction.h"
#include "Animation.h"
#include "CheckCollision.h"

using namespace std;
using namespace Animation;

Dyn::Dyn(float ix, float iy, float iz) 
    : l(1), x(ix), y(iy), z(iz), hue(0), shape(CUBE), bRotate(true), angle(0){
}

Dyn::~Dyn() { }

void Dyn::draw() {
    float dif[4];
    float spe[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    if (hue < 80) {
        dif[0] = 0.8;
        if (hue > 39) {
            dif[0] = 0.8 * (80 - hue) / 40;
        }
        dif[1] = 0.8;
        if (hue < 40) {
            dif[1] = 0.8 * hue / 40;
        }
        dif[2] = 0;
        dif[3] = 0.7;
    }
    else if (hue < 160) {
        dif[1] = 0.8;
        if (hue > 119) {
            dif[1] = 0.8 * (160 - hue) / 40;
        }
        dif[2] = 0.8;
        if (hue < 120) {
            dif[2] = 0.8 * (hue - 80) / 40;
        }
        dif[0] = 0;
        dif[3] = 0.7;
    }
    else {
        dif[2] = 0.8;
        if (hue > 199) {
            dif[2] = 0.8 * (240 - hue) / 40;
        }
        dif[0] = 0.8;
        if (hue < 200) {
            dif[0] = 0.8 * (hue - 160) / 40;
        }
        dif[1] = 0;
        dif[3] = 0.7;
    }

    double pa[3] = {-1, -1, 8};
    double pb[3] = {1, 1, 12};

    check.Update(string("shape"), CheckCollsion::Cube(pa, pb));
    glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
    glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 16);
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(angle, 0.5, 1, 0);
    switch (shape) {
        case CUBE:
            glutSolidCube(l);
            break;
        case BALL:
            glutSolidSphere(l, 24, 24);
            break;
        case CONE:
            glutSolidCone(l, l * 1.5, 24, 24);
            break;
        case CYLINDER:
            DrawCylinder(1);
            break;
        case PRISM:
            DrawPrism(1);
            break;
        case PRISMOID:
            DrawPrismoid(1);
            break;
        default:
            glutSolidCube(l);
            break;
    }
    glPopMatrix();
    r = dif[0];
    g = dif[1];
    b = dif[2];
}

void Dyn::rotate() {
    bRotate = !bRotate;
}

bool Dyn::if_rotate() {
    return bRotate;
}

void Dyn::angle_change() {
    if (angle == 359.1) {
        angle = 0;
    }
    else {
        angle += 0.1;
    }
}

void Dyn::hue_inc() {
    if (hue == 238) {
        hue = 0;
    }
    else {
        hue += 2;
    }
}

void Dyn::hue_dec() {
    if (hue == 0) {
        hue = 238;
    }
    else {
        hue -= 2;
    }
}

void Dyn::size_inc() {
    if (l <= 1.5) {
        l += 0.1;
    }
}

void Dyn::size_dec() {
    if (l >= 0.5) {
        l -= 0.1;
    }
}

void Dyn::shape_change() {
    if (shape == 5) {
        shape = 0;
    }
    else {
        shape += 1;
    }
}