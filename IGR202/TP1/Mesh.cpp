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

#include "Mesh.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>

using namespace std;

void Mesh::clear () {
    m_positions.clear ();
    m_normals.clear ();
    m_triangles.clear ();
}

void Mesh::loadOFF (const std::string & filename) {
    clear ();
	ifstream in (filename.c_str ());
    if (!in) 
        exit (1);
	string offString;
    unsigned int sizeV, sizeT, tmp;
    in >> offString >> sizeV >> sizeT >> tmp;
    m_positions.resize (sizeV);
    m_triangles.resize (sizeT);
    for (unsigned int i = 0; i < sizeV; i++)
        in >> m_positions[i];
    int s;
    for (unsigned int i = 0; i < sizeT; i++) {
        in >> s;
        for (unsigned int j = 0; j < 3; j++) 
            in >> m_triangles[i][j];
    }
    in.close ();
    centerAndScaleToUnit ();
    recomputeNormals ();
    //addBottomPlane(32, 1);
}

void Mesh::recomputeNormals () {
    m_normals.clear ();
    m_normals.resize (m_positions.size (), Vec3f (0.f, 0.f, 0.f));
    for (unsigned int i = 0; i < m_triangles.size (); i++) {
        Vec3f e01 = m_positions[m_triangles[i][1]] -  m_positions[m_triangles[i][0]];
        Vec3f e02 = m_positions[m_triangles[i][2]] -  m_positions[m_triangles[i][0]];
        Vec3f n = cross (e01, e02);
        n.normalize ();
        for (unsigned int j = 0; j < 3; j++)
            m_normals[m_triangles[i][j]] += n;
    }
    for (unsigned int i = 0; i < m_normals.size (); i++)
        m_normals[i].normalize ();
}

void Mesh::centerAndScaleToUnit () {
    Vec3f c;
    for  (unsigned int i = 0; i < m_positions.size (); i++)
        c += m_positions[i];
    c /= m_positions.size ();
    float maxD = dist (m_positions[0], c);
    for (unsigned int i = 0; i < m_positions.size (); i++){
        float m = dist (m_positions[i], c);
        if (m > maxD)
            maxD = m;
    }
    for  (unsigned int i = 0; i < m_positions.size (); i++)
        m_positions[i] = (m_positions[i] - c) / maxD;
}

void Mesh::addBottomPlane(unsigned int nbVertex, float width) {

    float pas = 2*width/((float)nbVertex);

    unsigned int prevVertexSize = m_positions.size();
    unsigned int prevTriangleSize = m_triangles.size();

    std::cerr << "triangle size :" << prevTriangleSize << " " << prevTriangleSize + 2*(nbVertex-1)*(nbVertex-1) << std::endl;
    std::cerr << "vertex size :" << prevVertexSize << " " << prevVertexSize + nbVertex*nbVertex <<  std::endl;

    m_positions.resize(prevVertexSize + nbVertex*nbVertex);
    m_normals.resize(prevVertexSize + nbVertex*nbVertex);
    m_triangles.resize(prevTriangleSize + 2*(nbVertex-1)*(nbVertex-1));

    for(unsigned int i = 0; i < nbVertex; i++){

        for(unsigned int j = 0; j < nbVertex; j++){

            m_positions[prevVertexSize + i*nbVertex + j] = Vec3f(-width + i*pas, -1.0, -width + j*pas);
            m_normals[prevVertexSize + i*nbVertex + j] = normalize(Vec3f(0.0, 1.0, 0.0));
        }
    }

    unsigned int l = 0;

    for(unsigned int i = 0; i < nbVertex - 1; i++) {

        for(unsigned int j = 0; j < nbVertex - 1; j++) {

            m_triangles[prevTriangleSize + l++] = Triangle(prevVertexSize + j + nbVertex*i, prevVertexSize + j + 1 + nbVertex*i, prevVertexSize + nbVertex*(i+1) + j + 1);
            m_triangles[prevTriangleSize + l++] = Triangle(prevVertexSize + j + nbVertex*i, prevVertexSize + j + 1 + nbVertex*(i+1), prevVertexSize + nbVertex*(i+1) + j);
        }   
    }        
}
