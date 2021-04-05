#pragma once
#include "Material.h"

class Random;

class Dielectric : public Material
{
public:
	Dielectric(double refractiveIndex, Random& randomGenerator);
	virtual std::optional<ScatterResult> Scatter(const Ray& incomingRay, const IntersectionRecord& intersection) const override;
private:
	double CalculateReflectance(double cosineTheta, double refractionRatio) const;

public:
	double RefractiveIndex;
private:
	Random& m_RandomGenerator;
};

