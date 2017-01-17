#pragma once

#include "Vec3.h"
#include "Bbox.h"
#include "Mesh.h"
#include <iostream>

class BVHNew {

	private :
		Bbox bbox;
		BVHNew* leftChild;
		BVHNew* rightChild;

		std::vector<int> indexes;

		static int max_density;
		static int nb_node;
		static int nb_leaves;

    const Mesh* mesh;

	public:
		BVHNew(): { }
		BVHNew(const Mesh &mesh) { }
		BVHNew(const Mesh &mesh, const Bbox &_bbox) { }

		const BVHNew* getLeftChild() const { return leftChild; }
		const BVHNew* getRightChild() const { return rightChild; }
		const Bbox getBbox() const { return bbox; }

		int getNbNodes() const { return nb_node; }
		int getNbLeaves() const { return nb_leaves; }

		void drawBVH(const Mesh &mesh, std::vector<float> &colors) const { }
};

int BVH::max_density = 100;
int BVH::nb_node = 0;
int BVH::nb_leaves = 0;
