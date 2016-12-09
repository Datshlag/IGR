#pragma once

#include "Vec3.h"

#define FLT_EPSILON 1.19209e-07

class LightRay{

	public:
		Vec3f origin;
		Vec3f direction;

		inline LightRay() {}
		inline LightRay(const Vec3f & origin, const Vec3f & direction): origin(origin), direction(direction){}

		inline bool intersects(const Vec3f & p0, const Vec3f & p1, const Vec3f & p2) {
				Vec3f e0 = p1 - p0;
				Vec3f e1 = p2 - p0;
				Vec3f n = normalize(cross(e0, e1));
				Vec3f q = cross(direction, e1);
				float a = dot(e0, q);
				if ((dot(n, direction) >= 0) || (abs(a) < FLT_EPSILON)) return false;

				Vec3f s = 1/a*(origin - p0);
				Vec3f r = cross(s, e0);
				float b0 = dot(s, q);
				float b1 = dot(r, direction);
				float b2 = 1 - b0 - b1;
				if( (b0 < 0) || (b1 < 0) || (b2 < 0)) return false;

				float t = dot(e1, r);
				return (t > FLT_EPSILON);
		}


		inline bool shareDirection(const Vec3f & v){ return (dot(direction, v) > 0); }
};