#pragma once

#include "Vec3.h"

class Bbox2
{
  public:
    Vec3f minCorner;
    Vec3f maxCorner;
    Vec3f color;

	Bbox2(): minCorner(0.0, 0.0, 0.0), maxCorner(0.0, 0.0, 0.0), color(0.0, 0.0, 0.0) { }
	Bbox2(Vec3f _minCorner, Vec3f _minCorner, Vec3f _color = NULL): minCorner(_minCorner), maxCorner(_maxCorner), color(_color) { }
	~Bbox2() { }
};
