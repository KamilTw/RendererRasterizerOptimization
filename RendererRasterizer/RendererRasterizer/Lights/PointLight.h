#pragma once
#include "Light.h"

class PointLight : public Light
{
public:
	vector<float3> calculate(vector<float3>& fragPosition, vector<float3>& fragNormal, vector<float3>& surfacePos, int& counter);
};