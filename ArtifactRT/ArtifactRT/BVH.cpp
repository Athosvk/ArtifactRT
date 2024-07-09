#include "BVH.h"

#include <cassert>
#include <algorithm>

#include "Sphere.inl"
#include "AABB.inl"


BVH::BVH(std::vector<Sphere>& primitives) : m_primitives(primitives) {
}

void BVH::build()
{
	m_nodes.reserve(m_primitives.size() * 2);
	AABB root_bounds;
	for (const Sphere& primitive : m_primitives) {
		AABB sphere_bounds = {
			primitive.Center - Vector3::Fill(primitive.Radius),
			primitive.Center + Vector3::Fill(primitive.Radius)
		};

		root_bounds = root_bounds.Grow(sphere_bounds);
	}

	BVHNode root_node;
	root_node.aabb = root_bounds;
	if (shouldSplit(root_node)) {
		subdivide(root_node);
	}
	m_nodes.shrink_to_fit();
}

bool BVH::shouldSplit(const BVHNode& node) const
{
	return node.tri_count > 5;
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
	auto primitives_end = &m_primitives[node.first_tri + node.tri_count];
	std::sort(primitives_start, primitives_end, [major_index](const Sphere& left, const Sphere& right) {
			return left.Center[major_index] < right.Center[major_index];
		});

	
	const Vector3 midpoint = node.aabb.Min + extents / 2;
	AABB left_aabb = AABB::NegativeBox();
	uint32_t left_count = 0;
	auto first_right = std::find_if(&m_primitives[node.first_tri], &m_primitives[node.first_tri + node.tri_count], [midpoint, major_index, left_aabb, &left_count](const Sphere& primitive) {
		left_aabb.Grow(primitive.Bounds());
		left_count += 1;
		return primitive.Center[major_index] > midpoint[major_index];
		});

	AABB right_aabb = AABB::NegativeBox();
	for (auto iter = first_right; iter != primitives_end; iter++) {
		right_aabb.Grow(iter->Bounds());
	}

	auto first_right_index = static_cast<uint32_t>(std::distance(primitives_start, first_right));

	BVHNode left_node = BVHNode{ left_aabb, true, { .tri_count = left_count, .first_tri = node.first_tri } };
	BVHNode right_node = BVHNode{ right_aabb, true, {.tri_count = node.tri_count - left_count, .first_tri = first_right_index } };

	m_nodes.push_back(left_node);
	m_nodes.push_back(right_node);

	if (shouldSplit(left_node)) {
		subdivide(left_node);
	}
	if (shouldSplit(right_node)) {
		subdivide(right_node);
	}

}

