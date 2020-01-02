#include "Fragment.h"

float3 Fragment::calculate(float3& fragPosition, float3& fragNormal, VertexProcessor& vertexProcessor)
{
	float3 color = {0, 0, 0};
	for (int i = 0; i < lights.size(); i++)
	{
		color += lights[i]->calculate(fragPosition, fragNormal, vertexProcessor);
	}

	return color;
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