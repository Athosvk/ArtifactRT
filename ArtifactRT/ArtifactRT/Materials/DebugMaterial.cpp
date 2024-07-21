#include "DebugMaterial.h"

#include "../HittableObject.h"
#include "../Ray.inl"


DebugMaterial::DebugMaterial(RGBColor color) : m_color(color) 
{
	
}

std::optional<ScatterResult> DebugMaterial::Scatter(const Ray& ray, const IntersectionRecord& intersection) const
{
	return ScatterResult{ m_color, Ray{ ray.Sample(intersection.T + 0.0001), ray.Direction }
};
}
