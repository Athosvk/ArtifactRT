#pragma once
#include <limits>

namespace Constants
{
	const double Infinity = std::numeric_limits<double>::infinity();
	const double Pi = 3.1415926535897932385;
}

inline double DegreesToRadians(double degrees);
inline double RadiansToDegrees(double degrees);
