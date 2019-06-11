#pragma once
#include "Utility.h"
// 柏林噪声这一块的算法不是很懂.....
class Perlin
{
public:
	float Noise(const Vec3& p) const
	{
		float u = p.x() - floor(p.x());
		float v = p.y() - floor(p.y());
		float w = p.z() - floor(p.z());
		int i = floor(p.x());
		int j = floor(p.y());
		int k = floor(p.z());

		Vec3 c[2][2][2];
		for(int x=0;x<2;x++)
		{
			for(int y=0;y<2;y++)
			{
				for(int z=0;z<2;z++)
				{
					c[x][y][z] = RanVec3[PermuteX[(i + x) & 255] ^ PermuteX[(j + y) & 255] ^ PermuteX[(k + z) & 255]];
				}
			}
		}
		// 按位异或 相异的为1
		return TrilinearInterpolate(c,u,v,w);
	}

	float Turb(const Vec3& p, int depth = 7) const
	{
		float accum = 0;
		Vec3 temp_p = p;
		float weight = 1.f;
		for (int i=0;i<depth;i++)
		{
			accum += weight * Noise(temp_p);
			weight *= 0.5;
			temp_p *= 2;
		}
		return fabs(accum);
	}
	static float *RanFloat;
	static int* PermuteX;
	static int* PermuteY;
	static int* PermuteZ;
	static Vec3 *RanVec3;
private:
	float TrilinearInterpolate(Vec3 c[2][2][2],float u,float v,float w) const
	{
		float uu = u * u*(3 - 2 * u);
		float vv = v * v*(3 - 2 * v);
		float ww = w * w*(3 - 2 * w);
		float accum = 0;
		for (int i=0;i<2;i++)
		{
			for (int j=0;j<2;j++)
			{
				for(int k=0;k<2;k++)
				{
					Vec3 weigt(u - i, v - j, w - k);
					accum += (i*uu + (1 - i)*(1 - uu))*(j*vv + (1 - j)*(1 - vv))*(k*ww + (1 - k)*(1 - ww))*dot(c[i][j][k],weigt);
				}
			}
		}
		return accum;
	}
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

static Vec3* PerlinVecGenerate()
{
	Vec3* p = new Vec3[256];
	for (int i=0;i<256;i++)
	{
		p[i] = unit_vector(Vec3(2 * GetCanonical() - 1, 2 * GetCanonical() - 1, 2 * GetCanonical() - 1));
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
Vec3* Perlin::RanVec3 = PerlinVecGenerate();
int * Perlin::PermuteX = PerlinGeneratePermute();
int * Perlin::PermuteY = PerlinGeneratePermute();
int * Perlin::PermuteZ = PerlinGeneratePermute();