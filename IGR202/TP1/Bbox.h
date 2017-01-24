#pragma once

#include "Vec3.h"

class Bbox
{
  public:
    Vec3f minCorner;
    Vec3f maxCorner;
    Vec3f color;
    Vec3f meanPos;

	Bbox(): minCorner(Vec3f(0.0, 0.0, 0.0)), maxCorner(Vec3f(0.0, 0.0, 0.0)), color(Vec3f(0.0, 0.0, 0.0)), meanPos(Vec3f(0.0, 0.0, 0.0)) { }
	Bbox(Vec3f _minCorner, Vec3f _maxCorner, Vec3f _meanPos, Vec3f _color = Vec3f(0.0, 0.0, 0.0)): minCorner(_minCorner), maxCorner(_maxCorner), color(_color), meanPos(_meanPos) { }
	~Bbox() { }
};
