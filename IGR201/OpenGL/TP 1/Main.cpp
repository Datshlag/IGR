// ----------------------------------------------
// Informatique Graphique 3D & R�alit� Virtuelle.
// Travaux Pratiques
// Introduction � OpenGL
// Copyright (C) 2015 Tamy Boubekeur
// All rights reserved.
// ----------------------------------------------

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <GL/glut.h>
#include <vector>

using namespace std;

// App parameters
static const unsigned int DEFAULT_SCREENWIDTH = 1024;
static const unsigned int DEFAULT_SCREENHEIGHT = 768;
static string appTitle ("Informatique Graphique & Realite Virtuelle - Travaux Pratiques - Introduction a OpenGL");
static GLint window;
static unsigned int screenWidth;
static unsigned int screenHeight;

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
const int n1=20;
const int n2=20;
static float spherePositionArray[3*n1*n2];
static unsigned int sphereIndexArray[6*n1*n2];

void polar2Cartesian (float phi, float theta, float r, float & x, float & y, float & z) {
	x = r * sin (theta) * cos (phi);
	y = r * sin (theta) * sin (phi);
	z = r * cos (theta);
  }

void glSphere(float xCoord, float yCoord, float zCoord, float radius){

	glMatrixMode (GL_MODELVIEW); // Indique que l’on va désormais altérer la matrice modèle-vue
	glPushMatrix (); // pousse la matrice courante sur un pile
	glTranslatef (xCoord, yCoord, zCoord); // applique une translation à la matrice
	//glScale(radius,radius,radius);

	// Put your drawing code (glBegin, glVertex, glCallList, glDrawArray, etc) here
	glDrawElements(GL_TRIANGLES, 6*n1*n2, GL_UNSIGNED_INT, (GLvoid*) sphereIndexArray);

	glPopMatrix (); // replace la matrice modèle vue courante original
}

void printUsage () {
	std::cerr << std::endl // send a line break to the standard error output
		 << appTitle << std::endl
         << "Author : Tamy Boubekeur" << std::endl << std::endl
         << "Usage : ./main [<file.off>]" << std::endl
         << "Cammandes clavier :" << std::endl
         << "------------------" << std::endl
         << " ?: Print help" << std::endl
		 << " w: Toggle wireframe mode" << std::endl
         << " <drag>+<left button>: rotate model" << std::endl
         << " <drag>+<right button>: move model" << std::endl
         << " <drag>+<middle button>: zoom" << std::endl
         << " q, <esc>: Quit" << std::endl << std::endl;
}

void init () {
	// OpenGL initialization
  glCullFace (GL_BACK);     // Specifies the faces to cull (here the ones pointing away from the camera)
  glEnable (GL_CULL_FACE); // Enables face culling (based on the orientation defined by the CW/CCW enumeration).
  glDepthFunc (GL_LESS); // Specify the depth test for the z-buffer
  glEnable (GL_DEPTH_TEST); // Enable the z-buffer in the rasterization
	glLineWidth (2.0); // Set the width of edges in GL_LINE polygon mode
  glClearColor (0.0f, 0.0f, 0.0f, 1.0f); // Background color

	// Camera initialization
	fovAngle = 45.f;
	nearPlane = 0.01;
	farPlane = 10.0;
	camPhi = 0; //M_PI/2.0;
	camTheta = M_PI/2.0;
	camDist2Target = 10;
	camTargetX = 0.0;
	camTargetY = 0.0;
	camTargetZ = 0.0;

	// Sphere stuff initialization
	float theta=0;
	float phi=0;
	float x=0;
	float y=0;
	float z=0;

	for(int k1=0; k1<n1; k1++)
	{

		for(int k2=0; k2<n2; k2++)
		{

			theta=k1*M_PI/n1;
			phi=k2*2*M_PI/n2;
			polar2Cartesian(phi,theta,1.0,x,y,z);

			spherePositionArray[3*n2*k1+3*k2]=x;
			spherePositionArray[3*n2*k1+3*k2+1]=y;
			spherePositionArray[3*n2*k1+3*k2+2]=z;


			sphereIndexArray[6*n2*k1+6*k2]=n2*k1+k2;
			sphereIndexArray[6*n2+k1+6*k2+1]=n2*(k1+1)+k2;
			sphereIndexArray[6*n2+k1+6*k2+2]=n2*(k1+1)+(k2+1);
			sphereIndexArray[6*n2+k1+6*k2+3]=n2*k1+k2;
			sphereIndexArray[6*n2+k1+6*k2+4]=n2*(k1+1)+(k2+1);
			sphereIndexArray[6*n2+k1+6*k2+5]=n2*k1+(k2+1);
		}
	}
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 3*sizeof(float), (GLvoid*) spherePositionArray);

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
    setupCamera ();
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Erase the color and z buffers.

		glSphere(0,0,0,1);



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
	    case 27:
	        exit (0);
	        break;
	     default:
	        printUsage ();
	        break;
    }
    glutPostRedisplay ();
}

void mouse (int button, int state, int x, int y) {
}

void motion (int x, int y) {
}

// This function is executed in an infinite loop. It updated the window title
// (frame-per-second, model size) and ask for rendering
void idle () {
}

int main (int argc, char ** argv) {
    glutInit (&argc, argv); // Initialize a glut app
    glutInitDisplayMode (GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE); // Setup a RGBA framebuffer to display, with a depth buffer (z-buffer), in double buffer mode (fill a buffer then update the screen)
    glutInitWindowSize (DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT); // Set the window app size on screen
    window = glutCreateWindow (appTitle.c_str ()); // create the window
    init (); // Your initialization code (OpenGL states, geometry, material, lights, etc)
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
