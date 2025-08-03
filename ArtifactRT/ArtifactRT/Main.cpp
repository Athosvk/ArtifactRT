#include <fstream>
#include <latch>

#include "Math/Vector3.inl"
#include "Math/Ray.inl"
#include "Math/Sphere.inl"
#include "Scene.h"
#include "Math/Math.h"
#include "Camera.h"
#include "Random.h"
#include "Materials/Lambertian.h"
#include "Materials/Metalic.h"
#include "Materials/Dielectric.h"
#include "Image/Image.h"
#include "Image/PNGEncoder.h"
#include "Image/PPMEncoder.h"
#include "Benchmarking/ScopedTimer.h"
#include "JobSystem.h"

#include <semaphore>

struct RenderTarget
{
	constexpr static double AspectRatio = 16.0 / 9.0;
	constexpr static uint32_t Width = 1920 / 2;
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

class RenderJob {
public:
	RenderJob(Image& image, uint32_t x, uint32_t y, const RenderTarget& renderTarget, const Camera& camera, const Scene& scene, std::latch& allDone)
		: m_Image(image),
		m_X(x),
		m_Y(y),
		m_RenderTarget(renderTarget),
		m_Camera(camera),
		m_Scene(scene),
		m_AllDone(allDone)
	{
	}

	void operator()(Random& randomGenerator) {
		RGBColor color;
		for (size_t sample = 0; sample < m_RenderTarget.SamplesPerPixel; sample++)
		{
			auto u = (m_Y + randomGenerator.Next()) / (m_RenderTarget.Width - 1);
			auto v = (m_X + randomGenerator.Next()) / (m_RenderTarget.Height - 1);
			color += SampleRayColor(m_Camera.CreateRay(u, v), m_Scene);
		}
		m_Image.Pixels[m_RenderTarget.Width * (m_RenderTarget.Height - m_X - 1) + m_Y] = color / m_RenderTarget.SamplesPerPixel;
		m_AllDone.count_down();
	}
private:
	Image& m_Image;
	uint32_t m_X;
	uint32_t m_Y;
	RenderTarget m_RenderTarget;
	const Camera& m_Camera;
	const Scene& m_Scene;

	std::latch& m_AllDone;
};

int main(int argumentCount, char** argumentVector)
{	
	RenderTarget render_target;
	Image image;
	image.Dimensions = { render_target.Width, render_target.Height };
	image.Pixels.resize(render_target.Width * render_target.Height);

	Camera camera(RenderTarget::AspectRatio);

	Random randomGenerator;
	Scene scene = CreateScene(randomGenerator);
	
	JobSystem<RenderJob> jobSystem;
	ScopedTimer timer;

	std::latch done(render_target.Width * render_target.Height);
	for (size_t i = 0; i < render_target.Height; ++i)
	{
		for (size_t j = 0; j < render_target.Width; ++j)
		{
			jobSystem.Spawn(RenderJob(image, i, j, render_target, camera, scene, done));
		}
	}

	std::cout << "All jobs spawned, waiting for " << render_target.Width * render_target.Height << " jobs" << std::endl;

	done.wait();
	std::cout << "Time taken: " << (timer.GetDurationNanoseconds() / 1e9) << " seconds\n";
	
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
	system("pause");
	return 0;
}
