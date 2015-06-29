#include <cmath>
#include <string>
#include "Door.h"
#include "DrawFunction.h"
#include "CheckCollision.h"
#include "Animation.h"

using namespace std;
using namespace Animation;

Door::Door(float x, float y, bool p) 
    : bOpen(false), Dx(x), Dy(y), Dp(p){
}

Door::~Door() { }

void Door::draw(GLuint texture) {
    char name[20];
    sprintf(name, "door_%d_%d", static_cast<int>(Dx), static_cast<int>(Dy));
    if (!bOpen) {
        glEnable(GL_TEXTURE_2D);
        // texture[1]
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

        double pa[3];
        double pb[3];

        pa[2] = 0;
        pb[2] = 20;
        if (Dp) {
            DrawCube(Dx, Dy, 10, 20, 1, 20);
            pa[0] = Dx - 10;
            pa[1] = Dy - 1.5;
            pb[0] = Dx + 10;
            pb[1] = Dy + 1.5;
            check.Update(string(name), CheckCollsion::Cube(pa, pb));
        }
        else {
            DrawCube(Dx, Dy, 10, 1, 20, 20);
            pa[0] = Dx - 1.5;
            pa[1] = Dy - 10;
            pb[0] = Dx + 1.5;
            pb[1] = Dy + 10;
            check.Update(string(name), CheckCollsion::Cube(pa, pb));
        }

        glDisable(GL_TEXTURE_2D);
    }
    else {
        check.Remove(string(name));
    }
}

void Door::approach(float x, float y) {
    if (fabs(x - Dx) + fabs(y - Dy) > 1 &&
        fabs(x - Dx) + fabs(y - Dy) < 15) {
        bOpen = !bOpen;
    }
}

void Door::approach_notice(float x, float y, GLuint texture) {
    if (!bOpen) {
        if (fabs(x - Dx) + fabs(y - Dy) > 1 &&
            fabs(x - Dx) + fabs(y - Dy) < 15) {
            glEnable(GL_TEXTURE_2D);
            // texture[2]
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

            if (Dp) {
                DrawCube(Dx, Dy, 10, 20, 1.1, 20);
            }
            else {
                DrawCube(Dx, Dy, 10, 1.1, 20, 20);
            }

            glDisable(GL_TEXTURE_2D);
        }
    }
}