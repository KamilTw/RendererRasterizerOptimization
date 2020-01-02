#include "VertexProcessor.h"
#include <iostream>

float3 VertexProcessor::toProj(float3& v, float w)
{
	float4 r = view2proj * world2view * obj2world * float4{ v.x, v.y, v.z, w };;

	if (w == 1)
	{
		return float3{ r.x / r.w , r.y / r.w , r.z / r.w };
	}
	else
	{
		return float3{ r.x , r.y, r.z };
	}
}

float3 VertexProcessor::toView(float3 v, float w)
{
	//float4 r = world2view * obj2world * float4{ v.x, v.y, v.z, w };
	float4 r = world2view * float4{ v.x, v.y, v.z, w };

	if (w == 1)
	{
		return float3{ r.x / r.w , r.y / r.w , r.z / r.w };
	}
	else
	{
		return float3{ r.x , r.y, r.z };
	}
}

float3 VertexProcessor::toWorld(float3 v, float w)
{
	float4 r = obj2world * float4{ v.x, v.y, v.z, w };;

	if (w == 1)
	{
		return float3{ r.x / r.w , r.y / r.w , r.z / r.w };
	}
	else
	{
		return float3{ r.x , r.y, r.z };
	}
}

void VertexProcessor::lt(Model *model)
{
	for (int i = 0; i < model->getVerticesAmount(); i++)
	{
		model->vertices[i] = toProj(model->vertices[i], 1);
		model->normals[i] = toProj(model->normals[i], 0);
		model->normals[i].normalize();
	}
}

void VertexProcessor::lt(Triangle* triangle)
{
	for (int i = 0; i < 3; i++)
	{
		triangle->v[i] = toProj(triangle->v[i], 1);
	}
}

void VertexProcessor::setPerspective(float fovy, float aspect, float near, float far)
{
	fovy *= M_PI / 360;		// FOVy / 2
	float f = cos(fovy) / sin(fovy);

	view2proj = { { {f / aspect,	0,		0,								0},
					{0,				f,		0,								0},
					{0,				0,		(far + near) / (near - far),	2 * far * near / (near - far)},
					{0,				0,		-1,								0}
				} };
	// -1 affects W coordinate based on Z vector value
	// W is a distance between camera Z and vertice Z
}

void VertexProcessor::setLookAt(float3 eye, float3 center, float3 up)
{
	float3 f = center - eye;
	f.normalize();
	up.normalize();
	float3 s = crossProduct(f, up);
	float3 u = crossProduct(s, f);

	world2view = { { {s.x,		s.y,	s.z,	-eye.x},
					 {u.x,		u.y,	u.z,	-eye.y},
					 {-f.x,		-f.y,	-f.z,	-eye.z},
					 {0,		0,		0,		1}
				} };
}

void VertexProcessor::setIdentity()
{
	obj2world = { { {1, 0, 0, 0},
					{0, 1, 0, 0},
					{0, 0, 1, 0},
					{0, 0, 0, 1}
				} };
}

void VertexProcessor::multByTranslation(float3 v)
{
	float4x4 m = { { {1, 0, 0, v.x},
					 {0, 1, 0, v.y},
					 {0, 0, 1, v.z},
					 {0, 0, 0, 1}
				 } };

	obj2world = m * obj2world;
}

void VertexProcessor::multByScale(float3 v)
{
	float4x4 m = { { {v.x, 0,   0,   0},
					 {0,   v.y, 0,   0},
					 {0,   0,   v.z, 0},
					 {0,   0,   0,   1}
				 } };

	obj2world = m * obj2world;
}

void VertexProcessor::multByRotation(float a, float3 v)
{
	float radians = (a * M_PI) / 180;
	float s = sin(radians);
	float c = cos(radians);

	float4x4 m = { { {v.x * v.x * (1 - c) + c,			   v.x * v.y * (1 - c) - v.z * s,		v.x * v.z * (1 - c) + v.y * s,	 0},
					 {v.y * v.x * (1 - c) + v.z * s,	   v.y * v.y * (1 - c) + c,				v.y * v.z * (1 - c) - v.x * s,	 0},
					 {v.x * v.z * (1 - c) - v.y * s,	   v.y * v.z * (1 - c) + v.x * s,		v.z * v.z * (1 - c) + c,		 0},
					 {0,								   0,									0,								 1}
				 } };

	obj2world = m * obj2world;
}