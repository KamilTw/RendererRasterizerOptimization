#pragma once
#include <vector>
#include "..//Math/Math.h"
#include "..//Lights/Light.h"

class Fragment
{
private:
	float3 position;
	float3 normal;
	std::vector<Light*> lights = std::vector<Light*>();

public:
	float3 calculate(float3& fragPosition, float3& fragNormal, VertexProcessor& vertexProcessor);
	float3 getPosition();
	float3 getNormal();
	void setPosition(float3 position);
	void setNormal(float3 normal);
	void addLight(Light *light);
};