#include "AABB.h"

AABB AABB::Grow(const AABB& other) const {
	return AABB{
		Min.Max(other.Min),
		Max.Max(other.Max)
	};
}

Vector3 AABB::Extents() const {
	return Max - Min;
}

inline constexpr AABB AABB::NegativeBox()
{
	return AABB { Vector3::PosInfinity(), Vector3::NegInfinity() };
}
