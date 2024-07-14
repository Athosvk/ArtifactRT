#include "AABB.h"

#include "Ray.h"
#include "HittableObject.h"

inline bool AABB::Intersects(const Ray& ray, const SampleBounds& sampleBounds)
{
	// Check if begin or end is fully containedjj
	if (Contains(ray.Sample(sampleBounds.MinSample)) || Contains(ray.Sample(sampleBounds.MaxSample))) {
		return true;
	}

}

inline bool AABB::Contains(Point3 point)
{
	return point.X >= Min.X && point.X <= Max.X &&
		point.Y >= Min.Y && point.Y <= Min.Y &&
		point.Z >= Min.Z && point.Z <= Min.Z;
}

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
