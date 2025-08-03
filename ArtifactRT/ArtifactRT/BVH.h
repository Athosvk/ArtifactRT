#pragma once
#include <vector>
#include <cstdint>

// Workaround for clang issue
#if _HAS_CXX17 == 0
#define _HAS_CXX17 1
#endif
#include <optional>

#include "Math/AABB.h"
#include "Math/Sphere.h"
#include "Materials/DebugMaterial.h"

struct BVHNode
{
	AABB AABB;
	bool IsLeaf;
	union {
		struct {
			std::uint32_t Left;
			std::uint32_t Right;
		};
		struct {
			std::uint32_t PrimitiveCount;
			std::uint32_t FirstPrimitive;
		};
	};
};

class BVH
{
public:
	BVH(std::vector<Sphere>&& primitives);

	std::optional<IntersectionRecord> FindFirstIntersection(const Ray& ray, const SampleBounds& sampleBounds) const;
private:
	void build();
	bool shouldSplit(const BVHNode& node) const;
	void subdivide(BVHNode& node);

	BVHNode m_root_node;
	std::vector<BVHNode> m_nodes;
	std::vector<Sphere> m_primitives;
	std::unique_ptr<DebugMaterial> m_debug_material;
	mutable std::vector<uint32_t> m_traversal_scratch_buffer;
};

