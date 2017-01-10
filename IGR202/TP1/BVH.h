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
		static int max_density;
		static int max_node;
		
	public: 
		BVH() { }
		BVH(const Mesh &mesh) {

			std::vector<int> v(mesh.triangles().size());
			for(unsigned int i = 0; i < v.size(); i++) {

				v[i]=i;
			}

			bbox = Bbox(mesh, v);

			std::vector<Bbox> childBbox = bbox.split(mesh);
			leftChild = new BVH(mesh, childBbox[0]);
			rightChild = new BVH(mesh, childBbox[1]);
			max_node--;
		}
		BVH(const Mesh &mesh, const Bbox &_bbox): bbox(_bbox), leftChild(NULL), rightChild(NULL) {

			if(bbox.getDensity() > max_density) {

				std::vector<Bbox> childBbox = bbox.split(mesh);
				leftChild = new BVH(mesh, childBbox[0]);
				rightChild = new BVH(mesh, childBbox[1]);
			}

			/*if(!bbox.isEmpty() && max_node > 0) {

				max_node--;
				std::vector<Bbox> childBbox = bbox.split(mesh);
				leftChild = new BVH(mesh, childBbox[0]);
				rightChild = new BVH(mesh, childBbox[1]);
			}*/
		}

		const Vec3f getMeanPos() { return bbox.getMeanPos(); }
		const Vec3f getMeanNormal() { return bbox.getMeanNormal(); }
		const void drawBVH(const Mesh &mesh, std::vector<float> &colors) {

			if(leftChild != NULL) leftChild->drawBVH(mesh, colors);
			if(rightChild != NULL) rightChild->drawBVH(mesh, colors);
			if(leftChild == NULL && rightChild == NULL) {

				bbox.drawBbox(mesh, colors);
			}
		}
};

int BVH::max_density = 100;
int BVH::max_node = 100;