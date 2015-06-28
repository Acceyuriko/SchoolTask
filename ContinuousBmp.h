#ifndef __CONTINUOUS_BMP__
#define __CONTINUOUS_BMP__

#include "windows.h"
#include "glut.h"
#include <cstdint>
#include <vector>

class ContinuousBmp {
public:
    ContinuousBmp(GLuint size, char* filePath);
    virtual ~ContinuousBmp();
    void InitTexture();
    void Display(GLdouble *bottomLeft, GLdouble *bottomRight, GLdouble *upRight, GLdouble *upLeft);

private:
    unsigned char *LoadBitmapFile(std::string &filename, BITMAPINFOHEADER *bitmapInfoHeader);
    void LoadTexture(int i);

public:
    static const GLuint BITMAP_ID = 0x4d42;

private:
    std::vector<GLuint> texture;
    std::string filePath;
    GLint count;
    int64_t lastTick;
};

#endif //__CONTINUOUS_BMP__