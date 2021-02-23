#include "Camera.h"

#include "Vector3.inl"
#include "Ray.inl"

Camera::Camera(double aspectRatio) :
	m_Origin(Vector3::GetZeroVector())
{
	double viewportHeight = 2.0;
	double viewportWidth = viewportHeight * aspectRatio;
	double focalLength = 1.0;

	m_Horizontal = Vector3(viewportWidth, 0.0, 0.0);
	m_Vertical = Vector3(0.0, viewportHeight, 0.0);
	m_LowerLeftCorner = m_Origin - m_Horizontal / 2 - m_Vertical / 2 - Vector3(0.0, 0.0, focalLength);
}

Ray Camera::CreateRay(double u, double v) const noexcept
{
	return Ray(m_Origin, m_LowerLeftCorner + u * m_Horizontal + v * m_Vertical - m_Origin);
}
