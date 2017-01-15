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
#define USING_VBO true

#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <cmath>

#include "Vec3.h"
#include "Camera.h"
#include "Mesh.h"
#include "GLProgram.h"
#include "Exception.h"
#include "LightSource.h"
#include "LightRay.h"
#include "BVH.h"
#include <time.h>

using namespace std;

static const unsigned int DEFAULT_SCREENWIDTH = 1024;
static const unsigned int DEFAULT_SCREENHEIGHT = 768;
static const string DEFAULT_MESH_FILE ("models/man.off");

static const string appTitle ("Informatique Graphique & Realite Virtuelle - Travaux Pratiques - Algorithmes de Rendu");
static const string myName ("Aloïs Pourchot");
static GLint window;
static unsigned int FPS = 0;
static bool fullScreen = false;

static Camera camera;
static Mesh mesh;
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

unsigned int positionIndex;
unsigned int normalIndex;
unsigned int colorIndex;

GLProgram * glProgram;

void loadVbo();
void renderVbo();
void initiliazeColor();
const inline void buildRotMatrix(const Vec3f & dir, const float  & alpha, float matrix[3][3]);

static std::vector<float> colorResponses; // Cached per-vertex color response, updated at each frame

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

    if(!USING_VBO) {

        glEnableClientState (GL_VERTEX_ARRAY);
        glEnableClientState (GL_NORMAL_ARRAY);
        glEnableClientState (GL_COLOR_ARRAY);
    }

  	mesh.loadOFF (modelFilename);
    bvh = new BVH(mesh);//Build BVH tree with the mesh

    std::cerr << "nb of nodes : " << bvh->getNbNodes() << std::endl;
    std::cerr << "nb of leaves : " << bvh->getNbLeaves() << std::endl;

    colorResponses.resize (4*mesh.positions().size());
    camera.resize (DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT);
    try {

        if(USING_VBO) glProgram = GLProgram::genVFProgram ("Simple GL Program", "shaderVBO.vert", "shaderVBO.frag"); // Load and compile pair of shaders
        else glProgram = GLProgram::genVFProgram ("Simple GL Program", "shader.vert", "shader.frag");
        glProgram->use (); // Activate the shader program
        std::cerr << glProgram->infoLog();

    } catch (Exception & e) {
        cerr << e.msg () << endl;
    }

    if(USING_VBO) loadVbo();

    initiliazeColor();

    lightSources = std::vector<LightSource>();
    lightSources.push_back(LightSource(Vec3<float>(4,4,1),Vec3<float>(5.0,0.0,0.0)));
    //lightSources.push_back(LightSource(Vec3<float>(3,0,0),Vec3<float>(0.0,1.0,1.0)));
    //lightSources.push_back(LightSource(Vec3<float>(3,4,5),Vec3<float>(1.0,1.0,0.0)));

    modeShader = glProgram->getUniformLocation("mode");
    shininessShader = glProgram->getUniformLocation("shininess");
    alphaShader = glProgram->getUniformLocation("alpha");
    F0Shader = glProgram->getUniformLocation("F0");
    lightPosShader = glProgram->getUniformLocation("lightPos");

    glProgram->setUniform1i(modeShader, mode);
    glProgram->setUniform1f(shininessShader, shininess);
    glProgram->setUniform1f(alphaShader, alpha);
    glProgram->setUniform1f(F0Shader, F0);
    glProgram->setUniform3f(lightPosShader, lightSources[0].getPos()[0], lightSources[0].getPos()[1], lightSources[0].getPos()[2]);
}

void loadVbo() {

    positionIndex = glGetAttribLocation (glProgram->id(), "VertexPosition") ;
    normalIndex = glGetAttribLocation (glProgram->id(), "VertexNormal") ;
    colorIndex = glGetAttribLocation (glProgram->id(), "VertexColor") ;

    glGenBuffers(1, &ibo);
    glGenBuffers(3, vbo);
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao); // Verrouillage du VAO
        // Buffer d'indices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); // Verrouillage du IBO
            // Allocation de la mémoire vidéo
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.triangles().size()*sizeof(Triangle), &(mesh.triangles())[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); // Verrouillage du VBO
            // Allocation de la mémoire vidéo
            glBufferData(GL_ARRAY_BUFFER, mesh.positions().size()*sizeof(mesh.positions()[0]), &(mesh.positions()[0]), GL_STATIC_DRAW);
            glEnableVertexAttribArray (positionIndex);
            glVertexAttribPointer(positionIndex, 3, GL_FLOAT, GL_FALSE, 0, NULL);


        glBindBuffer(GL_ARRAY_BUFFER, vbo[1]); // Verrouillage du VBO
            // Allocation de la mémoire vidéo
            glBufferData(GL_ARRAY_BUFFER, mesh.normals().size()*sizeof(mesh.normals()[0]), &(mesh.normals()[0]), GL_STATIC_DRAW);
            glEnableVertexAttribArray (normalIndex) ;
            glVertexAttribPointer(normalIndex, 3, GL_FLOAT, GL_FALSE, 0, NULL);

        glBindBuffer(GL_ARRAY_BUFFER, vbo[2]); // Verrouillage du VBO
            // Allocation de la mémoire vidéo
            glBufferData(GL_ARRAY_BUFFER, colorResponses.size()*sizeof(colorResponses[0]), &(colorResponses[0]), GL_DYNAMIC_DRAW);
            glEnableVertexAttribArray (colorIndex) ;
            glVertexAttribPointer(colorIndex, 4, GL_FLOAT, GL_FALSE, 0, NULL);

}


// Calcul sur le CPU des ombres portées, implémentation très lente
void computePerVertexShadow () {

    LightRay ray;
    bool intersects;

    Vec3f lightPos = lightSources[0].getPos();
    std::vector<Vec3f> positions = mesh.positions();
    std::vector<Triangle> triangles = mesh.triangles();
    unsigned int nbVertex = positions.size();
    unsigned int nbTriangle = triangles.size();

    unsigned int l = 0;
    int i;

    float rayTime;
    int nbIntersect;
    while(l < 4*nbVertex) {

        rayTime = 0.0;
        nbIntersect = 0;

        // On donne au vertex ses valeurs d'Albedo
        colorResponses[l++] = matAlbedo[0];
        colorResponses[l++] = matAlbedo[1];
        colorResponses[l++] = matAlbedo[2];

        i=l/4;

        // Rayon partant du vertex en direction de la source de lumière
        ray = LightRay(positions[i], normalize(lightPos-positions[i]));
        intersects = false;

        //std::cerr << i << std::endl;

        // On cherche si le rayon est bloqué par un triangle
        for(unsigned int j = nbTriangle; (j--)&&(!intersects); ) {

            clock_t t1, t2;
            t1 = clock();

            Triangle currTri = triangles[j];
            intersects = ray.intersectsTriangle(positions[currTri[0]], positions[currTri[1]], positions[currTri[2]]);

            t2 = clock();
            float diff = ((float)t2-(float)t1)/CLOCKS_PER_SEC;

            rayTime += diff;

            nbIntersect++;
        }

        std::cerr << " Rayon : " << i << " tests intersections : " << nbIntersect << " total time : " << rayTime << std::endl;

        // On change le signe de la 4ème coordonnée en fonction de s'il y a eu intersection
        if(intersects) colorResponses[l++] = -1.0;
        else colorResponses[l++] = 1.0;
    }


    // Envoi des données au buffer sur le GPU
    if(USING_VBO) {

        glBindBuffer(GL_ARRAY_BUFFER, vbo[2]); // Verrouillage du VBO
            // Remplacement du contenu en VRAM
            glBufferSubData(GL_ARRAY_BUFFER, 0, colorResponses.size()*sizeof(colorResponses[0]), &(colorResponses[0]));
    }

}

void computePerVertexShadowV2 () {

    LightRay ray;
    bool intersects;

    Vec3f lightPos = lightSources[0].getPos();
    std::vector<Vec3f> positions = mesh.positions();
    std::vector<Triangle> triangles = mesh.triangles();
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
        ray = LightRay(positions[i], normalize(lightPos-positions[i]));
        intersects = ray.intersectionBVH(bvh);
        std::cerr << " Rayon : " << i << " tests intersections : " << ray.countLocal << " total time : " << ray.rayTime << std::endl;


        // On change le signe de la 4ème coordonnée en fonction de s'il y a eu intersection
        if(intersects) colorResponses[l++] = -1.0;
        else colorResponses[l++] = 1.0;
    }


    // Envoi des données au buffer sur le GPU
    if(USING_VBO) {

        glBindBuffer(GL_ARRAY_BUFFER, vbo[2]); // Verrouillage du VBO
            // Remplacement du contenu en VRAM
            glBufferSubData(GL_ARRAY_BUFFER, 0, colorResponses.size()*sizeof(colorResponses[0]), &(colorResponses[0]));
    }
}

void computePerVertexAO (unsigned int numOfSamples, float radius, float Vamb) {

    srand((unsigned)time(NULL));

    std::vector<Vec3f> positions = mesh.positions();
    std::vector<Vec3f> normals = mesh.normals();
    std::vector<Triangle> triangles = mesh.triangles();
    unsigned int nbVertex = positions.size();
    unsigned int nbTriangle = triangles.size();

    LightRay lightRays[numOfSamples];
    bool intersects[numOfSamples];

    float  d, sum;

    Triangle currTri;
    Vec3f currPos;
    Vec3f n;
    Vec3f randVec;

    float M[3][3];

    unsigned int l = nbVertex;
    while(l != 0) {

        currPos = positions[l];
        n = normalize(normals[l]);

        // Calcul de la distribution initiale des rayons émis
        for(int i = numOfSamples; i--;) {

            lightRays[i].setOrigin(currPos);
            randVec = Vec3f(1, ((float)rand() *  M_PI/2) / (float)RAND_MAX, ((float)rand() *  2 * M_PI) / (float)RAND_MAX);
            randVec = polarToCartesian(randVec);

            buildRotMatrix(normalize(cross(Vec3f(0,0,1), n)), cartesianToPolar(n)[1], M);
            randVec = M * randVec;

            lightRays[i].setDirection(randVec);

            intersects[i] = false;
        }

        // Recherche des directions bloquantes
        for(unsigned int i = nbTriangle; i--;) {

            currTri = triangles[i];
            d =  length(currPos - (float) 1/3 * (positions[currTri[0]] + positions[currTri[1]] + positions[currTri[2]]));

            // Si le triangle est trop loin pas la peine de continuer
            if(d < radius) {

                for(int i = numOfSamples; i--;) {

                    // Si le rayon a déjà rencontré un triangle pas la peine de continuer
                    if(!intersects[i]) {

                        intersects[i] = lightRays[i].intersectsTriangle(positions[currTri[0]], positions[currTri[1]], positions[currTri[2]]);
                    }
                }
            }
        }

        // Calcul du coefficient d'AO
        sum = 0.0;
        for(int i = numOfSamples; i--;) {

            sum += intersects[i] * dot(n, lightRays[i].getDirection());
        }
        sum *= 1.0/numOfSamples;
        sum = 1.0 - sum;
        colorResponses[4*l+3] *= sum;
        //std::cerr << " vertex : " << l << " reponse : " << colorResponses[4*l+3] << std::endl;

        l--;
    }

    // Envoi des données au buffer sur le GPU
    if(USING_VBO){

        glBindBuffer(GL_ARRAY_BUFFER, vbo[2]); // Verrouillage du VBO
            // Remplacement du contenu en VRAM
            glBufferSubData(GL_ARRAY_BUFFER, 0, colorResponses.size()*sizeof(colorResponses[0]), &(colorResponses[0]));
    }

}

void initiliazeColor() {

    unsigned int l = 0;

    std::vector<Vec3f> positions = mesh.positions();
    std::vector<Triangle> triangles = mesh.triangles();

    unsigned int nbVertex = positions.size();

    // On met tous les vertex à la même valeur d'Albedo
    while(l < 4*nbVertex)
    {

        colorResponses[l++] = matAlbedo[0];
        colorResponses[l++] = matAlbedo[1];
        colorResponses[l++] = matAlbedo[2];
        colorResponses[l++] = 1.0;
    }

    if(USING_VBO){

        glBindBuffer(GL_ARRAY_BUFFER, vbo[2]); // Verrouillage du VBO
            // Remplacement du contenu en VRAM
            glBufferSubData(GL_ARRAY_BUFFER, 0, colorResponses.size()*sizeof(colorResponses[0]), &(colorResponses[0]));
    }

}

/*
// EXERCISE : the following color response shall be replaced with a proper reflectance evaluation/shadow test/etc.
void updatePerVertexColorResponse () {


    //BRDF using Lambert's model for the diffusion term and Blinn-Phong's model for the specular term

    if(mode==1)
    {
        for (unsigned int i = 0; i < colorResponses.size (); i++)
        {
            colorResponses[i] = Vec3f(0,0,0);

            for(unsigned int j=0; j < lightSources.size(); j++)
            {

                LightSource lightSource = lightSources[j];

                float d = dist(mesh.positions()[i],lightSource.getPos());
                float attenuation = 1/(1+d+d*d);

                Vec3f omegaI = normalize(lightSource.getPos() - mesh.positions()[i]);
                float scProduct = fmax(0,dot(omegaI, mesh.normals()[i]));
                Vec3f diffusion = attenuation*lightSource.getColor()*scProduct;

                float x,y,z;
                camera.getPos(x,y,z);
                Vec3f omega0 = normalize(Vec3f(x,y,z) - mesh.positions()[i]);
                Vec3f omegaH = normalize(omega0 + omegaI);
                float nDotOmegaH = fmax(0, dot(omegaH, mesh.normals()[i]));
                float phong = pow(nDotOmegaH,shininess);
                Vec3f brillance = attenuation*lightSource.getColor()*phong*scProduct;

                colorResponses[i] += brillance + diffusion;
            }
    	}
    }

    else
    {

        for(unsigned int i = 0; i <colorResponses.size(); i++)
        {

            colorResponses[i] = Vec3f(0,0,0);

            for(unsigned int j=0; j<lightSources.size(); j++)
            {

                LightSource lightSource = lightSources[j];

                float x,y,z;
                camera.getPos(x,y,z);

                float d = dist(mesh.positions()[i],lightSource.getPos());
                float attenuation = 1/(1+d+d*d);

                Vec3f omegaI = normalize(lightSource.getPos() - mesh.positions()[i]);
                Vec3f omega0 = normalize((Vec3f(x,y,z) - mesh.positions()[i]));
                Vec3f omegaH = normalize((omega0 + omegaI));
                Vec3f n = mesh.normals()[i];

                float nDotOmega0 = fmax(0,dot(n, omega0));
                float nDotOmegaI = dot(n, omegaI);
                float nDotOmegaH = dot(n, omegaH);
                float omega0DotOmegaH = dot(omega0, omegaH);
                float omegaIDotOmegaH = dot(omegaH,omegaI);

                float F,D,G;

                if(mode == 2) D = std::exp((pow(nDotOmegaH,2)-1)/(pow(alpha*nDotOmegaH,2)))/(M_PI*pow(alpha,2)*pow(nDotOmegaH,4));
                else if(mode == 3) D = pow(alpha,2)/(M_PI*pow(1+(pow(alpha,2)-1)*pow(nDotOmegaH,2),2));
                F = F0 + (1-F0)*pow(1-fmax(0,omegaIDotOmegaH),5);

                if(mode == 2) G = fmin(1,fmin(2*nDotOmegaH*nDotOmegaI/omega0DotOmegaH,2*nDotOmega0*nDotOmegaH/omega0DotOmegaH));
                else if(mode == 3) G = 2*nDotOmega0/(nDotOmega0 + pow(alpha*alpha+(1-alpha*alpha)*pow(nDotOmega0,2),1/2)) * 2*nDotOmegaI/(nDotOmegaI + pow(alpha*alpha+(1-alpha*alpha)*pow(nDotOmegaI,2),1/2));

                float scProduct = fmax(0,dot(omegaI, mesh.normals()[i]));
                Vec3f diffusion = attenuation*lightSource.getColor()*scProduct;

                Vec3f brillance = attenuation*lightSource.getColor()*D*F*G/(4*nDotOmegaI*nDotOmega0)*scProduct;

                colorResponses[i] += brillance + diffusion;
            }

        }
    }
}*/

int test = 0;
void renderScene () {

    glVertexPointer (3, GL_FLOAT, sizeof (Vec3f), (GLvoid*)(&(mesh.positions()[0])));
    glNormalPointer (GL_FLOAT, 3*sizeof (float), (GLvoid*)(&(mesh.normals()[0])));
    glColorPointer (4, GL_FLOAT, 4*sizeof(float), (GLvoid*)(&(colorResponses[0])));
    glDrawElements (GL_TRIANGLES, 3*mesh.triangles().size(), GL_UNSIGNED_INT, (GLvoid*)(&(mesh.triangles()[0])));
}

void renderVbo() {

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 3*mesh.triangles().size(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));
}

void reshape(int w, int h) {
    camera.resize (w, h);
}

void display () {
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera.apply ();
    if(!USING_VBO) renderScene ();
    else renderVbo();
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
            glProgram->setUniform3f(lightPosShader, lightSources[0].getPos()[0], lightSources[0].getPos()[1], lightSources[0].getPos()[2]);
			break;
		case 's':
			for(unsigned int j=0; j<lightSources.size(); j++)
            {

                lightSources[j].addT(-0.1);
            }
            glProgram->setUniform3f(lightPosShader, lightSources[0].getPos()[0], lightSources[0].getPos()[1], lightSources[0].getPos()[2]);
			break;
		case 'q':
			for(unsigned int j=0; j<lightSources.size(); j++)
            {

                lightSources[j].addP(-0.1);
            }
            glProgram->setUniform3f(lightPosShader, lightSources[0].getPos()[0], lightSources[0].getPos()[1], lightSources[0].getPos()[2]);
			break;
		case 'd':
			for(unsigned int j=0; j<lightSources.size(); j++)
            {

                lightSources[j].addP(0.1);
            }
            glProgram->setUniform3f(lightPosShader, lightSources[0].getPos()[0], lightSources[0].getPos()[1], lightSources[0].getPos()[2]);
			break;
		case 'a':
			for(unsigned int j=0; j<lightSources.size(); j++)
            {

                lightSources[j].addR(0.1);
            }
            glProgram->setUniform3f(lightPosShader, lightSources[0].getPos()[0], lightSources[0].getPos()[1], lightSources[0].getPos()[2]);
			break;
		case 'e':
			for(unsigned int j=0; j<lightSources.size(); j++)
            {

                lightSources[j].addR(-0.1);
            }
            glProgram->setUniform3f(lightPosShader, lightSources[0].getPos()[0], lightSources[0].getPos()[1], lightSources[0].getPos()[2]);
			break;
        case 't':
            /*shininess += 0.1;
            glProgram->setUniform1f(shininessShader, shininess);*/
            {
                clock_t t1, t2;
                t1 = clock();
                computePerVertexShadow();
                t2 = clock();
                float diff = ((float)t2-(float)t1)/CLOCKS_PER_SEC;
                std::cerr << diff <<endl;
            }
            break;
        case 'r':
            initiliazeColor();
            break;
        case 'g':
            /*shininess = fmax(0, shininess-0.1);
            glProgram->setUniform1f(shininessShader, shininess);*/
            {
                clock_t t1, t2;
                t1 = clock();
                computePerVertexShadowV2();
                t2 = clock();
                float diff = ((float)t2-(float)t1)/CLOCKS_PER_SEC;
                std::cerr << diff <<endl;
            }
            //computePerVertexAO (32, 0.5, 0.1);
            break;
        case 'y':
            alpha = fmin(1,alpha + 0.05);
            glProgram->setUniform1f(alphaShader, alpha);
            break;
        case 'h':
            bvh->drawBVH(mesh, colorResponses);
            if(USING_VBO){

                glBindBuffer(GL_ARRAY_BUFFER, vbo[2]); // Verrouillage du VBO
                    // Remplacement du contenu en VRAM
                    glBufferSubData(GL_ARRAY_BUFFER, 0, colorResponses.size()*sizeof(colorResponses[0]), &(colorResponses[0]));
            }
            /*alpha = fmax(0, alpha-0.05);
            glProgram->setUniform1f(alphaShader, alpha);*/
            break;
        case 'u':
            F0 += 0.1;
            glProgram->setUniform1f(F0Shader, F0);
            break;
        case 'j':
            F0 = fmax(0, F0-0.1);
            glProgram->setUniform1f(F0Shader, F0);
            break;
        case '1':
            mode = 1;
            glProgram->setUniform1i(modeShader, mode);
            break;
        case '2':
            mode = 2;
            glProgram->setUniform1i(modeShader, mode);
            break;
        case '3':
            mode = 3;
            glProgram->setUniform1i(modeShader, mode);
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
        unsigned int numOfTriangles = mesh.triangles ().size ();
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

inline const void buildRotMatrix(const Vec3f & dir, const float & alpha, float matrix[3][3]) {

    float cos = std::cos(alpha);
    float sin = std::sin(alpha);

    matrix[0][0] = dir[0] * dir[0] * (1 - cos) + cos;
    matrix[0][1] = dir[0] * dir[1] *(1 - cos) - dir[2] * sin;
    matrix[0][2] = dir[0] * dir[2] * (1 - cos) + dir[1] * sin;

    matrix[1][0] = dir[0] * dir[1] * (1 - cos) + dir[2] * sin;
    matrix[1][1] = dir[1] * dir[1] * (1 - cos) + cos;
    matrix[1][2] = dir[1] * dir[2] * (1 - cos) - dir[0] * sin;

    matrix[2][0] = dir[0] * dir[2] * (1 - cos) - dir[1] * sin;
    matrix[2][1] = dir[1] * dir[2] * (1 - cos) + dir[0] * sin;
    matrix[2][2] = dir[2] * dir[2] * (1 - cos) + cos;
}
