#include "Light.h"
using namespace std;

Light::Light(float x, float y, float z, int l, int r)
    : Lx(x), Ly(y), Lz(z), Ll(l), Lr(r) {
}

void Light::enable() {
    glEnable(GL_LIGHTING);
    GLfloat light_pos[] = {Lx, Ly, Lz, 1};
    GLfloat light_col[] = {Ll / 240, Ll / 240, Ll / 240, 1};
    GLfloat light_dir[] = {0, 0, -1};

    switch (Lr) {
        case 0: 
            glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
            glLightfv(GL_LIGHT0, GL_AMBIENT, light_col);
            glEnable(GL_LIGHT0);
            break;
        case 1:
            glLightfv(GL_LIGHT1, GL_POSITION, light_pos);
            glLightfv(GL_LIGHT1, GL_AMBIENT, light_col);
            glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 60);
            glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light_dir);
            glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.);
            glEnable(GL_LIGHT1);
            break;
        case 2:
            glLightfv(GL_LIGHT2, GL_POSITION, light_pos);
            glLightfv(GL_LIGHT2, GL_AMBIENT, light_col);
            glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 60);
            glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, light_dir);
            glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 2.);
            glEnable(GL_LIGHT2);
            break;
        case 3:
            glLightfv(GL_LIGHT3, GL_POSITION, light_pos);
            glLightfv(GL_LIGHT3, GL_AMBIENT, light_col);
            glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 60);
            glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, light_dir);
            glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 2.);
            glEnable(GL_LIGHT3);
            break;
        case 4:
            glLightfv(GL_LIGHT4, GL_POSITION, light_pos);
            glLightfv(GL_LIGHT4, GL_AMBIENT, light_col);
            glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, 60);
            glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, light_dir);
            glLightf(GL_LIGHT4, GL_SPOT_EXPONENT, 2.);
            glEnable(GL_LIGHT4);
            break;
    }
}

void Light::luminance_add(float x, float y) {
    if (abs(x - Lx) + abs(y - Ly) < 12) {
        if (Ll < 240) {
            Ll += 5;
        }
    }
}

void Light::luminance_sub(float x, float y) {
    if (abs(x - Lx) + abs(y - Ly) < 12) {
        if (Ll > 60) {
            Ll -= 5;
        }
    }
}