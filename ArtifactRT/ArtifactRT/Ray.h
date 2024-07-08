#pragma once
#include <optional>

#include "Vector3.h"

struct Sphere;

struct Ray
{
	Ray() = default;
	Ray(const Point3& origin, const Vector3& direction);

	Point3 Sample(double samplePoint) const;

	Point3 Origin;
	Vector3 Direction;
};
