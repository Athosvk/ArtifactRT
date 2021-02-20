#include <iostream>
#include <fstream>

#include "Vector3.inl"
#include "Ray.inl"

struct RenderTarget
{
	constexpr static double AspectRatio = 16.0 / 9.0;
	constexpr static int Width = 400;
	constexpr static int Height = int(Width / AspectRatio);
};

struct Camera
{
	constexpr static double AspectRatio = 16.0 / 9.0;
	constexpr static double ViewportHeight = 2.0;
	constexpr static double ViewportWidth = ViewportHeight * AspectRatio;
	constexpr static double FocalLength = 1.0;
};

RGBColor SampleRayColor(const Ray& ray)
{
	double t = 0.5 * ray.Direction.GetNormalized().Y + 1.0;
	return (1.0 - t) * RGBColor(1.0, 1.0, 1.0) +
		t * RGBColor(0.5, 0.7, 1.0);
}

int main(int argumentCount, char** argumentVector)
{	
	RenderTarget image;
	Camera viewport;

	Point3 origin = Vector3::GetZeroVector();
	Point3 horizontal = Vector3(viewport.ViewportWidth, 0.0, 0.0);
	Point3 vertical = Vector3(0.0, viewport.ViewportHeight, 0.0);
	Point3 lower_left_corner = origin - horizontal / 2 - vertical / 2 - Vector3(0.0, 0.0, viewport.FocalLength);

	std::fstream output_file;
	output_file.open("output.ppm", std::fstream::out);
	output_file << "P3\n" << image.Width << ' ' << image.Height << "\n255\n";

	for (int64_t i = image.Height - 1; i >= 0; --i)
	{
		// Flush on purpose, otherwise we see no result
		std::cout << "Scanlines remaining: " << i << ' ' << std::endl;
		for (size_t j = 0; j < image.Width; ++j)
		{
			auto u = double(j) / (image.Width - 1);
			auto v = double(i) / (image.Height - 1);
			Ray ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
			WriteColor(output_file, SampleRayColor(ray));
		}
	}
	return 0;
}