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
	assert(m_BVH.has_value());
	auto intersection = m_BVH->FindFirstIntersection(ray, sampleBounds);
	if (intersection) 
	{
		// TODO: Get a nearest T from the bvh so that we can compare to others,
		// if needed
		return intersection;
	}
	else
	{
		return {};
	}

}

void Scene::BuildBVH()
{
	m_BVH = BVH(std::vector<Sphere>(m_Spheres));
}
