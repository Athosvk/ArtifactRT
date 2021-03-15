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
#include "Materials/Lambertian.h"

struct RenderTarget
{
	constexpr static double AspectRatio = 16.0 / 9.0;
	constexpr static int Width = 400;
	constexpr static int Height = int(Width / AspectRatio);
	constexpr static int SamplesPerPixel = 100;
};

RGBColor SampleRayColor(const Ray& ray, const Scene& scene, unsigned bounces = 256)
{
	if (bounces == 0)
	{
		return RGBColor::GetZero();
	}
	SampleBounds boundaries{ 0.0001, Constants::Infinity };
	std::optional<IntersectionRecord> intersection = scene.FindFirstIntersection(ray, boundaries);
	if (intersection)
	{
		ScatterResult scatter_result = intersection->Material->Scatter(ray, *intersection);
		return 0.5 * SampleRayColor(scatter_result.ScatteredRay, 
			scene, bounces - 1);
	}

	double t = 0.5 * ray.Direction.Normalize().Y + 1.0;
	return (1.0 - t) * RGBColor(1.0, 1.0, 1.0) +
		t * RGBColor(0.5, 0.7, 1.0);
}

Scene CreateScene(Random& randomGenerator)
{
	Scene scene;
	std::unique_ptr<Material> material = std::make_unique<Lambertian>(RGBColor(1.0, 0.0, 0.0), randomGenerator);
	
	scene.Add(std::make_unique<Sphere>(Point3(0.0, 0.0, -1.0), 0.5, material.get()));
	scene.Add(std::make_unique<Sphere>(Point3(0.0, -100.5, -1), 100, material.get()));
	return scene;
}

std::mt19937 CreateRandomGenerator()
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


	Random randomGenerator;
	Scene scene = CreateScene(randomGenerator);
	for (int64_t i = image.Height - 1; i >= 0; --i)
	{
		for (size_t j = 0; j < image.Width; ++j)
		{
			RGBColor color;
			for (size_t sample = 0; sample < RenderTarget::SamplesPerPixel; sample++)
			{
				auto u = (j + randomGenerator.Next()) / (image.Width - 1);
				auto v = (i + randomGenerator.Next()) / (image.Height - 1);
				color += SampleRayColor(camera.CreateRay(u, v), scene);
			}
			WriteColor(output_file, color / RenderTarget::SamplesPerPixel);
		}
		std::cout << "Line: " << (image.Height - i) << " out of " << image.Height << "\n";
	}
	return 0;
}
