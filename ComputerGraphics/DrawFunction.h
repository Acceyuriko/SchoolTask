#ifndef __DRAW_FUNCTION_H__
#define __DRAW_FUNCTION_H__

#include <glut.h>
#include <cmath>
#include <vector>

void DrawCube(GLfloat x, GLfloat y, GLfloat z, GLfloat a, GLfloat b, GLfloat c);

/**
* general function to draw various prismoid
* @param topRadius the radius of top face
* @param bottomRadius the radius of bottom face
* @param height the height
* @param faceNum the number of side faces
*/
void DrawPrismoid(GLdouble topRadius, GLdouble bottomRadius, GLdouble height, GLint faceNum);

void DrawPrismoid(GLdouble size);

void DrawPrism(GLdouble size);

void DrawCylinder(GLdouble size);

void DrawWalls(GLuint texture);

void DrawWalls(void* param);

void DrawFloor(GLuint texture);

void DrawFloor(void* param);

void DrawObj(void * param);



#endif //__DRAW_FUNCTION_H__