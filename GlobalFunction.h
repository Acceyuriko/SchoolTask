#ifndef __GLOBAL_FUNCTION_H__
#define __GLOBAL_FUNCTION_H__

#include <glut.h>
#include <cmath>
#include <vector>

void Draw_Cube(GLfloat x, GLfloat y, GLfloat z, GLfloat a, GLfloat b, GLfloat c);

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



#endif //__GLOBAL_FUNCTION_H__