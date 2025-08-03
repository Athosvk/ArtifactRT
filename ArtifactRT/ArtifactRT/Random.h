#pragma once
#include <random>

#include "Math/Vector3.h"

class Random
{
public:
	Random();

	float Next();
	float Next(float min, float max);
	Vector3 NextVector(float min, float max);
	Point3 NextInUnitSphere();
	Vector3 NextUnitVector();
	Point3 NextInHemisphere(Vector3 normal);
private:
	std::mt19937 m_Generator;
	std::uniform_real_distribution<float> m_UniformDistribution;
	std::normal_distribution<float> m_NormalDistribution;
};

