#include "Metalic.h"

#include "../HittableObject.h"
#include "../Vector3.inl"
#include "../Random.h"
#include "../Ray.inl"

Metalic::Metalic(const RGBColor& albedo, float fuziness, Random& randomGenerator) :
	Albedo(albedo),
	Fuzziness(fuziness),
	m_RandomGenerator(randomGenerator)
{
}

std::optional<ScatterResult> Metalic::Scatter(const Ray& incomingRay, const IntersectionRecord& intersection) const
{
	Vector3 reflected_ray = incomingRay.Direction.Normalize().Reflect(intersection.Normal);

	// Offset by random to create imperfect (fuzzy) reflection
	reflected_ray += m_RandomGenerator.NextInUnitSphere() * Fuzziness;

	// Only scatter on the outside, not the inside; light doesn't reach so there's no light to reflect. Consider the rest "absorbed"
	return reflected_ray.GetDotProduct(intersection.Normal) > 0 ? ScatterResult { Albedo, Ray(intersection.IntersectionWorldSpace, reflected_ray) } : 
		std::optional<ScatterResult>(std::nullopt);
};
