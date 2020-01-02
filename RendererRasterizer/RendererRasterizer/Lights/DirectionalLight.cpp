#include "DirectionalLight.h"
#include "..//Shaders/Fragment.h"

float3 DirectionalLight::calculate(float3& fragPosition, float3& fragNormal, VertexProcessor& vertexProcessor)
{
	float3 direction = getPosition();
	direction.normalize();

	// Normal
	float3 N = vertexProcessor.toView(fragNormal, 0);
	N.normalize();

	// View direction (from object to observer)
	float3 V = vertexProcessor.toView(-fragPosition, 1);
	V.normalize();

	// R - reflected light direction vector from N
	float3 R = reflect(-direction, N);
	R.normalize();

	float diffCoefficient = max(dotProduct(direction, N), 0.0f);
	float specCoefficient = powf(max(dotProduct(R, V), 0.0f), getShininess());

	float3 ambient = getAmbient() * getLightColor();
	float3 diffuse = getDiffuse() * diffCoefficient * getLightColor();
	float3 specular = getSpecular() * specCoefficient * getLightColor();

	return ambient + diffuse + specular;
}