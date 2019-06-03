#pragma once
#ifndef HITABLE
#define HITABLE
#include "ray.h"

struct HitInfo {
	float t;
	Vec3 Point;
	Vec3 Normal;
};

class Hitable
{
public:
	virtual bool Hit(const Ray& r, float tMin, float tMax, HitInfo&  rec) const = 0;
};


#endif // !HITABLE