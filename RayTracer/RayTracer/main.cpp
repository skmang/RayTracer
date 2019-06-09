#include <limits>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <string>
#include <iomanip>
#include <thread>
#include "vec3.h"
#include "ray.h"
#include "Hitable.h"
#include "HitableList.h"
#include "Sphere.h"
#include "Camera.h"
#include "Utility.h"
#include "Material.h"
#include "ConstDef.h"
#include "BVHNode.h"

Vec3 GetColor(const Ray& r, Hitable* world, int depth) {
	HitInfo rec;
	if (world->Hit(r, 0.00001, std::numeric_limits<float>::max(), rec)) {
		Ray scatter;
		Vec3 attenuation;
		if (depth < 50 && rec.Material->Scatter(r, rec, attenuation, scatter))
		{
			// 颜色衰减 R G B 分量(范围在0到1)，为初始化材质时传入的值，会收敛至0，即为黑色的阴影
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

void ShowProgress(float current, float total,string info) {
	std::cout<< info << " " << current / total << "\r";
}

//Hitable* RandomScene()
//{
//	int n = 500;
//	Hitable** list = new Hitable*[n + 1];
//	list[0] = new Sphere(Vec3(0, -1000, 0), 1000, new Lambert(Vec3(0.3, 0.3, 0.3)));
//	int i = 1;
//	for (int a = -11; a < 11; a++)
//	{
//		for (int b = -11; b < 11; b++)
//		{
//			float chooseMat = GetCanonical();
//			Vec3 center(a + 0.9*GetCanonical(), 0.2, b + 0.9*GetCanonical());
//			if((center-Vec3(4,0.2,0)).length()>0.9)
//			{
//				if(chooseMat <0.7)
//				{
//					list[i++] = new MoveSphere(
//						center,
//						center + Vec3(0, GetCanonical()*0.5, 0),
//						0.0,
//						1.0,
//						0.2,
//						new Lambert(Vec3(GetCanonical(), GetCanonical(), GetCanonical()))
//					);
//				}else
//				{
//					list[i++] = new Sphere(center, 0.2, new Metal(
//						Vec3(0.5f*(1.f+ GetCanonical())
//							, 0.5f*(1.f + GetCanonical())
//							, 0.5f*(1.f + GetCanonical())),
//						0.5*GetCanonical()));
//				}
//			}
//		}
//	}
//	list[i++] = new Sphere(Vec3(-4, 1, 0), 1.0, new Metal(Vec3(0.61, 0.71, 0.7),0.3));
//	list[i++] = new Sphere(Vec3(0,1,0), 1.0, new Lambert(Vec3(0.1,0.71,0.36)));
//	list[i++] = new Sphere(Vec3(4, 1, 0), 1.0, new Metal(Vec3(0.5, 0.65, 0.87),0.2));
//
//	return new HitableList(list, i);
//}
Hitable* RandomSceneBVH()
{
	std::vector<Hitable*> hitableList;
	hitableList.push_back(
		new Sphere(Vec3(0, -1000, 0), 
			1000, 
			new Lambert(
				new CheckerTexture(
					new ConstantTexture(Vec3(0.2)),
					new ConstantTexture(Vec3(0.9))
				)
			)
		)
	);
	int i = 1;
	for (int a = -11; a < 11; a++) 
	{
		for (int b = -11; b < 11; b++)
		{
			float chooseMat = GetCanonical();
			Vec3 center(a + 0.9*GetCanonical(), 0.2, b + 0.9*GetCanonical());
			if ((center - Vec3(4, 0.2, 0)).length() > 0.9)
			{
				if (chooseMat < 0.7)
				{
					hitableList.push_back(
						new MoveSphere(
							center,
							center + Vec3(0, GetCanonical()*0.5, 0),
							0.0,
							1.0,
							0.2,
							new Lambert(new ConstantTexture(Vec3(GetCanonical(), GetCanonical(), GetCanonical())))
						));
				}
				else
				{
					hitableList.push_back(
						new Sphere(
							center, 
							0.2, 
							new Metal(Vec3(0.5f*(1.f + GetCanonical()), 0.5f*(1.f + GetCanonical()), 0.5f*(1.f + GetCanonical())),
					     	0.5*GetCanonical())
						)
					);
				}
			}
		}
	}
	hitableList.push_back(new Sphere(Vec3(-4, 1, 0), 1.0, new Metal(Vec3(0.61, 0.71, 0.7), 0)));
	hitableList.push_back(new Sphere(Vec3(0, 1, 0), 1.0, new Lambert(new ConstantTexture(Vec3(GetCanonical(), GetCanonical(), GetCanonical())))));
	hitableList.push_back(new Sphere(Vec3(4, 1, 0), 1.0, new Metal(Vec3(0.5, 0.65, 0.87), 0)));

	return new BVHNode(hitableList,0,1);
}

Hitable* TwoPerlinNoiseBalls()
{
	Texture *perlin = new NoiseTexture();
	Hitable** list = new Hitable*[2];
	list[0] = new Sphere(Vec3(0, -1000, 0), 1000, new Lambert(perlin));
	list[1] = new Sphere(Vec3(0, 2, 0), 2, new Lambert(perlin));
	return new HitableList(list,2);
}

struct MultithreadInfo
{
	MultithreadInfo(int x,int y,int xCount,int yCount,int xTotal,int yTotal,int ns,Camera* cam,Hitable* world)
	{
		X = x;
		Y = y;
		XCount = xCount;
		YCount = yCount;
		Cam = cam;
		World = world;
		Sample = ns;
		XTotal = xTotal;
		YTotal = yTotal;
	}
	int X;
	int Y;
	int XCount;
	int YCount;
	Camera* Cam;
	Hitable* World;
	int Sample;
	int XTotal;
	int YTotal;
};


void TraceRay(std::vector<std::vector<Vec3>>* buffer,MultithreadInfo info,bool* flags,int* progress,int tid)
{
	int xCount = info.X + info.XCount;
	for (int x = info.X;x< xCount;x++)
	{
		for (int y=info.Y;y<info.YCount;y++)
		{
			Vec3 color = Vec3(0, 0, 0);
			for (int i = 0; i < info.Sample; i++)
			{
				// 水平方向上映射至 [-2,2] 垂直方向上映射至[-1,1],因此可以覆盖整个屏幕空间
				float u = float(x + GetCanonical()) / float(info.XTotal);
				float v = float(y + GetCanonical()) / float(info.YTotal);
				Ray r = info.Cam->GetRay(u, v);
				Vec3 p = r.GetPointAtParam(2.0);
				color += GetColor(r, info.World, 0);
			}
			color /= float(info.Sample);
			int ir = int(255.99 * sqrt(color[0]));
			int ig = int(255.99 * sqrt(color[1]));
			int ib = int(255.99 * sqrt(color[2]));
			(*buffer)[x][y] = Vec3(ir, ig, ib);
			progress[tid]++;
		}
	}
	flags[tid] = true;
	std::cout << "Thread " << tid << " Completed " << std::endl;
}

int main()
{
	std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(3);

	// Image Settings
	int ns = 1;
	int nx = 800;
	int ny = 400;

	// 记录时间
	std::clock_t start;
	start = std::clock();

	// 初始化场景与相机
	//Hitable* world = RandomSceneBVH();
	Hitable* world = TwoPerlinNoiseBalls();
	Vec3 lookFrom = Vec3(13,2,3);
	Vec3 lookAt = Vec3(0, 0, 0);
	float focusDist = 10.0f;
	float aperture = 0.0f;
	float fov = 20;
	float aspectRatio = float(nx / ny);
	Camera cam(fov, aspectRatio, Vec3(0, 1, 0), lookFrom, lookAt, aperture, focusDist,0,1);

	// --多线程信息(先按10的整数来，简单.....)
	int xCount = nx / THREAD_COUNT;
	std::vector<MultithreadInfo> info;
	info.reserve(THREAD_COUNT);
	for (int i = 0; i < THREAD_COUNT; i++)
	{
		info.emplace_back(i*xCount, 0, xCount, ny, nx, ny, ns,&cam,world);
	}
	std::vector<std::vector<Vec3>> colorBuffer(nx, std::vector<Vec3>(ny));
	
	// --开多线程写buffer
	std::thread threads[THREAD_COUNT];
	// 某个线程完成的标记
	bool flags[THREAD_COUNT];
	// 某个线程的进度
	int progress[THREAD_COUNT];
	int totalCount = nx * ny;
	for (int i = 0; i < THREAD_COUNT; i++)
	{
		progress[i] = 0;
		flags[i] = false;
		threads[i] = std::thread(TraceRay,&colorBuffer,info[i],flags,progress,i);
	}

	// --不等待的话 可能有一小块渲染不完
	float lastProgress = 0;
	while (true)
	{
		int currentCount = 0;
		for (int i=0;i<THREAD_COUNT;i++)
		{
			currentCount += progress[i];
		}
		float progress = (float)currentCount / (float)totalCount;
		if(progress - lastProgress > 0.02)
		{
			std::cout << "Current Progress " << progress << "\r";
			lastProgress = progress;
		}
		bool canJoin = true;
		for (int i=0;i< THREAD_COUNT;i++)
		{
			if(!flags[i])
			{
				canJoin = false;
			}
		}
		if(canJoin)
		{
			break;
		}
	}

	for (int i=0;i<THREAD_COUNT;i++)
	{
		threads[i].join();
	}

	// --写入到文件
	std::ofstream os("../Image/Image_" + std::to_string(GetRandomNumber(1, 100000)) + ".ppm");
	os << "P3\n" << nx << " " << " " << ny << "\n255\n";
	for (int y = ny - 1; y >= 0; y--)
	{
		for (int x = 0; x < nx; x++)
		{
			os << colorBuffer[x][y][0] << " " << colorBuffer[x][y][1] << " " << colorBuffer[x][y][2] << "\n";
			ShowProgress(static_cast<float>(x + (ny - y) * nx), static_cast<float>(nx * ny),"写入PPM中...");
		}
	}

	// --输出时间
	const double duration = (std::clock() - start) / static_cast<double>(CLOCKS_PER_SEC);
	std::cout << "Time cost : " << duration << std::endl;
	int i;
	std::cin >> i;
	return 0;
}