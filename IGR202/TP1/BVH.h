#pragma once

#include "Vec3.h"
#include "Bbox.h"
#include "Mesh.h"
#include <iostream>

class BVH {

	private :
		Bbox bbox;
		BVH* leftChild;
		BVH* rightChild;

		const Mesh* mesh;

		std::vector<int> indexes;

		static unsigned int max_density;
		static unsigned int nb_node;
		static unsigned int nb_leaves;

		void split (const std::vector<int> &subIndexes1,
				  	const std::vector<int> &subIndexes2,
				  	const Bbox &subBbox1,
				  	const Bbox &subBbox2) const;

	public:
		~BVH();
		BVH();
		BVH(const Mesh* _mesh);
		BVH(const Mesh* _mesh, const std::vector<int> &_indexes, const Bbox &_bbox);

		void split (std::vector<int> &subIndexes1, 
            std::vector<int> &subIndexes2,
            Bbox &subBbox1,
            Bbox &subBbox2) const;

		const BVH* getLeftChild() const { return leftChild; }
		const BVH* getRightChild() const { return rightChild; }
		const Bbox getBbox() const { return bbox; }
		const std::vector<int> getIndexes() const { return indexes; }
		const Mesh* getMesh() const { return mesh; }

		int getNbNodes() const { return nb_node; }
		int getNbLeaves() const { return nb_leaves; }

		//void drawBVH(const Mesh &mesh, std::vector<float> &colors) const { }
};