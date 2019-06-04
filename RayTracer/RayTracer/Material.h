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
		scattered = Ray(hitInfo.Point, target - hitInfo.Point); // ��������ɢ��
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
		scattered = Ray(hitInfo.Point, Reflect(unit_vector(rayIn.GetDirection()), hitInfo.Normal)); // ����
		attenuation = Albedo;
		return dot(scattered.GetDirection(), hitInfo.Normal) > 0; // �����䷽���뷨�߼н�С��90��
	}
	Vec3 Albedo;
};