#include <limits>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <string>
#include <random>
#include "vec3.h"
#include "ray.h"
#include "Hitable.h"
#include "HitableList.h"
#include "Sphere.h"
const float PI = 3.14159265358979323846f;
const int MAX_RECURSIVE_COUNT = 2;

Vec3 GetColor(const Ray& r,Hitable* world) {
	HitInfo rec;
	if (world->Hit(r, 0.0, std::numeric_limits<float>::max(), rec)) {
		return 0.5*Vec3(rec.Normal.x() + 1, rec.Normal.y() + 1, rec.Normal.z() + 1);
	}
	else {
		Vec3 unit_vec = unit_vector(r.direction());
		float t = 0.5*(unit_vec.y() + 1.0);
		return Vec3(1.0, 1.0, 1.0) * (1 - t) + Vec3(0.5, 0.7, 1.0)*t;
	}
}

int GetRandomNumber(int min,int max) {
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(min, max); // distribution in range [1, 6]
	return dist(rng);
}
int main()
{
	auto time = std::chrono::system_clock::now();
	std::ofstream os("../Image/Image_"+std::to_string(GetRandomNumber(1,100000))+".ppm");

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
	Hitable* world = new HitableList(list,2);
	for (int y = ny-1; y >= 0; y--)
	{
		for (int x = 0; x < nx; x++)
		{
			// 0到1
			float u = float(x) / float(nx);
			// 0到1
			float v = float(y) / float(ny);
			// 水平方向上映射至 [-2,2] 垂直方向上映射至[-1,1],因此可以覆盖整个屏幕空间
			Ray r(origin, lower_left + u * horizontal + v * vertical);
			Vec3 p = r.point_at_t(2.0);
			Vec3 color = GetColor(r,world);
			int ir = int(255.99 * color[0]);
			int ig = int(255.99 * color[1]);
			int ib = int(255.99 * color[2]);
			os << ir << " " << ig << " " << ib << "\n";
		}
	}
	return 0;
}

////反射的向量计算
//Vector3 Reflect(const Vector3& in,const Vector3& normal)
//{
//	return in - 2 * (in.dot(normal))*normal;
//}
//
////找到最近相交的一个点，储存相关信息
//bool SceneInteract(const Vector3& origin, const Vector3& dir, std::vector<Sphere>& spheres, RayHitInfo& hitInfo, Material& material)
//{
//	float spheresDistance = std::numeric_limits<float>::max();
//	float currentDist;
//	for (size_t i = 0; i < spheres.size(); ++i)
//	{
//		if (spheres[i].RayIntersect(origin, dir, currentDist) && currentDist < spheresDistance)
//		{
//			spheresDistance = currentDist;
//			hitInfo.HitPosition = origin + currentDist * dir;
//			hitInfo.HitNormal = (hitInfo.HitPosition - spheres[i].Center).normalize();
//			material = spheres[i].Material;
//		}
//	}
//	return spheresDistance < 1000;
//}
//
//Vector3 CastRay(const Vector3& origin, const Vector3& dir,std::vector<Sphere>& spheres,const std::vector<Light>& lights,int& recursiveDepth)
//{
//	RayHitInfo hit_info;
//	Material material;
//	//没有交点 返回背景颜色
//	if(recursiveDepth > MAX_RECURSIVE_COUNT || !SceneInteract(origin, dir, spheres, hit_info, material))
//	{
//		return Vector3(0.2, 0.7, 0.8);
//	}
//	//有交点 根据材质 光源计算颜色
//	float diffuseLightIntensity = 0;
//	float specularLightIntensity = 0;
//	//反射计算
//	Vector3 reflectDir = Reflect(dir, hit_info.HitNormal);
//	Vector3 reflectPoint = reflectDir.dot(hit_info.HitNormal) < 0 ?
//		hit_info.HitPosition - hit_info.HitNormal*0.001 : hit_info.HitPosition + hit_info.HitNormal*0.001;
//	Vector3 reflectColor = CastRay(reflectPoint, reflectDir, spheres, lights, ++recursiveDepth);
//
//	for(size_t i=0;i<lights.size();++i)
//	{
//		Vector3 lightDir = (lights[i].Position - hit_info.HitPosition).normalize();
//		float tnear(9999999.0f);
//		bool hasShadow(false);
//
//		//如果从交点到光源有其他物体，说明有阴影，跳过这个灯的光照
//		for(size_t j=0;j<spheres.size();j++)
//		{
//			if(spheres[j].RayIntersect(hit_info.HitPosition,(lights[i].Position - hit_info.HitPosition).normalize(), tnear))
//			{
//				hasShadow = true;
//				break;
//			}
//		}
//		if(hasShadow)
//		{
//			continue;
//		}
//		specularLightIntensity += powf(std::max(0.f, Reflect(lightDir, hit_info.HitNormal).dot(dir)), material.SpecularExp)*lights[i].Intensity;
//		diffuseLightIntensity += lights[i].Intensity * std::max(0.f, lightDir.dot(hit_info.HitNormal));
//	}
//	return material.DiffuseColor * diffuseLightIntensity*material.Albedo.x
//	+Vector3(1.f,1.f,1.f)*specularLightIntensity*material.Albedo.y
//	+reflectColor*reflectColor * material.Albedo.z;
//}
//
//void Render()
//{ 
//	//图片长宽
//	const int width = 1024;
//	const int height = 768;
//	float aspectRatio = float(width) / height;
//	//相机参数
//	Vector3 origin = Vector3(0, 0, 0); 
//	Vector3 dir = Vector3(0, 0, -1);
//	float fov = 75.f * PI / 180.f;
//	//像素在相机空间的位置
//	Vector3 pixelInWorldPos(0,0,0);
//	//材质
//	Material ivory(Vector3(0.4, 0.4, 0.3), Vector3(0.6, 0.3,0.1), 50.f);
//	Material rubber(Vector3(0.3, 0.1, 0.1), Vector3(0.9, 0.1, 0.2), 10.f);
//	Material unknown(Vector3(0.5, 0.8, 0.1), Vector3(0.2, 0.4, 0.3), 30.f);
//	Material mirror(Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0, 10.0, 0.8), 1500.f);
//	//场景中的球体
//	std::vector<Sphere> spheres {
//		Sphere(Vector3(0.7,0.7,-1.3),.3f,mirror),
//		Sphere(Vector3(0.7,-0.7,-1.7),.4f,rubber),
//		Sphere(Vector3(-0.7,-0.7,-2.8),0.5f,ivory),
//		Sphere(Vector3(-0.7,0.7,-2),.3f,mirror),
//		Sphere(Vector3(-0.1,0.2,-1.4),.3f,unknown),
//	};
//	//场景中的光源
//	std::vector<Light> lights {
//		Light(Vector3(0,0,0),1.0f),
//		Light(Vector3(-20, 20,  20), 1.5),
//		Light(Vector3(30, 50, -25), 1.8),
//		Light(Vector3(30, 20,  30), 1.7)
//	};
//	
//	std::vector<Vector3> frameBuffer(width*height);
//	int total = width * height;
//	float progressNow = 0;
//	for(size_t j=0;j<height;j++)
//	{
//		for(size_t i=0;i<width;i++)
//		{
//			pixelInWorldPos.x = (2 * (i + 0.5) / (float)width - 1) * tan(fov / 2) * aspectRatio;;
//			pixelInWorldPos.y = (1 - 2 * (j + 0.5) / (float)height) * tan(fov / 2);
//			pixelInWorldPos.z = -1.f;
//			int recursiveCount = 0;
//			Vector3 c = CastRay(origin, (pixelInWorldPos - origin).normalize(), spheres, lights,recursiveCount);
//			//如果有分量大于1 按比例缩放 如（0.5,0.5,2） -> (0.25,0.25,1)
//			float max = std::max(c[0], std::max(c[1], c[2]));
//			if (max > 1.f)
//			{
//				c = c * (1.f / max);
//			}
//			frameBuffer[i + j * width] = c;
//			//进度显示
//			if(( (i + j * width) / (float)total - progressNow ) > 0.02)
//			{
//				progressNow = (i + j * width) / (float)total;
//				std::cout << static_cast<int>(progressNow * 100) << "%\r";
//			}
//		}
//	}
//	//写入到PPM文件
//	std::cout << "Write to file....\n";
//	std::string fileName;
//	std::getline(std::cin, fileName);
//	std::ofstream ofs("./"+ fileName+".ppm",std::ios::binary);
//	ofs << "P6\n" << width << " " << height << "\n255\n";
//    //转换为char存储至ppm
//	for(size_t i=0;i<width*height;i++)
//	{
//		for(size_t j=0;j<3;j++)
//		{
//			char colorValue = (char)(255 * std::max(0.f, std::min(1.f, frameBuffer[i][j])));
//			ofs << colorValue;
//		}
//	}
//	ofs.close();
//}