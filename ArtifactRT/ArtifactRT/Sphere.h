#pragma once
#include "Vector3.inl"
#include "HittableObject.h"
#include "Materials/Material.h"

struct Sphere : public HittableObject
{
	Sphere() = default;
	Sphere(Point3 center, double radius, Material* material);

	virtual std::optional<IntersectionRecord> Intersects(const Ray& ray, const SampleBounds& sampleBounds) const override;
public:
	Point3 Center = Point3::GetZero();
	double Radius = 0.0;

private:
	Material* m_Material;
};
