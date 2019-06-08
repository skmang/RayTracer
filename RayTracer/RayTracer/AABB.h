#pragma once
#include "Utility.h"
#include "ray.h"

class AABB
{
public:
	AABB() = default;
	AABB(Vec3 min, Vec3 max)
	{
		pMin = min;
		pMax = max;
	}

	Vec3 GetMinPoint() const { return pMin; }
	Vec3 GetMaxPoint() const { return pMax; }
	bool inline Intersect(const Ray& ray, float tMin, float tMax) const
	{
		// 三维盒子实际为3个一维的判断，只要x,y,z任意一轴有相交即为相交
		for (int i = 0; i < 3; i++)
		{
			float inverseDir = 1.0f / ray.GetDirection()[i];
			float t0 = std::min(
				inverseDir* (pMin[i] - ray.GetOrigin()[i]),
				inverseDir* (pMax[i] - ray.GetOrigin()[i])
			);
			float t1 = std::max(
				inverseDir* (pMin[i] - ray.GetOrigin()[i]),
				inverseDir* (pMax[i] - ray.GetOrigin()[i])
			);
			// 方向相反，则交换大小值
			if (inverseDir < 0)
			{
				std::swap(t0, t1);
			}

			tMin = std::max(tMin, t0);
			tMax = std::min(tMax, t1);

			if (tMin >= tMax)
			{
				return true;
			}
		}

		return false;
	}
private:
	Vec3 pMin;
	Vec3 pMax;
};

AABB SurroundingAABB(const AABB& a,const AABB& b)
{
	Vec3 pMin = Vec3(
		std::min(a.GetMinPoint().x(),b.GetMinPoint().x()),
		std::min(a.GetMinPoint().y(), b.GetMinPoint().y()),
		std::min(a.GetMinPoint().z(), b.GetMinPoint().z())
	);

	Vec3 pMax = Vec3(
		std::min(a.GetMaxPoint().x(), b.GetMaxPoint().x()),
		std::min(a.GetMaxPoint().y(), b.GetMaxPoint().y()),
		std::min(a.GetMaxPoint().z(), b.GetMaxPoint().z())
	);
	return AABB(pMin, pMax);
}