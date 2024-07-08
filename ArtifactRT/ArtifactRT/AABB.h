#pragma once
#include "Vector3.h"

struct AABB {
public:
	Vector3 Min;
	Vector3 Max;

	inline AABB Grow(const AABB& other) const;
	inline Vector3 Extents() const;
	constexpr inline static AABB NegativeBox();
};
