#include "ObjLoader.h"

using namespace std;

ObjLoader::ObjLoader() { }

ObjLoader::~ObjLoader() { }

void ObjLoader::Load(char* filename) {
    // index begin with 1, so 0 is useless
    vertex.push_back(Point(0.0, 0.0, 0.0));

    auto fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Couldn't open the file!");
        exit(1);
    }

    char line[100];
    char mark[5];
    char* pch;
    double x, y, z;
    vector<int> faceIndex;
    int index;
    while (fgets(line, 100, fp)) {
        switch (line[0]) {
            case 'v':
                sscanf(line, "%s %lf %lf %lf", mark, &x, &y, &z);
                vertex.push_back(Point(x, y, z));
                break;
            case 'f':
                pch = strtok(line, " ");
                pch = strtok(nullptr, " ");
                while (pch != nullptr) {
                    sscanf(pch, "%d", &index);
                    faceIndex.push_back(index);
                    pch = strtok(nullptr, " ");
                }
                faces.push_back(faceIndex);
                faceIndex.clear();
                break;
            default:
                break;
        }
    }
}

void ObjLoader::Draw() {
    glColor3d(1, 1, 1);
    for (vector<int>::size_type i = 0; i < faces.size(); i++) {
        glBegin(GL_POLYGON);
        auto n = faces[i].size();
        for (vector<int>::size_type j = 0; j < n; j++) {
            glVertex3dv(vertex[faces[i][j]].data);
            glVertex3dv(vertex[faces[i][(j + 1) % n]].data);
        }
        glEnd();
    }
}