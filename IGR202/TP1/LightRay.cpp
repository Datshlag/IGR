#include "LightRay.h"
#define EPSILON 0.0001f

int LightRay::count = 0;

bool LightRay::intersectsTriangle(const Vec3f & p0, const Vec3f & p1, const Vec3f & p2) const {

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
			
bool LightRay::intersectsBox(const Bbox &bbox) const {

	/*Vec3f minCorner = bbox.minCorner;
	Vec3f maxCorner = bbox.maxCorner;

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

	return true;*/

	Vec3f minCorner = bbox.minCorner;
	Vec3f maxCorner = bbox.maxCorner;

    float t1 = (minCorner[0] - origin[0]) * inv_direction[0]; 
    float t2 = (maxCorner[0] - origin[0]) * inv_direction[0]; 
    float t3 = (minCorner[1] - origin[1]) * inv_direction[1]; 
    float t4 = (maxCorner[1] - origin[1]) * inv_direction[1]; 
    float t5 = (minCorner[2] - origin[2]) * inv_direction[2]; 
    float t6 = (maxCorner[2] - origin[2]) * inv_direction[2]; 

    float tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
    float tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

    if (!(tmax > 0 && tmax > tmin)) return false;
    else return true;
}

bool LightRay::intersectsBVH(const BVH *bvh) const{

	Bbox bbox = bvh->getBbox();

	if(intersectsBox(bbox)) {

		const BVH * leftChild = bvh->getLeftChild();
		const BVH * rightChild = bvh->getRightChild();

		if(leftChild != NULL && rightChild != NULL){
			bool left = intersectsBVH(leftChild);
			return(left || intersectsBVH(rightChild));
		}

		else {

			return intersectsContent(bvh->getIndexes(), bvh->getMesh());
		}
	}

	return false;
}

bool LightRay::intersectsContent(const std::vector<int> &indexes, const Mesh *mesh) const {

	std::vector<Triangle> triangles = mesh->triangles();
	std::vector<Vec3f> positions = mesh->positions();
    Triangle currTri;
    bool intersects = false;

    for (unsigned int i = 0; i < indexes.size(); i++) {

        currTri = triangles[indexes[i]];
        intersects = intersectsTriangle(positions[currTri[0]], positions[currTri[1]], positions[currTri[2]]);
        if (intersects) break;
    }

    return intersects;
}