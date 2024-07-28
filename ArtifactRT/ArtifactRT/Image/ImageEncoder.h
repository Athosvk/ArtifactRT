#pragma once
#include <memory>

struct Image;

struct OutputBuffer
{
	std::unique_ptr<char[]> Output;
	size_t NumChars;
	bool BinaryData;
};

class ImageEncoder
{
public:
	virtual OutputBuffer encode(const Image& image) const = 0;
};
