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
	Material(const Vector3& color) :DiffuseColor(color){}
	Vector3 DiffuseColor;
};