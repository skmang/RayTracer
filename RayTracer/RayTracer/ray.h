#pragma once
#include "vec3.h"
class Ray
{
public:
	Ray() = default;
	Ray(const Vec3& o, const Vec3& d,float time) {
		_origin = o;
		_dir = d;
		_time = time;
	}

	Vec3 GetOrigin() const { return _origin; }
	Vec3 GetDirection() const { return _dir; }
	Vec3 GetPointAtParam(float t) const { return _origin + t*_dir; }
	float GetTime() const { return _time; }
private:
	Vec3 _origin;
	Vec3 _dir;
	float _time;
};

