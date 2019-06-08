#pragma once
#include "Hitable.h"
class HitableList : public Hitable
{
public:
	HitableList() {}
	HitableList(Hitable** l, int n) {
		list = l;
		Count = n;
	};
	// 通过 Hitable 继承
	bool Hit(const Ray & r, float tMin, float tMax, HitInfo & rec) const override;
	bool BoundingBox(float t0, float t1, AABB& aabb) const override;
	Hitable **list;
	int Count;
};


bool HitableList::Hit(const Ray & r, float tMin, float tMax, HitInfo & rec) const
{
	HitInfo hitInfo;
	bool hasHitSomeThing = false;
	double closeHit = tMax;
	for (int i = 0; i < Count; i++)
	{
		//找最近的交点
		if (list[i]->Hit(r, tMin, closeHit, hitInfo)) {
			hasHitSomeThing = true;
			closeHit = hitInfo.t;
			rec = hitInfo;
		}
	}
	return hasHitSomeThing;
}

inline bool HitableList::BoundingBox(float t0, float t1, AABB& aabb) const
{
	if (Count < 1)
	{
		return false;
	}
	// 开始构造包围体，如果有任意一个对象无法构造，则返回false
	AABB temp;
	bool firstTrue = list[0]->BoundingBox(t0, t1, temp);
	if (firstTrue == false)
	{
		return false;
	}
	else
	{
		aabb = temp;
	}

	for (int i = 1; i < Count; ++i)
	{
		if (list[i]->BoundingBox(t0, t1, temp))
		{
			aabb = SurroundingAABB(aabb, temp);
		}
		else
		{
			return false;
		}
	}
	return true;
}
