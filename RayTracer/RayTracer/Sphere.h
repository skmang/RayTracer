#pragma once
#include "Hitable.h"
class Sphere : public Hitable
{
public:
	Sphere() {}
	Sphere(Vec3 c,float r){
		Center = c;
		Radius = r;
	}
	Vec3 Center;
	float Radius;

	// Í¨¹ý Hitable ¼Ì³Ð
	virtual bool Hit(const Ray & r, float tMin, float tMax, HitInfo & rec) const override;
};

