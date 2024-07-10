#include "BVH.h"

#include <cassert>
#include <algorithm>
#include <memory>

#include "Sphere.inl"
#include "AABB.inl"


BVH::BVH(std::vector<const HittableObject*> primitives) : m_primitives(primitives) {
	build();
}

[[clang::optnone]]
void BVH::build()
{
	m_nodes.reserve(m_primitives.size() * 2);
	AABB root_bounds;
	for (const HittableObject* primitive : m_primitives) {
		root_bounds = root_bounds.Grow(primitive->GetBounds());
	}

	auto root_node = std::make_unique<BVHNode>(BVHNode{ root_bounds, true, });
	root_node->is_leaf = true;
	root_node->aabb = root_bounds;
	if (shouldSplit(*root_node)) {
		subdivide(*root_node);
	}
	m_nodes.shrink_to_fit();
}

bool BVH::shouldSplit(const BVHNode& node) const
{
	return node.prim_count > 1;
}

void BVH::subdivide(BVHNode& node)
{
	assert(node.is_leaf && shouldSplit(node));

	Vector3 extents = node.aabb.Extents();
	int major_index = 0;
	if (extents.Y > extents.X) {
		major_index = 1;
	} 
	else if (extents.Z > extents.Y) {
		major_index = 2;
	}

	auto primitives_start = &m_primitives[node.first_tri];
	auto primitives_end = &m_primitives[node.first_tri + node.prim_count];
	std::sort(primitives_start, primitives_end, [major_index](const HittableObject* left, const HittableObject* right) {
			return left->GetCenter()[major_index] < right->GetCenter()[major_index];
		});

	
	const Vector3 midpoint = node.aabb.Min + extents / 2;
	AABB left_aabb = AABB::NegativeBox();
	uint32_t left_count = 0;
	auto first_right = std::find_if(&m_primitives[node.first_tri], &m_primitives[node.first_tri + node.prim_count], [midpoint, major_index, left_aabb, &left_count](const HittableObject* primitive) {
		left_aabb.Grow(primitive->GetBounds());
		left_count += 1;
		return primitive->GetCenter()[major_index] > midpoint[major_index];
		});

	AABB right_aabb = AABB::NegativeBox();
	for (auto iter = first_right; iter != primitives_end; iter++) {
		right_aabb.Grow((*iter)->GetBounds());
	}

	auto first_right_index = static_cast<uint32_t>(std::distance(primitives_start, first_right));

	auto left_node = std::make_unique<BVHNode>(BVHNode{ left_aabb, true, { .prim_count = left_count, .first_tri = node.first_tri } });
	auto right_node = std::make_unique<BVHNode>(BVHNode{ right_aabb, true, {.prim_count = node.prim_count - left_count, .first_tri = first_right_index } });

	node.is_leaf = false;
	node.left = left_node.get();
	node.right = right_node.get();

	m_nodes.emplace_back(std::move(left_node));
	m_nodes.emplace_back(std::move(right_node));

	if (shouldSplit(*node.left)) {
		subdivide(*node.left);
	}
	if (shouldSplit(*node.right)) {
		subdivide(*node.right);
	}

}

