#include <iostream>
#include <fstream>

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
			double to_integer_range = 255.999;
			int r = int(double(i) / (height - 1) * to_integer_range);
			int g = int(double(j) / (width - 1) * to_integer_range);
			int b = int(0.25 * to_integer_range);
			
			output_file << r << ' ' << g << ' ' << b << '\n';
		}
	}
	return 0;
}