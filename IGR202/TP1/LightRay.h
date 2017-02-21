#pragma once

#include <iostream>
#include <cfloat>

#include "Vec3.h"
#include "BVH.h"
#include "Bbox.h"
#include "Mesh.h"

using namespace std;

class LightRay {

	public:
		LightRay() { }
		LightRay(const Vec3f & origin, const Vec3f & direction);

		Vec3f getDirection() const { return direction; }
		Vec3f getOrigin() const { return origin; }

		void setOrigin(const Vec3f & _origin) { origin = _origin; }
		void setDirection(const Vec3f & _direction) { direction = _direction; }

		/// Tests if the ray intersects a given triangle with a distance to the ray's origin that is smaller than the radius.
		bool intersectsTriangle(const Vec3f & p0, const Vec3f & p1, const Vec3f & p2, float radius) const;
		/// Tests if the ray intersects any given triangle of the bvh
		/// with a distance to the ray's origin that is smaller than the radius.
		bool intersectsBVH(const BVH *bvh, float radius) const;
		/// Tests if the ray intersects the given box with a distance to the ray's origin that is smaller than the radius.
		bool intersectsBox(const Bbox &bbox, float radius) const;
		/// Called iff the bvh is reduced to a leaf to check if the ray intersects one of the triangles that it contains,
		/// with a distance to the ray's origin that is smaller than the radius.
		bool intersectsContent(const std::vector<int> &indexes, const Mesh &mesh, float radius) const;

	private:
		Vec3f origin; //origin of the ray
		Vec3f direction; //direction of the ray
		Vec3f inv_direction; // 1/direction, for computational purpose
};	