#pragma once

#include "Vector3.inl"

#include "HittableObject.h"

struct Sphere : public HittableObject
{
	Sphere() = default;
	Sphere(Point3 center, double radius, RGBColor color);

	virtual std::optional<RayIntersectionRecord> Intersects(const Ray& ray, const SampleBounds& sampleBounds) const override;

public:
	Point3 Center = Point3::GetZero();
	double Radius = 0.0;
	RGBColor Color = RGBColor::GetZero();
};
