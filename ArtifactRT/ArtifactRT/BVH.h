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
			std::uint32_t tri_count;
			std::uint32_t first_tri;
		};
	};
};

class BVH
{
public:
	BVH(std::vector<Sphere>& primitives);

private:
	void build();
	bool shouldSplit(const BVHNode& node) const;
	void subdivide(BVHNode& node);

	std::vector<BVHNode> m_nodes;
	std::vector<Sphere> m_primitives;
};

