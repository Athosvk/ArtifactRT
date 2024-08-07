#pragma once
#include <vector>
#include <memory>

#include "HittableObject.h"
#include "Ray.h"
#include "BVH.h"

class Material;

class Scene
{
public:
	void Add(std::unique_ptr<HittableObject> object);
	void AddMaterial(std::unique_ptr<Material> material);
	std::optional<IntersectionRecord> FindFirstIntersection(const Ray& ray, const SampleBounds& sampleBounds) const;
	void BuildBVH();
private:
	std::vector<std::unique_ptr<HittableObject>> m_Objects;
	std::vector<std::unique_ptr<Material>> m_Materials;
	std::optional<BVH> m_BVH;
};

