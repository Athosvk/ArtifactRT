#pragma once
#include <optional>

#include "../Vector3.h"
#include "../Ray.h"

struct ScatterResult
{
	RGBColor Attenuation;
	Ray ScatteredRay;
};

struct IntersectionRecord;

class Material
{
public:
	virtual ScatterResult Scatter(const Ray& incomingRay, 
		const IntersectionRecord& intersection) const = 0;
};

