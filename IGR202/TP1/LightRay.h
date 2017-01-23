#pragma once

#include <iostream>
#include <cfloat>

#include "Vec3.h"
#include "BVH.h"
#include "Bbox.h"
#include "Mesh.h"

class LightRay {

	public:
		static int count;
		float rayTime = 0.0;
		int countLocal = 0;
		LightRay() { }
		LightRay(const Vec3f & origin, const Vec3f & direction): origin(origin), direction(direction) { 

			inv_direction = Vec3f(1.0/direction[0], 1.0/direction[1], 1.0/direction[2]);
		}

		void setOrigin(const Vec3f & _origin) { origin = _origin; }
		void setDirection(const Vec3f & _direction) { direction = _direction; }

		Vec3f getDirection() const { return direction; }
		Vec3f getOrigin() const { return origin; }

		bool intersectsTriangle(const Vec3f & p0, const Vec3f & p1, const Vec3f & p2) const;
		bool intersectsBVH(const BVH *bvh) const;
		bool intersectsBox(const Bbox &bbox) const;
		bool intersectsContent(const std::vector<int> &indexes, const Mesh *mesh) const;

	private:
		Vec3f origin;
		Vec3f direction;
		Vec3f inv_direction;
};	

int LightRay::count = 0;