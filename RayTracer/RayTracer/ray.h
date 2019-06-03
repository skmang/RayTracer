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

	Vec3 origin() const { return _origin; }
	Vec3 direction() const { return _dir; }
	Vec3 point_at_t(float t) const { return _origin + _dir * t; }
private:
	Vec3 _origin;
	Vec3 _dir;
};

