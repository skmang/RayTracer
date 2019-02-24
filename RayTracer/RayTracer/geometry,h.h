#pragma once
#include "vector3.h"
#include "iostream"
#include "Light.h"

class RayHitInfo
{
public:
	RayHitInfo():HitPosition(Vector3()),HitNormal(Vector3()){}
	RayHitInfo(Vector3& pos,Vector3& normal):HitPosition(pos),HitNormal(normal){}
	Vector3 HitPosition;
	Vector3 HitNormal;
};

class Sphere
{
public :
	Sphere(const Vector3& center,float radius,Material mat) : Center(center),Radius(radius),Material(mat){}
	Vector3 Center;
	float Radius;
	Material Material;
	bool RayIntersect(const Vector3& origin,const Vector3& dir,float &t_near) const
	{
		Vector3 originToCenter = Center - origin;
		float proj = originToCenter.dot(dir);
		if(proj < 0.0f)
		{
			return false;
		}
		float d2 = originToCenter.dot(originToCenter) - proj * proj;
		if(d2 > Radius*Radius)
		{
			return  false;
		}

		float thc = std::sqrtf(Radius*Radius - d2);

		float t0 = proj - thc;
		float t1 = proj + thc;
		if(t1 < t0)
		{
			std::swap(t0, t1);
		}

		if(t0 < 0)
		{
			t0 = t1;
			if(t1 < 0)
			{
				return false;
			}
		}

		t_near = t0;
		return true;
	}
};