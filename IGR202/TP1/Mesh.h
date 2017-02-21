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
#pragma once

#include <cmath>
#include <vector>

#include "Vec3.h"
#include "Triangle.h"

using namespace std;

/// A Mesh class, storing a list of vertices and a list of triangles indexed over it.
class Mesh {
    
    public:
        inline Mesh () {}
        inline virtual ~Mesh () {}

        inline vector<Vec3f> & positions () { return m_positions; }
        inline const vector<Vec3f> & positions () const { return m_positions; }
        inline  vector<Vec3f> & normals () { return m_normals; }
        inline const vector<Vec3f> & normals () const { return m_normals; }
        inline vector<Triangle> triangles () { return m_triangles; }
        inline const vector<Triangle> & triangles () const { return m_triangles; }

        /// Empty the positions, normals and triangles arrays.
        void clear ();

    	/// Loads the mesh from a <file>.off
    	void loadOFF (const string & filename);
        
        /// Compute smooth per-vertex normals
        void recomputeNormals ();

        /// scale to the unit cube and center at original
        void centerAndScaleToUnit ();

        /// Apply laplacian filter to mesh
        void topologicaLaplacianFilter();

        /// Apply topological laplacian filter
        void geometricalLaplacian();

        /// simplify mesh with OCS
        void simplify(unsigned int resolution);

        /// Add a bottom plane consisting of 2 * nbVertex * nbVertex triangles
        void addBottomPlane(unsigned int nbVertex, float width);

    private:
        vector<Vec3f> m_positions;
        vector<Vec3f> m_normals;
        vector<Triangle> m_triangles;
};
