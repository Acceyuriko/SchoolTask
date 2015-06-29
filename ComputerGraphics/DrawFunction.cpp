#define _USE_MATH_DEFINES
#include <cmath>
#include "DrawFunction.h"
#include "ObjLoader.h"
#include "Animation.h"
#include "CheckCollision.h"


using namespace std;
using namespace Animation;

void DrawCube(GLfloat x, GLfloat y, GLfloat z, GLfloat a, GLfloat b, GLfloat c) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(a, b, c);
    glScalef(0.5, 0.5, 0.5);

    glTranslatef(0, 0, 1);
    glBegin(GL_QUADS);
    glTexCoord2i(1, 1); glVertex2i(1, 1);
    glTexCoord2i(1, 0); glVertex2i(1, -1);
    glTexCoord2i(0, 0); glVertex2i(-1, -1);
    glTexCoord2i(0, 1); glVertex2i(-1, 1);
    glEnd();
    glTranslatef(0, 0, -2);
    glBegin(GL_QUADS);
    glTexCoord2i(1, 1); glVertex2i(1, 1);
    glTexCoord2i(1, 0); glVertex2i(1, -1);
    glTexCoord2i(0, 0); glVertex2i(-1, -1);
    glTexCoord2i(0, 1); glVertex2i(-1, 1);
    glEnd();
    glTranslatef(0, -1, 1);
    glBegin(GL_QUADS);
    glTexCoord2i(1, 1); glVertex3i(1, 0, 1);
    glTexCoord2i(1, 0); glVertex3i(1, 0, -1);
    glTexCoord2i(0, 0); glVertex3i(-1, 0, -1);
    glTexCoord2i(0, 1); glVertex3i(-1, 0, 1);
    glEnd();
    glTranslatef(0, 2, 0);
    glBegin(GL_QUADS);
    glTexCoord2i(1, 1); glVertex3i(1, 0, 1);
    glTexCoord2i(1, 0); glVertex3i(1, 0, -1);
    glTexCoord2i(0, 0); glVertex3i(-1, 0, -1);
    glTexCoord2i(0, 1); glVertex3i(-1, 0, 1);
    glEnd();
    glTranslatef(-1, -1, 0);
    glBegin(GL_QUADS);
    glTexCoord2i(1, 1); glVertex3i(0, 1, 1);
    glTexCoord2i(1, 0); glVertex3i(0, 1, -1);
    glTexCoord2i(0, 0); glVertex3i(0, -1, -1);
    glTexCoord2i(0, 1); glVertex3i(0, -1, 1);
    glEnd();
    glTranslatef(2, 0, 0);
    glBegin(GL_QUADS);
    glTexCoord2i(1, 1); glVertex3i(0, 1, 1);
    glTexCoord2i(1, 0); glVertex3i(0, 1, -1);
    glTexCoord2i(0, 0); glVertex3i(0, -1, -1);
    glTexCoord2i(0, 1); glVertex3i(0, -1, 1);
    glEnd();

    glPopMatrix();
}

void DrawPrismoid(GLdouble topRadius, GLdouble bottomRadius, GLdouble height, GLint faceNum) {
    // compute coordinate of points
    vector<vector<GLdouble>> topPoints(faceNum);
    for (GLint i = 0; i < faceNum; i++) {
        topPoints[i].push_back(topRadius * cos(static_cast<GLdouble>(i)* (2.0 * M_PI / faceNum)));
        topPoints[i].push_back(height / 2.0);
        topPoints[i].push_back(-topRadius * sin(static_cast<GLdouble>(i)* (2.0 * M_PI / faceNum)));
    }
    vector<vector<GLdouble>> bottomPoints(faceNum);
    for (GLint i = 0; i < faceNum; i++) {
        bottomPoints[i].push_back(bottomRadius * cos(static_cast<GLdouble>(i)* (2.0 * M_PI / faceNum)));
        bottomPoints[i].push_back(-height / 2.0);
        bottomPoints[i].push_back(-bottomRadius * sin(static_cast<GLdouble>(i)* (2.0 * M_PI / faceNum)));
    }

    // draw top face
    glBegin(GL_POLYGON);
    for (GLint i = 0; i < faceNum; i++) {
        glVertex3dv(&topPoints[i][0]);
    }
    glEnd();

    // draw bottom face
    glBegin(GL_POLYGON);
    for (GLint i = 0; i < faceNum; i++) {
        glVertex3dv(&bottomPoints[i][0]);
    }
    glEnd();

    glBegin(GL_POLYGON);
    // draw side face
    for (GLint i = 0; i < faceNum; i++) {
        GLint nextIndex = i == 0 ? faceNum - 1 : i - 1;
        glVertex3dv(&bottomPoints[i][0]);
        glVertex3dv(&topPoints[i][0]);
        glVertex3dv(&topPoints[nextIndex][0]);
        glVertex3dv(&bottomPoints[nextIndex][0]);
    }
    glEnd();
}

void DrawPrismoid(GLdouble size) {
    DrawPrismoid(size, 1.5 * size, 2 * size, 6);
}

void DrawPrism(GLdouble size) {
    DrawPrismoid(size, size, 2 * size, 6);
}

void DrawCylinder(GLdouble size) {
    DrawPrismoid(size, size, 2 * size, 100);
}

void DrawWalls(GLuint texture) {
    glEnable(GL_TEXTURE_2D);
    // texture[0]
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glPushMatrix();
	glScalef(2, 2, 1);

    double pa[3] = {10, 20, 0};
    double pb[3] = {20, 20, 40};
    char name[20];
    int granularity = 100;
    double thickness = 0.2;

    double step = 10.0 / static_cast<double>(granularity);
    for (int i = 0; i < granularity; i++) {
        pa[0] += step;
        pa[1] -= step;
        pb[0] = pa[0] - step - 1;
        pb[1] = pa[1] + step + 1;

        sprintf(name, "slant_wall0_%d", i);
        check.Update(string(name), CheckCollsion::Cube(pa, pb));
    }
    glBegin(GL_QUADS);
    glTexCoord2i(0, 1); glVertex3i(5, 10, 20);
    glTexCoord2i(1, 1); glVertex3i(10, 5, 20);
    glTexCoord2i(1, 0); glVertex3i(10, 5, 0);
    glTexCoord2i(0, 0); glVertex3i(5, 10, 0);
    glEnd();

    pa[0] = 10;
    pa[1] = -20;
    for (int i = 0; i < granularity; i++) {
        pa[0] += step;
        pa[1] += step;
        pb[0] = pa[0] - step - 1;
        pb[1] = pa[1] - step - 1;

        sprintf(name, "slant_wall1_%d", i);
        check.Update(string(name), CheckCollsion::Cube(pa, pb));
    }
    glBegin(GL_QUADS);
    glTexCoord2i(0, 1); glVertex3i(10, -5, 20);
    glTexCoord2i(1, 1); glVertex3i(5, -10, 20);
    glTexCoord2i(1, 0); glVertex3i(5, -10, 0);
    glTexCoord2i(0, 0); glVertex3i(10, -5, 0);
    glEnd();

    pa[0] = -10;
    pa[1] = -20;
    for (int i = 0; i < granularity; i++) {
        pa[0] -= step;
        pa[1] += step;
        pb[0] = pa[0] + step + 1;
        pb[1] = pa[1] - step - 1;

        sprintf(name, "slant_wall2_%d", i);
        check.Update(string(name), CheckCollsion::Cube(pa, pb));
    }
    glBegin(GL_QUADS);
    glTexCoord2i(0, 1); glVertex3i(-5, -10, 20);
    glTexCoord2i(1, 1); glVertex3i(-10, -5, 20);
    glTexCoord2i(1, 0); glVertex3i(-10, -5, 0);
    glTexCoord2i(0, 0); glVertex3i(-5, -10, 0);
    glEnd();

    pa[0] = -20;
    pa[1] = 10;
    for (int i = 0; i < granularity; i++) {
        pa[0] += step;
        pa[1] += step;
        pb[0] = pa[0] - step - 1;
        pb[1] = pa[1] - step - 1;

        sprintf(name, "slant_wall3_%d", i);
        check.Update(string(name), CheckCollsion::Cube(pa, pb));
    }
    glBegin(GL_QUADS);
    glTexCoord2i(0, 1); glVertex3i(-10, 5, 20);
    glTexCoord2i(1, 1); glVertex3i(-5, 10, 20);
    glTexCoord2i(1, 0); glVertex3i(-5, 10, 0);
    glTexCoord2i(0, 0); glVertex3i(-10, 5, 0);
    glEnd();

    // Wall 2 +y
    pa[0] = -10 + thickness;
    pa[1] = 20;
    pb[0] = -10 - thickness;
    pb[1] = 50;
    check.Update(string("wall2_0"), CheckCollsion::Cube(pa, pb));
    glBegin(GL_QUADS);
    glTexCoord2i(0, 1); glVertex3i(-5, 10, 20);
    glTexCoord2i(1, 1); glVertex3i(-5, 25, 20);
    glTexCoord2i(1, 0); glVertex3i(-5, 25, 0);
    glTexCoord2i(0, 0); glVertex3i(-5, 10, 0);
    glEnd();

    pa[0] = -10;
    pa[1] = 50 + thickness;
    pb[0] = 10;
    pb[1] = 50 - thickness;
    check.Update(string("wall2_1"), CheckCollsion::Cube(pa, pb));
    glBegin(GL_QUADS);
    glTexCoord2i(0, 1); glVertex3i(-5, 25, 20);
    glTexCoord2i(1, 1); glVertex3i(5, 25, 20);
    glTexCoord2i(1, 0); glVertex3i(5, 25, 0);
    glTexCoord2i(0, 0); glVertex3i(-5, 25, 0);
    glEnd();

    pa[0] = 10 + thickness;
    pa[1] = 20;
    pb[0] = 10 - thickness;
    pb[1] = 50;
    check.Update(string("wall2_2"), CheckCollsion::Cube(pa, pb));
    glBegin(GL_QUADS);
    glTexCoord2i(0, 1); glVertex3i(5, 25, 20);
    glTexCoord2i(1, 1); glVertex3i(5, 10, 20);
    glTexCoord2i(1, 0); glVertex3i(5, 10, 0);
    glTexCoord2i(0, 0); glVertex3i(5, 25, 0);
    glEnd();

    // Wall 3 +x
    pa[0] = 20;
    pa[1] = 10 + thickness;
    pb[0] = 50;
    pb[1] = 10 - thickness;
    check.Update(string("wall3_0"), CheckCollsion::Cube(pa, pb));
    glBegin(GL_QUADS);
    glTexCoord2i(0, 1); glVertex3i(10, 5, 20);
    glTexCoord2i(1, 1); glVertex3i(25, 5, 20);
    glTexCoord2i(1, 0); glVertex3i(25, 5, 0);
    glTexCoord2i(0, 0); glVertex3i(10, 5, 0);
    glEnd();

    pa[0] = 50 + thickness;
    pa[1] = -10;
    pb[0] = 50 - thickness;
    pb[1] = 10;
    check.Update(string("wall3_1"), CheckCollsion::Cube(pa, pb));
    glBegin(GL_QUADS);
    glTexCoord2i(0, 1); glVertex3i(25, 5, 20);
    glTexCoord2i(1, 1); glVertex3i(25, -5, 20);
    glTexCoord2i(1, 0); glVertex3i(25, -5, 0);
    glTexCoord2i(0, 0); glVertex3i(25, 5, 0);
    glEnd();

    pa[0] = 20;
    pa[1] = -10 - thickness;
    pb[0] = 50;
    pb[1] = -10 + thickness;
    check.Update(string("wall3_2"), CheckCollsion::Cube(pa, pb));
    glBegin(GL_QUADS);
    glTexCoord2i(0, 1); glVertex3i(25, -5, 20);
    glTexCoord2i(1, 1); glVertex3i(10, -5, 20);
    glTexCoord2i(1, 0); glVertex3i(10, -5, 0);
    glTexCoord2i(0, 0); glVertex3i(25, -5, 0);
    glEnd();

    // Wall 4 -y
    pa[0] = 10 + thickness;
    pa[1] = -20;
    pb[0] = 10 - thickness;
    pb[1] = -50;
    check.Update(string("wall4_0"), CheckCollsion::Cube(pa, pb));
    glBegin(GL_QUADS);
    glTexCoord2i(0, 1); glVertex3i(5, -10, 20);
    glTexCoord2i(1, 1); glVertex3i(5, -25, 20);
    glTexCoord2i(1, 0); glVertex3i(5, -25, 0);
    glTexCoord2i(0, 0); glVertex3i(5, -10, 0);
    glEnd();

    pa[0] = -10;
    pa[1] = -50 + thickness;
    pb[0] = 10;
    pb[1] = -50 - thickness;
    check.Update(string("wall4_1"), CheckCollsion::Cube(pa, pb));
    glBegin(GL_QUADS);
    glTexCoord2i(0, 1); glVertex3i(5, -25, 20);
    glTexCoord2i(1, 1); glVertex3i(-5, -25, 20);
    glTexCoord2i(1, 0); glVertex3i(-5, -25, 0);
    glTexCoord2i(0, 0); glVertex3i(5, -25, 0);
    glEnd();

    pa[0] = -10 + thickness;
    pa[1] = -20;
    pb[0] = -10 - thickness;
    pb[1] = -50;
    check.Update(string("wall4_2"), CheckCollsion::Cube(pa, pb));
    glBegin(GL_QUADS);
    glTexCoord2i(0, 1); glVertex3i(-5, -25, 20);
    glTexCoord2i(1, 1); glVertex3i(-5, -10, 20);
    glTexCoord2i(1, 0); glVertex3i(-5, -10, 0);
    glTexCoord2i(0, 0); glVertex3i(-5, -25, 0);
    glEnd();

    // Wall 5 -x
    pa[0] = -20;
    pa[1] = -10 + thickness;
    pb[0] = -50;
    pb[1] = -10 - thickness;
    check.Update(string("wall5_0"), CheckCollsion::Cube(pa, pb));
    glBegin(GL_QUADS);
    glTexCoord2i(0, 1); glVertex3i(-10, -5, 20);
    glTexCoord2i(1, 1); glVertex3i(-25, -5, 20);
    glTexCoord2i(1, 0); glVertex3i(-25, -5, 0);
    glTexCoord2i(0, 0); glVertex3i(-10, -5, 0);
    glEnd();

    pa[0] = -50 + thickness;
    pa[1] = -10;
    pb[0] = -50 - thickness;
    pb[1] = 10;
    check.Update(string("wall5_1"), CheckCollsion::Cube(pa, pb));
    glBegin(GL_QUADS);
    glTexCoord2i(0, 1); glVertex3i(-25, -5, 20);
    glTexCoord2i(1, 1); glVertex3i(-25, 5, 20);
    glTexCoord2i(1, 0); glVertex3i(-25, 5, 0);
    glTexCoord2i(0, 0); glVertex3i(-25, -5, 0);
    glEnd();

    pa[0] = -20;
    pa[1] = 10 + thickness;
    pb[0] = -50;
    pb[1] = 10 - thickness;
    check.Update(string("wall5_2"), CheckCollsion::Cube(pa, pb));
    glBegin(GL_QUADS);
    glTexCoord2i(0, 1); glVertex3i(-25, 5, 20);
    glTexCoord2i(1, 1); glVertex3i(-10, 5, 20);
    glTexCoord2i(1, 0); glVertex3i(-10, 5, 0);
    glTexCoord2i(0, 0); glVertex3i(-25, 5, 0);
    glEnd();

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void DrawWalls(void* param) {
    DrawWalls(*static_cast<GLuint*>(param));
}

void DrawFloor(GLuint texture) {
    glEnable(GL_TEXTURE_2D);
    //texture [1]
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glPushMatrix();
	glScalef(2, 2, 1);

    double pa[3] = {-50, -50, -0.1};
    double pb[3] = {50, 50, 0.1};

    check.Update(string("floor"), CheckCollsion::Cube(pa, pb));

	glBegin(GL_QUADS);              
        glTexCoord2i(0,0); glVertex3i(-25,25,0);
        glTexCoord2i(1,0); glVertex3i(25,25,0);
        glTexCoord2i(1,1); glVertex3i(25,-25,0);
        glTexCoord2i(0,1); glVertex3i(-25,-25,0);
	glEnd();

    pa[2] = 19.9;
    pb[2] = 20.1;

    check.Update(string("ceiling"), CheckCollsion::Cube(pa, pb));

	glBegin(GL_QUADS);              
        glTexCoord2i(0,0); glVertex3i(-25,25,20);
        glTexCoord2i(1,0); glVertex3i(25,25,20);
        glTexCoord2i(1,1); glVertex3i(25,-25,20);
        glTexCoord2i(0,1); glVertex3i(-25,-25,20);
	glEnd();

	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}

void DrawFloor(void* param) {
    DrawFloor(*static_cast<GLuint*>(param));
}

void DrawObj(void *param) {
    glPushMatrix();
    glTranslated(-20.0, 0.0, 0.0);
    glRotated(90, 1.0, 0, 0);
    static_cast<ObjLoader*>(param)->Draw();
    glPopMatrix();
}
