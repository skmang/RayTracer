#pragma once
#include <math.h>
#include "ray.h"
#include "Utility.h"
#include "ConstDef.h"
class Camera
{
public:
	Camera(float vfov,float aspectRatio,Vec3 vup,Vec3 lookFrom,Vec3 lookAt,float aperture,float focusDist) {
		LenRadius = aperture / 2.0;
		VerticalFOV = vfov;
		AspectRatio = aspectRatio;
		HalfHeight = tan(vfov*PI/360) * focusDist;
		HalfWidth = HalfHeight * aspectRatio;
		Origin = lookFrom;
		W = unit_vector(lookFrom - lookAt);
		U = unit_vector(cross(vup,W));
		V = cross(W, U);

		LeftBottom = Origin - W*focusDist - U * HalfWidth - V * HalfHeight;
		Horizontal = 2 * HalfWidth*U;
		Vertical = 2 * HalfHeight*V;
	}

	Ray GetRay(float s, float t) {
		Vec3 rd = LenRadius * GetPointInUnitCircle();
		Vec3 offset = U*rd.x() + V*rd.y();
		return Ray(Origin+offset, LeftBottom + s*Horizontal + t* Vertical- Origin-offset);
	}

	Vec3 Origin;
	Vec3 LeftBottom;
	Vec3 Horizontal;
	Vec3 Vertical;
	float VerticalFOV;
	float AspectRatio;
	float HalfWidth;
	float HalfHeight;
	float LenRadius;

	Vec3 U;
	Vec3 V;
	Vec3 W;

};

