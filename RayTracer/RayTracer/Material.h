#pragma once
#include "Utility.h"
#include "ray.h"
#include  "Hitable.h"
#include "Texture.h"

class Material
{
public:
	
	virtual bool Scatter(const Ray& rayIn, const HitInfo& hitInfo, Vec3& attenuation, Ray& scattered) const = 0;
	virtual Vec3 Emmit(float u,float v,const Vec3& p) const
	{
		return Vec3(0, 0, 0);
	}
};

class Lambert :public Material
{
public:
	Lambert(Texture* v) :Albedo(v) {}
	bool Scatter(const Ray& rayIn, const HitInfo& hitInfo, Vec3& attenuation, Ray& scattered) const
	{
		Vec3 target = hitInfo.Point + hitInfo.Normal + GetPointInUnitSphere();
		scattered = Ray(hitInfo.Point, target - hitInfo.Point,rayIn.GetTime()); // 随机方向的散射
		attenuation = Albedo->GetColor(0,0,hitInfo.Point);
		return true;
	}

	Texture* Albedo;
};

class Metal :public Material
{
public:
	Metal(const Vec3& v,float f) :Albedo(v)
	{
		// 最大值为1
		Fuzz = std::min(1.0f, f);
	}
	bool Scatter(const Ray& rayIn, const HitInfo& hitInfo, Vec3& attenuation, Ray& scattered) const
	{
		Vec3 reflected = reflect(unit_vector(rayIn.GetDirection()), hitInfo.Normal);
		scattered = Ray(hitInfo.Point,reflected+Fuzz*GetPointInUnitSphere(), rayIn.GetTime()); // 反射
		attenuation = Albedo;
		float v = dot(scattered.GetDirection(), hitInfo.Normal);
		//std::cout << "与金属球体相交 -> Dir: " << scattered.GetDirection() <<"Normal: "<< hitInfo.Normal << " Dot:" << v << std::endl;
		return v > 0; // 即反射方向与法线夹角小于90度
	}
	Vec3 Albedo;
	float Fuzz;
private:
	Vec3 reflect(const Vec3& v,const Vec3& n) const
	{
		return v - 2 * dot(v, n)*n;
	}
};

class DiffuseLight:public Material
{
public:
	DiffuseLight() = default;
	DiffuseLight(Texture* t):emit(t){}

	bool Scatter(const Ray& rayIn, const HitInfo& hitInfo, Vec3& attenuation, Ray& scattered) const override
	{
		return false;
	};

	Vec3 Emmit(float u,float v,const Vec3& p) const override
	{
		return emit->GetColor(u, v, p);
	}
	Texture* emit;
};