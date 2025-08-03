#include "Lambertian.h"

#include "../HittableObject.h"
#include "../Random.h"
#include "../Math/Vector3.inl"
#include "../Math/Ray.inl"

Lambertian::Lambertian(RGBColor albedo, Random& random) : 
	Albedo(albedo),
	m_RandomGenerator(random)
{
}

std::optional<ScatterResult> Lambertian::Scatter(const Ray& incomingRay, const IntersectionRecord& intersection) const
{
	Point3 bounce_direction; 
	
	switch (EDiffuseType::Hemispherical)
	{
		case EDiffuseType::Hemispherical:
			bounce_direction = m_RandomGenerator.NextInHemisphere(intersection.Normal);
			break;
		case EDiffuseType::Lambertian:
		{
			bounce_direction = m_RandomGenerator.NextUnitVector() + intersection.Normal;
			if (bounce_direction.IsNearZero())
			{
				bounce_direction = intersection.Normal;
			}
			break;
		}
		case EDiffuseType::LambertianApprox:
			// This is almost definitely slower than lambertian, but kept in as a reference
			bounce_direction = m_RandomGenerator.NextInUnitSphere() + intersection.Normal;

			if (bounce_direction.IsNearZero())
			{
				bounce_direction = intersection.Normal;
			}
			break;
	}

	return ScatterResult { Albedo, Ray(intersection.IntersectionWorldSpace, bounce_direction) };
}
