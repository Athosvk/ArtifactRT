#pragma once
#include "Material.h"

#include "../Math/Vector3.h"

enum class EDiffuseType
{
	Hemispherical = 0,
	Lambertian = 1,
	LambertianApprox = 2
};

class Random;

class Lambertian : public Material
{
public:
	explicit Lambertian(RGBColor albedo, Random& random);

	virtual std::optional<ScatterResult> Scatter(const Ray& incomingRay, const IntersectionRecord& intersection) const override;
public:
	RGBColor Albedo;
private:
	Random& m_RandomGenerator;
};

