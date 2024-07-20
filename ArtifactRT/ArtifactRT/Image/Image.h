#pragma once
#include <cstdint>
#include <vector>

#include "../Vector3.h"

struct Dimensions
{
	uint32_t Width;
	uint32_t Height;
};

struct Image
{
	std::vector<uint8_t> pixelsAsBytes() const;

	Dimensions Dimensions = { 0, 0 };
	std::vector<RGBColor> Pixels;
};
