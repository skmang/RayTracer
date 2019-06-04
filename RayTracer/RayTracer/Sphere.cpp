#include "Sphere.h"

bool Sphere::Hit(const Ray & ray, float tMin, float tMax, HitInfo & rec) const
{
	Vec3 oc = ray.GetOrigin() - Center;
	float a = dot(ray.GetDirection(), ray.GetDirection());
	float b = 2.0f * dot(ray.GetDirection(), oc);
	float c = dot(oc, oc) - Radius * Radius;
	float discriminant = b * b - 4.0f*a*c;
	if (discriminant > 0) {
		float temp = (-b - sqrt(discriminant) / (2.0f * a));
		if (temp < tMax && temp > tMin) {
			rec.Point = ray.GetPointAtParam(temp);
			rec.Normal = unit_vector(rec.Point - Center);
			rec.t = temp;
			return true;
		}
		temp = (-b + sqrt(discriminant) / (2.0f * a));
		if (temp < tMax && temp > tMin) {
			rec.Point = ray.GetPointAtParam(temp);
			rec.Normal = unit_vector(rec.Point - Center);
			rec.t = temp;
			return true;
		}
		return false;
	}
	return false;
}
