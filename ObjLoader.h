#ifndef __OBJLOADER_H__
#define __OBJLOADER_H__

#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <glut.h>


class ObjLoader {
public:
    ObjLoader();
    ~ObjLoader();

    void Load(char* filename);
    void Draw();

    class Point {
    public:
	    double data[3];
        Point(double x, double y, double z) {
            data[0] = x;
            data[1] = y;
            data[2] = z;
        }
	};

private:
    std::vector<Point> vertex;
    std::vector<std::vector<int>> faces;
};


#endif // __OBJLOADER_H__