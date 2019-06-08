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
	// ͨ�� Hitable �̳�
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
		//������Ľ���
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
	// ��ʼ�����Χ�壬���������һ�������޷����죬�򷵻�false
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
