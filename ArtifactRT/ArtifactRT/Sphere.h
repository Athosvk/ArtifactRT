#pragma once

#include "Vector3.inl"

struct Sphere
{
	Sphere() = default;
	Sphere(Point3 center, double radius, RGBColor color);

	Point3 Center = Point3::GetZeroVector();
	double Radius = 0.0;
	RGBColor Color = RGBColor::GetZeroVector();
};
