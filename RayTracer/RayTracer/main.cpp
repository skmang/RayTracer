#include <limits>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "geometry,h.h"
#include "vector3.h"
#include "light.h"
#include <string>
const float PI = 3.14159265358979323846f;
const int MAX_RECURSIVE_COUNT = 2;
//反射的向量计算
Vector3 Reflect(const Vector3& in,const Vector3& normal)
{
	return in - 2 * (in.dot(normal))*normal;
}

//找到最近相交的一个点，储存相关信息
bool SceneInteract(const Vector3& origin, const Vector3& dir, std::vector<Sphere>& spheres, RayHitInfo& hitInfo, Material& material)
{
	float spheresDistance = std::numeric_limits<float>::max();
	float currentDist;
	for (size_t i = 0; i < spheres.size(); ++i)
	{
		if (spheres[i].RayIntersect(origin, dir, currentDist) && currentDist < spheresDistance)
		{
			spheresDistance = currentDist;
			hitInfo.HitPosition = origin + currentDist * dir;
			hitInfo.HitNormal = (hitInfo.HitPosition - spheres[i].Center).normalize();
			material = spheres[i].Material;
		}
	}
	return spheresDistance < 1000;
}

Vector3 CastRay(const Vector3& origin, const Vector3& dir,std::vector<Sphere>& spheres,const std::vector<Light>& lights,int& recursiveDepth)
{
	RayHitInfo hit_info;
	Material material;
	//没有交点 返回背景颜色
	if(recursiveDepth > MAX_RECURSIVE_COUNT || !SceneInteract(origin, dir, spheres, hit_info, material))
	{
		return Vector3(0.2, 0.7, 0.8);
	}
	//有交点 根据材质 光源计算颜色
	float diffuseLightIntensity = 0;
	float specularLightIntensity = 0;
	//反射计算
	Vector3 reflectDir = Reflect(dir, hit_info.HitNormal);
	Vector3 reflectPoint = reflectDir.dot(hit_info.HitNormal) < 0 ?
		hit_info.HitPosition - hit_info.HitNormal*0.001 : hit_info.HitPosition + hit_info.HitNormal*0.001;
	Vector3 reflectColor = CastRay(reflectPoint, reflectDir, spheres, lights, ++recursiveDepth);

	for(size_t i=0;i<lights.size();++i)
	{
		Vector3 lightDir = (lights[i].Position - hit_info.HitPosition).normalize();
		float tnear(9999999.0f);
		bool hasShadow(false);

		//如果从交点到光源有其他物体，说明有阴影，跳过这个灯的光照
		for(size_t j=0;j<spheres.size();j++)
		{
			if(spheres[j].RayIntersect(hit_info.HitPosition,(lights[i].Position - hit_info.HitPosition).normalize(), tnear))
			{
				hasShadow = true;
				break;
			}
		}
		if(hasShadow)
		{
			continue;
		}
		specularLightIntensity += powf(std::max(0.f, Reflect(lightDir, hit_info.HitNormal).dot(dir)), material.SpecularExp)*lights[i].Intensity;
		diffuseLightIntensity += lights[i].Intensity * std::max(0.f, lightDir.dot(hit_info.HitNormal));
	}
	return material.DiffuseColor * diffuseLightIntensity*material.Albedo.x
	+Vector3(1.f,1.f,1.f)*specularLightIntensity*material.Albedo.y
	+reflectColor*reflectColor * material.Albedo.z;
}

void Render()
{ 
	//图片长宽
	const int width = 1024;
	const int height = 768;
	float aspectRatio = float(width) / height;
	//相机参数
	Vector3 origin = Vector3(0, 0, 0); 
	Vector3 dir = Vector3(0, 0, -1);
	float fov = 75.f * PI / 180.f;
	//像素在相机空间的位置
	Vector3 pixelInWorldPos(0,0,0);
	//材质
	Material ivory(Vector3(0.4, 0.4, 0.3), Vector3(0.6, 0.3,0.1), 50.f);
	Material rubber(Vector3(0.3, 0.1, 0.1), Vector3(0.9, 0.1, 0.2), 10.f);
	Material unknown(Vector3(0.5, 0.8, 0.1), Vector3(0.2, 0.4, 0.3), 30.f);
	Material mirror(Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0, 10.0, 0.8), 1500.f);
	//场景中的球体
	std::vector<Sphere> spheres {
		Sphere(Vector3(0.7,0.7,-1.3),.3f,mirror),
		Sphere(Vector3(0.7,-0.7,-1.7),.4f,rubber),
		Sphere(Vector3(-0.7,-0.7,-2.8),0.5f,ivory),
		Sphere(Vector3(-0.7,0.7,-2),.3f,mirror),
		Sphere(Vector3(-0.1,0.2,-1.4),.3f,unknown),
	};
	//场景中的光源
	std::vector<Light> lights {
		Light(Vector3(0,0,0),1.0f),
		Light(Vector3(-20, 20,  20), 1.5),
		Light(Vector3(30, 50, -25), 1.8),
		Light(Vector3(30, 20,  30), 1.7)
	};
	
	std::vector<Vector3> frameBuffer(width*height);
	int total = width * height;
	float progressNow = 0;
	for(size_t j=0;j<height;j++)
	{
		for(size_t i=0;i<width;i++)
		{
			pixelInWorldPos.x = (2 * (i + 0.5) / (float)width - 1) * tan(fov / 2) * aspectRatio;;
			pixelInWorldPos.y = (1 - 2 * (j + 0.5) / (float)height) * tan(fov / 2);
			pixelInWorldPos.z = -1.f;
			int recursiveCount = 0;
			Vector3 c = CastRay(origin, (pixelInWorldPos - origin).normalize(), spheres, lights,recursiveCount);
			//如果有分量大于1 按比例缩放 如（0.5,0.5,2） -> (0.25,0.25,1)
			float max = std::max(c[0], std::max(c[1], c[2]));
			if (max > 1.f)
			{
				c = c * (1.f / max);
			}
			frameBuffer[i + j * width] = c;
			//进度显示
			if(( (i + j * width) / (float)total - progressNow ) > 0.02)
			{
				progressNow = (i + j * width) / (float)total;
				std::cout << static_cast<int>(progressNow * 100) << "%\r";
			}
		}
	}
	//写入到PPM文件
	std::cout << "Write to file....\n";
	std::string fileName;
	std::getline(std::cin, fileName);
	std::ofstream ofs("./"+ fileName+".ppm",std::ios::binary);
	ofs << "P6\n" << width << " " << height << "\n255\n";
    //转换为char存储至ppm
	for(size_t i=0;i<width*height;i++)
	{
		for(size_t j=0;j<3;j++)
		{
			char colorValue = (char)(255 * std::max(0.f, std::min(1.f, frameBuffer[i][j])));
			ofs << colorValue;
		}
	}
	ofs.close();
}

int main()
{
	Render();
	return 0;
}