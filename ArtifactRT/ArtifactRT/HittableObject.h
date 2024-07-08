#pragma once
#include <optional>

#include "Vector3.h"

struct Ray;
class Material;

struct IntersectionRecord
{
	Point3 Point;
	double RaySamplePoint;
	bool FrontFace;
	Vector3 Normal;
	Material* Material;
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

	virtual std::optional<IntersectionRecord> Intersects(const Ray& ray, const SampleBounds& sampleBounds) const = 0;
};

