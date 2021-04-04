#pragma once
#include "Material.h"

class Random;

class Metalic : public Material
{
public:
	Metalic(const RGBColor& albedo, float fuzziness, Random& randomGenerator);
	virtual std::optional<ScatterResult> Scatter(const Ray& incomingRay, const IntersectionRecord& intersection) const override;
public:
	RGBColor Albedo;
	float Fuzziness;
private:
	Random& m_RandomGenerator;
};
