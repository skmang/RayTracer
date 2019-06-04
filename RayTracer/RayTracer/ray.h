#pragma once
#include "vec3.h"
class Ray
{
public:
	Ray();
	Ray(const Vec3& o, const Vec3& d) {
		_origin = o;
		_dir = d;
	}

	Vec3 GetOrigin() const { return _origin; }
	Vec3 GetDirection() const { return _dir; }
	Vec3 GetPointAtParam(float t) const { return _origin + _dir * t; }
private:
	Vec3 _origin;
	Vec3 _dir;
};

