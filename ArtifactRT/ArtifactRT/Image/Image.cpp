#include "Image.h"

std::vector<uint8_t> Image::pixelsAsBytes() const
{
    std::vector<uint8_t> bytes;
    bytes.reserve(Pixels.size() * 3);

    // Purposely not 256 so that we can map to integer range
    // with some form of rounding. I.e. 0.99 becomes 255. 
    // Otherwise only perfect white would round to 255
	double to_integer_range = 255.999;
    for (auto pixel : Pixels) 
    {
        bytes.push_back(static_cast<uint8_t>(pixel.X * to_integer_range));
        bytes.push_back(static_cast<uint8_t>(pixel.Y * to_integer_range));
        bytes.push_back(static_cast<uint8_t>(pixel.Z * to_integer_range));
    }
    return bytes;
}
