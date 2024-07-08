#pragma once
#include "Ray.h"

#include "Vector3.inl"
#include "Sphere.h"
#include "Sphere.inl"

inline Ray::Ray(const Point3& origin, const Vector3& direction) :
	Origin(origin),
	Direction(direction)
{
}

inline Point3 Ray::Sample(double samplePoint) const
{
	return Origin + samplePoint * Direction;
}
