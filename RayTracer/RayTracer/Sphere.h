#pragma once
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
	// Í¨¹ý Hitable ¼Ì³Ð
	virtual bool Hit(const Ray & r, float tMin, float tMax, HitInfo & rec) const override;
};

