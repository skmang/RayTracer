#pragma once
#include "vec3.h"
#include "Perlin.h"
class Texture
{
public:
	virtual Vec3 GetColor(float u, float v, const Vec3& point) const = 0;
};

class ConstantTexture:public Texture
{
public:
	ConstantTexture() = default;
	ConstantTexture(Vec3 c) :Color(c){}
	virtual Vec3 GetColor(float u, float v, const Vec3& point) const override
	{
		return Color;
	}
	Vec3 Color;
};

class CheckerTexture : public Texture
{
public:
	CheckerTexture() = default;
	CheckerTexture(Texture* t1, Texture* t2)
	{
		_texture1 = t1;
		_texture2 = t2;
	}
    Vec3 GetColor(float u, float v, const Vec3& point) const override
	{
		// f(x) = Sin(wx+p) ����Ϊ 2PI/w, ���ڵ�һ�뼴Ϊ�������������ӵĳߴ磬pֵ����ƫ������
		float sines = std::sin(6.283*point.x())*std::sin(6.283*point.y())*std::sin(6.283*point.z());
		if(sines > 0)
		{
			return _texture1->GetColor(u, v, point);
		}else
		{
			return _texture2->GetColor(u, v, point);
		}
	}
private:
	Texture* _texture1;
	Texture* _texture2;
};

class NoiseTexture : public Texture
{
public:
	NoiseTexture() = default;
	virtual Vec3 GetColor(float u, float v, const Vec3& point) const override
	{
		return Vec3(1, 1, 1)*Noise.Noise(point);
	}
	Perlin Noise;
};