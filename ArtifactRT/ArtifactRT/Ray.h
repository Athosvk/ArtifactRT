#pragma once

#include "Vector3.h"

struct Ray
{
	Ray() = default;
	inline Ray(const Point3& origin, const Vector3& direction);

	inline Point3 Sample(double samplePoint) const;

	Point3 Origin;
	Vector3 Direction;
};
