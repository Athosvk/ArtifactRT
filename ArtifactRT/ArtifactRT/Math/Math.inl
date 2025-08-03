#pragma once
#include "Math.h"

inline double DegreesToRadians(double degrees)
{
	return degrees * Constants::Pi / 180.0;
}

inline double RadiansToDegrees(double radians)
{
	return (radians / Constants::Pi) * 180.0;
}
