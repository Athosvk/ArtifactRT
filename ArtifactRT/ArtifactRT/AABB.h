#pragma once
#include "Vector3.h"

struct Ray;
struct SampleBounds;

struct AABB {
public:
	Vector3 Min;
	Vector3 Max;

	inline bool Intersects(const Ray& ray, const SampleBounds& sampleBounds);
	inline bool Contains(Point3 point);
	inline AABB Grow(const AABB& other) const;
	inline Vector3 Extents() const;
	constexpr inline static AABB NegativeBox();
};
