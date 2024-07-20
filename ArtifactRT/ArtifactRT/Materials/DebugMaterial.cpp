#include "DebugMaterial.h"


DebugMaterial::DebugMaterial(RGBColor color) : m_color(color) 
{
	
}

std::optional<ScatterResult> DebugMaterial::Scatter(const Ray&, const IntersectionRecord&) const
{
	return ScatterResult{ m_color, {} };
}
