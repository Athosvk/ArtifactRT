#pragma once
#include <vector>
#include <memory>

#include "HittableObject.h"
#include "Sphere.h"
#include "Ray.h"
#include "BVH.h"

class Material;

class Scene
{
public:
	void AddSphere(Sphere&& object);
	void AddMaterial(std::unique_ptr<Material> material);
	std::optional<IntersectionRecord> FindFirstIntersection(const Ray& ray, const SampleBounds& sampleBounds) const;
	void BuildBVH();
private:
	std::vector<std::unique_ptr<HittableObject>> m_Objects;
	std::vector<Sphere> m_Spheres;
	std::vector<std::unique_ptr<Material>> m_Materials;
	std::optional<BVH> m_BVH;
};

