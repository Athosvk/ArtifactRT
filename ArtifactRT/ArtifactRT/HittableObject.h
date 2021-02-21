#pragma once
#include <optional>

#include "Vector3.h"

struct Ray;

struct RayIntersectionRecord
{
	Point3 Point;
	double RaySamplePoint;
	bool FrontFace;
	Vector3 Normal;
};

struct SampleBounds
{
	double MinSample;
	double MaxSample;
};

class HittableObject
{
public:
	virtual std::optional<RayIntersectionRecord> Intersects(const Ray& ray, const SampleBounds& sampleBounds) const = 0;
};

