#include "Scene.h"

void Scene::Add(std::unique_ptr<HittableObject> object)
{
	m_Objects.emplace_back(std::move(object));
}

std::optional<IntersectionRecord> Scene::FindFirstIntersection(const Ray& ray, const SampleBounds& sampleBounds) const
{
	std::optional<IntersectionRecord> closest;
	SampleBounds sample_bounds = sampleBounds;

	for (const auto& object : m_Objects)
	{
		std::optional<IntersectionRecord> intersection = object->Intersects(ray, sample_bounds);
		if (intersection)
		{
			sample_bounds.MaxSample = intersection->RaySamplePoint;
			closest = intersection;
		}
	}

	return closest;
}
