#pragma once
#include <optional>

#include "../Math/Vector3.h"
#include "../Math/Ray.h"

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

