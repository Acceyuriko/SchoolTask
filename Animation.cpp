#include "Animation.h"
using namespace std;

namespace Animation{
    void Reshape(int width, int height) {
        if (height == 0) {
            height = 1;
        }

        // update view
        glViewport(0, 0, width, height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        auto whRatio = static_cast<GLfloat>(width) / static_cast<GLfloat>(height);
        gluPerspective(vision_angle, whRatio, 0.1f, 100.0f);

        glMatrixMode(GL_MODELVIEW);
    }

    void Idle() {
        glutPostRedisplay();
    }

    void ProcessKey(unsigned char k, int x, int y) {

        switch (k) {
            // Move forward
            case 'w': {
                eye[0] += step_radium *(float(sin(double(M_PI*eye_angle_1 / 180))));
                center[0] += step_radium *(float(sin(double(M_PI*eye_angle_1 / 180))));
                eye[1] += step_radium *(float(cos(double(M_PI*eye_angle_1 / 180))));
                center[1] += step_radium *(float(cos(double(M_PI*eye_angle_1 / 180))));
                break;
            }
            // Move backward
            case 's': {
                eye[0] -= step_radium *(float(sin(double(M_PI*eye_angle_1 / 180))));
                center[0] -= step_radium *(float(sin(double(M_PI*eye_angle_1 / 180))));
                eye[1] -= step_radium *(float(cos(double(M_PI*eye_angle_1 / 180))));
                center[1] -= step_radium *(float(cos(double(M_PI*eye_angle_1 / 180))));
                break;
            }
            // Turn left
            case 'a': {
                eye_angle_1 -= 1;
                break;
            }
            // Turn right
            case 'd': {
                eye_angle_1 += 1;
                break;
            }
            // Look upward
            case 'q': {
                eye_angle_2 += 1;
                break;
            }
            // Look downward
            case 'e': {
                eye_angle_2 -= 1;
                break;
            }
            case 'z': {
                if (vision_angle <= 75)
                    vision_angle += 0.5;
                break;
            }
            case 'x': {
                if (vision_angle >= 25)
                    vision_angle -= 0.5;
                break;
            }
            case 'j': {
                door1.approach(eye[0], eye[1]);
                door2.approach(eye[0], eye[1]);
                door3.approach(eye[0], eye[1]);
                door4.approach(eye[0], eye[1]);
                break;
            }
            case 'k': {
                if ((abs(center[0]) < 20) && (abs(center[1]) < 20) && (abs(center[2]) < 20)) dyn1.hue_dec();
                break;
            }
            case 'l': {
                if ((abs(center[0]) < 20) && (abs(center[1]) < 20) && (abs(center[2]) < 20)) dyn1.hue_inc();
                break;
            }
            case 'r': {
                if ((abs(center[0]) < 20) && (abs(center[1]) < 20) && (abs(center[2]) < 20)) dyn1.rotate();
                break;
            }
            case 'u': {
                if ((abs(center[0]) < 20) && (abs(center[1]) < 20) && (abs(center[2]) < 20)) dyn1.shape_change();
                break;
            }
            case 'i': {
                if ((abs(center[0]) < 20) && (abs(center[1]) < 20) && (abs(center[2]) < 20)) dyn1.size_dec();
                break;
            }
            case 'o': {
                if ((abs(center[0]) < 20) && (abs(center[1]) < 20) && (abs(center[2]) < 20)) dyn1.size_inc();
                break;
            }
            case 'n': {
                light1.luminance_sub(eye[0], eye[1]);
                light2.luminance_sub(eye[0], eye[1]);
                light3.luminance_sub(eye[0], eye[1]);
                light4.luminance_sub(eye[0], eye[1]);
                break;
            }
            case 'm': {
                light1.luminance_add(eye[0], eye[1]);
                light2.luminance_add(eye[0], eye[1]);
                light3.luminance_add(eye[0], eye[1]);
                light4.luminance_add(eye[0], eye[1]);
                break;
            }
            case 'g': {
                if (!bOrbit) {
                    bOrbit = true;
                    eye_temp[0] = eye[0];
                    eye_temp[1] = eye[1];
                    eye_temp[2] = eye[2];
                    center_temp[0] = center[0];
                    center_temp[1] = center[1];
                    center_temp[2] = center[2];
                    eye_angle_temp = eye_angle_1;
                    eye[0] = 0;
                    eye[1] = -14;
                    eye[2] = 10;
                    eye_angle_1 = 0;
                    center[0] = eye[0];
                    center[1] = eye[1] + eye_radium;
                    center[2] = eye[2];
                    break;
                }
                if (bOrbit) {
                    bOrbit = false;
                    eye[0] = eye_temp[0];
                    eye[1] = eye_temp[1];
                    eye[2] = eye_temp[2];
                    center[0] = center_temp[0];
                    center[1] = center_temp[1];
                    center[2] = center_temp[2];
                    eye_angle_1 = eye_angle_temp;
                    break;
                }
                break;
            }
        }
    }

    void Redraw() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity(); // Reset The Current Modelview Matrix
        if (!bOrbit) {
            center[0] = eye[0] + eye_radium *(float(sin(double(M_PI*eye_angle_1 / 180))));
            center[1] = eye[1] + eye_radium *(float(cos(double(M_PI*eye_angle_1 / 180))));
            center[2] = eye[2] + eye_radium *(float(sin(double(M_PI*eye_angle_2 / 180))));
            gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], 0, 0, 1);
        }
        else if (bOrbit) {
            if (eye_angle_1 == 359.98)
                eye_angle_1 = 0;
            else eye_angle_1 += 0.02;
            eye[0] = (-1) * 14 * (float(sin(double(M_PI*eye_angle_1 / 180))));
            eye[1] = (-1) * 14 * (float(cos(double(M_PI*eye_angle_1 / 180))));
            center[0] = (-1) * 6 * (float(sin(double(M_PI*eye_angle_1 / 180))));
            center[1] = (-1) * 6 * (float(cos(double(M_PI*eye_angle_1 / 180))));
            gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], 0, 0, 1);
        }

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glEnable(GL_DEPTH_TEST);

        DrawList(); // Draw Static Objects
        DrawScene(); // Draw Dynamic Objects


        static GLdouble points[][3] = {
                {-10, 29, 0},
                {10, 29, 0},
                {10, 29, 20},
                {-10, 29, 20}
        };

        gif.Display(points[0], points[1], points[2], points[3]);

        if (dyn1.if_rotate()) {
            dyn1.angle_change();
        }

        dyn1.draw();

        light0.enable();
        light1.enable();
        light2.enable();
        light3.enable();
        light4.enable();

        glutSwapBuffers();
    }

    GLint GenList(void(*func)(void* param), void* param) {
        GLint lid = glGenLists(1);
        glNewList(lid, GL_COMPILE);

        func(param);

        glEndList();

        list.push_back(lid);

        return lid;
    }

    unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader) {
        FILE *filePtr;
        BITMAPFILEHEADER bitmapFileHeader;
        unsigned char	*bitmapImage;
        GLuint	imageIdx = 0;
        unsigned char	tempRGB;

        filePtr = fopen(filename, "rb");
        if (filePtr == NULL) return NULL;
        fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
        if (bitmapFileHeader.bfType != BITMAP_ID) {
            fprintf(stderr, "Error in LoadBitmapFile: the file is not a bitmap file\n");
            return NULL;
        }

        fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
        fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
        bitmapImage = new unsigned char[bitmapInfoHeader->biSizeImage];
        if (!bitmapImage) {
            fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
            return NULL;
        }

        fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);
        if (bitmapImage == NULL) {
            fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
            return NULL;
        }

        for (imageIdx = 0; imageIdx < bitmapInfoHeader->biSizeImage; imageIdx += 3) {
            tempRGB = bitmapImage[imageIdx];
            bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
            bitmapImage[imageIdx + 2] = tempRGB;
        }
        fclose(filePtr);
        return bitmapImage;
    }

    void LoadTexture(int i, char *filename) {
        BITMAPINFOHEADER bitmapInfoHeader;
        unsigned char*   bitmapData;

        bitmapData = LoadBitmapFile(filename, &bitmapInfoHeader);
        glBindTexture(GL_TEXTURE_2D, texture[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

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

    void InitTexture() {
        glGenTextures(4, texture);
        LoadTexture(0, "img/Wall.bmp");
        LoadTexture(1, "img/Door.bmp");
        LoadTexture(2, "img/Notice.bmp");
    }


    void DrawScene() {
        //The Desk
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture[1]);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

        DrawCube(0, 0, 5.5, 5, 4, 1);
        DrawCube(1.5, 1, 2.5, 1, 1, 5);
        DrawCube(-1.5, 1, 2.5, 1, 1, 5);
        DrawCube(1.5, -1, 2.5, 1, 1, 5);
        DrawCube(-1.5, -1, 2.5, 1, 1, 5);

        glDisable(GL_TEXTURE_2D);

        //The Door
        door1.draw(texture[1]);
        door1.approach_notice(eye[0], eye[1], texture[2]);
        door2.draw(texture[1]);
        door2.approach_notice(eye[0], eye[1], texture[2]);
        door3.draw(texture[1]);
        door3.approach_notice(eye[0], eye[1], texture[2]);
        door4.draw(texture[1]);
        door4.approach_notice(eye[0], eye[1], texture[2]);

    }

    void DrawList() {
        for (vector<GLint>::size_type i = 0; i < list.size(); i++) {
            glCallList(list[i]);
        }
    }

    void Init(int argc, char *argv[]) {
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
        glutInitWindowSize(576, 576);

        int windowHandle = glutCreateWindow("GLUT Animation");

        InitTexture();

        loader.Load("obj/horse.obj");
        GenList(DrawWalls, &texture[0]);
        GenList(DrawFloor, &texture[1]);
        GenList(DrawObj, &loader);

        gif.InitTexture();
        glutDisplayFunc(Redraw);
        glutReshapeFunc(Reshape);
        glutKeyboardFunc(ProcessKey);
        glutIdleFunc(Idle);

        glutMainLoop();
    }

    GLfloat vision_angle = 45;
    GLfloat fRotate = 0.0f;
    GLfloat fScale = 1.0f;

    GLfloat eye[3] = {0, -14, 10};
    GLfloat eye_radium = 8;
    GLfloat eye_angle_1 = 0;
    GLfloat eye_angle_2 = 0;

    GLfloat step_radium = 0.20;

    GLfloat center[3] = {0, -6, 0};

    float eye_temp[3];
    float center_temp[3];
    float eye_angle_temp = 0;

    bool bOrbit = false;

    Door door1(0, 29, true);
    Door door2(29, 0, false);
    Door door3(0, -29, true);
    Door door4(-29, 0, false);

    Dyn dyn1(0, 0, 10);

    Light light0(0, 0, 19, 240, 0);
    Light light1(18, 0, 19, 240, 1);
    Light light2(0, -18, 19, 240, 2);
    Light light3(-18, 0, 19, 240, 3);
    Light light4(0, 18, 19, 240, 4);

    ContinuousBmp gif(138, "src_gif/");

    ObjLoader loader;

    GLuint texture[4];
    std::vector<GLint> list;
}