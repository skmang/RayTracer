#pragma once
#include "Utility.h"
#include "ray.h"
#include  "Hitable.h"
class Material
{
public:
	virtual bool Scatter(const Ray& rayIn, const HitInfo& hitInfo, Vec3& attenuation, Ray& scattered);
};

class Lambert:public Material
{
public:
	Lambert(const Vec3& v) :Albedo(v) {}
	bool Scatter(const Ray& rayIn, const HitInfo& hitInfo, Vec3& attenuation, Ray& scattered) override
	{
		Vec3 target = hitInfo.Point + hitInfo.Normal + GetPointInUnitSphere();
		scattered = Ray(hitInfo.Point, target - hitInfo.Point); // 随机方向的散射
		attenuation = Albedo;
		return true;
	}

	Vec3 Albedo;
};

class Metal:public Material
{
public:
	Metal(const Vec3& v):Albedo(v){}
	bool Scatter(const Ray& rayIn, const HitInfo& hitInfo, Vec3& attenuation, Ray& scattered) override
	{
		scattered = Ray(hitInfo.Point, Reflect(unit_vector(rayIn.GetDirection()), hitInfo.Normal)); // 反射
		attenuation = Albedo;
		return dot(scattered.GetDirection(), hitInfo.Normal) > 0; // 即反射方向与法线夹角小于90度
	}
	Vec3 Albedo;
};