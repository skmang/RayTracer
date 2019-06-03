#include "HitableList.h"

HitableList::HitableList()
{
}

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