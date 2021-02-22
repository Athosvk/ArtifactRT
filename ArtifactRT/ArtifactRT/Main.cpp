#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <random>

#include "Vector3.inl"
#include "Ray.inl"
#include "Sphere.inl"
#include "Scene.h"
#include "Math.h"
#include "Camera.h"

struct RenderTarget
{
	constexpr static double AspectRatio = 16.0 / 16.0;
	constexpr static int Width = 400;
	constexpr static int Height = int(Width / AspectRatio);
	constexpr static int SamplesPerPixel = 50;
};

RGBColor SampleRayColor(const Ray& ray, const Scene& scene)
{
	SampleBounds boundaries{ 0.0, Constants::Infinity };
	std::optional<RayIntersectionRecord> intersection = scene.FindFirstIntersection(ray, boundaries);
	if (intersection)
		return 0.5 * (intersection->Normal + RGBColor(1, 1, 1));

	double t = 0.5 * ray.Direction.GetNormalized().Y + 1.0;
	return (1.0 - t) * RGBColor(1.0, 1.0, 1.0) +
		t * RGBColor(0.5, 0.7, 1.0);
}

Scene CreateScene()
{
	Scene scene;
	scene.Add(std::make_unique<Sphere>(Point3(0.0, 0.0, -1.0), 0.5, RGBColor(1.0, 0.0, 0.0)));
	scene.Add(std::make_unique<Sphere>(Point3(0.0, -100.5, -1), 100, RGBColor(1.0, 0.0, 0.0)));
	return scene;
}

std::mt19937 CreateRandomGeneraor()
{
	// For iniial seeding only
	std::random_device device;
	return std::mt19937(device());
}

int main(int argumentCount, char** argumentVector)
{	
	RenderTarget image;

	Camera camera(RenderTarget::AspectRatio);

	std::fstream output_file;
	output_file.open("output_with_aa_" + std::to_string(RenderTarget::SamplesPerPixel) + ".ppm", std::fstream::out);
	output_file << "P3\n" << image.Width << ' ' << image.Height << "\n255\n";

	std::mt19937 random_generator = CreateRandomGeneraor();
	std::uniform_real_distribution distribution(0.0, 1.0);

	Scene scene = CreateScene();

	for (int64_t i = image.Height - 1; i >= 0; --i)
	{
		for (size_t j = 0; j < image.Width; ++j)
		{
			RGBColor color;
			for (size_t sample = 0; sample < RenderTarget::SamplesPerPixel; sample++)
			{
				auto u = (j + distribution(random_generator)) / (image.Width - 1);
				auto v = (i + distribution(random_generator)) / (image.Height - 1);
				color += SampleRayColor(camera.CreateRay(u, v), scene);
			}
			WriteColor(output_file, color / RenderTarget::SamplesPerPixel);
		}
		std::cout << "Line: " << (image.Height - i) << " out of " << image.Width << "\n";
	}
	return 0;
}
