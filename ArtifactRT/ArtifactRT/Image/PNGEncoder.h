#pragma once
#include <vector>
#include <memory>

#include "Image.h"
#include "../Vector3.h"

struct OutputBuffer
{
	std::unique_ptr<char[]> Output;
	size_t NumBytes;
};

class PNGEncoder
{
public:
	OutputBuffer encode(const Image& image);
};
