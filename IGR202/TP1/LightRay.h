#pragma once

#include "Vec3.h"
#include "BVH.h"
#include "Bbox.h"
#include "Mesh.h"
#include <iostream>

#define EPSILON 0.0000001

class LightRay {

	public:
		inline LightRay() { }
		inline LightRay(const Vec3f & origin, const Vec3f & direction): origin(origin), direction(direction) { 

			inv_direction = Vec3f(1.0/direction[0], 1.0/direction[1], 1.0/direction[2]);
		}

		void setOrigin(const Vec3f & _origin) { origin = _origin; }
		void setDirection(const Vec3f & _direction) { direction = _direction; }

		Vec3f getDirection() const { return direction; }
		Vec3f getOrigin() const { return origin; }

		inline bool intersectsTriangle(const Vec3f & p0, const Vec3f & p1, const Vec3f & p2) const {

			Vec3f e0 = p1 - p0;
			Vec3f e1 = p2 - p0;
			Vec3f n = normalize(cross(e0, e1));
			Vec3f h = cross(direction, e1);
			float a = dot(e0, h);

			if ( a > -EPSILON && a < EPSILON ) return false;

			Vec3f s = 1/a*(origin - p0);
			Vec3f q = cross(s, e0);
			float u = dot(s, h);

			if (u < 0.0 || u > 1.0) return false;

			float v = dot(q, direction);

			if (v < 0.0 || u + v > 1.0) return false;


			float t = dot(e1, q);
			return (t > EPSILON);
		}


		inline bool intersectsBox(const Bbox &box) const {

			Vec3f min = box.getMinCorner();
			Vec3f max = box.getMaxCorner();

			

		//bad implementation
		inline bool intersectsBoxObsolete(const Bbox &bbox) const {

			Vec3f minCorner = bbox.getMinCorner();
			Vec3f maxCorner = bbox.getMaxCorner();

			float xMin, yMin, zMin, xMax, yMax, zMax;

			xMin = minCorner[0];
			yMin = minCorner[1];
			zMin = minCorner[2];
			xMax = maxCorner[0];
			yMax = maxCorner[1];
			zMax = maxCorner[2];

			Vec3f aCorner, bCorner, cCorner, dCorner, eCorner, fCorner, gCorner, hCorner;

			aCorner = minCorner;
			bCorner = Vec3f(xMin, yMax, zMin);
			cCorner = Vec3f(xMax, yMin, zMin);
			dCorner = Vec3f(xMin, yMin, zMax);
			eCorner = Vec3f(xMin, yMax, zMax);
			fCorner = Vec3f(xMax, yMax, zMin);
			gCorner = Vec3f(xMax, yMin, zMax);
			hCorner = maxCorner;

			bool intersects = false;

			intersects = intersects || intersectsTriangle(aCorner, dCorner, bCorner);
			intersects = intersects || intersectsTriangle(bCorner, dCorner, eCorner);

			intersects = intersects || intersectsTriangle(bCorner, cCorner, aCorner);
			intersects = intersects || intersectsTriangle(fCorner, cCorner, bCorner);

			intersects = intersects || intersectsTriangle(bCorner, eCorner, hCorner);
			intersects = intersects || intersectsTriangle(bCorner, hCorner, fCorner);

			intersects = intersects || intersectsTriangle(hCorner, gCorner, cCorner);
			intersects = intersects || intersectsTriangle(hCorner, cCorner, fCorner);

			intersects = intersects || intersectsTriangle(cCorner, gCorner, dCorner);
			intersects = intersects || intersectsTriangle(cCorner, dCorner, aCorner);

			intersects = intersects || intersectsTriangle(eCorner, dCorner, gCorner);
			intersects = intersects || intersectsTriangle(eCorner, gCorner, hCorner);

			return intersects;
		}

		inline bool intersectsBoxContent(const Bbox &bbox) {

			std::vector<int> indexes = bbox.getTrianglesIndexes();
			const Mesh *mesh = bbox.getMesh();

			bool intersects;

			/*std::vector<Vec3f> positions = mesh->positions();
    		std::vector<Triangle> triangles = mesh->triangles();

			for(unsigned int j = 0; j < indexes.size(); j++) {

	            Triangle currTri = triangles[indexes[j]];
	            intersects = intersectsTriangle(positions[currTri[0]], positions[currTri[1]], positions[currTri[2]]);
	            if(intersects) break;
	        }*/

	        //std::cerr << "got to bottom : " << intersects << std::endl;*/

	        return intersects;
		}

		inline bool intersectionBVH(const BVH &bvh) {

			const Bbox bbox = bvh.getBbox();

			if(intersectsBox(bbox) != intersectsBoxObsolete(bbox)) std::cerr << intersectsBox(bbox) << " " << intersectsBoxObsolete(bbox) << std::endl;

			if(intersectsBox(bbox)) {

				const BVH * leftChild = bvh.getLeftChild();
				const BVH * rightChild = bvh.getRightChild();

				bool leftIntersect = false;
				bool rightIntersect = false;

				if(leftChild != NULL) leftIntersect = intersectionBVH(*leftChild);
				if(rightChild != NULL) rightIntersect = intersectionBVH(*rightChild);
				if((rightChild == NULL) && (leftChild == NULL)) return intersectsBoxContent(bbox);
				else return leftIntersect||rightIntersect;
			}

			return false;
		}

	private:
		Vec3f origin;
		Vec3f direction;
		Vec3f inv_direction;
		int sign[3];
};	