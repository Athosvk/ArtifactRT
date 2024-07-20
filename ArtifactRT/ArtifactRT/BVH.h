#pragma once
#include <vector>
#include <cstdint>
#if _HAS_CXX17 == 0
#define _HAS_CXX17 1
#endif
#include <optional>

#include "AABB.h"
#include "Sphere.h"
#include "Materials/DebugMaterial.h"

struct BVHNode
{
	AABB AABB;
	bool IsLeaf;
	union {
		struct {
			BVHNode* Left;
			BVHNode* Right;
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
	BVH(std::vector<const HittableObject*> primitives);

	std::optional<IntersectionRecord> FindFirstIntersection(const Ray& ray, const SampleBounds& sampleBounds) const;
private:
	void build();
	bool shouldSplit(const BVHNode& node) const;
	void subdivide(BVHNode& node);

	BVHNode m_root_node;
	std::vector<std::unique_ptr<BVHNode>> m_nodes;
	std::vector<const HittableObject*> m_primitives;
	std::unique_ptr<DebugMaterial> m_debug_material;
};

