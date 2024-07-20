#pragma once
#include "Sphere.h"
#include "Ray.inl"

Sphere::Sphere(Point3 center, double radius, Material* material) :
	Center(center),
	Radius(radius),
	m_Material(material)
{
}

std::optional<IntersectionRecord> Sphere::Intersects(const Ray& ray, const SampleBounds& sampleBounds) const
{
	Vector3 distance = ray.Origin - Center;
	double a = ray.Direction.GetDotProduct();
	double half_b = distance.GetDotProduct(ray.Direction);
	double c = distance.GetDotProduct() - Radius * Radius;

	double discriminant = half_b * half_b - a * c;
	// < 0 no solutions
	// == 0 1 solution
	// > 9 2 solutions
	if (discriminant < 0)
	{
		return std::nullopt;
	}
	double discriminantRoot = std::sqrt(discriminant);
	double possible_hit_point = (-half_b - discriminantRoot) / a;
	if (possible_hit_point < sampleBounds.MinSample || possible_hit_point > sampleBounds.MaxSample)
	{
		possible_hit_point = (-half_b + discriminantRoot) / a;
		if (possible_hit_point < sampleBounds.MinSample || possible_hit_point > sampleBounds.MaxSample)
		{
			return std::nullopt;
		}
	}
	IntersectionRecord ray_intersection_record;
	ray_intersection_record.IntersectionWorldSpace = ray.Sample(possible_hit_point);
	ray_intersection_record.T = possible_hit_point;

	// Normalizing via radius here saves determining the length later to normalize
	Vector3 outward_normal = (ray_intersection_record.IntersectionWorldSpace - Center) / Radius;
	ray_intersection_record.FrontFace = ray.Direction.GetDotProduct(outward_normal) < 0.0;
	ray_intersection_record.Normal = ray_intersection_record.FrontFace ? outward_normal : -outward_normal;
	ray_intersection_record.Material = m_Material;
	return ray_intersection_record;
}

inline AABB Sphere::GetBounds() const
{
	return AABB { Center - Vector3::Fill(Radius), Center + Vector3::Fill(Radius)};
}

inline Point3 Sphere::GetCenter() const
{
	return Center;
}
