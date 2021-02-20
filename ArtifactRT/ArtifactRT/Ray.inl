#pragma once

#include "Ray.h"

Ray::Ray(const Point3& origin, const Vector3& direction) :
	Origin(origin),
	Direction(direction)
{
}
