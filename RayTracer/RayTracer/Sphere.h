#pragma once
#include "Utility.h"
#include "Hitable.h"
class Sphere : public Hitable
{
public:
	Sphere() {}
	Sphere(Vec3 c,float r,Material* mat){
		Center = c;
		Radius = r;
		Mat = mat;
	}
	Vec3 Center;
	float Radius;
	Material* Mat;
	// 通过 Hitable 继承
    bool Hit(const Ray & r, float tMin, float tMax, HitInfo & rec) const override;
	bool BoundingBox(float t0, float t1, AABB& aabb) const override;
};

bool Sphere::Hit(const Ray & ray, float tMin, float tMax, HitInfo & rec) const
{
	Vec3 oc = ray.GetOrigin() - Center;
	float a = dot(ray.GetDirection(), ray.GetDirection());
	float b = dot(oc, ray.GetDirection());
	float c = dot(oc, oc) - Radius * Radius;
	float discriminant = b * b - a * c;
	rec.Material = Mat;
	if (discriminant > 0) {
		float temp = (-b - sqrt(b*b - a * c)) / a;
		if (temp < tMax && temp > tMin) {
			rec.t = temp;
			rec.Point = ray.GetPointAtParam(rec.t);
			// todo 为啥要除以半径呢

			rec.Normal = (rec.Point - Center) / Radius;
			//Vec3 unit = unit_vector((rec.Point - Center));
			//std::cout << "单位向量 " << unit << " 除以半径 " << rec.Normal << std::endl;
			return true;
		}
		temp = (-b + sqrt(b*b - a * c)) / a;
		if (temp < tMax && temp > tMin) {
			rec.t = temp;
			rec.Point = ray.GetPointAtParam(rec.t);
			rec.Normal = (rec.Point - Center) / Radius;
			//Vec3 unit = unit_vector((rec.Point - Center));
			//std::cout << "单位向量 " << unit << " 除以半径 " << rec.Normal << std::endl;
			return true;
		}
		return false;
	}
	return false;
}

inline bool Sphere::BoundingBox(float t0, float t1, AABB& aabb) const
{
	aabb = AABB(Center - Vec3(Radius), Center + Vec3(Radius));
	return true;
}


class MoveSphere : public Hitable
{
public:
	MoveSphere(){}
	MoveSphere(Vec3 center1,Vec3 center2,float moveStartTime,float moveEndTime, float r, Material* mat) {
		StartCenter = center1;
		EndCenter = center2;
		MoveStartTime = moveStartTime;
		MoveEndTime = moveEndTime;
		Radius = r;
		Mat = mat;
	}

	Vec3 GetMoveCenter(float time) const
	{
		// 限制移动范围至开始点与结束点之间
		float movedTime = std::min((time - MoveStartTime) / (MoveEndTime - MoveStartTime),1.0f);
		Vec3 center = StartCenter + (EndCenter - StartCenter)*((time - MoveStartTime) / (MoveEndTime - MoveStartTime));
		return center;
	}

	Vec3 StartCenter;
	Vec3 EndCenter;
	float MoveStartTime;
	float MoveEndTime;
	float Radius;
	Material* Mat;
	// 通过 Hitable 继承
    bool Hit(const Ray & r, float tMin, float tMax, HitInfo & rec) const override;
	bool BoundingBox(float t0, float t1, AABB& aabb) const override;
};

bool MoveSphere::Hit(const Ray &ray, float tMin, float tMax, HitInfo & rec) const
{
	Vec3 oc = ray.GetOrigin() - GetMoveCenter(ray.GetTime());
	float a = dot(ray.GetDirection(), ray.GetDirection());
	float b = dot(oc, ray.GetDirection());
	float c = dot(oc, oc) - Radius * Radius;
	float discriminant = b * b - a * c;
	rec.Material = Mat;
	if (discriminant > 0) {
		float temp = (-b - sqrt(b*b - a * c)) / a;
		if (temp < tMax && temp > tMin) {
			rec.t = temp;
			rec.Point = ray.GetPointAtParam(rec.t);
			rec.Normal = (rec.Point - GetMoveCenter(ray.GetTime())) / Radius;
			return true;
		}
		temp = (-b + sqrt(b*b - a * c)) / a;
		if (temp < tMax && temp > tMin) {
			rec.t = temp;
			rec.Point = ray.GetPointAtParam(rec.t);
			rec.Normal = (rec.Point - GetMoveCenter(ray.GetTime())) / Radius;
			return true;
		}
		return false;
	}
	return false;
}

inline bool MoveSphere::BoundingBox(float t0, float t1, AABB& aabb) const
{
	AABB a = AABB(GetMoveCenter(t0) - Vec3(Radius), GetMoveCenter(t0) + Vec3(Radius));
	AABB b = AABB(GetMoveCenter(t1) - Vec3(Radius), GetMoveCenter(t1) + Vec3(Radius));
	aabb = SurroundingAABB(a, b);
	return true;
}