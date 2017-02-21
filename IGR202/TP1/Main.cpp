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
#include <iomanip>
#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <cfloat>
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
string modelFileName;

static const string appTitle ("Informatique Graphique & Realite Virtuelle - Travaux Pratiques - Algorithmes de Rendu");
static const string myName ("Aloïs Pourchot");
static GLint window;
static unsigned int FPS = 0;
static bool fullScreen = false;
static bool toonShader = false;
static bool drawBVH = false;

static Camera camera;
static Mesh* mesh;
static vector<LightSource> lightSources;
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
GLint lightPosToonShader;

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

void glInit();
void loadLights();
void loadMesh(string);
void loadVbo();
void loadGLPrograms();
void renderVbo();
void initiliazeColor();

static vector<float> colorResponses;
vector<float> linePos;

//Printing how to use the program
void printUsage () {
	cerr << endl
		 << appTitle << endl
         << "Author: " << myName << endl << endl
         << "Usage: ./main [<file.off>]" << endl
         << "Commands:" << endl
         << "------------------" << endl
         << " ?: Print help" << endl
		 << " w: Toggle wireframe mode" << endl
         << " <drag> + <left button>: Rotate model" << endl
         << " <drag> + <right button>: Move model" << endl
         << " <drag> + <middle button>: Zoom" << endl
         << " z, s, q, d, a, e: Move light" << endl
         << "------------------" << endl
         << " t: Compute shadows" << endl
         << " g: Compute AO" << endl
         << " n: Show BVH" << endl
         << " r: Reload colors" << endl
         << "------------------" << endl
         << " y, h: Increase, decrease shinisess" << endl
         << " u, j: Increase, decrease alpha" << endl
         << " i, k: Increase, decrease F0" << endl
         << " 0: NPR" << endl
         << " 1: Lambert + Blinn-Phong" << endl
         << " 2: Cook-Torrance" << endl
         << " 3: GGX" << endl
         << "------------------" << endl
         << " c: Topological laplacian filter" << endl
         << " v: Geometrical laplacian filter" << endl
         << " b: OCS mesh simplifying algorithm" << endl
         << " x: Reload mesh" << endl
         << "------------------" << endl
         << " <esc>: Quit" << endl;
}

//Simple advance bar to show the user the progression of the computings
void advanceBar(float percent)
{
    int nbBar = int(percent/5);
    if(percent < 100)
        cout << setfill('0') << setw(2) << int(percent) << " % |";
    else
        cout << "Done |";
    for(int i = 0; i < nbBar; ++i)
        cout << "█";
    for(int i = nbBar; i < 20; ++i)
        cout << " ";
    cout << "|\r";
    cout.flush();
}

//Initiliazing stuff
void init (string _modelFileName) {
    
    glInit();
    loadMesh(_modelFileName);
    loadLights();
    loadGLPrograms();
    loadVbo();
    initiliazeColor();
}

//Initiliazing glew
void glInit() {

    glewExperimental = GL_TRUE;
    glewInit (); // init glew, which takes in charges the modern OpenGL calls (v>1.2, shaders, etc)
    glCullFace (GL_BACK);     // Specifies the faces to cull (here the ones pointing away from the camera)
    glEnable (GL_CULL_FACE); // Enables face culling (based on the orientation defined by the CW/CCW enumeration).
    glDepthFunc (GL_LESS); // Specify the depth test for the z-buffer
    glEnable (GL_DEPTH_TEST); // Enable the z-buffer in the rasterization
    glEnable (GL_NORMALIZE);
    glLineWidth (2.0); // Set the width of edges in GL_LINE polygon mode
    glClearColor (0.1f, 0.1f, 0.1f, 1.0f); // Background color

    camera.resize (DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT);
}

//Loading the mesh and the BVH
void loadMesh(string _modelFileName) { 

    modelFileName = _modelFileName;
    mesh = new Mesh();
    mesh->loadOFF (modelFileName);
    if(bvh) delete bvh;
    bvh = new BVH(*mesh);//Build BVH tree with the mesh
}

//Loading the light
void loadLights () {
    
    lightSources.push_back(LightSource(Vec3<float>(10.0,0.0,0.0),Vec3<float>(1.0,0.0,1.0)));
}

//Creating the glPrograms and linking parameters between those programs and this one.
void loadGLPrograms() {
    try {
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
    }    
    catch (Exception & e) {
        cerr << "Photorealistic shader " << e.msg () << endl;
    }

    try {
        toonProgram = GLProgram::genVFProgram ("Simple GL Program", "shaderVBO.vert", "toonVBO.frag");
        lightPosToonShader = toonProgram->getUniformLocation("lightPos");
        toonProgram->setUniform3f(lightPosToonShader, lightSources[0].getPos()[0], lightSources[0].getPos()[1], lightSources[0].getPos()[2]);
    }
    catch (Exception & e) {
        cerr << "Toon shader " << e.msg () << endl;
    }
    
    try {
        
        lineProgram = GLProgram::genVFProgram ("Line Program", "shaderLineVBO.vert", "shaderLineVBO.frag");
    }
    catch (Exception & e) {
        cerr << "Line shader " << e.msg () << endl;
    }
}

//Creating vao and vbo and sending data to the gpu
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
        //Index buffer
        glBindBuffer(GL_ARRAY_BUFFER, lineVbo);
            linePos = bvh->getLinesPositions();
            sizeLinePos = linePos.size();
            glBufferData(GL_ARRAY_BUFFER, sizeLinePos * sizeof(GL_FLOAT), &linePos[0], GL_STATIC_DRAW);
            glVertexAttribPointer(lineIndex, 3, GL_FLOAT, GL_FALSE, 0, NULL);
            glEnableVertexAttribArray(lineIndex);

    //LOADING VAO AND VBO FOR PHOTOREALISTIC AND CARTOON SHADERS
    glBindVertexArray(vao); //Locking VAO
        //Index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); //Locking IBO
            //Video memory allocation
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->triangles().size()*sizeof(Triangle), &(mesh->triangles())[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); //Locking VBO
             //Video memory allocation
            glBufferData(GL_ARRAY_BUFFER, mesh->positions().size()*sizeof(mesh->positions()[0]), &(mesh->positions()[0]), GL_STATIC_DRAW);
            glVertexAttribPointer(positionIndex, 3, GL_FLOAT, GL_FALSE, 0, NULL);
            glEnableVertexAttribArray (positionIndex);

        glBindBuffer(GL_ARRAY_BUFFER, vbo[1]); //Locking VBO
            //Video memory allocation
            glBufferData(GL_ARRAY_BUFFER, mesh->normals().size()*sizeof(mesh->normals()[0]), &(mesh->normals()[0]), GL_STATIC_DRAW);
            glVertexAttribPointer(normalIndex, 3, GL_FLOAT, GL_FALSE, 0, NULL);
            glEnableVertexAttribArray (normalIndex) ;

        glBindBuffer(GL_ARRAY_BUFFER, vbo[2]); //Locking VBO
             //Video memory allocation
            colorResponses.resize (4*mesh->positions().size());
            glBufferData(GL_ARRAY_BUFFER, colorResponses.size()*sizeof(colorResponses[0]), &(colorResponses[0]), GL_STATIC_DRAW);
            glVertexAttribPointer(colorIndex, 4, GL_FLOAT, GL_FALSE, 0, NULL);
            glEnableVertexAttribArray (colorIndex) ;
}

//Initializing the colors sent to the gpu using the material albedo
void initiliazeColor() {

    unsigned int l = 0;

    vector<Vec3f> positions = mesh->positions();
    vector<Triangle> triangles = mesh->triangles();

    unsigned int nbVertex = positions.size();

    //All vertices receive the albedo value
    while(l < 4*nbVertex)
    {

        colorResponses[l++] = matAlbedo[0];
        colorResponses[l++] = matAlbedo[1];
        colorResponses[l++] = matAlbedo[2];
        colorResponses[l++] = 1.0;
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]); //Locking VBO
        //Video memory allocation
        glBufferSubData(GL_ARRAY_BUFFER, 0, colorResponses.size()*sizeof(colorResponses[0]), &(colorResponses[0]));
}

//Called to toggle the use of the toonshader
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

//Called to compute the shadows
void computePerVertexShadow () {

    LightRay ray;
    bool intersects;

    Vec3f lightPos = lightSources[0].getPos();
    auto positions = mesh->positions();
    auto triangles = mesh->triangles();
    unsigned int nbVertex = positions.size();

    unsigned int l = 0;
    for(unsigned int i = 0; i < nbVertex; i++) {

        //Vertex receives albedo value
        colorResponses[l++] = matAlbedo[0];
        colorResponses[l++] = matAlbedo[1];
        colorResponses[l++] = matAlbedo[2];

        //Ray emerging from the vertex in direction of the lightsource
        ray = LightRay(positions.at(i), normalize(lightPos-positions.at(i)));
        intersects = ray.intersectsBVH(bvh, FLT_MAX);

        //If intersection occurs we change the sign
        if(intersects) colorResponses[l++] = -1.0;
        else colorResponses[l++] = 1.0;

        advanceBar(float(i)/nbVertex*100);
    }
    advanceBar(100);


    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]); //Locking VBO
        //Video memory allocation
        glBufferSubData(GL_ARRAY_BUFFER, 0, colorResponses.size()*sizeof(colorResponses[0]), &(colorResponses[0]));
}

//Computing the AO coefficients. For better visibility, toggle toon mode with '0'.
void computePerVertexAO (unsigned int numOfSamples, float radius) {

    random_device rd;
    default_random_engine generator(rd());
    uniform_real_distribution<float> range(-1.f,1.f);

    vector<Vec3f> positions = mesh->positions();
    vector<Vec3f> normals = mesh->normals();
    vector<Triangle> triangles = mesh->triangles();

    unsigned int nbVertex = positions.size();

    LightRay lightRay;

    float  sum = 0.0;

    Triangle currTri;
    Vec3f currPos;
    Vec3f n;
    Vec3f u, v;

    for(unsigned int j = 0; j < nbVertex; j++) {

        currPos = positions.at(j);
        n = normalize(normals.at(j));
        sum = 0.0;

        for(int i = numOfSamples; i--;) {

            //Creating the random ray
            n.getTwoOrthogonals(u, v);
            float n1 = range(generator);
            float n2 = range(generator);
            Vec3f direction = n + n1*u + n2*v;

            direction.normalize();
            lightRay = LightRay(currPos, direction);

            //If no intersection we receive more light from this direction
            if(!lightRay.intersectsBVH(bvh, radius)) sum += dot(n, direction);
        }

        sum *= 1.0f/numOfSamples;
        colorResponses[4*j+3] *= sum;
        advanceBar(float(j)/nbVertex*100);
    }
    advanceBar(100);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]); //Locking VBO
        //Video memory allocation
        glBufferSubData(GL_ARRAY_BUFFER, 0, colorResponses.size()*sizeof(colorResponses[0]), &(colorResponses[0]));
}

//Called to load the appropriate shaders and render the scene
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

//Reshaping camera
void reshape(int w, int h) {
    camera.resize (w, h);
}

//Main graphic loop
void display () {
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera.apply ();

    render();
    for(GLenum err; (err = glGetError()) != GL_NO_ERROR;)
    {
      cerr << err << endl;
    }

    glFlush ();
    glutSwapBuffers ();
}

//Reacting according to the user's input, please take a look at "printUsage" for more easily readable description. Very ugly.
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
            toonProgram->setUniform3f(lightPosToonShader, lightSources[0].getPos()[0], lightSources[0].getPos()[1], lightSources[0].getPos()[2]);
            break;
		case 's':
			for(unsigned int j=0; j<lightSources.size(); j++)
            {

                lightSources[j].addT(-0.1);
            }
            photoRealistProgram->setUniform3f(lightPosShader, lightSources[0].getPos()[0], lightSources[0].getPos()[1], lightSources[0].getPos()[2]);
            toonProgram->setUniform3f(lightPosToonShader, lightSources[0].getPos()[0], lightSources[0].getPos()[1], lightSources[0].getPos()[2]);
			break;
		case 'q':
			for(unsigned int j=0; j<lightSources.size(); j++)
            {

                lightSources[j].addP(-0.1);
            }
            photoRealistProgram->setUniform3f(lightPosShader, lightSources[0].getPos()[0], lightSources[0].getPos()[1], lightSources[0].getPos()[2]);
            toonProgram->setUniform3f(lightPosToonShader, lightSources[0].getPos()[0], lightSources[0].getPos()[1], lightSources[0].getPos()[2]);
			break;
		case 'd':
			for(unsigned int j=0; j<lightSources.size(); j++)
            {

                lightSources[j].addP(0.1);
            }
            photoRealistProgram->setUniform3f(lightPosShader, lightSources[0].getPos()[0], lightSources[0].getPos()[1], lightSources[0].getPos()[2]);
            toonProgram->setUniform3f(lightPosToonShader, lightSources[0].getPos()[0], lightSources[0].getPos()[1], lightSources[0].getPos()[2]);
			break;
		case 'a':
			for(unsigned int j=0; j<lightSources.size(); j++)
            {

                lightSources[j].addR(0.1);
            }
            photoRealistProgram->setUniform3f(lightPosShader, lightSources[0].getPos()[0], lightSources[0].getPos()[1], lightSources[0].getPos()[2]);
            toonProgram->setUniform3f(lightPosToonShader, lightSources[0].getPos()[0], lightSources[0].getPos()[1], lightSources[0].getPos()[2]);
			break;
		case 'e':
			for(unsigned int j=0; j<lightSources.size(); j++)
            {

                lightSources[j].addR(-0.1);
            }
            photoRealistProgram->setUniform3f(lightPosShader, lightSources[0].getPos()[0], lightSources[0].getPos()[1], lightSources[0].getPos()[2]);
            toonProgram->setUniform3f(lightPosToonShader, lightSources[0].getPos()[0], lightSources[0].getPos()[1], lightSources[0].getPos()[2]);
			break;
        case 't':
            computePerVertexShadow();
            break;
        case 'r':
            initiliazeColor();
            break;
        case 'g':
            computePerVertexAO(128, 0.2f);
            break;
        case 'y':
            alpha = fmin(1.f, alpha + 0.05);
            photoRealistProgram->setUniform1f(alphaShader, alpha);
            break;
        case 'h':
            alpha = fmax(0.f, alpha - 0.05);
            photoRealistProgram->setUniform1f(alphaShader, alpha);
            break;
        case 'i':
            shininess += 0.1;
            photoRealistProgram->setUniform1f(shininessShader, shininess);
            break;
        case 'k':
            shininess = fmax(0, shininess-0.1);
            photoRealistProgram->setUniform1f(shininessShader, shininess);
            break;
        case 'n':
            drawBVH = !drawBVH;
            break;
        case 'v':
            mesh->topologicaLaplacianFilter();
            colorResponses.resize (4*mesh->positions().size());
            initiliazeColor();
            glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); // Verrouillage du VBO
            // Allocation de la mémoire vidéo
                glBufferData(GL_ARRAY_BUFFER, mesh->positions().size()*sizeof(mesh->positions()[0]), &(mesh->positions()[0]), GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, vbo[1]); // Verrouillage du VBO
                // Remplacement du contenu en VRAM
                glBufferSubData(GL_ARRAY_BUFFER, 0, mesh->normals().size()*sizeof(mesh->normals()[0]), &(mesh->normals()[0]));
            break;
        case 'x' :
            loadMesh(modelFileName);
            loadVbo();
            initiliazeColor();
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
            mesh->simplify(64);
            loadVbo();
            initiliazeColor();
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
            if(toonShader) toggleToonShader();
            mode = 1;
            photoRealistProgram->setUniform1i(modeShader, mode);
            break;
        case '2':
            if(toonShader) toggleToonShader();
            mode = 2;
            photoRealistProgram->setUniform1i(modeShader, mode);
            break;
        case '3':
            if(toonShader) toggleToonShader();
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

//Handling mouse events
void mouse (int button, int state, int x, int y) {

    camera.handleMouseClickEvent (button, state, x, y);
}

//Handling mouse events
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
