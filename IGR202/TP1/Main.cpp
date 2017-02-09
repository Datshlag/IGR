// --------------------------------------------------------------------------
// Copyright(C) 2009-2016
// Tamy Boubekeur
//
// Permission granted to use this code only for teaching projects and
// private practice.
//
// Do not distribute this code outside the teaching assignements.
// All rights reserved.
// --------------------------------------------------------------------------

#ifndef BUFFER_OFFSET

    #define BUFFER_OFFSET(offset) ((char*)NULL + (offset))

#endif

#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <time.h>

#include "Vec3.h"
#include "Camera.h"
#include "Mesh.h"
#include "GLProgram.h"
#include "Exception.h"
#include "LightSource.h"
#include "LightRay.h"
#include "BVH.h"

using namespace std;

static const unsigned int DEFAULT_SCREENWIDTH = 1024;
static const unsigned int DEFAULT_SCREENHEIGHT = 768;
static const string DEFAULT_MESH_FILE ("models/max_50K.off");

static const string appTitle ("Informatique Graphique & Realite Virtuelle - Travaux Pratiques - Algorithmes de Rendu");
static const string myName ("Aloïs Pourchot");
static GLint window;
static unsigned int FPS = 0;
static bool fullScreen = false;
bool toonShader = false;
bool drawBVH = false;

static Camera camera;
static Mesh* mesh;
static std::vector<LightSource> lightSources;
static BVH* bvh;

static int mode = 1;
static float shininess = 1.5;
static float alpha = 0.5;
static float F0 = 0.1;
static Vec3f matAlbedo = Vec3f(0.8, 0.2, 0.2);

GLint modeShader;
GLint shininessShader;
GLint alphaShader;
GLint F0Shader;
GLint lightPosShader;

GLuint vbo[3];
GLuint vao;
GLuint ibo;

GLuint lineVao;
GLuint lineVbo;

unsigned int positionIndex;
unsigned int normalIndex;
unsigned int colorIndex;
unsigned int lineIndex;
unsigned int sizeLinePos;

GLProgram * photoRealistProgram;
GLProgram * toonProgram;
GLProgram * lineProgram;

void loadVbo();
void loadGLPrograms();
void renderVbo();
void initiliazeColor();

static std::vector<float> colorResponses;
std::vector<float> linePos;

void printUsage () {
	std::cerr << std::endl
		 << appTitle << std::endl
         << "Author: " << myName << std::endl << std::endl
         << "Usage: ./main [<file.off>]" << std::endl
         << "Commands:" << std::endl
         << "------------------" << std::endl
         << " ?: Print help" << std::endl
		 << " w: Toggle wireframe mode" << std::endl
         << " <drag>+<left button>: rotate model" << std::endl
         << " <drag>+<right button>: move model" << std::endl
         << " <drag>+<middle button>: zoom" << std::endl
         << " z, s, q, d, a, e: move light" << std::endl
         << " t, g: increase, decrease shinisess" << std::endl
         << " y, h: increase, decrease alpha" << std::endl
         << " u, j: increase, decrease F0" << std::endl
         << " 1: Lambert + Blinn-Phong" << std::endl
         << " 2: Cook-Torrance" << std::endl
         << " 3: GGX" << std::endl
         << " q, <esc>: Quit" << std::endl << std::endl;
}

void init (const char * modelFilename) {
    glewExperimental = GL_TRUE;
    glewInit (); // init glew, which takes in charges the modern OpenGL calls (v>1.2, shaders, etc)

    glCullFace (GL_BACK);     // Specifies the faces to cull (here the ones pointing away from the camera)
    glEnable (GL_CULL_FACE); // Enables face culling (based on the orientation defined by the CW/CCW enumeration).
    glDepthFunc (GL_LESS); // Specify the depth test for the z-buffer
    glEnable (GL_DEPTH_TEST); // Enable the z-buffer in the rasterization
    glEnable (GL_NORMALIZE);
    glLineWidth (2.0); // Set the width of edges in GL_LINE polygon mode
    glClearColor (0.1f, 0.1f, 0.1f, 1.0f); // Background color

    mesh = new Mesh();
  	mesh->loadOFF (modelFilename);
    bvh = new BVH(mesh);//Build BVH tree with the mesh

    colorResponses.resize (4*mesh->positions().size());
    camera.resize (DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT);

    //Add light sources here (PS: pls don't)
    lightSources = std::vector<LightSource>();
    lightSources.push_back(LightSource(Vec3<float>(4,4,1),Vec3<float>(5.0,0.0,0.0)));

    loadGLPrograms();
    loadVbo();
    initiliazeColor();
}

void loadGLPrograms() {

    photoRealistProgram = GLProgram::genVFProgram ("Simple GL Program", "shaderVBO.vert", "shaderVBO.frag");

        modeShader = photoRealistProgram->getUniformLocation("mode");
        shininessShader = photoRealistProgram->getUniformLocation("shininess");
        alphaShader = photoRealistProgram->getUniformLocation("alpha");
        F0Shader = photoRealistProgram->getUniformLocation("F0");
        lightPosShader = photoRealistProgram->getUniformLocation("lightPos");

        photoRealistProgram->setUniform1i(modeShader, mode);
        photoRealistProgram->setUniform1f(shininessShader, shininess);
        photoRealistProgram->setUniform1f(alphaShader, alpha);
        photoRealistProgram->setUniform1f(F0Shader, F0);
        photoRealistProgram->setUniform3f(lightPosShader, lightSources[0].getPos()[0], lightSources[0].getPos()[1], lightSources[0].getPos()[2]);

    toonProgram = GLProgram::genVFProgram ("Simple GL Program", "shaderVBO.vert", "toonVBO.frag");

        lightPosShader = toonProgram->getUniformLocation("lightPos");
        toonProgram->setUniform3f(lightPosShader, lightSources[0].getPos()[0], lightSources[0].getPos()[1], lightSources[0].getPos()[2]);

    lineProgram = GLProgram::genVFProgram ("Line Program", "shaderLineVBO.vert", "shaderLineVBO.frag");

}

void loadVbo() {
 
    positionIndex = 0;
    normalIndex = 1;
    colorIndex = 2;
    lineIndex = 0;

    glGenBuffers(1, &ibo);
    glGenBuffers(3, vbo);
    glGenBuffers(1, &lineVbo);

    glGenVertexArrays(1, &vao);
    glGenVertexArrays(1, &lineVao);

    //LOADING VAO AND VBO FOR LINE SHADER
    glBindVertexArray(lineVao);
        //Buffer d'indices
        glBindBuffer(GL_ARRAY_BUFFER, lineVbo);
            linePos = bvh->getPosBuffer();
            sizeLinePos = linePos.size();
            glBufferData(GL_ARRAY_BUFFER, sizeLinePos * sizeof(GL_FLOAT), &linePos[0], GL_STATIC_DRAW);
            glVertexAttribPointer(lineIndex, 3, GL_FLOAT, GL_FALSE, 0, NULL);
            glEnableVertexAttribArray(lineIndex);

    //LOADING VAO AND VBO FOR PHOTOREALISTIC AND CARTOON SHADERS
    glBindVertexArray(vao); // Verrouillage du VAO
        // Buffer d'indices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); // Verrouillage du IBO
            // Allocation de la mémoire vidéo
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->triangles().size()*sizeof(Triangle), &(mesh->triangles())[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); // Verrouillage du VBO
            // Allocation de la mémoire vidéo
            glBufferData(GL_ARRAY_BUFFER, mesh->positions().size()*sizeof(mesh->positions()[0]), &(mesh->positions()[0]), GL_STATIC_DRAW);
            glVertexAttribPointer(positionIndex, 3, GL_FLOAT, GL_FALSE, 0, NULL);
            glEnableVertexAttribArray (positionIndex);

        glBindBuffer(GL_ARRAY_BUFFER, vbo[1]); // Verrouillage du VBO
            // Allocation de la mémoire vidéo
            glBufferData(GL_ARRAY_BUFFER, mesh->normals().size()*sizeof(mesh->normals()[0]), &(mesh->normals()[0]), GL_STATIC_DRAW);
            glVertexAttribPointer(normalIndex, 3, GL_FLOAT, GL_FALSE, 0, NULL);
            glEnableVertexAttribArray (normalIndex) ;

        glBindBuffer(GL_ARRAY_BUFFER, vbo[2]); // Verrouillage du VBO
            // Allocation de la mémoire vidéo
            glBufferData(GL_ARRAY_BUFFER, colorResponses.size()*sizeof(colorResponses[0]), &(colorResponses[0]), GL_STATIC_DRAW);
            glVertexAttribPointer(colorIndex, 4, GL_FLOAT, GL_FALSE, 0, NULL);
            glEnableVertexAttribArray (colorIndex) ;
}


void initiliazeColor() {

    unsigned int l = 0;

    std::vector<Vec3f> positions = mesh->positions();
    std::vector<Triangle> triangles = mesh->triangles();

    unsigned int nbVertex = positions.size();

    // On met tous les vertex à la même valeur d'Albedo
    while(l < 4*nbVertex)
    {

        colorResponses[l++] = matAlbedo[0];
        colorResponses[l++] = matAlbedo[1];
        colorResponses[l++] = matAlbedo[2];
        colorResponses[l++] = 1.0;
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]); // Verrouillage du VBO
        // Remplacement du contenu en VRAM
        glBufferSubData(GL_ARRAY_BUFFER, 0, colorResponses.size()*sizeof(colorResponses[0]), &(colorResponses[0]));
}

void toggleToonShader() {

    if(!toonShader){

        try {

            toonProgram->use();
            toonShader = true;
        } 
        catch (Exception & e) {
            cerr << e.msg () << endl;
        }
    }
    else {

        try {

            photoRealistProgram->use();
            toonShader = false;

        } 
        catch (Exception & e) {
            cerr << e.msg () << endl;
        }

    }
}

void computePerVertexShadow () {

    LightRay ray;
    bool intersects;

    Vec3f lightPos = lightSources[0].getPos();
    std::vector<Vec3f> positions = mesh->positions();
    std::vector<Triangle> triangles = mesh->triangles();
    unsigned int nbVertex = positions.size();

    unsigned int l = 0;
    int i;
    while(l < 4*nbVertex) {

        // On donne au vertex ses valeurs d'Albedo
        colorResponses[l++] = matAlbedo[0];
        colorResponses[l++] = matAlbedo[1];
        colorResponses[l++] = matAlbedo[2];

        i=l/4;

        // Rayon partant du vertex en direction de la source de lumière
        ray = LightRay(positions.at(i), normalize(lightPos-positions.at(i)));
        intersects = ray.intersectsBVH(bvh);

        // On change le signe de la 4ème coordonnée en fonction de s'il y a eu intersection
        if(intersects) colorResponses[l++] = -1.0;
        else colorResponses[l++] = 1.0;
    }


    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]); // Verrouillage du VBO
        // Remplacement du contenu en VRAM
        glBufferSubData(GL_ARRAY_BUFFER, 0, colorResponses.size()*sizeof(colorResponses[0]), &(colorResponses[0]));
}

void computePerVertexAO (unsigned int numOfSamples) {

    std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_real_distribution<float> range(-1.f,1.f);

    std::vector<Vec3f> positions = mesh->positions();
    std::vector<Vec3f> normals = mesh->normals();
    std::vector<Triangle> triangles = mesh->triangles();

    unsigned int nbVertex = positions.size();

    LightRay lightRay;

    float  sum = 0.0;

    Triangle currTri;
    Vec3f currPos;
    Vec3f n;
    Vec3f u, v;

    Vec3f XVector = {1.f, 0.f, 0.f};
    Vec3f YVector = {0.f, 1.f, 0.f};

    unsigned int l = nbVertex - 1;

    while(l != 0) {

        sum = 0.0;

        currPos = positions.at(l);
        n = normalize(normals.at(l));

        for(int i = numOfSamples; i--;) {

            if (dot(normals[i], XVector) < 0.80f)
                u = cross(normals.at(i), XVector);
            else
                u = cross(normals.at(i), YVector);

            u.normalize();
            v = cross(normals[i], u);
            v.normalize();

            float n1 = range(generator);
            float n2 = range(generator);

            Vec3f direction = normals[i] + n1*u + n2*v;

            lightRay = LightRay(currPos, direction);
            direction.normalize();

            if(!lightRay.intersectsBVH(bvh)) sum += dot(n, direction);
        }

        sum *= 1.0f/numOfSamples;
        colorResponses[4*l+3] *= sum;

        l--;
    }

    // Envoi des données au buffer sur le GPU
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]); // Verrouillage du VBO
        // Remplacement du contenu en VRAM
        glBufferSubData(GL_ARRAY_BUFFER, 0, colorResponses.size()*sizeof(colorResponses[0]), &(colorResponses[0]));
}

void render() {

    if(drawBVH) {

        lineProgram->use();
        glBindVertexArray(lineVao);
            glDrawArrays(GL_LINES, 0, sizeLinePos);
    }

    if(toonShader) toonProgram->use();
    else photoRealistProgram->use();

    glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 3*mesh->triangles().size(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));
}

void reshape(int w, int h) {
    camera.resize (w, h);
}

void display () {
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera.apply ();

    render();
    for(GLenum err; (err = glGetError()) != GL_NO_ERROR;)
    {
      cerr << err << std::endl;
    }

    glFlush ();
    glutSwapBuffers ();
}

void key (unsigned char keyPressed, int x, int y) {
    switch (keyPressed) {
	    case 'f':
	        if (fullScreen) {
	            glutReshapeWindow (camera.getScreenWidth (), camera.getScreenHeight ());
	            fullScreen = false;
	        } else {
	            glutFullScreen ();
	            fullScreen = true;
	        }
	        break;
		case 'z':
			for(unsigned int j=0; j<lightSources.size(); j++)
            {

                lightSources[j].addT(0.1);
            }
            photoRealistProgram->setUniform3f(lightPosShader, lightSources[0].getPos()[0], lightSources[0].getPos()[1], lightSources[0].getPos()[2]);
            toonProgram->setUniform3f(lightPosShader, lightSources[0].getPos()[0], lightSources[0].getPos()[1], lightSources[0].getPos()[2]);
			break;
		case 's':
			for(unsigned int j=0; j<lightSources.size(); j++)
            {

                lightSources[j].addT(-0.1);
            }
            photoRealistProgram->setUniform3f(lightPosShader, lightSources[0].getPos()[0], lightSources[0].getPos()[1], lightSources[0].getPos()[2]);
            toonProgram->setUniform3f(lightPosShader, lightSources[0].getPos()[0], lightSources[0].getPos()[1], lightSources[0].getPos()[2]);
			break;
		case 'q':
			for(unsigned int j=0; j<lightSources.size(); j++)
            {

                lightSources[j].addP(-0.1);
            }
            photoRealistProgram->setUniform3f(lightPosShader, lightSources[0].getPos()[0], lightSources[0].getPos()[1], lightSources[0].getPos()[2]);
            toonProgram->setUniform3f(lightPosShader, lightSources[0].getPos()[0], lightSources[0].getPos()[1], lightSources[0].getPos()[2]);
			break;
		case 'd':
			for(unsigned int j=0; j<lightSources.size(); j++)
            {

                lightSources[j].addP(0.1);
            }
            photoRealistProgram->setUniform3f(lightPosShader, lightSources[0].getPos()[0], lightSources[0].getPos()[1], lightSources[0].getPos()[2]);
            toonProgram->setUniform3f(lightPosShader, lightSources[0].getPos()[0], lightSources[0].getPos()[1], lightSources[0].getPos()[2]);
			break;
		case 'a':
			for(unsigned int j=0; j<lightSources.size(); j++)
            {

                lightSources[j].addR(0.1);
            }
            photoRealistProgram->setUniform3f(lightPosShader, lightSources[0].getPos()[0], lightSources[0].getPos()[1], lightSources[0].getPos()[2]);
            toonProgram->setUniform3f(lightPosShader, lightSources[0].getPos()[0], lightSources[0].getPos()[1], lightSources[0].getPos()[2]);
			break;
		case 'e':
			for(unsigned int j=0; j<lightSources.size(); j++)
            {

                lightSources[j].addR(-0.1);
            }
            photoRealistProgram->setUniform3f(lightPosShader, lightSources[0].getPos()[0], lightSources[0].getPos()[1], lightSources[0].getPos()[2]);
            toonProgram->setUniform3f(lightPosShader, lightSources[0].getPos()[0], lightSources[0].getPos()[1], lightSources[0].getPos()[2]);
			break;
        case 't':
            computePerVertexShadow();
            break;
        case 'r':
            initiliazeColor();
            break;
        case 'g':
            computePerVertexAO(32);
            break;
        case 'y':
            alpha = fmin(1,alpha + 0.05);
            photoRealistProgram->setUniform1f(alphaShader, alpha);
            break;
        case 'h':
            //bvh->drawBVH(mesh, colorResponses);
            /*alpha = fmax(0, alpha-0.05);
            photoRealistProgram->setUniform1f(alphaShader, alpha);*/
            break;
        case 'v':
            mesh->laplacianFilter();
            glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); // Verrouillage du VBO
            // Allocation de la mémoire vidéo
                glBufferData(GL_ARRAY_BUFFER, mesh->positions().size()*sizeof(mesh->positions()[0]), &(mesh->positions()[0]), GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, vbo[1]); // Verrouillage du VBO
                // Remplacement du contenu en VRAM
                glBufferSubData(GL_ARRAY_BUFFER, 0, mesh->normals().size()*sizeof(mesh->normals()[0]), &(mesh->normals()[0]));
            break;
        case 'x' :
            mesh->loadOFF(DEFAULT_MESH_FILE);
            glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); // Verrouillage du VBO
            // Allocation de la mémoire vidéo
                glBufferData(GL_ARRAY_BUFFER, mesh->positions().size()*sizeof(mesh->positions()[0]), &(mesh->positions()[0]), GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, vbo[1]); // Verrouillage du VBO
                // Remplacement du contenu en VRAM
                glBufferSubData(GL_ARRAY_BUFFER, 0, mesh->normals().size()*sizeof(mesh->normals()[0]), &(mesh->normals()[0]));
            break;
        case 'c':
            mesh->geometricalLaplacian();
            glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); // Verrouillage du VBO
            // Allocation de la mémoire vidéo
                glBufferData(GL_ARRAY_BUFFER, mesh->positions().size()*sizeof(mesh->positions()[0]), &(mesh->positions()[0]), GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, vbo[1]); // Verrouillage du VBO
                // Remplacement du contenu en VRAM
                glBufferSubData(GL_ARRAY_BUFFER, 0, mesh->normals().size()*sizeof(mesh->normals()[0]), &(mesh->normals()[0]));
            break;
        case 'b' :
            drawBVH = !drawBVH;
            break;
        case 'u':
            F0 += 0.1;
            photoRealistProgram->setUniform1f(F0Shader, F0);
            break;
        case 'j':
            F0 = fmax(0, F0-0.1);
            photoRealistProgram->setUniform1f(F0Shader, F0);
            break;
        case '0':
            toggleToonShader();
            break;
        case '1':
            mode = 1;
            photoRealistProgram->setUniform1i(modeShader, mode);
            break;
        case '2':
            mode = 2;
            photoRealistProgram->setUniform1i(modeShader, mode);
            break;
        case '3':
            mode = 3;
            photoRealistProgram->setUniform1i(modeShader, mode);
            break;
	    case 27:
	        exit (0);
	        break;
	    case 'w':
	        GLint mode[2];
			glGetIntegerv (GL_POLYGON_MODE, mode);
			glPolygonMode (GL_FRONT_AND_BACK, mode[1] ==  GL_FILL ? GL_LINE : GL_FILL);
	        break;
	    default:
	        printUsage ();
	        break;
    }
}

void mouse (int button, int state, int x, int y) {
    camera.handleMouseClickEvent (button, state, x, y);
}

void motion (int x, int y) {
    camera.handleMouseMoveEvent (x, y);
}

void idle () {
    static float lastTime = glutGet ((GLenum)GLUT_ELAPSED_TIME);
    static unsigned int counter = 0;
    counter++;
    float currentTime = glutGet ((GLenum)GLUT_ELAPSED_TIME);
    if (currentTime - lastTime >= 1000.0f) {
        FPS = counter;
        counter = 0;
        static char winTitle [128];
        unsigned int numOfTriangles = mesh->triangles ().size ();
        sprintf (winTitle, "Number Of Triangles: %d - FPS: %d", numOfTriangles, FPS);
        string title = appTitle + " - By " + myName  + " - " + winTitle;
        glutSetWindowTitle (title.c_str ());
        lastTime = currentTime;
    }
    glutPostRedisplay ();
}

int main (int argc, char ** argv) {
    if (argc > 2) {
        printUsage ();
        exit (1);
    }
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize (DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT);
    window = glutCreateWindow (appTitle.c_str ());
    init (argc == 2 ? argv[1] : DEFAULT_MESH_FILE.c_str ());
    glutIdleFunc (idle);
    glutReshapeFunc (reshape);
    glutDisplayFunc (display);
    glutKeyboardFunc (key);
    glutMotionFunc (motion);
    glutMouseFunc (mouse);
    printUsage ();
    glutMainLoop ();
    return 0;
}
