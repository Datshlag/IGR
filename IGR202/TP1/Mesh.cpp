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
#include <cmath>
#include <cfloat>

void Mesh::clear () {
    m_positions.clear ();
    m_normals.clear ();
    m_triangles.clear ();
}

void Mesh::loadOFF (const string & filename) {
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
    //addBottomPlane(128, 1);
}

void Mesh::recomputeNormals () {
    m_normals.clear ();
    m_normals.resize (m_positions.size (), Vec3f (0.f, 0.f, 0.f));

    for (unsigned int i = 0; i < m_triangles.size (); i++) {

        Vec3f e01 = m_positions[m_triangles[i][1]] -  m_positions[m_triangles[i][0]];
        Vec3f e02 = m_positions[m_triangles[i][2]] -  m_positions[m_triangles[i][0]];
        Vec3f e12 = m_positions[m_triangles[i][2]] -  m_positions[m_triangles[i][1]];

        Vec3f n = cross (e01, e02);
        float a, b, c;
        a = asin(n.length());
        b = asin(cross(e01, e12).length());
        c = asin(cross(e02, e12).length());

        n.normalize ();
        //On pondère avec l'angle de l'arête incidente
        m_normals[m_triangles[i][0]] += n * a;
        m_normals[m_triangles[i][1]] += n * b;
        m_normals[m_triangles[i][2]] += n * c;
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

    float pas = 2 * width / ((float) nbVertex);

    unsigned int prevVertexSize = m_positions.size();
    unsigned int prevTriangleSize = m_triangles.size();

    cout << "triangle size :" << prevTriangleSize << " " << prevTriangleSize + 2*(nbVertex-1)*(nbVertex-1) << endl;
    cout << "vertex size :" << prevVertexSize << " " << prevVertexSize + nbVertex*nbVertex <<  endl;

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

void Mesh::laplacianFilter(){

    vector<vector<int> > neighbours (m_positions.size(), vector<int>());
    vector<Vec3f> new_positions (m_positions.size(), Vec3f(0, 0, 0));

    for(unsigned int j = 0; j < m_triangles.size(); j++) {

        auto curr_tri = m_triangles.at(j);

        int v0 = curr_tri[0];
        int v1 = curr_tri[1];
        int v2 = curr_tri[2];

        neighbours[v0].push_back(v1);
        neighbours[v0].push_back(v2);

        neighbours[v1].push_back(v0);
        neighbours[v1].push_back(v2);

        neighbours[v2].push_back(v0);
        neighbours[v2].push_back(v1);
    }

    for(unsigned int k = 10; k--;) {

        for(unsigned int i = 0; i < m_positions.size(); i++) {
    
            vector<int> curr_neighbours = neighbours[i];
            Vec3f neighbors_bar;
    
            for(unsigned int l = 0; l < curr_neighbours.size(); l++) {
    
                neighbors_bar += m_positions.at(curr_neighbours[l]);
            }
            neighbors_bar *= 1.0f/curr_neighbours.size();
            new_positions[i] = neighbors_bar;
        }
    }

    m_positions = new_positions;
    recomputeNormals();
}

void Mesh::geometricalLaplacian() {

    vector<vector<Triangle> > neighbours (m_positions.size(), vector<Triangle>());
    vector<Vec3f> new_positions = m_positions;

    for(unsigned int j = 0; j < m_triangles.size(); j++) {

        auto curr_tri = m_triangles.at(j);

        int v0 = curr_tri[0];
        int v1 = curr_tri[1];
        int v2 = curr_tri[2];

        neighbours[v0].push_back(curr_tri);
        neighbours[v1].push_back(curr_tri);
        neighbours[v2].push_back(curr_tri);
    }

    for(unsigned int i = 0; i < m_positions.size(); i++) {

        auto curr_neighbours = neighbours[i];
        Vec3f curr_pos = m_positions[i];

        Vec3f laplacian = Vec3f(0, 0, 0);
        float v = 0.0;

        for(unsigned int j = 0; j < curr_neighbours.size(); j++) {

            Triangle curr_tri = curr_neighbours[j];
            int pos;

            if(curr_tri[0] == i) pos = 0;
            if(curr_tri[1] == i) pos = 1;
            if(curr_tri[2] == i) pos = 2;

            Vec3f e01 = normalize(m_positions[curr_tri[(pos + 1)%3]] - m_positions[curr_tri[pos]]);
            Vec3f e02 = normalize(m_positions[curr_tri[(pos + 2)%3]] - m_positions[curr_tri[pos]]);
            Vec3f e12 = normalize(m_positions[curr_tri[(pos + 2)%3]] - m_positions[curr_tri[(pos + 1)%3]]);

            Vec3f e0 = normalize(m_positions[curr_tri[pos]]);
            Vec3f e1 = normalize(m_positions[curr_tri[(pos + 1)%3]]);
            Vec3f e2 = normalize(m_positions[curr_tri[(pos + 2)%3]]);

            float alpha01 =  dot(-e01, e12) / cross(e01, e12).length() ;
            float alpha02 =  dot(-e02, -e12) / cross(e02, e12).length() ;

            laplacian += m_positions[curr_tri[(pos + 1)%3]] * alpha02 / 2.f;
            laplacian += m_positions[curr_tri[(pos + 2)%3]] * alpha01 / 2.f;            
            v += alpha01;
            v += alpha02;
        }

        new_positions[i] = laplacian * 2.f/v;
    }

    m_positions = new_positions;
    recomputeNormals();
}

void Mesh::simplify(unsigned int resolution) {

    float minX, minY, minZ;
    minX = minY = minZ = FLT_MAX;
    float maxX, maxY, maxZ;
    maxX = maxY = maxZ = FLT_MIN;

    float posX, posY, posZ;
    Vec3f currPos;

    //Calcul d'un cube englobant
    for(unsigned int i = 0; i < m_positions.size(); i++) {

        currPos = m_positions.at(i);
        posX = currPos[0];
        posY = currPos[1];
        posZ = currPos[2];

        if(posX < minX) minX = posX;
        if(posY < minY) minY = posY;
        if(posZ < minZ) minZ = posZ;
        if(posX > maxX) maxX = posX;
        if(posY > maxY) maxY = posY;
        if(posZ > maxZ) maxZ = posZ;
    }

    //Making the box a little larger
    maxX *= 1.1;
    maxY *= 1.1;
    maxZ *= 1.1;
    minX *= 1.1;
    minY *= 1.1;
    minZ *= 1.1;

    //grille 3d
    vector<vector<vector<Vec3f> > > grille_3D(resolution, vector<vector<Vec3f> >(resolution, vector<Vec3f> (resolution, Vec3f(0, 0, 0) ) ) );
    vector<vector<vector<int> > > grille_3D_count(resolution, vector<vector<int> >(resolution, vector<int> (resolution, 0) ) );

    //Calcul des représentants
    int caseX, caseY, caseZ;
    for(unsigned int i = 0; i < m_positions.size(); i++) {

        currPos = m_positions.at(i);
        posX = currPos[0];
        posY = currPos[1];
        posZ = currPos[2];

        caseX = floor((posX - minX) * (resolution - 1) / (maxX - minX));
        caseY = floor((posY - minY) * (resolution - 1) / (maxY - minY));
        caseZ = floor((posZ - minZ) * (resolution - 1) / (maxZ - minZ));

        grille_3D[caseX][caseY][caseZ] += currPos;
        grille_3D_count[caseX][caseY][caseZ]++;

    }

    vector<Vec3f> new_positions = vector<Vec3f>();
    for(unsigned int i = 0; i < resolution; i++) {

        for(unsigned int j = 0; j < resolution; j++) {

            for(unsigned int k = 0; k < resolution; k++) {

                if(grille_3D_count[i][j][k] > 0) {

                    grille_3D[i][j][k] *= 1.0f/grille_3D_count[i][j][k];
                    new_positions.push_back(grille_3D[i][j][k]);
                }
                else new_positions.push_back(Vec3f(0.0, 0.0, 0.0));
            }
        }
    }

    //Nouveaux triangles
    Triangle currTriangle;
    int caseX0, caseX1, caseX2;
    int caseY0, caseY1, caseY2;
    int caseZ0, caseZ1, caseZ2;
    int case0, case1, case2;
    vector<Triangle> new_triangles = vector<Triangle>();

    for(unsigned int i = 0; i < m_triangles.size(); i++) {

        currTriangle = m_triangles.at(i);

        currPos = m_positions[currTriangle[0]];
        posX = currPos[0];
        posY = currPos[1];
        posZ = currPos[2];

        caseX0 = floor((posX - minX) * (resolution - 1)/(maxX - minX));
        caseY0 = floor((posY - minY) * (resolution - 1)/(maxY - minY));
        caseZ0 = floor((posZ - minZ) * (resolution - 1)/(maxZ - minZ));

        currPos = m_positions[currTriangle[1]];
        posX = currPos[0];
        posY = currPos[1];
        posZ = currPos[2];

        caseX1 = floor((posX - minX) * (resolution - 1)/(maxX - minX));
        caseY1 = floor((posY - minY) * (resolution - 1)/(maxY - minY));
        caseZ1 = floor((posZ - minZ) * (resolution - 1)/(maxZ - minZ));

        currPos = m_positions[currTriangle[2]];
        posX = currPos[0];
        posY = currPos[1];
        posZ = currPos[2];

        caseX2 = floor((posX - minX) * (resolution - 1)/(maxX - minX));
        caseY2 = floor((posY - minY) * (resolution - 1)/(maxY - minY));
        caseZ2 = floor((posZ - minZ) * (resolution - 1)/(maxZ - minZ));

        case0 = resolution * resolution * caseX0 + resolution * caseY0 + caseZ0;
        case1 = resolution * resolution * caseX1 + resolution * caseY1 + caseZ1;
        case2 = resolution * resolution * caseX2 + resolution * caseY2 + caseZ2;

        if(case0 != case1 || case1 != case2) {

            new_triangles.push_back(Triangle(case0, case1, case2));     
        }

    }

    m_positions = new_positions;
    m_triangles = new_triangles;
    recomputeNormals();

    cout << " vertices : " << m_positions.size()
    << " triangles : " << m_triangles.size() << endl;

    for(unsigned int i=0; i < m_triangles.size(); i++) {

        cout << m_triangles[i][0] << " "  << m_triangles[i][1] << " " << m_triangles[i][2] << endl;
        cout << m_positions[m_triangles[i][0]] << endl << m_positions[m_triangles[i][1]] << endl << m_positions[m_triangles[i][2]] << endl << endl;
    }

}
