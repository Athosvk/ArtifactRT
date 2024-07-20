#pragma once
#include <optional>

#include "../Vector3.h"
#include "../Ray.h"

struct ScatterResult
{
	RGBColor Attenuation;
	std::optional<Ray> ScatteredRay;
};

struct IntersectionRecord;

class Material
{
public:
	virtual ~Material() = default;

	virtual std::optional<ScatterResult> Scatter(const Ray& incomingRay, 
		const IntersectionRecord& intersection) const = 0;
};

