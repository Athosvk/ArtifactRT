#pragma once
#include "Material.h"
#include "../Math/Vector3.h"

class DebugMaterial : public Material
{
public:
	DebugMaterial(RGBColor color);
	virtual ~DebugMaterial() = default;

	virtual std::optional<ScatterResult> Scatter(const Ray& incomingRay, 
		const IntersectionRecord& intersection) const;
private:
	RGBColor m_color;
};
