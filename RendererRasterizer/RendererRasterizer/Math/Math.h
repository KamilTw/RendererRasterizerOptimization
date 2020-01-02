#pragma once
#include <algorithm>
#include <string>

const double M_PI = 3.14159265358979323846;
const float epsilon = 0.0001f;

union int3
{
	int data[3] = {0, 0, 0};
	struct
	{
		int a, b, c;
	};

	int operator[](int i)
	{
		return data[i];
	}

};

union float3
{
	float data[3];
	struct
	{
		float r, g, b;
	};
	struct
	{
		float x, y, z;
	};

	float operator[](int i)
	{
		return data[i];
	}

	float getLength()
	{
		return sqrt(x * x + y * y + z * z);
	}

	void normalize()
	{
		float len = getLength();
		if (len > epsilon)
		{
			x /= len;
			y /= len;
			z /= len;
		}
		else
		{
			x = y = z = 0.0f;
		}
	}
};

union float4
{
	float data[4] = {0, 0, 0, 1};
	struct
	{
		float r, g, b, a;
	};
	struct
	{
		float x, y, z, w;
	};

	float operator[](int i)
	{
		return data[i];
	}
};

struct float4x4
{
	float4 data[4] = { {0, 0, 0, 0},
					   {0, 0, 0, 0},
					   {0, 0, 0, 0}, 
					   {0, 0, 0, 0}
					 };

	float4 operator[](int i)
	{
		return data[i];
	}
};

struct Material
{
	float3 ka;
	float3 kd;
	float3 ks;
};

inline float3 operator-(float3 v)
{
	return float3{ -v.x, -v.y, -v.z };
}

inline float3 operator+(float3 v1, float3 v2)
{
	return float3{ v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}

inline float3 operator-(float3 v1, float3 v2)
{
	return float3{ v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}

inline float3 operator+=(float3 &v1, float3 v2)
{
	v1.x += v2.x;
	v1.y += v2.y;
	v1.z += v2.z;
	return v1;
}

inline float3 operator*(float3 v, float n)
{
	return float3{ v.x * n, v.y * n, v.z * n };
}

inline float3 operator/(float3 v, float n)
{
	return float3{ v.x / n, v.y / n, v.z / n };
}

inline float3 operator*(float3 v1, float3 v2)
{
	return float3{ v1.x * v2.x, v1.y * v2.y, v1.z * v2.z };
}

inline float4 operator+(float4 c1, float4 c2)
{
	return float4{ c1.r + c2.r, c1.g + c2.g, c1.b + c2.b, c1.a };
}

inline float4 operator*(float4 c, float n)
{
	return float4{ c.r * n, c.g * n, c.b * n, c.a };
}

inline float4 operator*(float4 v1, float4 v2)
{
	return float4{ v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.a * v2.a };
}

inline float min(float& x, float& y, float& z)
{
	return std::min(x, std::min(y, z));
}

inline float max(float& x, float& y, float& z)
{
	return std::max(x, std::max(y, z));
}

inline float3 crossProduct(float3 v1, float3 v2)
{
	return float3{ v1.y * v2.z - v2.y * v1.z,
				   v1.z * v2.x - v2.z * v1.x,
				   v1.x * v2.y - v2.x * v1.y };
}

inline float dotProduct(float3 v1, float3 v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline float dotProduct(float4 v1, float4 v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

inline float4x4 operator*(float4x4 m1, float4x4 m2)
{
	float4x4 newMatrix;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			float sum = 0;
			for (int k = 0; k < 4; k++)
			{
				sum += m1[i][k] * m2[k][j];
			}
			newMatrix.data[i].data[j] = sum;
		}
	}
	return newMatrix;
}

inline float4 operator*(float4x4 m, float4 v)
{
	float4 newVector;
	for (int i = 0; i < 4; i++)
	{
		float sum = 0;
		for (int j = 0; j < 4; j++)
		{
			sum += m[i][j] * v[j];
		}
		newVector.data[i] = sum;
	}
	return newVector;
}

inline float4 operator*(float4x4& m, float3& v)
{
	float4 vectorWithW = { v.x, v.y, v.z, 1 };
	float4 newVector;
	for (int i = 0; i < 4; i++)
	{
		float sum = 0;
		for (int j = 0; j < 4; j++)
		{
			sum += m[i][j] * vectorWithW[j];
		}
		newVector.data[i] = sum;
	}
	return newVector;
}

inline float4x4 transpose(float4x4 m)
{
	float4x4 newMatrix;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			newMatrix.data[j].data[i] = m[i][j];
		}
	}
	return newMatrix;
}

inline float3 reflect(float3 v, float3 n)
{
	 return v - n * dotProduct(n, v) * 2.0f;
}

inline float saturate(float value)
{
	return std::min(std::max(value, 0.0f), 1.0f);
}

inline float3 saturate(float3 v)
{
	return float3{ saturate(v.x), saturate(v.y), saturate(v.z) };
}