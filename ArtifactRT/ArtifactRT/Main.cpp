#include <fstream>

#include "Vector3.inl"
#include "Ray.inl"
#include "Sphere.inl"
#include "Scene.h"
#include "Math.h"
#include "Camera.h"
#include "Random.h"
#include "Materials/Lambertian.h"
#include "Materials/Metalic.h"
#include "Benchmarking/ScopedTimer.h"

struct RenderTarget
{
	constexpr static double AspectRatio = 16.0 / 9.0;
	constexpr static int Width = 800;
	constexpr static int Height = int(Width / AspectRatio);
	constexpr static int SamplesPerPixel = 100;
};

RGBColor SampleSkybox(const Ray& ray)
{
	double t = 0.5 * ray.Direction.Normalize().Y + 1.0;
	return (1.0 - t) * RGBColor(1.0, 1.0, 1.0) +
		t * RGBColor(0.5, 0.7, 1.0);
}

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
		std::optional<ScatterResult> scatter_result = intersection->Material->Scatter(ray, *intersection);
		if (scatter_result)
			return scatter_result->Attenuation * SampleRayColor(scatter_result->ScatteredRay, scene, bounces - 1);
		else
			return RGBColor::GetZero();
	}
	return SampleSkybox(ray);
}

Scene CreateScene(Random& randomGenerator)
{
	Scene scene;
	std::unique_ptr<Material> material_ground = std::make_unique<Lambertian>(RGBColor(0.8, 0.8, 0.0), randomGenerator);
	std::unique_ptr<Material> material_center_sphere = std::make_unique<Lambertian>(RGBColor(0.7, 0.3, 0.3), randomGenerator);
	std::unique_ptr<Material> material_left_sphere = std::make_unique<Metalic>(RGBColor(0.8, 0.8, 0.8), 0.3f, randomGenerator);
	std::unique_ptr<Material> material_right_sphere = std::make_unique<Metalic>(RGBColor(0.8, 0.6, 0.2), 1.0f, randomGenerator);
	
	scene.Add(std::make_unique<Sphere>(Point3( 0.0, -100.5, -1.0), 100.0, material_ground.get()));
    scene.Add(std::make_unique<Sphere>(Point3( 0.0,    0.0, -1.0),   0.5, material_center_sphere.get()));
    scene.Add(std::make_unique<Sphere>(Point3(-1.0,    0.0, -1.0),   0.5, material_left_sphere.get()));
    scene.Add(std::make_unique<Sphere>(Point3( 1.0,    0.0, -1.0),   0.5, material_right_sphere.get()));
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
	
	ScopedTimer timer;
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
	std::cout << "Time taken: " << (timer.GetDurationNanoseconds() / 1e9) << " seconds\n";
	system("pause");
	return 0;
}
