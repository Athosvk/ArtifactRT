#include "AABB.h"

#include "Ray.h"
#include "HittableObject.h"

inline bool AABB::Intersects(const Ray& ray, const SampleBounds& sampleBounds) const
{
	// Check if begin or end is fully contained
	if (Contains(ray.Sample(sampleBounds.MinSample)) || Contains(ray.Sample(sampleBounds.MaxSample)))
	{
		return true;
	}

	// From: https://tavianator.com/2011/ray_box.html
	SampleBounds output = sampleBounds;
	Vector3 inv_direction = Vector3::Fill(1.0) / ray.Direction;
	Vector3 near_plane_t = (Min - ray.Origin) * inv_direction;
	Vector3 far_plane_t = (Max - ray.Origin) * inv_direction;

	Vector3 min = near_plane_t.Min(far_plane_t);
	Vector3 max = near_plane_t.Max(far_plane_t);
	
	for (int i = 0; i < 3; i++) {
		output.MinSample = std::max(output.MinSample, min[i]);
		output.MaxSample = std::min(output.MaxSample, max[i]);
	}
	return output.MinSample < output.MaxSample;
}

inline bool AABB::Contains(Point3 point) const
{
	return point.X >= Min.X && point.X <= Max.X &&
		point.Y >= Min.Y && point.Y <= Min.Y &&
		point.Z >= Min.Z && point.Z <= Min.Z;
}

AABB AABB::Grow(const AABB& other) const {
	return AABB{
		Min.Min(other.Min),
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
