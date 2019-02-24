#pragma once
#include "vector3.h"
class Light
{
public:
	Light(const Vector3& pos,const float& i):Position(pos),Intensity(i){}
	Vector3 Position;
	float Intensity;
};

class Material
{
public :
	Material():DiffuseColor(Vector3(0,0,0)){}
	Material(const Vector3& color,const Vector3& albedo,const float& spec) :DiffuseColor(color),Albedo(albedo),SpecularExp(spec){}
	Vector3 DiffuseColor;
	Vector3 Albedo;
	float SpecularExp;
};