#include <limits>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "geometry,h.h"
#include "vector3.h"
#include "light.h"
const float PI = 3.14159265358979323846f;

//�ҵ�����ཻ��һ���㣬���������Ϣ
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

Vector3 CastRay(const Vector3& origin, const Vector3& dir,std::vector<Sphere>& spheres,const std::vector<Light>& lights)
{
	RayHitInfo hit_info;
	Material material;
	//û�н��� ���ر�����ɫ
	if(!SceneInteract(origin,dir,spheres,hit_info,material))
	{
		return Vector3(0.2, 0.7, 0.8);
	}
	//�н��� ���ݲ��� ��Դ������ɫ
	float diffuseLightIntensity = 0;
	for(size_t i=0;i<lights.size();++i)
	{
		Vector3 lightDir = (lights[i].Position - hit_info.HitPosition).normalize();
		diffuseLightIntensity += lights[i].Intensity * std::max(0.f, lightDir.dot(hit_info.HitNormal));
	}
	return material.DiffuseColor * diffuseLightIntensity;
}

void Render()
{
	//ͼƬ����
	const int width = 1024;
	const int height = 768;
	float aspectRatio = float(width) / height;
	//�������
	Vector3 origin = Vector3(0, 0, 0); 
	Vector3 dir = Vector3(0, 0, -1);
	float fov = 75.f * PI / 180.f;
	//����������ռ��λ��
	Vector3 pixelInWorldPos(0,0,0);
	//�����е�����
	std::vector<Sphere> spheres {
		Sphere(Vector3(1,1,-2),.5f,Material(Vector3(.8f,.2f,.3f))),
		Sphere(Vector3(0,0,-2),.5f,Material(Vector3(.2f,.8f,.3f))),
	};
	//�����еĹ�Դ
	std::vector<Light> lights {
		Light(Vector3(0,0,0),0.5f)
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
			frameBuffer[i + j * width] = CastRay(origin,(pixelInWorldPos - origin).normalize(), spheres,lights);
			//������ʾ
			if(( (i + j * width) / (float)total - progressNow ) > 0.02)
			{
				progressNow = (i + j * width) / (float)total;
				std::cout << static_cast<int>(progressNow * 100) << "%\r";
			}
		}
	}
	//д�뵽PPM�ļ�
	std::cout << "Write to file....";
	std::ofstream ofs("./out.ppm",std::ios::binary);
	ofs << "P6\n" << width << " " << height << "\n255\n";
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