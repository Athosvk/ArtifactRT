#pragma once
#include <optional>

#include "Vector3.inl"
#include "HittableObject.h"
#include "Materials/Material.h"
#include "AABB.h"

struct Sphere : public HittableObject
{
	Sphere() = default;
	inline Sphere(Point3 center, double radius, Material* material);
	virtual ~Sphere() override = default;

	inline virtual std::optional<IntersectionRecord> FindIntersection(const Ray& ray, const SampleBounds& sampleBounds) const final;
	inline virtual AABB GetBounds() const override final;
	inline virtual Point3 GetCenter() const override final;
public:
	Point3 Center = Point3::Zero();
	double Radius = 0.0;

private:
	Material* m_Material;
};
