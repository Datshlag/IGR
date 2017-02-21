#pragma once

#include "Vec3.h"
#include "Bbox.h"
#include "Mesh.h"

#include <iostream>
#include <cfloat>

using namespace std;

class BVH {

	private :
		Bbox bbox; //Bounding box
		BVH* leftChild; //left child
		BVH* rightChild; //right child
		const Mesh& mesh; //reference to the mesh for all the computing part
		vector<int> indexes; //null if not a leaf, else contains the indexes in mesh.triangles() of the triangles in the bounding box.
		static unsigned int max_density; //We choose to create a leaf as soon as there is less than max_density triangles in the bounding volume

	public:
		~BVH();
		BVH(const Mesh &_mesh);
		BVH(const Mesh &_mesh, const vector<int> &_indexes, const Bbox &_bbox);

		const BVH* getLeftChild() const { return leftChild; }
		const BVH* getRightChild() const { return rightChild; }
		const Bbox getBbox() const { return bbox; }
		const vector<int> &getIndexes() const { return indexes; }
		const Mesh &getMesh() const { return mesh; }
		vector<float> getLinesPositions();

		//Used to split the current bvh and create its sons.
		void split (vector<int> &subIndexes1, 
            vector<int> &subIndexes2,
            Bbox &subBbox1,
            Bbox &subBbox2) const;

		//Used to compute the positions of the lines used to display the bounding volumes
		void computeLinesPositions (vector<float> &linePos, unsigned int depth);
};