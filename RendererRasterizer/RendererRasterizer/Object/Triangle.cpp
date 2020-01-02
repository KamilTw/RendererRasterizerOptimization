#include "Triangle.h"

Triangle::Triangle(float3 v1, float3 v2, float3 v3, float3 c1, float3 c2, float3 c3)
{
	v[0] = v1;
	v[1] = v2;
	v[2] = v3;
	c[0] = c1;
	c[1] = c2;
	c[2] = c3;
}