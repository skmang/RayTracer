#include <limits>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <string>
#include <random>
#include <iomanip>
#include "vec3.h"
#include "Ray.h"
#include "Hitable.h"
#include "HitableList.h"
#include "Sphere.h"
#include "Camera.h"
#include "Utility.h"
const float PI = 3.14159265358979323846f;
const int MAX_RECURSIVE_COUNT = 2;

Vec3 GetColor(const Ray& r, Hitable* world) {
	HitInfo rec;
	if (world->Hit(r, 0.00001, std::numeric_limits<float>::max(), rec)) {
		return 0.5*GetColor(Ray(r.GetOrigin(),rec.Normal + GetPointInUnitSphere()),world);
	}
	else {
		Vec3 unit_vec = unit_vector(r.GetDirection());
		float t = 0.5*(unit_vec.y() + 1.0);
		return Vec3(1.0, 1.0, 1.0) * (1 - t) + Vec3(0.5, 0.7, 1.0)*t;
	}
}

void ShowProgress(float current , float total) {
	std::cout << current / total << "\r";
}

int main()
{
	std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(3);
	auto time = std::chrono::system_clock::now();
	std::ofstream os("../Image/Image_" + std::to_string(GetRandomNumber(1, 100000)) + ".ppm");
	int ns = 30;
	int nx = 800;
	int ny = 400;
	os << "P3\n" << nx << " " << " " << ny << "\n255\n";
	Vec3 lower_left(-2.0, -1.0, -1.0);
	Vec3 horizontal(4.0, 0, 0);
	Vec3 vertical(0, 2.0, 0);
	Vec3 origin(0, 0, 0);
	Hitable* list[2];
	list[0] = new Sphere(Vec3(0, 0, -1), 0.5f);
	list[1] = new Sphere(Vec3(0, -100.5f, -1), 100.0f);
	Hitable* world = new HitableList(list, 2);
	Camera cam;
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
				color += GetColor(r, world);
			}
			color /= float(ns);
			int ir = int(255.99 * sqrt(color[0]));
			int ig = int(255.99 * sqrt(color[1]));
			int ib = int(255.99 * sqrt(color[2]));
			os << ir << " " << ig << " " << ib << "\n";
			ShowProgress((float)(x + (ny - y) * nx),(float)(nx*ny));
		}
	}
	return 0;
}