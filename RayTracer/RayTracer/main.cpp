#include <limits>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <string>
#include <iomanip>
#include "vec3.h"
#include "ray.h"
#include "Hitable.h"
#include "HitableList.h"
#include "Sphere.h"
#include "Camera.h"
#include "Utility.h"
#include "Material.h"
#include "ConstDef.h"
Vec3 GetColor(const Ray& r, Hitable* world, int depth) {
	HitInfo rec;
	if (world->Hit(r, 0.00001, std::numeric_limits<float>::max(), rec)) {
		Ray scatter;
		Vec3 attenuation;
		if (depth < 50 && rec.Material->Scatter(r, rec, attenuation, scatter))
		{
			return attenuation * GetColor(scatter, world, depth + 1);
		}
		else
		{
			return Vec3(0, 0, 0);
		}
	}
	else {
		Vec3 unit_vec = unit_vector(r.GetDirection());
		float t = 0.5*(unit_vec.y() + 1.0);
		return Vec3(1.0, 1.0, 1.0) * (1 - t) + Vec3(0.5, 0.7, 1.0)*t;
	}
}

void ShowProgress(float current, float total) {
	std::cout << current / total << "\r";
}

Hitable* RandomScene()
{
	int n = 500;
	Hitable** list = new Hitable*[n + 1];
	list[0] = new Sphere(Vec3(0, -1000, 0), 1000, new Lambert(Vec3(0.3, 0.3, 0.3)));
	int i = 1;
	for (int a = -11; a < 11; a++)
	{
		for (int b = -11; b < 11; b++)
		{
			float chooseMat = GetCanonical();
			Vec3 center(a + 0.9*GetCanonical(), 0.2, b + 0.9*GetCanonical());
			if((center-Vec3(4,0.2,0)).length()>0.9)
			{
				if(chooseMat <0.7)
				{
					list[i++] = new MoveSphere(
						center,
						center + Vec3(0, GetCanonical()*0.5, 0),
						0.0,
						1.0,
						0.2,
						new Lambert(Vec3(GetCanonical(), GetCanonical(), GetCanonical()))
					);
				}else
				{
					list[i++] = new Sphere(center, 0.2, new Metal(
						Vec3(0.5f*(1.f+ GetCanonical())
							, 0.5f*(1.f + GetCanonical())
							, 0.5f*(1.f + GetCanonical())),
						0.5*GetCanonical()));
				}
			}
		}
	}
	list[i++] = new Sphere(Vec3(-4, 1, 0), 1.0, new Metal(Vec3(0.61, 0.71, 0.7),0.3));
	list[i++] = new Sphere(Vec3(0,1,0), 1.0, new Lambert(Vec3(0.1,0.71,0.36)));
	list[i++] = new Sphere(Vec3(4, 1, 0), 1.0, new Metal(Vec3(0.5, 0.65, 0.87),0.2));
	return new HitableList(list, i);
}

int main()
{
	std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(3);
	std::ofstream os("../Image/Image_" + std::to_string(GetRandomNumber(1, 100000)) + ".ppm");

	// Image Settings
	int ns = 4;
	int nx = 1366;
	int ny = 768;

	// Scene
	Hitable* list[4];
	list[0] = new Sphere(Vec3(0, 0, -1), 0.5f, new Lambert(Vec3(0.8, 0.3, 0.3)));
	list[1] = new Sphere(Vec3(0, -100.5f, -1), 100.0f, new Lambert(Vec3(0.8, 0.3, 0.0)));
	list[2] = new Sphere(Vec3(1, 0, -1), 0.5f, new Metal(Vec3(0.8, 0.6, 0.2), 0.5));
	list[3] = new Sphere(Vec3(-1, 0, -1), 0.5f, new Metal(Vec3(0.8, 0.8, 0.8), 0.9));
	Hitable* world = RandomScene();
	//Hitable* world = new HitableList(list, 4);
	// Cam
	Vec3 lookFrom = Vec3(13,2,3);
	Vec3 lookAt = Vec3(0, 0, 0);
	float focusDist = 10.0f;
	float aperture = 0.0f;
	float fov = 20;
	float aspectRatio = float(nx / ny);
	Camera cam(fov, aspectRatio, Vec3(0, 1, 0), lookFrom, lookAt, aperture, focusDist,0,1);

	os << "P3\n" << nx << " " << " " << ny << "\n255\n";
	for (int y = ny - 1; y >= 0; y--)
	{
		for (int x = 0; x < nx; x++)
		{
			Vec3 color = Vec3(0, 0, 0);
			for (int i = 0; i < ns; i++)
			{
				// 水平方向上映射至 [-2,2] 垂直方向上映射至[-1,1],因此可以覆盖整个屏幕空间
				float u = float(x + GetCanonical()) / float(nx);
				float v = float(y + GetCanonical()) / float(ny);
				Ray r = cam.GetRay(u, v);
				Vec3 p = r.GetPointAtParam(2.0);
				color += GetColor(r, world, 0);
			}
			color /= float(ns);
			int ir = int(255.99 * sqrt(color[0]));
			int ig = int(255.99 * sqrt(color[1]));
			int ib = int(255.99 * sqrt(color[2]));
			os << ir << " " << ig << " " << ib << "\n";
			ShowProgress((float)(x + (ny - y) * nx), (float)(nx*ny));
		}
	}
	//std::getchar();
	return 0;
}