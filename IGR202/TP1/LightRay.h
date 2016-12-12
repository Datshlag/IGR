#pragma once

#include "Vec3.h"
#include <iostream>

#define EPSILON 0.0000001

class LightRay{

	public:
		inline LightRay() {}
		inline LightRay(const Vec3f & origin, const Vec3f & direction): origin(origin), direction(direction){}

		inline void setOrigin(const Vec3f & _origin) { origin = _origin; }
		inline void setDirection(const Vec3f & _direction) { direction = _direction; }

		inline Vec3f getDirection() { return direction; }
		inline Vec3f getOrigin() { return origin; }

		inline bool intersects(const Vec3f & p0, const Vec3f & p1, const Vec3f & p2) {
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

	private:
		Vec3f origin;
		Vec3f direction;
};