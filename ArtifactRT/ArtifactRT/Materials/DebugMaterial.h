#pragma once
#include "Material.h"
#include "../Vector3.h"

#if _HAS_CXX17 == 0
#define _HAS_CXX17 1
#endif

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
