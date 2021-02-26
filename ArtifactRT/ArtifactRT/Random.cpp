#include "Random.h"

Random::Random() :
	m_UniformDistribution(0.0, 1.0),
	m_NormalDistribution(0.0, 1.0)
{
	std::random_device random_device;
	m_Generator.seed(random_device());
}

float Random::Next()
{
	return m_UniformDistribution(m_Generator);
}

float Random::Next(float min, float max)
{
	return min + (max - min) * Next();
}

Vector3 Random::NextVector(float min, float max)
{
	return Vector3(Next(min, max), Next(min, max), Next(min, max));
}

Point3 Random::NextInUnitSphere()
{
	while (true)
	{
		auto randomVector = NextVector(-1, 1);
		if (randomVector.GetLengthSquared() <= 1)
			continue;
		return randomVector;
	}
}

Vector3 Random::NextUnitVector()
{
	// See https://math.stackexchange.com/a/87238
	// A normally distributed vector with mean 0 and variance 1 is uniformly distributed
	// on a unit sphere when normalized
	return Vector3(m_NormalDistribution(m_Generator), 
					m_NormalDistribution(m_Generator), 
					m_NormalDistribution(m_Generator)).Normalize();
}
