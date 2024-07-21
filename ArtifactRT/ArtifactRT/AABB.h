#pragma once
#include "Vector3.h"

struct Ray;
struct SampleBounds;

struct AABB 
{
public:
	Vector3 Min;
	Vector3 Max;

	inline std::optional<double> Intersects(const Ray& ray, const SampleBounds& sampleBounds) const;
	inline bool Contains(Point3 point) const;
	inline AABB Grow(const AABB& other) const;
	inline Vector3 Extents() const;
	constexpr inline static AABB NegativeBox();
};
