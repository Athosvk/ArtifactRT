#pragma once
#include <vector>
#include <cstdint>

#include "AABB.h"
#include "Sphere.h"

struct BVHNode
{

	AABB aabb;
	bool is_leaf;
	union {
		struct {
			BVHNode* left;
			BVHNode* right;
		};
		struct {
			std::uint32_t prim_count;
			std::uint32_t first_tri;
		};
	};
};

class BVH
{
public:
	BVH(std::vector<const HittableObject*> primitives);

private:
	void build();
	bool shouldSplit(const BVHNode& node) const;
	void subdivide(BVHNode& node);

	std::vector<std::unique_ptr<BVHNode>> m_nodes;
	std::vector<const HittableObject*> m_primitives;
};

