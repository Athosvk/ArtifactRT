#include "BVH.h"

#include <cassert>
#include <algorithm>
#include <memory>

#include "Sphere.inl"
#include "AABB.inl"
#include "Materials/DebugMaterial.h"

BVH::BVH(std::vector<const HittableObject*> primitives) : m_primitives(primitives),
	m_debug_material(std::make_unique<DebugMaterial>(RGBColor(1, 1, 0))) 
{
	build();
	m_traversal_scratch_buffer.resize(m_nodes.size());
}

std::optional<IntersectionRecord> BVH::FindFirstIntersection(const Ray& ray, const SampleBounds& sampleBounds) const
{
	uint16_t traversal_steps = 0;

	m_traversal_scratch_buffer[0] = &m_root_node;
	uint32_t traversal_buffer_back = 1;

	auto sample_bounds = sampleBounds;
	std::optional<IntersectionRecord> closest_intersection = {};
	while (traversal_buffer_back != 0)
	{
		const BVHNode& current_node = *m_traversal_scratch_buffer.at(traversal_buffer_back - 1);
		traversal_buffer_back--;
		auto aabb_intersection = current_node.AABB.Intersects(ray, sampleBounds);
		if (aabb_intersection)
		{
			if (current_node.IsLeaf)
			{
				for (int i = current_node.FirstPrimitive; i < current_node.FirstPrimitive + current_node.PrimitiveCount; i++)
				{
					std::optional<IntersectionRecord> intersection = m_primitives[i]->FindIntersection(ray, sample_bounds);
					if (intersection)
					{
						// Discard any future found intersections automatically that may be further away than
						// the the intersection found here
						sample_bounds.MaxSample = intersection->T;
						closest_intersection = intersection;
					}
				}

			} 
			else
			{
				m_traversal_scratch_buffer[traversal_buffer_back + 2 - 1] = current_node.Left;
				m_traversal_scratch_buffer[traversal_buffer_back + 1 - 1] = current_node.Right;
				traversal_buffer_back += 2;
			}
			traversal_steps++;
		}
	}
	
	if (closest_intersection)
	{
		closest_intersection->TraversalSteps = traversal_steps;
		return closest_intersection;
	}
	return {};
	if (traversal_steps > 1)
	{
		return IntersectionRecord{ Vector3::Zero(), 0.0, true, Vector3::Forward(),
			static_cast<Material*>(m_debug_material.get()), traversal_steps };
	}
	return {};
}

void BVH::build()
{
	m_nodes.reserve(m_primitives.size() * 2);
	AABB root_bounds = AABB::NegativeBox();
	for (const HittableObject* primitive : m_primitives) {
		root_bounds = root_bounds.Grow(primitive->GetBounds());
	}

	auto root_node = BVHNode{ root_bounds, true };
	root_node.IsLeaf = true;
	root_node.AABB = root_bounds;
	root_node.FirstPrimitive = 0;
	root_node.PrimitiveCount = m_primitives.size();
	m_root_node = root_node;
	if (shouldSplit(m_root_node)) {
		subdivide(m_root_node);
	}
	m_nodes.shrink_to_fit();
}

bool BVH::shouldSplit(const BVHNode& node) const
{
	return node.PrimitiveCount > 1;
}

void BVH::subdivide(BVHNode& node)
{
	assert(node.IsLeaf && shouldSplit(node));

	Vector3 extents = node.AABB.Extents();
	int major_index = 0;
	if (extents.Y > extents.X) {
		major_index = 1;
	} 
	else if (extents.Z > extents.Y) {
		major_index = 2;
	}

	auto primitives_start = &m_primitives[node.FirstPrimitive];
	auto primitives_end = &m_primitives[node.FirstPrimitive + node.PrimitiveCount - 1];
	std::sort(primitives_start, primitives_end, [major_index](const HittableObject* left, const HittableObject* right) {
			return left->GetCenter()[major_index] < right->GetCenter()[major_index];
		});

	
	const Vector3 midpoint = node.AABB.Min + extents / 2;
	AABB left_aabb = AABB::NegativeBox();
	uint32_t left_count = 0;
	auto first_right = std::find_if(&m_primitives[node.FirstPrimitive], &m_primitives[node.FirstPrimitive + node.PrimitiveCount - 1], 
		[midpoint, major_index, left_aabb, &left_count](const HittableObject* primitive) {
		left_aabb.Grow(primitive->GetBounds());
		left_count += 1;
		return primitive->GetCenter()[major_index] > midpoint[major_index];
		});

	AABB right_aabb = AABB::NegativeBox();
	for (auto iter = first_right; iter != primitives_end; iter++) {
		right_aabb.Grow((*iter)->GetBounds());
	}

	auto first_right_index = static_cast<uint32_t>(std::distance(primitives_start, first_right));

	auto left_node = std::make_unique<BVHNode>(BVHNode{ left_aabb, true, { .PrimitiveCount = left_count, .FirstPrimitive = node.FirstPrimitive } });
	auto right_node = std::make_unique<BVHNode>(BVHNode{ right_aabb, true, {.PrimitiveCount = node.PrimitiveCount - left_count, .FirstPrimitive = first_right_index } });

	node.IsLeaf = false;
	node.Left = left_node.get();
	node.Right = right_node.get();

	m_nodes.emplace_back(std::move(left_node));
	m_nodes.emplace_back(std::move(right_node));

	if (shouldSplit(*node.Left)) {
		subdivide(*node.Left);
	}
	if (shouldSplit(*node.Right)) {
		subdivide(*node.Right);
	}
}

