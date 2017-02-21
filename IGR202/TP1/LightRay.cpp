#include "LightRay.h"

#define EPSILON 0.0000001f

LightRay::LightRay(const Vec3f & origin, const Vec3f & direction): origin(origin), direction(direction) { 

	//puts nan if dividing by zero, not really a problem here.
	inv_direction = Vec3f(1.0/direction[0], 1.0/direction[1], 1.0/direction[2]);
}

// Using the algorithm from the lessons.
bool LightRay::intersectsTriangle(const Vec3f & p0, const Vec3f & p1, const Vec3f & p2, float radius) const {

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
	return (t > EPSILON && t < radius);
}

// Using "slabs"	method. The idea of the algorithm can be found here : https://www.siggraph.org/education/materials/HyperGraph/raytrace/rtinter3.htm
bool LightRay::intersectsBox(const Bbox &bbox, float radius) const {

	Vec3f minCorner = bbox.minCorner;
	Vec3f maxCorner = bbox.maxCorner;

    float tx1 = (minCorner[0] - origin[0]) * inv_direction[0]; 
    float tx2 = (maxCorner[0] - origin[0]) * inv_direction[0]; 
    float ty1 = (minCorner[1] - origin[1]) * inv_direction[1]; 
    float ty2 = (maxCorner[1] - origin[1]) * inv_direction[1]; 
    float tz1 = (minCorner[2] - origin[2]) * inv_direction[2]; 
    float tz2 = (maxCorner[2] - origin[2]) * inv_direction[2]; 

    float tmin = max(max(min(tx1, tx2), min(ty1, ty2)), min(tz1, tz2));
    float tmax = min(min(max(tx1, tx2), max(ty1, ty2)), max(tz1, tz2));

    if ((tmax > 0 && tmax > tmin && tmin < radius)) return true;
    else return false;
}

// Recursively testing if the ray intersects one of the sons of the current node. If all we have is a leaf, we call the method below.
bool LightRay::intersectsBVH(const BVH *bvh, float radius) const{

	Bbox bbox = bvh->getBbox();
	if(intersectsBox(bbox, radius)) {

		const BVH * leftChild = bvh->getLeftChild();
		const BVH * rightChild = bvh->getRightChild();

		if(leftChild  && rightChild)
			return(intersectsBVH(leftChild, radius) || intersectsBVH(rightChild, radius));

		else 
			return intersectsContent(bvh->getIndexes(), bvh->getMesh(), radius);
	}

	return false;
}

// Called when bvh is just a leaf. We test the intersection with the triangles of the leaf.
bool LightRay::intersectsContent(const vector<int> &indexes, const Mesh &mesh, float radius) const {

	const vector<Triangle>& triangles = mesh.triangles();
	const vector<Vec3f>& positions = mesh.positions();
    Triangle currTri;
    bool intersects = false;

    for (unsigned int i = 0; i < indexes.size(); i++) {

        currTri = triangles[indexes[i]];
        intersects = intersectsTriangle(positions[currTri[0]], positions[currTri[1]], positions[currTri[2]], radius);
        if (intersects) break;
    }

    return intersects;
}