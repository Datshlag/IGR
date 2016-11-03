// ----------------------------------------------
// Informatique Graphique 3D & R�alit� Virtuelle.
// Travaux Pratiques
// Introduction � OpenGL
// Copyright (C) 2015 Tamy Boubekeur
// All rights reserved.
// ----------------------------------------------

/*
Aloïs Pourchot

Questions traitées : toutes, sauf l'implémentation du bruit de Perlin. La fonctionnalité permettant en théorie de déplacer
le modèle ne fonctionne pas correctement. La fonction permettant  d'ouvrir une texture au format ppm ne fonctionne pas dans 
le cas où le fichier présente des commentaires. Deux textures de planètes sont présentes dans l'archive à titre purement visuel
dans le cas où lesdites textures ne seraient pas présentes, un échiquier sera généré dans le méthode d'ouverture. La méthode genCheckerBoard
fonctionne bien avec les paramètres fixés ci-dessous, mais il se pourrait qu'en changeant la quantification des sphères, elle ne rende plus vraiment
un échiquier. Je n'ai malheureusement plus le temps d'y toucher.
*/
#define GL_GLEXT_PROTOTYPES

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <math.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include "GL/glext.h"
#include <vector>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#ifndef M_PI
	#define M_PI 3.1415
#endif 

using namespace std;

// App parameters
static const unsigned int DEFAULT_SCREENWIDTH = 800;
static const unsigned int DEFAULT_SCREENHEIGHT = 600;
static string appTitle ("Informatique Graphique & Realite Virtuelle - Travaux Pratiques - Introduction a OpenGL");
static GLint window;
static unsigned int screenWidth;
static unsigned int screenHeight;
static bool movingCamera;
static bool zooming;
static bool movingObject;
static bool settingCameraUp = true;

// Camera parameters
static float fovAngle;
static float aspectRatio;
static float nearPlane;
static float farPlane;
static float camPhi; // Expressing the camera position in polar coordinate, in the frame of the target
static float camTheta;
static float camDist2Target;
static float camTargetX;
static float camTargetY;
static float camTargetZ;

// Sphere parameters
const int n1=127;
const int n2=127;

static GLfloat spherePositionArray[3*6*n1*n2];
static GLfloat sphereColorArray[3*6*n1*n2];
static GLfloat sphereNormalArray[3*6*n1*n2];
static GLuint sphereIndexArray[6*n1*n2];
static GLfloat sphereTexCoordArray[2*6*n1*n2];

// Buffer id
static GLuint indexBufferId;

//Texture parameters
static GLuint textures[2];
static unsigned char *image;
unsigned int width;
unsigned int height;

// Light parameters
static int activeLights[8]={0,0,0,0,0,0,0,0};
static int lightEnabled=1;

// Time parameters
static float currentTime;
static float acceleration = 0.0;
static float v0 = 1.0;
static float t0 = 0.0;
static float x0 = 0.0;


void polar2Cartesian (float phi, float theta, float r, float & x, float & y, float & z) {
	x = r * sin (theta) * cos (phi);
	y = r * sin (theta) * sin (phi);
	z = r * cos (theta);
  }

void getCurrentCircleCoordinate(float alpha, float phi, float theta, float radius, float &x, float &y, float &z){

	float t = currentTime*0.001;
	x = -radius*cos(alpha*(acceleration/2*(t-t0)*(t-t0)+v0*(t-t0)+x0))*sin(phi) + radius*sin(alpha*(acceleration/2*(t-t0)*(t-t0)+v0*(t-t0)+x0))*cos(theta)*cos(phi);
	y = radius*cos(alpha*(acceleration/2*(t-t0)*(t-t0)+v0*(t-t0)+x0))*cos(phi)+radius*sin(alpha*(acceleration/2*(t-t0)*(t-t0)+v0*(t-t0)+x0))*cos(theta)*sin(phi);
	z = -radius*sin(alpha*(acceleration/2*(t-t0)*(t-t0)+v0*(t-t0)+x0))*sin(theta);
}

void glSphereVBO(float xCoord, float yCoord, float zCoord, float radius){

	glMatrixMode (GL_MODELVIEW); // Indique que l’on va désormais altérer la matrice modèle-vue
	glPushMatrix (); // pousse la matrice courante sur un pile
	glTranslatef (xCoord, yCoord, zCoord); // applique une translation à la matrice
	glScalef(radius,radius,radius);
	glRotatef(currentTime*1/(30*2*M_PI),0,1,0);

	glDrawElements(GL_TRIANGLES, 6*n1*n2, GL_UNSIGNED_INT, (GLvoid *) 0);

	glPopMatrix (); // remplace la matrice modèle vue courante original
}

void genCheckerBoard(unsigned int w, unsigned int h){

	width=w/32;
	height=h/32;
	image = new unsigned char[3*width*height];
	int k=0;
	for(unsigned int i=0 ; i<width; i++)
	{
		for(unsigned int j=0; j<height; j++)
		{


			if((i+j)%2==0){ image[3*k]=255; image[3*k+1]=0; image[3*k+2]=0; }
			else{ image[3*k]=0; image[3*k+1]=0; image[3*k+2]=255;}
			k++;
		}
	}
}

void glSphereWithMat(float xCoord, float yCoord, float zCoord, float radius,
					float difR, float difG, float difB,
					float specR, float specG, float specB,
					float shininess){

	GLfloat material_color[4] = {difR, difG, difB, 1.0f};
	GLfloat material_specular[4] = {specR, specG, specB, 1.0};
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_color);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

	glSphereVBO(xCoord, yCoord, zCoord, radius);

	glPopMatrix ();
}

void glSphereSimple(float xCoord, float yCoord, float zCoord, float radius)
{

	glMatrixMode (GL_MODELVIEW); // Indique que l’on va désormais altérer la matrice modèle-vue
	glPushMatrix (); // pousse la matrice courante sur un pile
	glTranslatef (xCoord, yCoord, zCoord); // applique une translation à la matrice
	glScalef(radius,radius,radius);

	float theta=0;
	float phi=0;
	float x=0;
	float y=0;
	float z=0;

	glBegin(GL_TRIANGLES);
	for(int k1=0; k1<n1; k1++)
	{

		for(int k2=0; k2<n2; k2++)
		{

			theta=k1*M_PI/n1;
			phi=k2*2*M_PI/n2;
			polar2Cartesian(phi,theta,1.0,x,y,z);
			glTexCoord2f(1.0*k2/n2, 1.0*k1/n1);
			glColor3f(1,1,1);
			glVertex3f(x,y,z);

			theta=(k1+1)*M_PI/n1;
			phi=k2*2*M_PI/n2;
			polar2Cartesian(phi,theta,1.0,x,y,z);
			glColor3f(1,1,1);
			glVertex3f(x,y,z);

			theta=(k1+1)*M_PI/n1;
			phi=(k2+1)*2*M_PI/n2;
			polar2Cartesian(phi,theta,1.0,x,y,z);
			glColor3f(1,1,1);
			glVertex3f(x,y,z);

			theta=k1*M_PI/n1;
			phi=k2*2*M_PI/n2;
			polar2Cartesian(phi,theta,1.0,x,y,z);
			glColor3f(1,1,1);
			glVertex3f(x,y,z);

			theta=(k1+1)*M_PI/n1;
			phi=(k2+1)*2*M_PI/n2;
			polar2Cartesian(phi,theta,1.0,x,y,z);
			glColor3f(1,1,1);
			glVertex3f(x,y,z);

			theta=k1*M_PI/n1;
			phi=(k2+1)*2*M_PI/n2;
			polar2Cartesian(phi,theta,1.0,x,y,z);
			glColor3f(1,1,1);
			glVertex3f(x,y,z);
		}
	}
	glEnd();

	glPopMatrix (); // replace la matrice modèle vue courante original
}

void loadImage(const char *filename)
{
    std::ifstream f(filename);
    if (f.fail())
    {
        cout << "Could not open file: " << filename << endl;
        genCheckerBoard(512,512);
        return;
    }

    // get type of file
    int mode = 0;
    string s;
    f >> s;
    if (s == "P3")
        mode = 3;
    else if (s == "P6")
        mode = 6;
    
    // get w
    f >> width;

    // get h
    f >> height;
    
    // get bits
    int bits = 0;
    f >> bits;
    
    // error checking
    if (mode != 3 && mode != 6)
    {
        cout << "Unsupported magic number" << endl;
        f.close();
        return;
    }
    if (width < 1)
    {
        cout << "Unsupported width: " << width << endl;
        f.close();
        return;
    }
    if (height < 1)
    {
        cout << "Unsupported height: " << height << endl;
        f.close();
        return;
    }
    if (bits < 1 || bits > 255)
    {
        cout << "Unsupported number of bits: " << bits << endl;
        f.close();
        return;
    }

    // load image data
    image = new unsigned char[3*width*height];

    if (mode == 6)
    {
        f.get();
        f.read((char*)image, 3*height*width);
    }
    if (mode == 3)
    {
        for (unsigned int i = 0; i < width*height; i++)
        {
            int v;
            f >> v;
            image[3*i] = v;
            f >> v;
            image[3*i+1] = v;
            f >> v;
            image[3*i+2] = v;
        }
    }

    // close file
    f.close();
}

void printUsage () {
	std::cerr << std::endl // send a line break to the standard error output
		 << appTitle << std::endl
         << "Author : Tamy Boubekeur" << std::endl << std::endl
         << "Student : Aloïs Pourchot" << std::endl << std::endl
         << "Usage : ./main [<file.off>]" << std::endl
         << "Commandes clavier :" << std::endl
         << "------------------" << std::endl
         << " ?: Print help" << std::endl
		 << " w: Toggle wireframe mode" << std::endl
		 << " 9 : Toggle light mode" << std::endl
		 << " 1-8 : Toggle light source" << std::endl
         << " <drag>+<left button>: rotate model" << std::endl
         << " <drag>+<right button>: move model (this feature is messy)" << std::endl
         << " <drag>+<middle button>: zoom" << std::endl
         << " +: increase sphere acceleration"<< std::endl
         << " -: decrease sphere acceleration"<<std::endl
         << " q, <esc>: Quit" << std::endl << std::endl;
}

void init () {
	// OpenGL initialization
  	glCullFace (GL_BACK);     // Specifies the faces to cull (here the ones pointing away from the camera)
 	glEnable (GL_CULL_FACE); // Enables face culling (based on the orientation defined by the CW/CCW enumeration).
  	glDepthFunc (GL_LESS); // Specify the depth test for the z-buffer
 	glEnable (GL_DEPTH_TEST); // Enable the z-buffer in the rasterization
 	glEnable (GL_LIGHTING) ; // Enable lighting
  	glLineWidth (2.0); // Set the width of edges in GL_LINE polygon mode
  	glClearColor (0.0f, 0.0f, 0.0f, 1.0f); // Background color

	// Camera initialization
	fovAngle = 45.f;
	nearPlane = 0.01;
	farPlane = 500.0;
	camPhi = 0;
	camTheta = 0;
	camDist2Target = 30;
	camTargetX = 0.0;
	camTargetY = 0.0;
	camTargetZ = 0.0;

	// Sphere stuff initialization
	float theta=0;
	float phi=0;
	float x=0;
	float y=0;
	float z=0;
	int i=0;

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 3*sizeof (float), (GLvoid*) spherePositionArray);
	
	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(3, GL_FLOAT, 3*sizeof (float), (GLvoid*) sphereColorArray);

	glEnableClientState (GL_NORMAL_ARRAY);
	glNormalPointer (GL_FLOAT, 3*sizeof (float), (GLvoid*) sphereNormalArray);
	glEnable(GL_NORMALIZE);

	glEnableClientState (GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer (2, GL_FLOAT, 2*sizeof (float), (GLvoid*) sphereTexCoordArray);

	for(int k1=0; k1<n1; k1++)
	{

		for(int k2=0; k2<n2; k2++)
		{

			theta=k1*M_PI/n1;
			phi=k2*2*M_PI/n2;
			polar2Cartesian(phi,theta,1.0,x,y,z);
			spherePositionArray[3*i]=x;
			spherePositionArray[3*i+1]=y;
			spherePositionArray[3*i+2]=z;
			sphereNormalArray[3*i]=x;
			sphereNormalArray[3*i+1]=y;
			sphereNormalArray[3*i+2]=z;
			sphereColorArray[3*i]=0.9;
			sphereColorArray[3*i+1]=0.9;
			sphereColorArray[3*i+2]=0.9;
			sphereTexCoordArray[2*i]=1.0*k2/n2;
			sphereTexCoordArray[2*i+1]=1.0*k1/n1;
			sphereIndexArray[i]=i;
			i++;

			theta=(k1+1)*M_PI/n1;
			phi=k2*2*M_PI/n2;
			polar2Cartesian(phi,theta,1.0,x,y,z);
			spherePositionArray[3*i]=x;
			spherePositionArray[3*i+1]=y;
			spherePositionArray[3*i+2]=z;
			sphereNormalArray[3*i]=x;
			sphereNormalArray[3*i+1]=y;
			sphereNormalArray[3*i+2]=z;
			sphereColorArray[3*i]=0.9;
			sphereColorArray[3*i+1]=0.9;
			sphereColorArray[3*i+2]=0.9;
			sphereTexCoordArray[2*i]=1.0*k2/n2;
			sphereTexCoordArray[2*i+1]=1.0*k1/n1;
			sphereIndexArray[i]=i;
			i++;

			theta=(k1+1)*M_PI/n1;
			phi=(k2+1)*2*M_PI/n2;
			polar2Cartesian(phi,theta,1.0,x,y,z);
			spherePositionArray[3*i]=x;
			spherePositionArray[3*i+1]=y;
			spherePositionArray[3*i+2]=z;
			sphereNormalArray[3*i]=x;
			sphereNormalArray[3*i+1]=y;
			sphereNormalArray[3*i+2]=z;
			sphereColorArray[3*i]=0.9;
			sphereColorArray[3*i+1]=0.9;
			sphereColorArray[3*i+2]=0.9;
			sphereTexCoordArray[2*i]=1.0*k2/n2;
			sphereTexCoordArray[2*i+1]=1.0*k1/n1;
			sphereIndexArray[i]=i;
			i++;

			theta=k1*M_PI/n1;
			phi=k2*2*M_PI/n2;
			polar2Cartesian(phi,theta,1.0,x,y,z);
			spherePositionArray[3*i]=x;
			spherePositionArray[3*i+1]=y;
			spherePositionArray[3*i+2]=z;
			sphereNormalArray[3*i]=x;
			sphereNormalArray[3*i+1]=y;
			sphereNormalArray[3*i+2]=z;
			sphereColorArray[3*i]=0.9;
			sphereColorArray[3*i+1]=0.9;
			sphereColorArray[3*i+2]=0.9;
			sphereTexCoordArray[2*i]=1.0*k2/n2;
			sphereTexCoordArray[2*i+1]=1.0*k1/n1;
			sphereIndexArray[i]=i;
			i++;

			theta=(k1+1)*M_PI/n1;
			phi=(k2+1)*2*M_PI/n2;
			polar2Cartesian(phi,theta,1.0,x,y,z);
			spherePositionArray[3*i]=x;
			spherePositionArray[3*i+1]=y;
			spherePositionArray[3*i+2]=z;
			sphereNormalArray[3*i]=x;
			sphereNormalArray[3*i+1]=y;
			sphereNormalArray[3*i+2]=z;
			sphereColorArray[3*i]=0.9;
			sphereColorArray[3*i+1]=0.9;
			sphereColorArray[3*i+2]=0.9;
			sphereTexCoordArray[2*i]=1.0*k2/n2;
			sphereTexCoordArray[2*i+1]=1.0*k1/n1;
			sphereIndexArray[i]=i;
			i++;

			theta=k1*M_PI/n1;
			phi=(k2+1)*2*M_PI/n2;
			polar2Cartesian(phi,theta,1.0,x,y,z);
			spherePositionArray[3*i]=x;
			spherePositionArray[3*i+1]=y;
			spherePositionArray[3*i+2]=z;
			sphereNormalArray[3*i]=x;
			sphereNormalArray[3*i+1]=y;
			sphereNormalArray[3*i+2]=z;
			sphereColorArray[3*i]=0.9;
			sphereColorArray[3*i+1]=0.9;
			sphereColorArray[3*i+2]=0.9;
			sphereTexCoordArray[2*i]=1.0*k2/n2;
			sphereTexCoordArray[2*i+1]=1.0*k1/n1;
			sphereIndexArray[i]=i;
			i++;
		}
	}

	// Buffer initialization for the sphere
	glGenBuffers(1, &indexBufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphereIndexArray), sphereIndexArray, GL_STATIC_DRAW);

	// Lighting initialization
	GLfloat light_position_0[4] = {100.0f, 100.0f, 100.0f, 1.0f};
	GLfloat color_0[4] = {0.0f, 0.0f, 0.9f, 1.0f};
	glLightfv (GL_LIGHT0, GL_POSITION, light_position_0);
	glLightfv (GL_LIGHT0, GL_DIFFUSE, color_0);
	glLightfv (GL_LIGHT0, GL_SPECULAR, color_0);

	GLfloat light_position_1[4] = {-100.0f, -100.0f, -100.0f, 1.0f};
	GLfloat color_1[4] = {0.0f, 0.9f, 0.0f, 1.0f};
	glLightfv (GL_LIGHT1, GL_POSITION, light_position_1);
	glLightfv (GL_LIGHT1, GL_DIFFUSE, color_1);
	glLightfv (GL_LIGHT1, GL_SPECULAR, color_1);

	GLfloat light_position_2[4] = {-100.0f, 100.0f, 100.0f, 1.0f};
	GLfloat color_2[4] = {0.9f, 0.0f, 0.0f, 1.0f};
	glLightfv (GL_LIGHT2, GL_POSITION, light_position_2);
	glLightfv (GL_LIGHT2, GL_DIFFUSE, color_2);
	glLightfv (GL_LIGHT2, GL_SPECULAR, color_2);

	GLfloat light_position_3[4] = {100.0f, -100.0f, 100.0f, 1.0f};
	GLfloat color_3[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	glLightfv (GL_LIGHT3, GL_POSITION, light_position_3);
	glLightfv (GL_LIGHT3, GL_DIFFUSE, color_3);
	glLightfv (GL_LIGHT3, GL_SPECULAR, color_3);

	GLfloat light_position_4[4] = {100.0f, 100.0f, -100.0f, 1.0f};
	GLfloat color_4[4] = {0.5f, 0.5f, 0.5f, 1.0f};
	glLightfv (GL_LIGHT4, GL_POSITION, light_position_4);
	glLightfv (GL_LIGHT4, GL_DIFFUSE, color_4);
	glLightfv (GL_LIGHT4, GL_SPECULAR, color_4);

	GLfloat light_position_5[4] = {-100.0f, -100.0f, 100.0f, 1.0f};
	GLfloat color_5[4] = {0.9f, 0.7f, 0.1f, 1.0f};
	glLightfv (GL_LIGHT5, GL_POSITION, light_position_5);
	glLightfv (GL_LIGHT5, GL_DIFFUSE, color_5);
	glLightfv (GL_LIGHT5, GL_SPECULAR, color_5);

	GLfloat light_position_6[4] = {100.0f, -100.0f, -100.0f, 1.0f};
	GLfloat color_6[4] = {0.5f, 0.1f, 0.8f, 1.0f};
	glLightfv (GL_LIGHT6, GL_POSITION, light_position_6);
	glLightfv (GL_LIGHT6, GL_DIFFUSE, color_6);
	glLightfv (GL_LIGHT6, GL_SPECULAR, color_6);

	GLfloat light_position_7[4] = {-100.0f, 100.0f, -100.0f, 1.0f};
	GLfloat color_7[4] = {0.5f, 0.2f, 0.6f, 1.0f};
	glLightfv (GL_LIGHT7, GL_POSITION, light_position_7);
	glLightfv (GL_LIGHT7, GL_DIFFUSE, color_7); 
	glLightfv (GL_LIGHT7, GL_SPECULAR, color_7);

	// Texture stuff initialization
	glEnable(GL_TEXTURE_2D); 
	glGenTextures(2, textures);

	loadImage("textures/planetcenter.ppm");

	glBindTexture (GL_TEXTURE_2D, textures[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D (GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	loadImage("textures/planetorbit.ppm");

	glBindTexture (GL_TEXTURE_2D, textures[1]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D (GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	delete(image);
}

void setupCamera () {
	glMatrixMode (GL_PROJECTION); // Set the projection matrix as current. All upcoming matrix manipulations will affect it.
	glLoadIdentity ();
	gluPerspective (fovAngle, aspectRatio, nearPlane, farPlane); // Set the current projection matrix with the camera intrinsics
	glMatrixMode (GL_MODELVIEW); // Set the modelview matrix as current. All upcoming matrix manipulations will affect it.
	glLoadIdentity ();
	float camPosX, camPosY, camPosZ;
	polar2Cartesian (camPhi, camTheta, camDist2Target, camPosX, camPosY, camPosZ);
	camPosX += camTargetX;
	camPosY += camTargetY;
	camPosZ += camTargetZ;
	gluLookAt (camPosX, camPosY, camPosZ, camTargetX, camTargetY, camTargetZ, 0.0, 1.0, 0.0); // Set up the current modelview matrix with camera transform
}

void reshape (int w, int h) {
    screenWidth = w;
	screenHeight = h;
	aspectRatio = static_cast<float>(w)/static_cast<float>(h);
	glViewport (0, 0, (GLint)w, (GLint)h); // Dimension of the drawing region in the window
	setupCamera ();
}

void display () {
	if(settingCameraUp){
		setupCamera ();
		settingCameraUp=false;
	}
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Erase the color and z buffers.

    glBindTexture (GL_TEXTURE_2D, textures[0]);
	glSphereWithMat(0,0,0,1.5,0.8f,0.2f,0.2f,0.6f,0.4f,0.0f,0.4f);

	float x,y,z;

	glBindTexture (GL_TEXTURE_2D, textures[1]);
	getCurrentCircleCoordinate(0.1,0, 0, 10.0, x,y,z);
	glSphereWithMat(x,y,z,0.5,0.5f,0.5f,0.5f,0.0f,0.4f,0.5f,0.2f);

	getCurrentCircleCoordinate(0.09,2*M_PI/9, M_PI/2, 10.5, x,y,z);
	glSphereWithMat(x,y,z,0.4,0.5f,0.5f,0.5f,0.0f,0.4f,0.5f,0.2f);

	getCurrentCircleCoordinate(0.08,2*2*M_PI/9, M_PI/3, 11.0, x,y,z);
	glSphereWithMat(x,y,z,0.2,0.5f,0.5f,0.5f,0.0f,0.4f,0.5f,0.2f);

	getCurrentCircleCoordinate(0.07,2*3*M_PI/9, M_PI/4, 11.5, x,y,z);
	glSphereWithMat(x,y,z,0.6,0.5f,0.5f,0.5f,0.0f,0.4f,0.5f,0.2f);

	getCurrentCircleCoordinate(0.06,2*4*M_PI/9, M_PI/5, 12.0, x,y,z);
	glSphereWithMat(x,y,z,0.2,0.5f,0.5f,0.5f,0.0f,0.4f,0.5f,0.2f);

	getCurrentCircleCoordinate(0.05,2*5*M_PI/9, M_PI/6, 12.5, x,y,z);
	glSphereWithMat(x,y,z,0.1,0.5f,0.5f,0.5f,0.0f,0.4f,0.5f,0.2f);

	getCurrentCircleCoordinate(0.04,2*6*M_PI/9, M_PI/7, 13.0, x,y,z);
	glSphereWithMat(x,y,z,0.3,0.5f,0.5f,0.5f,0.0f,0.4f,0.5f,0.2f);

	getCurrentCircleCoordinate(0.03,2*7*M_PI/9, M_PI/8, 14.0, x,y,z);
	glSphereWithMat(x,y,z,0.7,0.5f,0.5f,0.5f,0.0f,0.4f,0.5f,0.2f);

	getCurrentCircleCoordinate(0.02,2*8*M_PI/9, M_PI/9, 14.5, x,y,z);
	glSphereWithMat(x,y,z,0.4,0.5f,0.5f,0.5f,0.0f,0.4f,0.5f,0.2f);

    glFlush (); // Ensures any previous OpenGL call has been executed
    glutSwapBuffers ();  // swap the render buffer and the displayed (screen) one
}

void keyboard (unsigned char keyPressed, int x, int y) {
    switch (keyPressed)
		{
			case 'w':
					GLint mode[2];
					glGetIntegerv (GL_POLYGON_MODE, mode);
					glPolygonMode (GL_FRONT_AND_BACK, mode[1] ==  GL_FILL ? GL_LINE : GL_FILL);
	       		break;
		    case 'q':
		    case '1':
		    	if(activeLights[0]) { glDisable(GL_LIGHT0); activeLights[0]=0; }
		    	else { glEnable(GL_LIGHT0); activeLights[0]=1; }
		    	break;
		    case '2':
		    	if(activeLights[1]) { glDisable(GL_LIGHT1); activeLights[1]=0; }
		    	else { glEnable(GL_LIGHT1); activeLights[1]=1; }
		    	break;
		    case '3':
		    	if(activeLights[2]) { glDisable(GL_LIGHT2); activeLights[2]=0; }
		    	else { glEnable(GL_LIGHT2); activeLights[2]=1; }
		    	break;
		    case '4':
		    	if(activeLights[3]) { glDisable(GL_LIGHT3); activeLights[3]=0; }
		    	else { glEnable(GL_LIGHT3); activeLights[3]=1; }
		    	break;
		    case '5':
		    	if(activeLights[4]) { glDisable(GL_LIGHT4); activeLights[4]=0; }
		    	else { glEnable(GL_LIGHT4); activeLights[4]=1; }
		    	break;
		    case '6':
		    	if(activeLights[5]) { glDisable(GL_LIGHT5); activeLights[5]=0; }
		    	else { glEnable(GL_LIGHT5); activeLights[5]=1; }
		    	break;
		    case '7':
		    	if(activeLights[6]) { glDisable(GL_LIGHT6); activeLights[6]=0; }
		    	else { glEnable(GL_LIGHT6); activeLights[6]=1; }
		    	break;
		    case '8':
		    	if(activeLights[7]) { glDisable(GL_LIGHT7); activeLights[7]=0; }
		    	else { glEnable(GL_LIGHT7); activeLights[7]=1; }
		    	break;
	    	case '9':
	    		if(lightEnabled){ glDisable(GL_LIGHTING); lightEnabled=0;}
	    		else{ glEnable(GL_LIGHTING); lightEnabled=1 ;}
	    		break;
    		case '+':
    			x0 = acceleration/2*(currentTime*0.001-t0)*(currentTime*0.001-t0)+v0*(currentTime*0.001-t0)+x0;
    			v0 = v0 + acceleration*(currentTime*0.001-t0);
    			t0 = currentTime*0.001;
    			acceleration+=0.1;
    			break;
			case '-':
				x0 = acceleration/2*(currentTime*0.001-t0)*(currentTime*0.001-t0)+v0*(currentTime*0.001-t0)+x0;
    			v0 = v0 + acceleration*(currentTime*0.001-t0);
    			t0 = currentTime*0.001;
				acceleration-=0.1;
				break;
		    case 27:
		        exit (0);
		        break;
		    default:
	        	printUsage ();
	        	break;
    }
    glutPostRedisplay ();
}


float currxf;
float curryf;
float oldxf;
float oldyf;
float modelview[16];

void mouse (int button, int state, int x, int y) {

	if(button==GLUT_LEFT_BUTTON)
	{
		if(state==GLUT_DOWN)
		{
			movingCamera=true;
			oldxf=x;
			currxf=x;
			oldyf=y;
			curryf=y;
		}
		else 
		{
			movingCamera=false;
		}
	}
	else if(button==GLUT_MIDDLE_BUTTON)
	{
		if(state==GLUT_DOWN)
		{

			zooming=true;
			oldyf=y;
			curryf=y;
		}
		else
		{
			zooming=false;
		}
	}
	else if(button==GLUT_RIGHT_BUTTON)
	{
		if(state==GLUT_DOWN)
		{

			movingObject=true;
			oldxf=x;
			currxf=x;
			oldyf=y;
			curryf=y;
		}
		else
		{
			movingObject=false;
		}
	}
}
void motion (int x, int y) {

	if(movingCamera)
	{
		oldxf=currxf;
		oldyf=curryf;
		currxf=x;
		curryf=y;
		glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
		glMatrixMode(GL_MODELVIEW); // Set the modelview matrix as current. All upcoming matrix manipulations will affect it.
		glRotatef(currxf-oldxf, modelview[1], modelview[5] ,modelview[9]);
		glRotatef(curryf-oldyf, modelview[0], modelview[4], modelview[8]);
	}
	if(zooming)
	{

		oldyf=curryf;
		curryf=y;
		glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
		glMatrixMode(GL_MODELVIEW); // Set the modelview matrix as current. All upcoming matrix manipulations will affect it.
		glScalef(1+(curryf-oldyf)/100,1+(curryf-oldyf)/100,1+(curryf-oldyf)/100);
	}
	if(movingObject)
	{

		oldxf=currxf;
		oldyf=curryf;
		currxf=x;
		curryf=y;
		glMatrixMode(GL_MODELVIEW);
		glTranslatef(currxf-oldxf,curryf-oldyf,0);
	}
}

// This function is executed in an infinite loop. It updated the window title
// (frame-per-second, model size) and ask for rendering
void idle () {

	glutPostRedisplay ();
	currentTime = glutGet ((GLenum)GLUT_ELAPSED_TIME);
}

int main (int argc, char ** argv) {

    glutInit (&argc, argv); // Initialize a glut app
    glutInitDisplayMode (GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE); // Setup a RGBA framebuffer to display, with a depth buffer (z-buffer), in double buffer mode (fill a buffer then update the screen)
    glutInitWindowSize (DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT); // Set the window app size on screen
    window = glutCreateWindow (appTitle.c_str ()); // create the window
    init(); // Your initialization code (OpenGL states, geometry, material, lights, etc)
    glutReshapeFunc (reshape); // Callback function executed whenever glut need to setup the projection matrix
	glutDisplayFunc (display); // Callback function executed when the window app need to be redrawn
    glutKeyboardFunc (keyboard); // Callback function executed when the keyboard is used
    glutMouseFunc (mouse); // Callback function executed when a mouse button is clicked
	glutMotionFunc (motion); // Callback function executed when the mouse move
	glutIdleFunc (idle); // Callback function executed continuously when no other event happens (good for background procesing or animation for instance).
    printUsage (); // By default, display the usage help of the program
    glutMainLoop ();
    return 0;
}
