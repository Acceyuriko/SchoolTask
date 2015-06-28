#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "glut.h"

#define BITMAP_ID 0x4D42

#define CUBE 0
#define BALL 1
#define CYLINDER 2
#define CONE 3
#define PRISM 4
#define PRISMOID 5

const float pi = 3.14159;

float fTranslate;
float fRotate = 0.0f;
float fScale = 1.0f;

float eye[] = {0, -14, 10};
float eye_radium = 8;
float eye_angle_1 = 0;
float eye_angle_2 = 0;

float step_radium = 0.12;

float center[] = {0, -6, 0};
float vision_angle = 45;

float eye_temp[3];
float center_temp[3];
float eye_angle_temp;

bool bOrbit = false;

int wHeight = 0;
int wWidth = 0;

GLuint texture[4];

GLint tableList = 0;

void Draw_Cube(GLfloat x, GLfloat y, GLfloat z, GLfloat a, GLfloat b, GLfloat c)
{
	glPushMatrix();
	glTranslatef(x, y, z);
	glScalef(a, b, c);
	glScalef(0.5, 0.5, 0.5);

	glTranslatef(0, 0, 1);
	glBegin(GL_QUADS);              
        glTexCoord2i(1,1); glVertex2i(1,1);
        glTexCoord2i(1,0); glVertex2i(1,-1);
        glTexCoord2i(0,0); glVertex2i(-1,-1);
        glTexCoord2i(0,1); glVertex2i(-1,1);
	glEnd();
	glTranslatef(0, 0, -2);
	glBegin(GL_QUADS);              
        glTexCoord2i(1,1); glVertex2i(1,1);
        glTexCoord2i(1,0); glVertex2i(1,-1);
        glTexCoord2i(0,0); glVertex2i(-1,-1);
        glTexCoord2i(0,1); glVertex2i(-1,1);
	glEnd();
	glTranslatef(0, -1, 1);
	glBegin(GL_QUADS);              
        glTexCoord2i(1,1); glVertex3i(1,0,1);
        glTexCoord2i(1,0); glVertex3i(1,0,-1);
        glTexCoord2i(0,0); glVertex3i(-1,0,-1);
        glTexCoord2i(0,1); glVertex3i(-1,0,1);
	glEnd();
	glTranslatef(0, 2, 0);
	glBegin(GL_QUADS);              
        glTexCoord2i(1,1); glVertex3i(1,0,1);
        glTexCoord2i(1,0); glVertex3i(1,0,-1);
        glTexCoord2i(0,0); glVertex3i(-1,0,-1);
        glTexCoord2i(0,1); glVertex3i(-1,0,1);
	glEnd();
	glTranslatef(-1, -1, 0);
	glBegin(GL_QUADS);              
        glTexCoord2i(1,1); glVertex3i(0,1,1);
        glTexCoord2i(1,0); glVertex3i(0,1,-1);
        glTexCoord2i(0,0); glVertex3i(0,-1,-1);
        glTexCoord2i(0,1); glVertex3i(0,-1,1);
	glEnd();
	glTranslatef(2, 0, 0);
	glBegin(GL_QUADS);              
        glTexCoord2i(1,1); glVertex3i(0,1,1);
        glTexCoord2i(1,0); glVertex3i(0,1,-1);
        glTexCoord2i(0,0); glVertex3i(0,-1,-1);
        glTexCoord2i(0,1); glVertex3i(0,-1,1);
	glEnd();

	glPopMatrix();
}

class Door
{
	bool bOpen;
	float Dx;
	float Dy;
	bool Dp;
public:
	Door(float x, float y, bool p)
	{
		bOpen = false;
		Dx = x;
		Dy = y;
		Dp = p;
	}
	void draw()
	{
		if (!bOpen)
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texture[1]); 
			glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

			if (Dp)
				Draw_Cube(Dx, Dy, 10, 20, 1, 20);
			else
				Draw_Cube(Dx, Dy, 10, 1, 20, 20);

			glDisable(GL_TEXTURE_2D);
		}
	}
	void approach(float x, float y)
	{
		if ( (abs(x-Dx)+abs(y-Dy))>3 && (abs(x-Dx)+abs(y-Dy))<15 )
		{
			bOpen = !bOpen;
		}
	}
	void approach_notice(float x, float y)
	{
		if (!bOpen)
		{
			if ( (abs(x-Dx)+abs(y-Dy))>3 && (abs(x-Dx)+abs(y-Dy))<15 )
			{
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, texture[2]); 
				glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

				if (Dp)
					Draw_Cube(Dx, Dy, 10, 20, 1.1, 20);
				else
					Draw_Cube(Dx, Dy, 10, 1.1, 20, 20);

				glDisable(GL_TEXTURE_2D);
			}
		}
	}
};

Door Door1(0, 29, true);
Door Door2(29, 0, false);
Door Door3(0, -29, true);
Door Door4(-29, 0, false);

class Dyn
{
	float l; // radium, length
	float x; // x
	float y; // y
	float z; // z
	int hue;
	int shape;
	float angle;
	bool bRotate;
	float r;
	float g;
	float b;
public:
	Dyn(float ix, float iy, float iz)
	{
		l = 1;
		x = ix;
		y = iy;
		z = iz;
		hue = 0;
		shape = CUBE;
		bRotate = true;
		angle = 0;
	}
	void draw()
	{
		float dif[4];
		float spe[4] = {1.0f, 1.0f, 1.0f, 1.0f};
		if (hue < 80)
		{
			dif[0] = 0.8;
			if (hue > 39) dif[0] = 0.8 * (80 - hue) / 40;
			dif[1] = 0.8;
			if (hue < 40) dif[1] = 0.8 * (hue) / 40;
			dif[2] = 0;
			dif[3] = 0.7;
		}
		else if (hue < 160)
		{
			dif[1] = 0.8;
			if (hue > 119) dif[1] = 0.8 * (160 - hue) / 40;
			dif[2] = 0.8;
			if (hue < 120) dif[2] = 0.8 * (hue - 80) / 40;
			dif[0] = 0;
			dif[3] = 0.7;
		}
		else
		{
			dif[2] = 0.8;
			if (hue > 199) dif[2] = 0.8 * (240 - hue) / 40;
			dif[0] = 0.8;
			if (hue < 200) dif[0] = 0.8 * (hue - 160) / 40;
			dif[1] = 0;
			dif[3] = 0.7;
		}
		glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
		//glMaterialfv(GL_FRONT, GL_SPECULAR, spe);
		glMateriali(GL_FRONT_AND_BACK,GL_SHININESS,16);
		glPushMatrix();
		glTranslatef(x, y, z);
		glRotatef(angle, 0.5, 1, 0);
		switch(shape)
		{
		case CUBE:
			{
				glutSolidCube(l);
				break;
			}
		case BALL:
			{
				glutSolidSphere(l, 24, 24);
				break;
			}
		case CONE:
			{
				glutSolidCone(l, l*1.5, 24, 24);
				break;
			}
		default:
			{
				glutSolidCube(l);
				break;
			}
		}
		glPopMatrix();
		r = dif[0];
		g = dif[1];
		b = dif[2];
	}
	void rotate()
	{
		bRotate = !bRotate;
	}
	bool if_rotate()
	{
		return bRotate;
	}
	void angle_change()
	{
		if (angle == 359.1) angle = 0;
		else angle += 0.1;
	}
	void hue_add()
	{
		if (hue == 238) hue = 0;
		else hue += 2;
	}
	void hue_sub()
	{
		if (hue == 0) hue = 238;
		else hue -= 2;
	}
	void size_add()
	{
		if (l <= 1.5) l += 0.1;
	}
	void size_sub()
	{
		if (l >= 0.5) l -= 0.1;
	}
	void shape_change()
	{
		if (shape == 5) shape = 0;
		else shape += 1;
	}
};

Dyn Dyn1(0, 0, 10);

class Light
{
	float Lx;
	float Ly;
	float Lz;
	int Ll;
	int Lr;
public:
	Light(float x, float y, float z, int l, int r)
	{
		Lx = x;
		Ly = y;
		Lz = z;
		Ll = l;
		Lr = r;
	}
	void enable()
	{
		glEnable(GL_LIGHTING);
		GLfloat light_pos[] = {Lx,Ly,Lz,1};
		GLfloat light_col[] = {Ll/240,Ll/240,Ll/240,1};
		switch(Lr)
		{
		case 0:{
			glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
			glLightfv(GL_LIGHT0, GL_AMBIENT, light_col);
			glEnable(GL_LIGHT0);
			break;
			   }
		case 1:{
			glLightfv(GL_LIGHT1, GL_POSITION, light_pos);
			glLightfv(GL_LIGHT1, GL_AMBIENT, light_col);
			GLfloat light_dir[] = {0,0,-1};
			glLightf(GL_LIGHT1,  GL_SPOT_CUTOFF, 60);
			glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light_dir);
			glLightf(GL_LIGHT1,GL_SPOT_EXPONENT,2.);
			//glLightf(GL_LIGHT1,GL_LINEAR_ATTENUATION,1.0);
			glEnable(GL_LIGHT1);
			break;
			   }
		case 2:{
			glLightfv(GL_LIGHT2, GL_POSITION, light_pos);
			glLightfv(GL_LIGHT2, GL_AMBIENT, light_col);
			GLfloat light_dir[] = {0,0,-1};
			glLightf(GL_LIGHT2,  GL_SPOT_CUTOFF, 60);
			glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, light_dir);
			glLightf(GL_LIGHT2,GL_SPOT_EXPONENT,2.);
			//glLightf(GL_LIGHT2,GL_LINEAR_ATTENUATION,1.0);
			glEnable(GL_LIGHT2);
			break;
			   }
		case 3:{
			glLightfv(GL_LIGHT3, GL_POSITION, light_pos);
			glLightfv(GL_LIGHT3, GL_AMBIENT, light_col);
			GLfloat light_dir[] = {0,0,-1};
			glLightf(GL_LIGHT3,  GL_SPOT_CUTOFF, 60);
			glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, light_dir);
			glLightf(GL_LIGHT3,GL_SPOT_EXPONENT,2.);
			//glLightf(GL_LIGHT3,GL_LINEAR_ATTENUATION,1.0);
			glEnable(GL_LIGHT3);
			break;
			   }
		case 4:{
			glLightfv(GL_LIGHT4, GL_POSITION, light_pos);
			glLightfv(GL_LIGHT4, GL_AMBIENT, light_col);
			GLfloat light_dir[] = {0,0,-1};
			glLightf(GL_LIGHT4,  GL_SPOT_CUTOFF, 60);
			glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, light_dir);
			glLightf(GL_LIGHT4,GL_SPOT_EXPONENT,2.);
			//glLightf(GL_LIGHT4,GL_LINEAR_ATTENUATION,1.0);
			glEnable(GL_LIGHT4);
			break;
			   }
		}
	}
	void luminance_add(float x, float y)
	{
		if ( (abs(x-Lx)+abs(y-Ly)<12) )
		{
			if (Ll < 240) Ll += 5;
		}
	}
	void luminance_sub(float x, float y)
	{
		if ( (abs(x-Lx)+abs(y-Ly)<12) )
		{
			if (Ll > 60) Ll -= 5;
		}
	}
};

Light Light0( 0, 0, 19, 240, 0 );
Light Light1( 18, 0, 19, 240, 1 );
Light Light2( 0, -18, 19, 240, 2 );
Light Light3( -18, 0, 19, 240, 3 );
Light Light4( 0, 18, 19, 240, 4 );

unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader) 
{
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

	for (imageIdx = 0; imageIdx < bitmapInfoHeader->biSizeImage; imageIdx+=3) { 
		tempRGB = bitmapImage[imageIdx]; 
		bitmapImage[imageIdx] = bitmapImage[imageIdx + 2]; 
		bitmapImage[imageIdx + 2] = tempRGB; 
	}
	fclose(filePtr); 
	return bitmapImage; 
}

void texload(int i,char *filename)
{
	
    BITMAPINFOHEADER bitmapInfoHeader; 
	unsigned char*   bitmapData; 

   bitmapData = LoadBitmapFile(filename, &bitmapInfoHeader);
   glBindTexture(GL_TEXTURE_2D, texture[i]);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmapInfoHeader.biWidth, bitmapInfoHeader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmapData);	 
}

void texinit()
{	           
	glGenTextures(4, texture); 
	texload(0,"Monet.bmp");
    texload(1,"Crack.bmp");
	texload(2,"Spot.bmp");
	
	unsigned char tex[512*512*3]; 

	for(int i = 0; i < 512; i++)
	{
		for(int j = 0; j < 512*3; j += 3)
		{
			if( ((j/96) % 2) + ((i/32) % 2) != 1 )
		    {
				tex[i*512*3+j] = 255; 
				tex[i*512*3+j+1] = 0;
				tex[i*512*3+j+2] = 0;
		    }
		    else
		    {
				tex[i*512*3+j] = 0; 
				tex[i*512*3+j+1] = 0;
				tex[i*512*3+j+2] = 0;
		    }
		}
	}
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture[3]);                             
 	glPixelStorei(GL_UNPACK_ALIGNMENT,1); 
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,512,512,0,GL_RGB,GL_UNSIGNED_BYTE,tex);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
}

void Draw_Scene()
{

	//The Desk

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[1]); 
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

	Draw_Cube(0, 0, 5.5, 5, 4, 1);
	Draw_Cube(1.5, 1, 2.5, 1, 1, 5);
	Draw_Cube(-1.5, 1, 2.5, 1, 1, 5);
	Draw_Cube(1.5, -1, 2.5, 1, 1, 5);
	Draw_Cube(-1.5, -1, 2.5, 1, 1, 5);

	glDisable(GL_TEXTURE_2D);

	//The Door

	Door1.draw();
	Door1.approach_notice(eye[0], eye[1]);
	Door2.draw();
	Door2.approach_notice(eye[0], eye[1]);
	Door3.draw();
	Door3.approach_notice(eye[0], eye[1]);
	Door4.draw();
	Door4.approach_notice(eye[0], eye[1]);

}

GLint GenTableList()
{
	GLint lid=glGenLists(1);
	glNewList(lid, GL_COMPILE);

	GLint tex_func = GL_MODULATE;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[0]); 
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,tex_func);

	glPushMatrix();
	glScalef(2, 2, 1);

	// Wall 1
	glBegin(GL_QUADS);              
        glTexCoord2i(0,1); glVertex3i(5,10,20);
        glTexCoord2i(1,1); glVertex3i(10,5,20);
        glTexCoord2i(1,0); glVertex3i(10,5,0);
        glTexCoord2i(0,0); glVertex3i(5,10,0);
	glEnd();
	glBegin(GL_QUADS);              
        glTexCoord2i(0,1); glVertex3i(10,-5,20);
        glTexCoord2i(1,1); glVertex3i(5,-10,20);
        glTexCoord2i(1,0); glVertex3i(5,-10,0);
        glTexCoord2i(0,0); glVertex3i(10,-5,0);
	glEnd();
	glBegin(GL_QUADS);              
        glTexCoord2i(0,1); glVertex3i(-5,-10,20);
        glTexCoord2i(1,1); glVertex3i(-10,-5,20);
        glTexCoord2i(1,0); glVertex3i(-10,-5,0);
        glTexCoord2i(0,0); glVertex3i(-5,-10,0);
	glEnd();
	glBegin(GL_QUADS);              
        glTexCoord2i(0,1); glVertex3i(-10,5,20);
        glTexCoord2i(1,1); glVertex3i(-5,10,20);
        glTexCoord2i(1,0); glVertex3i(-5,10,0);
        glTexCoord2i(0,0); glVertex3i(-10,5,0);
	glEnd();

	// Wall 2
	glBegin(GL_QUADS);              
        glTexCoord2i(0,1); glVertex3i(-5,10,20);
        glTexCoord2i(1,1); glVertex3i(-5,25,20);
        glTexCoord2i(1,0); glVertex3i(-5,25,0);
        glTexCoord2i(0,0); glVertex3i(-5,10,0);
	glEnd();
	glBegin(GL_QUADS);              
        glTexCoord2i(0,1); glVertex3i(-5,25,20);
        glTexCoord2i(1,1); glVertex3i(5,25,20);
        glTexCoord2i(1,0); glVertex3i(5,25,0);
        glTexCoord2i(0,0); glVertex3i(-5,25,0);
	glEnd();
	glBegin(GL_QUADS);              
        glTexCoord2i(0,1); glVertex3i(5,25,20);
        glTexCoord2i(1,1); glVertex3i(5,10,20);
        glTexCoord2i(1,0); glVertex3i(5,10,0);
        glTexCoord2i(0,0); glVertex3i(5,25,0);
	glEnd();

	// Wall 3
	glBegin(GL_QUADS);              
        glTexCoord2i(0,1); glVertex3i(10,5,20);
        glTexCoord2i(1,1); glVertex3i(25,5,20);
        glTexCoord2i(1,0); glVertex3i(25,5,0);
        glTexCoord2i(0,0); glVertex3i(10,5,0);
	glEnd();
	glBegin(GL_QUADS);              
        glTexCoord2i(0,1); glVertex3i(25,5,20);
        glTexCoord2i(1,1); glVertex3i(25,-5,20);
        glTexCoord2i(1,0); glVertex3i(25,-5,0);
        glTexCoord2i(0,0); glVertex3i(25,5,0);
	glEnd();
	glBegin(GL_QUADS);              
        glTexCoord2i(0,1); glVertex3i(25,-5,20);
        glTexCoord2i(1,1); glVertex3i(10,-5,20);
        glTexCoord2i(1,0); glVertex3i(10,-5,0);
        glTexCoord2i(0,0); glVertex3i(25,-5,0);
	glEnd();

	// Wall 4
	glBegin(GL_QUADS);              
        glTexCoord2i(0,1); glVertex3i(5,-10,20);
        glTexCoord2i(1,1); glVertex3i(5,-25,20);
        glTexCoord2i(1,0); glVertex3i(5,-25,0);
        glTexCoord2i(0,0); glVertex3i(5,-10,0);
	glEnd();
	glBegin(GL_QUADS);              
        glTexCoord2i(0,1); glVertex3i(5,-25,20);
        glTexCoord2i(1,1); glVertex3i(-5,-25,20);
        glTexCoord2i(1,0); glVertex3i(-5,-25,0);
        glTexCoord2i(0,0); glVertex3i(5,-25,0);
	glEnd();
	glBegin(GL_QUADS);              
        glTexCoord2i(0,1); glVertex3i(-5,-25,20);
        glTexCoord2i(1,1); glVertex3i(-5,-10,20);
        glTexCoord2i(1,0); glVertex3i(-5,-10,0);
        glTexCoord2i(0,0); glVertex3i(-5,-25,0);
	glEnd();

	// Wall 5
	glBegin(GL_QUADS);              
        glTexCoord2i(0,1); glVertex3i(-10,-5,20);
        glTexCoord2i(1,1); glVertex3i(-25,-5,20);
        glTexCoord2i(1,0); glVertex3i(-25,-5,0);
        glTexCoord2i(0,0); glVertex3i(-10,-5,0);
	glEnd();
	glBegin(GL_QUADS);              
        glTexCoord2i(0,1); glVertex3i(-25,-5,20);
        glTexCoord2i(1,1); glVertex3i(-25,5,20);
        glTexCoord2i(1,0); glVertex3i(-25,5,0);
        glTexCoord2i(0,0); glVertex3i(-25,-5,0);
	glEnd();
	glBegin(GL_QUADS);              
        glTexCoord2i(0,1); glVertex3i(-25,5,20);
        glTexCoord2i(1,1); glVertex3i(-10,5,20);
        glTexCoord2i(1,0); glVertex3i(-10,5,0);
        glTexCoord2i(0,0); glVertex3i(-25,5,0);
	glEnd();

	glPopMatrix();

	// Floor
	glBindTexture(GL_TEXTURE_2D, texture[1]); 
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,tex_func);

	glPushMatrix();
	glScalef(2, 2, 1);

	glBegin(GL_QUADS);              
        glTexCoord2i(0,0); glVertex3i(-25,25,0);
        glTexCoord2i(1,0); glVertex3i(25,25,0);
        glTexCoord2i(1,1); glVertex3i(25,-25,0);
        glTexCoord2i(0,1); glVertex3i(-25,-25,0);
	glEnd();

	glBegin(GL_QUADS);              
        glTexCoord2i(0,0); glVertex3i(-25,25,20);
        glTexCoord2i(1,0); glVertex3i(25,25,20);
        glTexCoord2i(1,1); glVertex3i(25,-25,20);
        glTexCoord2i(0,1); glVertex3i(-25,-25,20);
	glEnd();

	glPopMatrix();

	glDisable(GL_TEXTURE_2D);

	glEndList();
	return lid;
}

void Draw_List()
{
	glCallList(tableList);
}

void updateView(int width, int height)
{
	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	float whRatio = (GLfloat)width/(GLfloat)height;
	gluPerspective(vision_angle, whRatio,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
}

void reshape(int width, int height)
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	wHeight = height;
	wWidth = width;

	updateView(wHeight, wWidth);
}

void idle()
{
	glutPostRedisplay();
}

void key(unsigned char k, int x, int y)
{
	switch(k)
	{
	//case 'q': {exit(0); break; }

	//case ' ': {bAnim = !bAnim; break;}

	// Move forward
	case 'w': {
		eye[0] += step_radium *(float(sin(double(pi*eye_angle_1/180))));
		center[0] += step_radium *(float(sin(double(pi*eye_angle_1/180))));
		eye[1] += step_radium *(float(cos(double(pi*eye_angle_1/180))));
		center[1] += step_radium *(float(cos(double(pi*eye_angle_1/180))));
		break;
			  }
	// Move backward
	case 's': {
		eye[0] -= step_radium *(float(sin(double(pi*eye_angle_1/180))));
		center[0] -= step_radium *(float(sin(double(pi*eye_angle_1/180))));
		eye[1] -= step_radium *(float(cos(double(pi*eye_angle_1/180))));
		center[1] -= step_radium *(float(cos(double(pi*eye_angle_1/180))));
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
		Door1.approach(eye[0], eye[1]);
		Door2.approach(eye[0], eye[1]);
		Door3.approach(eye[0], eye[1]);
		Door4.approach(eye[0], eye[1]);
		break;
			  }
	case 'k': {
		if ( (abs(center[0])<20) && (abs(center[1])<20) && (abs(center[2])<20) ) Dyn1.hue_sub();
		break;
			  }
	case 'l': {
		if ( (abs(center[0])<20) && (abs(center[1])<20) && (abs(center[2])<20) ) Dyn1.hue_add();
		break;
			  }
	case 'r': {
		if ( (abs(center[0])<20) && (abs(center[1])<20) && (abs(center[2])<20) ) Dyn1.rotate();
		break;
			  }
	case 'u': {
		if ( (abs(center[0])<20) && (abs(center[1])<20) && (abs(center[2])<20) ) Dyn1.shape_change();
		break;
			  }
	case 'i': {
		if ( (abs(center[0])<20) && (abs(center[1])<20) && (abs(center[2])<20) ) Dyn1.size_sub();
		break;
			  }
	case 'o': {
		if ( (abs(center[0])<20) && (abs(center[1])<20) && (abs(center[2])<20) ) Dyn1.size_add();
		break;
			  }
	case 'n': {
		Light1.luminance_sub(eye[0], eye[1]);
		Light2.luminance_sub(eye[0], eye[1]);
		Light3.luminance_sub(eye[0], eye[1]);
		Light4.luminance_sub(eye[0], eye[1]);
		break;
			  }
	case 'm': {
		Light1.luminance_add(eye[0], eye[1]);
		Light2.luminance_add(eye[0], eye[1]);
		Light3.luminance_add(eye[0], eye[1]);
		Light4.luminance_add(eye[0], eye[1]);
		break;
			  }
	case 'g': {
		if (!bOrbit)
		{
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
		if (bOrbit)
		{
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

	updateView(wHeight, wWidth);
}

void redraw()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity(); // Reset The Current Modelview Matrix
	if (!bOrbit)
	{
		center[0] = eye[0] + eye_radium *(float(sin(double(pi*eye_angle_1/180))));
		center[1] = eye[1] + eye_radium *(float(cos(double(pi*eye_angle_1/180))));
		center[2] = eye[2] + eye_radium *(float(sin(double(pi*eye_angle_2/180))));
		gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], 0, 0, 1);
	}
	else if (bOrbit)
	{
		if (eye_angle_1 == 359.98)
			eye_angle_1 = 0;
		else eye_angle_1 += 0.02;
		eye[0] = (-1) * 14 * (float(sin(double(pi*eye_angle_1/180))));
		eye[1] = (-1) * 14 * (float(cos(double(pi*eye_angle_1/180))));
		center[0] = (-1) * 6 * (float(sin(double(pi*eye_angle_1/180))));
		center[1] = (-1) * 6 * (float(cos(double(pi*eye_angle_1/180))));
		gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], 0, 0, 1);
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_DEPTH_TEST);

	Draw_List(); // Draw Static Objects
	Draw_Scene(); // Draw Dynamic Objects

	if (Dyn1.if_rotate()) Dyn1.angle_change();
	Dyn1.draw();

	Light0.enable();
	Light1.enable();
	Light2.enable();
	Light3.enable();
	Light4.enable();

	glutSwapBuffers();

}

int main (int argc,  char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(576,576);
	int windowHandle = glutCreateWindow("Simple GLUT App");

	texinit();

	tableList = GenTableList();
	glutDisplayFunc(redraw);
	glutReshapeFunc(reshape);	
	glutKeyboardFunc(key);
	glutIdleFunc(idle);

	glutMainLoop();
	return 0;
}


