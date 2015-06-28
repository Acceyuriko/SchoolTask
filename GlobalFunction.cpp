#include "GlobalFunction.h"

using namespace std;

void Draw_Cube(GLfloat x, GLfloat y, GLfloat z, GLfloat a, GLfloat b, GLfloat c) {
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
    const GLdouble PI = 3.1415926535;

    // compute coordinate of points
    vector<vector<GLdouble>> topPoints(faceNum);
    for (GLint i = 0; i < faceNum; i++) {
        topPoints[i].push_back(topRadius * cos(static_cast<GLdouble>(i)* (2.0 * PI / faceNum)));
        topPoints[i].push_back(height / 2.0);
        topPoints[i].push_back(-topRadius * sin(static_cast<GLdouble>(i)* (2.0 * PI / faceNum)));
    }
    vector<vector<GLdouble>> bottomPoints(faceNum);
    for (GLint i = 0; i < faceNum; i++) {
        bottomPoints[i].push_back(bottomRadius * cos(static_cast<GLdouble>(i)* (2.0 * PI / faceNum)));
        bottomPoints[i].push_back(-height / 2.0);
        bottomPoints[i].push_back(-bottomRadius * sin(static_cast<GLdouble>(i)* (2.0 * PI / faceNum)));
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