#pragma once
#include "Utility.h"
class Perlin
{
public:
	float Noise(const Vec3& p) const
	{
		float u = p.x() - floor(p.x());
		float v = p.y() - floor(p.y());
		float w = p.z() - floor(p.z());
		int i = int(4 * p.x()) & 255; // 255 = 1111 1111 ,小于255的数会保持不变
		int j = int(4 * p.y()) & 255;
		int k = int(4 * p.z()) & 255;
		// 按位异或 相异的为1
		return RanFloat[PermuteX[i]^PermuteY[j]^PermuteZ[k]];
	}

	static float *RanFloat;
	static int* PermuteX;
	static int* PermuteY;
	static int* PermuteZ;
};

static float* PerlinGenerate()
{
	float * p = new float[256];
	for(int i=0;i<256;i++)
	{
		p[i] = GetCanonical();
	}
	return p;
}

// 打乱数组
void Permute(int *p,int n)
{
	for (int i=n-1;i>=0;i--)
	{
		int target = int(GetCanonical()*(i + 1));
		int tmp = p[i];
		p[i] = p[target];
		p[target] = tmp;
	}
}

static int * PerlinGeneratePermute()
{
	int* p = new int[256];
	for (int i=0;i<256;i++)
	{
		p[i] = i;
	}
	Permute(p, 256);
	return p;
}

float* Perlin::RanFloat = PerlinGenerate();
int * Perlin::PermuteX = PerlinGeneratePermute();
int * Perlin::PermuteY = PerlinGeneratePermute();
int * Perlin::PermuteZ = PerlinGeneratePermute();