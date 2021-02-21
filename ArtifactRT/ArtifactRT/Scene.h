#pragma once
#include <vector>
#include <memory>

#include "HittableObject.h"
#include "Ray.h"

class Scene
{
public:
	void Add(std::unique_ptr<HittableObject> object);
	std::optional<RayIntersectionRecord> FindFirstIntersection(const Ray& ray, const SampleBounds& sampleBounds) const;

private:
	std::vector<std::unique_ptr<HittableObject>> m_Objects;
};

