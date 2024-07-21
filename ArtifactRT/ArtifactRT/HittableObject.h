#pragma once
#include <optional>

#include "Vector3.h"
#include "AABB.h"

struct Ray;
class Material;

struct IntersectionRecord
{
	Point3 IntersectionWorldSpace;
	double T;
	bool FrontFace;
	Vector3 Normal;
	Material* Material;
	uint16_t TraversalSteps;
};

struct SampleBounds
{
	double MinSample;
	double MaxSample;
};

class HittableObject
{
public:
	virtual ~HittableObject() = default;

	virtual std::optional<IntersectionRecord> FindIntersection(const Ray& ray, const SampleBounds& sampleBounds) const = 0;
	virtual AABB GetBounds() const = 0;
	virtual Point3 GetCenter() const = 0;
};

