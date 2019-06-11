#pragma once
#ifndef HITABLE
#define HITABLE
#include "Ray.h"
#include "AABB.h"

class Material;

struct HitInfo {
	float t;
	float u;
	float v;
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


class RectangleXY :public Hitable
{
public:
	RectangleXY(float _x0,float _x1,float _y0,float _y1,float _z,Material* mat)
	{
		x0 = _x0;
		x1 = _x1;
		y0 = _y0;
		y1 = _y1;
		z = _z;
		Mat = mat;
	}

	bool Hit(const Ray& r, float tMin, float tMax, HitInfo& rec) const override
	{
		float t = (z - r.GetOrigin().z()) / r.GetDirection().z();
		if(t<tMin || t>tMax)
		{
			return false;
		}
		float x = r.GetOrigin().x() + t * r.GetDirection().x();
		float y = r.GetOrigin().y() + t * r.GetDirection().y();
		if(x < x0||x>x1||y<y0||y>y1)
		{
			return false;
		}
		rec.u = (x - x0) / (x1 - x0);
		rec.v = (y - y0) / (y1 - y0);
		rec.t = t;
		rec.Material = Mat;
		rec.Point = r.GetPointAtParam(t);
		rec.Normal = (0, 0, 1);
		return true;
	}
	bool BoundingBox(float t0, float t1, AABB& aabb) const override
	{
		aabb = AABB(Vec3(x0, y0, z - 0.0001), Vec3(x1, y1, z + 0.0001));
		return true;
	};
	float x0, x1, y0, y1, z;
	Material* Mat;
};

#endif // !HITABLE