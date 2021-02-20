#pragma once

#include "Ray.h"

#include "Vector3.inl"
#include "Sphere.h"

inline Ray::Ray(const Point3& origin, const Vector3& direction) :
	Origin(origin),
	Direction(direction)
{
}

inline Point3 Ray::Sample(double samplePoint) const
{
	return Origin + samplePoint * Direction;
}

inline bool Ray::Intersects(const Sphere& sphere) const
{
	Vector3 distance = Origin - sphere.Center;
	double a = Direction.GetDotProduct();
	double b = 2.0 * distance.GetDotProduct(Direction);
	double c = distance.GetDotProduct() - sphere.Radius * sphere.Radius;

	double discriminant = b * b - 4 * a * c;
	// < 0 no solutions
	// == 0 1 solution
	// > 9 2 solutions
	// Incorrect in the book? One solution also means we hit the sphere
	return discriminant >= 0;
}
