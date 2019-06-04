#pragma once
#include "Ray.h"
class Camera
{
public:
	Camera() {
		Origin = Vec3(0, 0, 0);
		LeftBottom = Vec3(-2.0, -1.0, -1.0);
		Horizontal = Vec3(4.0, 0, 0);
		Vertical = Vec3(0, 2.0, 0);
	}
	Ray GetRay(float u, float v) {
		return Ray(Origin, LeftBottom + u*Horizontal + v* Vertical- Origin);
	}

	Vec3 Origin;
	Vec3 LeftBottom;
	Vec3 Horizontal;
	Vec3 Vertical;
};

