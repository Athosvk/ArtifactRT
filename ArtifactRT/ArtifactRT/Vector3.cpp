#include "Vector3.h"

std::ostream& WriteColor(std::ostream& outputStream, const RGBColor& color)
{
	double to_integer_range = 255.999;
	return outputStream << static_cast<int>(to_integer_range * std::sqrt(color.X)) << ' '
		<< static_cast<int>(to_integer_range * std::sqrt(color.Y)) << ' '
		<< static_cast<int>(to_integer_range * std::sqrt(color.Z)) << '\n';
}
