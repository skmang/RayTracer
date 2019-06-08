#pragma once
#include "Hitable.h"
#include <vector>
#include <algorithm>
#include "AABB.h"
using namespace std;
class BVHNode : public Hitable
{
public:
	BVHNode() = default;
	BVHNode(vector<Hitable*> hitableList, float time0, float time1);
	bool Hit(const Ray& r, float tMin, float tMax, HitInfo& rec) const override;
	bool BoundingBox(float t0, float t1, AABB& aabb) const override;
	Hitable* Left;
	Hitable* Right;
	AABB BoundBox;
};

BVHNode::BVHNode(vector<Hitable*> hitableList, float time0, float time1)
{
	int axis = int(3 * GetCanonical());
	if (axis == 0)
	{
		std::sort(hitableList.begin(), hitableList.end(), [](const Hitable* lhs, const Hitable* rhs)
		{
			AABB box_left, box_right;
			if (!lhs->BoundingBox(0, 0, box_left) || !rhs->BoundingBox(0, 0, box_right))
			{
				std::cerr << "Construct bvh error : No bounding box!" << std::endl;
			}
			return box_left.GetMinPoint().x() < box_right.GetMinPoint().x();
		});
	}
	else if (axis == 1)
	{
		std::sort(hitableList.begin(), hitableList.end(), [](const Hitable* lhs, const Hitable* rhs)
		{
			AABB box_left, box_right;
			if (!lhs->BoundingBox(0, 0, box_left) || !rhs->BoundingBox(0, 0, box_right))
			{
				std::cerr << "Construct bvh error : No bounding box!" << std::endl;
			}
			return box_left.GetMinPoint().y() < box_right.GetMinPoint().y();
		});
	}
	else
	{
		std::sort(hitableList.begin(), hitableList.end(), [](const Hitable* lhs, const Hitable* rhs)
		{
			AABB box_left, box_right;
			if (!lhs->BoundingBox(0, 0, box_left) || !rhs->BoundingBox(0, 0, box_right))
			{
				std::cerr << "Construct bvh error : No bounding box!" << std::endl;
			}
			return box_left.GetMinPoint().z() < box_right.GetMinPoint().z();
		});
	}

	if (hitableList.size() == 1)
	{
		Left = Right = hitableList[0];
	}
	else if (hitableList.size() == 2)
	{
		Left = hitableList[0];
		Right = hitableList[1];
	}
	else
	{
		vector<Hitable*>::const_iterator begin = hitableList.cbegin();
		vector<Hitable*>::const_iterator end = hitableList.cbegin() + hitableList.size() / 2;
		const vector<Hitable*> newListLeft(begin, end);
		const vector<Hitable*> newListRight(end, hitableList.cend());
		Left = new BVHNode(newListLeft, time0, time1);
		Right = new BVHNode(newListRight, time0, time1);
	}

	AABB boxLeft, boxRight;
	if (!Left->BoundingBox(time0, time1, boxLeft) || !Right->BoundingBox(time0, time1, boxRight))
	{
		std::cerr << "Construct bvh error : No bounding box!" << std::endl;
	}
	BoundBox = SurroundingAABB(boxLeft, boxRight);
}

bool BVHNode::Hit(const Ray& r, float tMin, float tMax, HitInfo& rec) const
{
	if (BoundBox.Intersect(r, tMin, tMax))
	{
		HitInfo leftHit, rightHit;
		bool hitLeft = Left->Hit(r, tMin, tMax, leftHit);
		bool hitRight = Right->Hit(r, tMin, tMax, rightHit);
		if (hitLeft && hitRight)
		{
			if (leftHit.t < rightHit.t)
			{
				rec = leftHit;
			}
			else
			{
				rec = rightHit;
			}
			return true;
		}

		if (hitLeft)
		{
			rec = leftHit;
			return true;
		}

		if (hitRight)
		{
			rec = rightHit;
			return true;
		}

		return false;
	}
	return false;
}


bool BVHNode::BoundingBox(float t0, float t1, ::AABB& aabb) const
{
	aabb = BoundBox;
	return true;
}
