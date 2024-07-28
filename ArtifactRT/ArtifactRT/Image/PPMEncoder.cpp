#include "PPMEncoder.h"
#include "Image.h"

#include <sstream>
#include <string>
#include <memory>
#include <utility>
#include <iostream>

OutputBuffer PPMEncoder::encode(const Image& image) const
{
	auto bytes = image.pixelsAsBytes();

	std::string initial_alloc;
	initial_alloc.reserve((bytes.size()
		// Each color channel uses another char as separator (space or new line)
		* 2));
	std::stringstream output_chars(initial_alloc);
	output_chars << "P3\n" << image.Dimensions.Width << ' ' << image.Dimensions.Height << "\n255\n";
	for (auto iter = bytes.begin(); iter != bytes.end(); iter +=3)
	{
		// Assumes RGB color (i.e. 3 channels)
		output_chars << std::to_string(*iter) << ' ';
		output_chars << std::to_string(*(iter + 1)) << ' ';
		output_chars << std::to_string(*(iter + 2)) << '\n';
	}
	auto output_str = output_chars.str();
	std::cout << output_str;
	OutputBuffer output_buffer;
	output_buffer.BinaryData = false;
	output_buffer.NumChars = output_str.size();
	output_buffer.Output = std::make_unique<char[]>(output_str.size());

	std::copy(output_str.begin(), output_str.end(), output_buffer.Output.get());
	return output_buffer;
}
