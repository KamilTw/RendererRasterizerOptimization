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
	vector<float3> calculate(vector<float3>& fragPosition, vector<float3>& fragNormal, vector<float3>& surfacePos, int& counter);
	float3 getPosition();
	float3 getNormal();
	void setPosition(float3 position);
	void setNormal(float3 normal);
	void addLight(Light *light);
};