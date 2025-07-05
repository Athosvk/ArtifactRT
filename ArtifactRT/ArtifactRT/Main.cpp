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
#include "Materials/Dielectric.h"
#include "Image/Image.h"
#include "Image/PNGEncoder.h"
#include "Image/PPMEncoder.h"
#include "Benchmarking/ScopedTimer.h"

struct RenderTarget
{
	constexpr static double AspectRatio = 16.0 / 9.0;
	constexpr static uint32_t Width = 960;
	constexpr static uint32_t Height = int(Width / AspectRatio);
	constexpr static uint32_t SamplesPerPixel = 3;
};

RGBColor SampleSkybox(const Ray& ray)
{
	double t = 0.5 * ray.Direction.Normalize().Y + 1.0;
	return (1.0 - t) * RGBColor(1.0, 1.0, 1.0) +
		t * RGBColor(0.5, 0.7, 1.0);
}

RGBColor SampleRayColor(const Ray& ray, const Scene& scene, unsigned bounces = 10)
{
	if (bounces == 0)
	{
		return RGBColor::Zero();
	}
	SampleBounds boundaries{ 0.0001, Constants::Infinity };
	std::optional<IntersectionRecord> intersection = scene.FindFirstIntersection(ray, boundaries);
	if (intersection)
	{
		std::optional<ScatterResult> scatter_result = intersection->Material->Scatter(ray, *intersection);
		if (scatter_result)
		{
			if (!scatter_result->ScatteredRay)
				return scatter_result->Attenuation;
			else 
				return scatter_result->Attenuation * SampleRayColor(scatter_result->ScatteredRay.value(), scene, bounces - 1);
		}
		else
		{
			return RGBColor::Zero();
		}
	}
	return SampleSkybox(ray);
}

Scene CreateScene(Random& randomGenerator)
{
	Scene scene;
	std::unique_ptr<Material> material_ground = std::make_unique<Lambertian>(RGBColor(0.8, 0.8, 0.0), randomGenerator);
	std::unique_ptr<Material> material_center_sphere = std::make_unique<Lambertian>(RGBColor(0.7, 0.3, 0.3), randomGenerator);
	std::unique_ptr<Material> material_left_sphere = std::make_unique<Dielectric>(1.5f, randomGenerator);
	std::unique_ptr<Material> material_right_sphere = std::make_unique<Metalic>(RGBColor(0.8, 0.6, 0.2), 0.0f, randomGenerator);
	
	scene.AddSphere(Sphere(Point3( 0.0, -105.5, -1.0), 100.0, material_ground.get()));
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			scene.AddSphere(Sphere(Point3(-2.0 + 0.5 * i, -2.0 + 0.5 * j, -5.0),   0.45, material_left_sphere.get()));
		}
	}
    scene.AddSphere(Sphere(Point3( 0.0,    0.0, -5.0),   0.5, material_center_sphere.get()));
    scene.AddSphere(Sphere(Point3(-1.0,    0.1, -5.0),   0.5, material_left_sphere.get()));
    scene.AddSphere(Sphere(Point3( 1.9,    0.2, -2.0),   0.5, material_right_sphere.get()));
    scene.AddSphere(Sphere(Point3( 1.2,    0.4, -3.0),   0.5, material_right_sphere.get()));
    scene.AddSphere(Sphere(Point3( 1.1,    0.9, -4.0),   0.5, material_right_sphere.get()));
    scene.AddSphere(Sphere(Point3( 1.4,    0.0, -8.0),   0.5, material_right_sphere.get()));
    scene.AddSphere(Sphere(Point3( 1.2,    0.0, -9.0),   0.5, material_right_sphere.get()));
    scene.AddSphere(Sphere(Point3( 3.0,    0.0, -10.0),   0.5, material_right_sphere.get()));
    scene.AddSphere(Sphere(Point3( 2.0,    0.0, -10.0),   0.5, material_right_sphere.get()));

	scene.AddMaterial(std::move(material_ground));
	scene.AddMaterial(std::move(material_center_sphere));
	scene.AddMaterial(std::move(material_left_sphere));
	scene.AddMaterial(std::move(material_right_sphere));
	scene.BuildBVH();
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
	RenderTarget render_target;
	Image image;
	image.Dimensions = { render_target.Width, render_target.Height };

	Camera camera(RenderTarget::AspectRatio);

	Random randomGenerator;
	Scene scene = CreateScene(randomGenerator);
	
	ScopedTimer timer;
	for (int64_t i = render_target.Height - 1; i >= 0; --i)
	{
		for (size_t j = 0; j < render_target.Width; ++j)
		{
			RGBColor color;
			for (size_t sample = 0; sample < RenderTarget::SamplesPerPixel; sample++)
			{
				auto u = (j + randomGenerator.Next()) / (render_target.Width - 1);
				auto v = (i + randomGenerator.Next()) / (render_target.Height - 1);
				color += SampleRayColor(camera.CreateRay(u, v), scene);
			}
			
			image.Pixels.emplace_back(color / RenderTarget::SamplesPerPixel);
		}
		std::cout << "Traced line: " << (render_target.Height - i) << " out of " << render_target.Height << "\n";
	}
	
	{
		PNGEncoder encoder;
		OutputBuffer png_buffer = encoder.encode(image);

		std::ofstream output_file;
		output_file.open("output.png", std::fstream::out | std::fstream::binary);
		output_file.write(png_buffer.Output.get(), png_buffer.NumChars);

		output_file.close();
	}
	{
		PPMEncoder encoder;
		OutputBuffer ppm_buffer = encoder.encode(image);
		
		std::ofstream output_file;
		output_file.open("output.ppm", std::fstream::out);
		output_file.write(ppm_buffer.Output.get(), ppm_buffer.NumChars);
		output_file.close();
		
	}
	std::cout << "Time taken: " << (timer.GetDurationNanoseconds() / 1e9) << " seconds\n";
	system("pause");
	return 0;
}
