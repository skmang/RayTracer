#include "Sphere.h"

bool Sphere::Hit(const Ray & ray, float tMin, float tMax, HitInfo & rec) const
{
	Vec3 oc = ray.GetOrigin() - Center;
	float a = dot(ray.GetDirection(), ray.GetDirection());
	float b = dot(oc,ray.GetDirection());
	float c = dot(oc, oc) - Radius * Radius;
	float discriminant = b * b -a*c;
	rec.Material = Mat;
	if (discriminant > 0) {
		float temp =(-b - sqrt(b*b-a*c))/a;
		if (temp < tMax && temp > tMin) {
			rec.t = temp;
			rec.Point = ray.GetPointAtParam(rec.t);
			// todo 为啥要除以半径呢

			rec.Normal = (rec.Point - Center)/Radius;
			//Vec3 unit = unit_vector((rec.Point - Center));
			//std::cout << "单位向量 " << unit << " 除以半径 " << rec.Normal << std::endl;
			return true;
		}
		temp = (-b + sqrt(b*b-a*c)) / a;
		if (temp < tMax && temp > tMin) {
			rec.t = temp;
			rec.Point = ray.GetPointAtParam(rec.t);
			rec.Normal = (rec.Point - Center)/Radius;
			//Vec3 unit = unit_vector((rec.Point - Center));
			//std::cout << "单位向量 " << unit << " 除以半径 " << rec.Normal << std::endl;
			return true;
		}
		return false;
	}
	return false;
}
