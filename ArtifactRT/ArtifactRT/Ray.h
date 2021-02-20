#pragma once

#include "Vector3.h"

struct Ray
{
	Ray() = default;
	Ray(const Point3& origin, const Vector3& direction);

	Point3 Origin;
	Vector3 Direction;
};
