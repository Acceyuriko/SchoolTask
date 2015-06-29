#include "ContinuousBmp.h"
#include <string>
using namespace std;

void ContinuousBmp::Display(GLdouble *bottomLeft, GLdouble *bottomRight, GLdouble *upRight, GLdouble *upLeft) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[count]);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0, 0.0);
    glVertex3dv(bottomLeft);
    glTexCoord2f(1.0, 0.0);
    glVertex3dv(bottomRight);
    glTexCoord2f(1.0, 1.0);
    glVertex3dv(upRight);
    glTexCoord2f(0.0, 1.0);
    glVertex3dv(upLeft);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    auto temp = GetTickCount64();
    if (temp - lastTick > 80) {
        count++;
        if (count == texture.size()) {
            count = 0;
        }

        lastTick = temp;
    }
}

unsigned char *ContinuousBmp::LoadBitmapFile(std::string &filename, BITMAPINFOHEADER *bitmapInfoHeader) {
    auto filePtr = fopen(filename.c_str(), "rb");
    if (filePtr == nullptr) {
        return nullptr;
    }

    BITMAPFILEHEADER bitmapFileHeader;

    fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
    if (bitmapFileHeader.bfType != BITMAP_ID) {
        fprintf(stderr, "Error in LoadBitmapFile: the file is not a bitmap file \n");
        return nullptr;
    }

    fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
    fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

    // may memory leak
    auto bitmapImage = new unsigned char[bitmapInfoHeader->biSizeImage];
    if (bitmapImage == nullptr) {
        fprintf(stderr, "error in LoadBitmapFile: memory error\n");
        return nullptr;
    }

    fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);
    if (bitmapImage == nullptr) {
        fprintf(stderr, "error in LoadBitmapFile: memory error\n");
        return nullptr;
    }

    // convert bgr to rgb
    for (unsigned int imageIdx = 0; imageIdx < bitmapInfoHeader->biSizeImage; imageIdx += 3) {
        auto tempRgb = bitmapImage[imageIdx];
        bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
        bitmapImage[imageIdx + 2] = tempRgb;
    }

    fclose(filePtr);
    return bitmapImage;
}

void ContinuousBmp::LoadTexture(int i) {
    string filename = filePath + to_string(i + 1) + ".bmp";
    BITMAPINFOHEADER bitmapInfoHeader;
    auto bitmapData = LoadBitmapFile(filename, &bitmapInfoHeader);
    glBindTexture(GL_TEXTURE_2D, texture[i]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        bitmapInfoHeader.biWidth,
        bitmapInfoHeader.biHeight,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        bitmapData
        );
}

void ContinuousBmp::InitTexture() {
    glGenTextures(texture.size(), &texture[0]);
    for (unsigned int i = 0; i < texture.size(); i++) {
        LoadTexture(i);
    }
}

ContinuousBmp::ContinuousBmp(GLuint size, char* filePath)
    :texture(size), filePath(filePath) {
    count = 0;
    lastTick = 0;
}

ContinuousBmp::~ContinuousBmp() {
}