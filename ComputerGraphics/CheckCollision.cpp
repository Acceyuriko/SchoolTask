#include "CheckCollision.h"
using namespace std;

bool CheckCollsion::Cube::Check(double point[3]) {
    if (point[0] >= min(pointA[0], pointB[0]) &&
        point[0] <= max(pointA[0], pointB[0])) {
        if (point[1] >= min(pointA[1], pointB[1]) &&
    	    point[1] <= max(pointA[1], pointB[1])) {
            if (point[2] >= min(pointA[2], pointB[2]) &&
    		    point[2] <= max(pointA[2], pointB[2])) {
    		    return true;
    		}
    	}
    }

    return false;
}

CheckCollsion::CheckCollsion() { }

CheckCollsion::~CheckCollsion() { }

void CheckCollsion::Update(string& name, Cube cube) {
    auto count = objects.count(name);
    if (count == 0) {
        objects.insert(pair<string, Cube>(name, cube));
    }
    else {
        auto temp = objects.find(name);
        temp->second.Update(cube);
    }
}

void CheckCollsion::Remove(string& name) {
    auto t = objects.find(name);
    if (t != objects.end()) {
        objects.erase(t);
    }
}

bool CheckCollsion::Check(double point[3], string& name) {
    auto it = objects.find(name);
    if (it == objects.end()) {
        return false;
    }

    return it->second.Check(point);
}

bool CheckCollsion::CheckAll(double point[3]) {
    bool r = false;
    for (auto it = objects.begin(); it != objects.end(); it++) {
        if (it->second.Check(point)) {
            r = true;
        }
    }

    return r;
}