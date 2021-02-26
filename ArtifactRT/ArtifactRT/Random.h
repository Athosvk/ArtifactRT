#pragma once
#include <random>

#include "Vector3.h"

class Random
{
public:
	Random();

	float Next();
	float Next(float min, float max);
	Vector3 NextVector(float min, float max);
	Point3 NextInUnitSphere();
	Vector3 NextUnitVector();
private:
	std::mt19937 m_Generator;
	std::uniform_real_distribution<float> m_UniformDistribution;
	std::normal_distribution<float> m_NormalDistribution;
};

