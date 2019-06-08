#pragma once
#include "Utility.h"
#include "ray.h"
#include  "Hitable.h"

class Material
{
public:
	virtual bool Scatter(const Ray& rayIn, const HitInfo& hitInfo, Vec3& attenuation, Ray& scattered) const = 0;
};

class Lambert :public Material
{
public:
	Lambert(const Vec3& v) :Albedo(v) {}
	bool Scatter(const Ray& rayIn, const HitInfo& hitInfo, Vec3& attenuation, Ray& scattered) const
	{
		Vec3 target = hitInfo.Point + hitInfo.Normal + GetPointInUnitSphere();
		scattered = Ray(hitInfo.Point, target - hitInfo.Point,rayIn.GetTime()); // ��������ɢ��
		attenuation = Albedo;
		return true;
	}

	Vec3 Albedo;
};

class Metal :public Material
{
public:
	Metal(const Vec3& v,float f) :Albedo(v)
	{
		// ���ֵΪ1
		Fuzz = std::min(1.0f, f);
	}
	bool Scatter(const Ray& rayIn, const HitInfo& hitInfo, Vec3& attenuation, Ray& scattered) const
	{
		Vec3 reflected = reflect(unit_vector(rayIn.GetDirection()), hitInfo.Normal);
		scattered = Ray(hitInfo.Point,reflected+Fuzz*GetPointInUnitSphere(), rayIn.GetTime()); // ����
		attenuation = Albedo;
		float v = dot(scattered.GetDirection(), hitInfo.Normal);
		//std::cout << "����������ཻ -> Dir: " << scattered.GetDirection() <<"Normal: "<< hitInfo.Normal << " Dot:" << v << std::endl;
		return v > 0; // �����䷽���뷨�߼н�С��90��
	}
	Vec3 Albedo;
	float Fuzz;
private:
	Vec3 reflect(const Vec3& v,const Vec3& n) const
	{
		return v - 2 * dot(v, n)*n;
	}
};