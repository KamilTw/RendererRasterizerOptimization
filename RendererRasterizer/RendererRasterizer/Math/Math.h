#pragma once
#include <algorithm>
#include <string>
#include <vector>
#include <intrin.h>

using namespace std;

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
	__m128 mmv;
	//float data[3];
	struct
	{
		float r, g, b;
	};
	struct
	{
		float x, y, z;
	};
	struct
	{
		float data[3];
	};

	float3() : mmv(_mm_setzero_ps()) {};
	float3(float x, float y, float z) : mmv(_mm_set_ps(0, z, y, x)) {}
	float3(__m128 m) : mmv(m) {};

	float operator[](int i)
	{
		return data[i];
	}

	float getLength()
	{
		return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(mmv, mmv, 0x71)));
		//return sqrt(x * x + y * y + z * z);
	}

	void normalize()
	{
		//mmv = _mm_div_ps(mmv, _mm_sqrt_ps(_mm_dp_ps(mmv, mmv, 0x7F)));

		// fast normalization
		mmv = _mm_mul_ps(mmv, _mm_rsqrt_ps(_mm_dp_ps(mmv, mmv, 0x77)));

		/*float len = getLength();
		if (len > epsilon)
		{
			x /= len;
			y /= len;
			z /= len;
		}
		else
		{
			x = y = z = 0.0f;
		}*/
	}
};

union float4
{
	__m128 mmv;
	//float data[4] = {0, 0, 0, 1};
	struct
	{
		float r, g, b, a;
	};
	struct
	{
		float x, y, z, w;
	};
	struct
	{
		float data[4];
	};

	float4() : mmv(_mm_setzero_ps()) {};
	float4(float x, float y, float z, float w) : mmv(_mm_set_ps(w, z, y, x)) {}
	float4(__m128 m) : mmv(m) {};

	float operator[](int i)
	{
		//return _mm_cvtss_f32(_mm_shuffle_ps(mmv, mmv, _MM_SHUFFLE(i, i, i, i)));

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
	return _mm_mul_ps(v.mmv, _mm_set1_ps(-1.0f));
	//return float3{ -v.x, -v.y, -v.z };
}

inline float3 operator+(float3 v1, float3 v2)
{
	return _mm_add_ps(v1.mmv, v2.mmv);
	//return float3{ v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}

inline float3 operator-(float3 v1, float3 v2)
{
	return _mm_sub_ps(v1.mmv, v2.mmv);
	//return float3{ v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}

inline float3 operator+=(float3 &v1, float3 v2)
{
	return _mm_add_ps(v1.mmv, v2.mmv);
	/*v1.x += v2.x;
	v1.y += v2.y;
	v1.z += v2.z;
	return v1;*/
}

inline float3 operator*(float3 v, float n)
{
	return _mm_mul_ps(v.mmv, _mm_set1_ps(n));
	//return float3{ v.x * n, v.y * n, v.z * n };
}

inline float3 operator/(float3 v, float n)
{
	return _mm_div_ps(v.mmv, _mm_set1_ps(n));
	//return float3{ v.x / n, v.y / n, v.z / n };
}

inline float3 operator*(float3 v1, float3 v2)
{
	return _mm_mul_ps(v1.mmv, v2.mmv);
	//return float3{ v1.x * v2.x, v1.y * v2.y, v1.z * v2.z };
}

inline float4 operator+(float4 c1, float4 c2)
{
	return _mm_add_ps(c1.mmv, c2.mmv);
	//return float4{ c1.r + c2.r, c1.g + c2.g, c1.b + c2.b, c1.a };
}

inline float4 operator*(float4 c, float n)
{
	return _mm_mul_ps(c.mmv, _mm_set1_ps(n));
	//return float4{ c.r * n, c.g * n, c.b * n, c.a };
}

inline float4 operator*(float4 v1, float4 v2)
{
	return _mm_mul_ps(v1.mmv, v2.mmv);
	//return float4{ v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.a * v2.a };
}

inline float min(float& x, float& y, float& z)
{
	return _mm_cvtss_f32(_mm_min_ps(_mm_set1_ps(x), _mm_min_ps(_mm_set1_ps(y), _mm_set1_ps(z))));

	//return std::min(x, std::min(y, z));
}

inline float max(float& x, float& y, float& z)
{
	return _mm_cvtss_f32(_mm_max_ps(_mm_set1_ps(x), _mm_max_ps(_mm_set1_ps(y), _mm_set1_ps(z))));

	//return std::max(x, std::max(y, z));
}

inline float3 crossProduct(float3 v1, float3 v2)
{
	return _mm_sub_ps(
		_mm_mul_ps(
			_mm_shuffle_ps(v1.mmv, v1.mmv, _MM_SHUFFLE(3, 0, 2, 1)),
			_mm_shuffle_ps(v2.mmv, v2.mmv, _MM_SHUFFLE(3, 1, 0, 2))),
		_mm_mul_ps(
			_mm_shuffle_ps(v1.mmv, v1.mmv, _MM_SHUFFLE(3, 1, 0, 2)),
			_mm_shuffle_ps(v2.mmv, v2.mmv, _MM_SHUFFLE(3, 0, 2, 1)))
	);

	/*return float3{ v1.y * v2.z - v2.y * v1.z,
				   v1.z * v2.x - v2.z * v1.x,
				   v1.x * v2.y - v2.x * v1.y };*/
}

inline float dotProduct(float3 v1, float3 v2)
{
	return _mm_cvtss_f32(_mm_dp_ps(v1.mmv, v2.mmv, 0x71));
	//return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline float dotProduct(float4 v1, float4 v2)
{
	return _mm_cvtss_f32(_mm_dp_ps(v1.mmv, v2.mmv, 0x71));
	//return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
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
	__m128 x = _mm_loadu_ps((const float*)&v);
	__m128 A0 = _mm_loadu_ps((const float*)&m.data[0]);
	__m128 A1 = _mm_loadu_ps((const float*)&m.data[1]);
	__m128 A2 = _mm_loadu_ps((const float*)&m.data[2]);
	__m128 A3 = _mm_loadu_ps((const float*)&m.data[3]);

	__m128 m0 = _mm_mul_ps(A0, x);
	__m128 m1 = _mm_mul_ps(A1, x);
	__m128 m2 = _mm_mul_ps(A2, x);
	__m128 m3 = _mm_mul_ps(A3, x);

	__m128 sum_01 = _mm_hadd_ps(m0, m1);
	__m128 sum_23 = _mm_hadd_ps(m2, m3);
	__m128 result = _mm_hadd_ps(sum_01, sum_23);

	float4 r;
	_mm_storeu_ps((float*)&r, result);

	return r;

	/*float4 newVector;
	for (int i = 0; i < 4; i++)
	{
		float sum = 0;
		for (int j = 0; j < 4; j++)
		{
			sum += m[i][j] * v[j];
		}
		newVector.data[i] = sum;
	}

	return newVector;*/
}

inline vector<float4> mulN(float4x4& m, vector<float4>& v, int N)
{
	vector<float4> r;
	r.reserve(N);

	__m128 A0 = _mm_loadu_ps((const float*)&m.data[0]);
	__m128 A1 = _mm_loadu_ps((const float*)&m.data[1]);
	__m128 A2 = _mm_loadu_ps((const float*)&m.data[2]);
	__m128 A3 = _mm_loadu_ps((const float*)&m.data[3]);

	for (int i = 0; i < N; ++i)
	{
		__m128 x = _mm_loadu_ps((const float*)&v[i]);

		__m128 m0 = _mm_mul_ps(A0, x);
		__m128 m1 = _mm_mul_ps(A1, x);
		__m128 m2 = _mm_mul_ps(A2, x);
		__m128 m3 = _mm_mul_ps(A3, x);

		__m128 sum_01 = _mm_hadd_ps(m0, m1);
		__m128 sum_23 = _mm_hadd_ps(m2, m3);
		__m128 result = _mm_hadd_ps(sum_01, sum_23);

		_mm_storeu_ps((float*)&r[i], result);
	}

	return r;
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
	float3 v = _mm_max_ps(_mm_set1_ps(value), _mm_set1_ps(0.0f));
	v = _mm_min_ps(_mm_set1_ps(v.x), _mm_set1_ps(1.0f));

	return v.x;
	//return std::min(std::max(value, 0.0f), 1.0f);
}

inline float3 saturate(float3 v)
{
	return float3{ saturate(v.x), saturate(v.y), saturate(v.z) };
}