#pragma once
#include "Hitable.h"
class HitableList : public Hitable
{
public:
	HitableList();
	HitableList(Hitable** l, int n) { 
		list = l;
		Count = n;
	};
	// Í¨¹ý Hitable ¼Ì³Ð
	virtual bool Hit(const Ray & r, float tMin, float tMax, HitInfo & rec) const override;
	Hitable **list;
	int Count;
};

