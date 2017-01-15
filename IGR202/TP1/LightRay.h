#pragma once

#include <iostream>
#include <cfloat>

#include "Vec3.h"
#include "BVH.h"
#include "Bbox.h"
#include "Mesh.h"

#define EPSILON 0.0000001

class LightRay {

	public:
		static int count;
		float rayTime = 0.0;
		int countLocal = 0;
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
			
		//ray bbox intersection test implementaiton based on the "slabs" method : https://www.siggraph.org/education/materials/HyperGraph/raytrace/rtinter3.htm
		inline bool intersectsBox(const Bbox &bbox) const {

			Vec3f minCorner = bbox.getMinCorner();
			Vec3f maxCorner = bbox.getMaxCorner();

			float tNear, tFar, tMin, tMax;
			tNear = -INFINITY;
			tFar = INFINITY;

			//X axis plane
			if(origin[0] < EPSILON) {

				if((origin[0] < minCorner[0] - EPSILON) || (origin[0] > maxCorner[0] + EPSILON)) return false; 
			}

			tMin = (minCorner[0] - origin[0]) * inv_direction[0];
			tMax = (maxCorner[0] - origin[0]) * inv_direction[0];

			if(tMin > tMax) std::swap(tMin, tMax);
			if(tMin > tNear) tNear = tMin;
			if(tMax < tFar) tFar = tMax;

			//missed box
			if(tNear > tFar) return false;
			//box behind
			if(tFar < 0) return false;

			//Y axis plane
			if(origin[1] < EPSILON) {

				if((origin[1] < minCorner[1] - EPSILON) || (origin[1] > maxCorner[1] + EPSILON)) return false; 
			}

			tMin = (minCorner[1] - origin[1]) * inv_direction[1];
			tMax = (maxCorner[1] - origin[1]) * inv_direction[1];

			if(tMin > tMax) std::swap(tMin, tMax);
			if(tMin > tNear) tNear = tMin;
			if(tMax < tFar) tFar = tMax;

			if(tNear > tFar) return false;
			if(tFar < 0) return false;

			//Z axis plane
			if(origin[2] < EPSILON) {

				if((origin[2] < minCorner[2] - EPSILON) || (origin[2] > maxCorner[2] + EPSILON)) return false; 
			}

			tMin = (minCorner[2] - origin[2]) * inv_direction[2];
			tMax = (maxCorner[2] - origin[2]) * inv_direction[2];

			if(tMin > tMax) std::swap(tMin, tMax);
			if(tMin > tNear) tNear = tMin;
			if(tMax < tFar) tFar = tMax;

			if(tNear > tFar) return false;
			if(tFar < 0) return false;

			return true;
		}

		inline bool intersectsBoxContent(const Bbox &bbox) {

			clock_t t1, t2;
            t1 = clock();

			std::vector<int> indexes = bbox.getTrianglesIndexes();
			const Mesh *mesh = bbox.getMesh();

			bool intersects;

			std::vector<Vec3f> positions = mesh->positions();
    		std::vector<Triangle> triangles = mesh->triangles();

    		Triangle currTri;

			for(unsigned int j = 0; j < indexes.size(); j++) {

				countLocal++;
	            currTri = triangles[indexes[j]];
	            intersects = intersectsTriangle(positions[currTri[0]], positions[currTri[1]], positions[currTri[2]]);
	            if(intersects) break;
	        }

            t2 = clock();
            float diff = ((float)t2-(float)t1)/CLOCKS_PER_SEC;

            rayTime += diff;

	        return intersects;
		}

		inline bool intersectionBVH(const BVH *bvh) {

			const Bbox bbox = bvh->getBbox();

			if(intersectsBox(bbox)) {

				const BVH * leftChild = bvh->getLeftChild();
				const BVH * rightChild = bvh->getRightChild();

				if(leftChild != NULL && rightChild != NULL){
					bool left = intersectionBVH(leftChild);
					return(left || intersectionBVH(rightChild));
				}

				else return intersectsBoxContent(bbox);
			}

			return false;
		}

	private:
		Vec3f origin;
		Vec3f direction;
		Vec3f inv_direction;
};	

int LightRay::count = 0;