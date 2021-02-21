#include "Scene.h"

void Scene::Add(std::unique_ptr<HittableObject> object)
{
	m_Objects.emplace_back(std::move(object));
}

std::optional<RayIntersectionRecord> Scene::FindFirstIntersection(const Ray& ray, const SampleBounds& sampleBounds) const
{
	std::optional<RayIntersectionRecord> closest;
	SampleBounds sample_bounds = sampleBounds;

	for (const auto& object : m_Objects)
	{
		std::optional<RayIntersectionRecord> intersection = object->Intersects(ray, sample_bounds);
		if (intersection)
		{
			sample_bounds.MaxSample = intersection->RaySamplePoint;
			closest = intersection;
		}
	}

	return closest;
}
