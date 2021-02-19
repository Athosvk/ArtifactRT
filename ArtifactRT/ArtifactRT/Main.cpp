#include <iostream>
#include <fstream>

#include "Vector3.inl"

int main(int argumentCount, char** argumentVector)
{
	const size_t height = 256u;
	const size_t width = 256u;

	std::fstream output_file;
	output_file.open("output.ppm", std::fstream::out);
	auto fail = output_file.fail();
	output_file << "P3\n" << width << ' ' << height << "\n255\n";

	for (int64_t i = height - 1; i >= 0; --i)
	{
		for (size_t j = 0; j < width; ++j)
		{
			RGBColor color = RGBColor(double(i) / (height - 1),
				double(j) / (width - 1),
				0.25);
			WriteColor(output_file, color);
		}
	}
	return 0;
}