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

inline std::optional<Point3> Ray::Intersects(const Sphere& sphere) const
{
	Vector3 distance = Origin - sphere.Center;
	double a = Direction.GetDotProduct();
	double half_b = distance.GetDotProduct(Direction);
	double c = distance.GetDotProduct() - sphere.Radius * sphere.Radius;

	double discriminant = half_b * half_b - a * c;
	// < 0 no solutions
	// == 0 1 solution
	// > 9 2 solutions
	// Incorrect in the book? One solution also means we hit the sphere
	if (discriminant < 0)
	{
		return std::nullopt;
	}
	double hit_point = (-half_b - std::sqrt(discriminant)) / a;
	if (hit_point < 0)
	{
		return std::nullopt;
	}
	return Sample(hit_point);
}
