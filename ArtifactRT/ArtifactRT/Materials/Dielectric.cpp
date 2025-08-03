#include "Dielectric.h"

#include "../HittableObject.h"
#include "../Math/Vector3.inl"
#include "../Random.h"
#include "../Math/Ray.inl"

Dielectric::Dielectric(double refractiveIndex, Random& randomGenerator) : 
	RefractiveIndex(refractiveIndex),
	m_RandomGenerator(randomGenerator)
{
}

std::optional<ScatterResult> Dielectric::Scatter(const Ray& incomingRay, const IntersectionRecord& intersection) const
{
	double refractive_index_ratio = intersection.FrontFace ? (1.0 / RefractiveIndex) : RefractiveIndex;

	Vector3 incoming_ray_direction = incomingRay.Direction.Normalize();
	double cos_theta = std::min((-incoming_ray_direction).GetDotProduct(intersection.Normal), 1.0);
	double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

	Vector3 scatter_direction;

	// Past critical angle; total internal reflection occurs
	if (refractive_index_ratio * sin_theta > 1.0 || CalculateReflectance(cos_theta, refractive_index_ratio) > m_RandomGenerator.Next())
	{
		scatter_direction = incoming_ray_direction.Reflect(intersection.Normal);
	}
	else
	{
		scatter_direction = incoming_ray_direction.Refract(intersection.Normal, refractive_index_ratio);
	}
	return ScatterResult { RGBColor(1.0, 1.0, 1.0), Ray(intersection.IntersectionWorldSpace, scatter_direction) };
}

double Dielectric::CalculateReflectance(double cosineTheta, double refractionRatio) const
{
	// Schlick's reflectance approximation
	double r0 = std::pow((1 - refractionRatio) / (1 + refractionRatio), 2);
	return r0 + (1 - r0) * pow(1 - cosineTheta, 5);
}
