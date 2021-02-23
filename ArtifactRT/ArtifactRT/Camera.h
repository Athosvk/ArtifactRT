#pragma once
#include "Vector3.h"
#include "Ray.h"

class Camera
{
public:
	explicit Camera(double aspecRatio);

	Ray CreateRay(double u, double v) const noexcept;
private:
	Point3 m_Origin;
	Point3 m_LowerLeftCorner;
	Vector3 m_Horizontal;
	Vector3 m_Vertical;
};

