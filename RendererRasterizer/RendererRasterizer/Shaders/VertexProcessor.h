#pragma once
#include "..//Math/Math.h"
#include "..//Object/Model.h"

class VertexProcessor
{
private:
	float4x4 obj2world;
	float4x4 world2view;
	float4x4 view2proj;

public:
	float3 toProj(float3& v, float w);
	float3 toView(float3 v, float w);
	float3 toWorld(float3 v, float w);
	void lt(Model* model);
	void lt(Triangle* triangle);
	void setPerspective(float fovy, float aspect, float near, float far);
	void setLookAt(float3 eye, float3 center, float3 up);
	void setIdentity();
	void multByTranslation(float3 m);
	void multByScale(float3 v);
	void multByRotation(float a, float3 v);
};