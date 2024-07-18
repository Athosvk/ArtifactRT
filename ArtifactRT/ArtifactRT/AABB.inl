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
	for (int i = 0; i < 3; i++)
	{
		if (ray.Direction[i] != 0.0)
		{
			double near_plane_t = (Min[i] - ray.Origin[i]) / ray.Direction[i];
			double far_plane_t = (Max[i] - ray.Origin[i]) / ray.Direction[i];

			output.MinSample = std::max(output.MinSample, std::min(near_plane_t, far_plane_t));
			output.MaxSample = std::min(output.MaxSample, std::max(near_plane_t, far_plane_t));
		}
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
