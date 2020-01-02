#pragma once
#include "Light.h"

class DirectionalLight : public Light
{
public:
	float3 calculate(float3& fragPosition, float3& fragNormal, VertexProcessor& vertexProcessor);
};