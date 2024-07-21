#include "Scene.h"
#include "Materials/Material.h"
#include "Sphere.inl"

void Scene::Add(std::unique_ptr<HittableObject> object)
{
	m_Objects.emplace_back(std::move(object));
}

void Scene::AddMaterial(std::unique_ptr<Material> material)
{
	m_Materials.push_back(std::move(material));
}

std::optional<IntersectionRecord> Scene::FindFirstIntersection(const Ray& ray, const SampleBounds& sampleBounds) const
{
	std::optional<IntersectionRecord> closest;
	SampleBounds sample_bounds = sampleBounds;

	if (m_BVH) {
		auto intersection = m_BVH->FindFirstIntersection(ray, sample_bounds);
		if (intersection) 
		{
			// TODO: Get a nearest T from the bvh so that we can compare to others,
			// if needed
			return intersection;
		}
	}

	for (const auto& object : m_Objects)
	{
		std::optional<IntersectionRecord> intersection = object->FindIntersection(ray, sample_bounds);
		if (intersection)
		{
			// Discard any intersections automatically that may be further away than
			// the the intersection found here
			sample_bounds.MaxSample = intersection->T;
			closest = intersection;
		}
	}

	return closest;
}

void Scene::BuildBVH()
{
	std::vector<const HittableObject*> objects;
	objects.reserve(m_Objects.size());
	for (const auto& object : m_Objects)
	{
		objects.emplace_back(object.get());
	}

	m_BVH = BVH(objects);
}
