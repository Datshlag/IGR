#pragma once

#include "Vec3.h"
#include "Bbox.h"
#include "Mesh.h"
#include "Bbox.h"
#include <iostream>

class BVH {

	private : 
		Bbox bbox;
		BVH* leftChild;
		BVH* rightChild;
		static int max_density;
		static int max_node;
		static int nb_node;
		
	public: 
		BVH(): leftChild(NULL), rightChild(NULL) { }
		BVH(const Mesh &mesh) {

			std::vector<int> v(mesh.triangles().size());
			for(unsigned int i = 0; i < v.size(); i++) {

				v[i]=i;
			}

			bbox = Bbox(&mesh, v);

			std::vector<Bbox> childBbox = bbox.split();
			leftChild = new BVH(mesh, childBbox[0]);
			rightChild = new BVH(mesh, childBbox[1]);
			//max_node--;
		}
		BVH(const Mesh &mesh, const Bbox &_bbox): bbox(_bbox), leftChild(NULL), rightChild(NULL) {

			if(bbox.getDensity() > max_density) {

				std::vector<Bbox> childBbox = bbox.split();
				leftChild = new BVH(mesh, childBbox[0]);
				rightChild = new BVH(mesh, childBbox[1]);
				nb_node++;
			}

			/*if(!bbox.isEmpty() && max_node > 0) {

				max_node--;
				std::vector<Bbox> childBbox = bbox.split(mesh);
				leftChild = new BVH(mesh, childBbox[0]);
				rightChild = new BVH(mesh, childBbox[1]);
			}*/
		}

		Vec3f getMeanPos() const { return bbox.getMeanPos(); }
		Vec3f getMeanNormal() const { return bbox.getMeanNormal(); }

		const BVH* getLeftChild() const { return leftChild; }
		const BVH* getRightChild() const { return rightChild; }

		const Bbox getBbox() const { return bbox; }

		int getNbNodes() const { return nb_node; }

		void drawBVH(const Mesh &mesh, std::vector<float> &colors) const {

			if(leftChild != NULL) leftChild->drawBVH(mesh, colors);
			if(rightChild != NULL) rightChild->drawBVH(mesh, colors);
			if(leftChild == NULL && rightChild == NULL) {

				bbox.drawBbox(colors);
			}
		}
};

int BVH::max_density = 100;
int BVH::max_node = 100;
int BVH::nb_node = 0;