#include "Scene.h"

#include <cassert>

#include "Materials/Material.h"
#include "Sphere.inl"

void Scene::AddSphere(Sphere&& sphere)
{
	m_Spheres.emplace_back(sphere);
}

void Scene::AddMaterial(std::unique_ptr<Material> material)
{
	m_Materials.push_back(std::move(material));
}

std::optional<IntersectionRecord> Scene::FindFirstIntersection(const Ray& ray, const SampleBounds& sampleBounds) const
{
	std::optional<IntersectionRecord> closest;

#if defined(USE_BVH)
	assert(m_BVH.has_value());
	// Note: This currently doesn't correctly sample all primitives
	auto intersection = m_BVH->FindFirstIntersection(ray, sampleBounds);
	if (intersection) 
	{
		// TODO: Get a nearest T from the bvh so that we can compare to others,
		// if needed
		closest = intersection;
	}
#else

	for (const Sphere& sphere : m_Spheres)
	{
		auto intersection = sphere.FindIntersection(ray, sampleBounds);
		if (intersection && (!closest || intersection->T < closest->T))
		{
			closest = intersection;
		}
	}
#endif
	return closest;
}

void Scene::BuildBVH()
{
	m_BVH = BVH(std::vector<Sphere>(m_Spheres));
}
