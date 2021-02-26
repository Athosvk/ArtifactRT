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
#include "Random.h"

struct RenderTarget
{
	constexpr static double AspectRatio = 16.0 / 9.0;
	constexpr static int Width = 400;
	constexpr static int Height = int(Width / AspectRatio);
	constexpr static int SamplesPerPixel = 100;
};

RGBColor SampleRayColor(const Ray& ray, const Scene& scene, Random& randomGenerator, unsigned bounces = 256)
{
	if (bounces == 0)
	{
		return RGBColor::GetZero();
	}
	SampleBounds boundaries{ 0.0001, Constants::Infinity };
	std::optional<RayIntersectionRecord> intersection = scene.FindFirstIntersection(ray, boundaries);
	if (intersection)
	{
		Point3 bounce_direction = randomGenerator.NextUnitVector() + intersection->Normal;
		return 0.5 * SampleRayColor(Ray(intersection->Point, bounce_direction), 
			scene, randomGenerator, bounces - 1);
	}

	double t = 0.5 * ray.Direction.Normalize().Y + 1.0;
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
	output_file.open("output.ppm", std::fstream::out);
	output_file << "P3\n" << image.Width << ' ' << image.Height << "\n255\n";

	Scene scene = CreateScene();

	Random randomGenerator;
	for (int64_t i = image.Height - 1; i >= 0; --i)
	{
		for (size_t j = 0; j < image.Width; ++j)
		{
			RGBColor color;
			for (size_t sample = 0; sample < RenderTarget::SamplesPerPixel; sample++)
			{
				auto u = (j + randomGenerator.Next()) / (image.Width - 1);
				auto v = (i + randomGenerator.Next()) / (image.Height - 1);
				color += SampleRayColor(camera.CreateRay(u, v), scene, randomGenerator);
			}
			WriteColor(output_file, color / RenderTarget::SamplesPerPixel);
		}
		std::cout << "Line: " << (image.Height - i) << " out of " << image.Height << "\n";
	}
	return 0;
}
