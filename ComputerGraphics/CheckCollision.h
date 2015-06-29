#ifndef __CHECK_COLLISION_H__
#define __CHECK_COLLISION_H__

#include <map>
#include <cstring>
#include <string>
#include <algorithm>

class CheckCollsion {
public:
    class Cube{
    public:
        Cube() {
            std::memset(pointA, 0, 3 * sizeof(double));
            std::memset(pointB, 0, 3 * sizeof(double));
        };
        Cube(double a[3], double b[3]) {
            std::memcpy(pointA, a, 3 * sizeof(double));
            std::memcpy(pointB, b, 3 * sizeof(double));
        }
        void Update(double a[3], double b[3]) {
            std::memcpy(pointA, a, 3 * sizeof(double));
            std::memcpy(pointB, b, 3 * sizeof(double));
        }
        void Update(Cube& cube) {
            std::memcpy(pointA, cube.pointA, 3 * sizeof(double));
            std::memcpy(pointB, cube.pointB, 3 * sizeof(double));
        }

        bool Check(double point[3]);

    private:
        double pointA[3];
        double pointB[3];
    };

public:
    CheckCollsion();
    ~CheckCollsion();
    void Update(std::string& name, Cube cube);
    void Remove(std::string& name);
    bool Check(double point[3], std::string& name);
    bool CheckAll(double point[3]);

private:
    std::map<std::string, Cube> objects;
};

#endif // __CHECK_COLLISION_H__