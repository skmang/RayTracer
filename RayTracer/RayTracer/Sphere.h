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
	virtual bool Hit(const Ray & r, float tMin, float tMax, HitInfo & rec) const override;
};

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
	virtual bool Hit(const Ray & r, float tMin, float tMax, HitInfo & rec) const override;

};
