#include <cmath>
#include "Door.h"
#include "DrawFunction.h"

using namespace std;

Door::Door(float x, float y, bool p) 
    : bOpen(false), Dx(x), Dy(y), Dp(p){
}

Door::~Door() { }

void Door::draw(GLuint texture) {
    if (!bOpen) {
        glEnable(GL_TEXTURE_2D);
        // texture[1]
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

        if (Dp) {
            DrawCube(Dx, Dy, 10, 20, 1, 20);
        }
        else {
            DrawCube(Dx, Dy, 10, 1, 20, 20);
        }

        glDisable(GL_TEXTURE_2D);
    }
}

void Door::approach(float x, float y) {
    if (abs(x - Dx) + abs(y - Dy) > 3 &&
        abs(x - Dx) + abs(y - Dy) < 15) {
        bOpen = !bOpen;
    }
}

void Door::approach_notice(float x, float y, GLuint texture) {
    if (!bOpen) {
        if (abs(x - Dx) + abs(y - Dy) > 3 &&
            abs(x - Dx) + abs(y - Dy) < 15) {
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