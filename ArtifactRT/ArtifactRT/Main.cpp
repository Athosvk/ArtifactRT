#include <iostream>
#include <fstream>
#include <array>
#include <algorithm>

#include "Vector3.inl"
#include "Ray.inl"
#include "Sphere.inl"

struct RenderTarget
{
	constexpr static double AspectRatio = 16.0 / 9.0;
	constexpr static int Width = 1200;
	constexpr static int Height = int(Width / AspectRatio);
};

struct Camera
{
	constexpr static double AspectRatio = 16.0 / 9.0;
	constexpr static double ViewportHeight = 2.0;
	constexpr static double ViewportWidth = ViewportHeight * AspectRatio;
	constexpr static double FocalLength = 1.0;
};

RGBColor SampleRayColor(const Ray& ray, std::array<Sphere, 6> spheres)
{
	std::array<Sphere, 6>::const_iterator hit_sphere = std::find_if(spheres.begin(), spheres.end(), [ray](const Sphere& sphere)
	{
		return ray.Intersects(sphere);
	});
	if (hit_sphere != spheres.end())
	{
		return hit_sphere->Color;
	}
	double t = 0.5 * ray.Direction.GetNormalized().Y + 1.0;
	return (1.0 - t) * RGBColor(1.0, 1.0, 1.0) +
		t * RGBColor(0.5, 0.7, 1.0);
}

std::array<Sphere, 6> CreateSpheres()
{
	std::array<Sphere, 6> spheres;
	for (size_t i = 0; i < 6; i++)
	{
		spheres[i] = Sphere(Point3(-1.0 + 0.2 * i, 0.0, -1 - i * 0.25), i * 0.2, 
			RGBColor(0.15 * i, 0.5, 0.5));
	}
	return spheres;
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

	std::array<Sphere, 6> spheres = CreateSpheres();

	for (int64_t i = image.Height - 1; i >= 0; --i)
	{
		// Flush on purpose, otherwise we see no result
		std::cout << "Scanlines remaining: " << i << ' ' << std::endl;
		for (size_t j = 0; j < image.Width; ++j)
		{
			auto u = double(j) / (image.Width - 1);
			auto v = double(i) / (image.Height - 1);
			Ray ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
			WriteColor(output_file, SampleRayColor(ray, spheres));
		}
	}
	return 0;
}