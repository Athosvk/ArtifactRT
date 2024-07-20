#pragma once
#include <vector>

#include "Image.h"
#include "../Vector3.h"

class PNGEncoder
{
public:
	std::vector<char> encode(const Image& image);
};
