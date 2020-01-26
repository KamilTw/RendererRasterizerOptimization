#include "Fragment.h"

vector<float3> Fragment::calculate(vector<float3>& fragPosition, vector<float3>& fragNormal, vector<float3>& surfacePos, int& counter)
{
	vector<vector<float3>> lightColors;
	lightColors.resize(lights.size());

	vector<float3> allLightColor;
	allLightColor.resize(counter);

	for (int i = 0; i < lights.size(); ++i)
	{
		lightColors[i] = lights[i]->calculate(fragPosition, fragNormal, surfacePos, counter);	
	}

	for (int i = 0; i < lights.size(); ++i)
	{
		for (int j = 0; j < counter; ++j)
		{
			allLightColor[j] = allLightColor[j] + lightColors[i][j];
		}
	}
	return allLightColor;
}

float3 Fragment::getPosition()
{
	return position;
}

float3 Fragment::getNormal()
{
	return normal;
}

void Fragment::setPosition(float3 position)
{
	this->position = position;
}

void Fragment::setNormal(float3 normal)
{
	this->normal = normal;
}

void Fragment::addLight(Light *light)
{
	lights.push_back(light);
}