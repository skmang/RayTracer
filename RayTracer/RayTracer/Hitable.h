#pragma once
#ifndef HITABLE
#define HITABLE
#include "Ray.h"
#include "AABB.h"

class Material;

struct HitInfo {
	float t;
	Vec3 Point;
	Vec3 Normal;
	Material* Material;
};

class Hitable
{
public:
	virtual bool Hit(const Ray& r, float tMin, float tMax, HitInfo&  rec) const = 0;
	virtual bool BoundingBox(float t0, float t1, AABB& aabb) const = 0;
};


#endif // !HITABLE