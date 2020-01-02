#include "PointLight.h"

float3 PointLight::calculate(float3& fragPosition, float3& fragNormal, VertexProcessor& vertexProcessor)
{
	float3 lightPos = getPosition();
	lightPos.z = -lightPos.z;

	// Normal
	float3 N = vertexProcessor.toView(fragNormal, 0);
	N.normalize();

	// View direction (from object to observer)
	float3 V = vertexProcessor.toView(-fragPosition, 1);

	// Needed to distance in attenuation
	float3 surfacePos = vertexProcessor.toWorld(-fragNormal, 1);

	// L - light direction (from fragment to light source)
	float3 L = lightPos - V;
	L.normalize();
	V.normalize();

	// R - reflected light direction vector from N
	float3 R = reflect(-L, N);

	float diffCoefficient = max(dotProduct(L, N), 0.0f);
	float specCoefficient = powf(max(dotProduct(R, V), 0.0f), getShininess());

	float distance = (getPosition() - surfacePos).getLength();
	float attenuation = 1.0f / (getConstantAtten() + getLinearAtten() * distance + getQuadraticAtten() * distance * distance);

	float3 ambient = getAmbient() * getLightColor();
	float3 diffuse = getDiffuse() * diffCoefficient * getLightColor();
	float3 specular = getSpecular() * specCoefficient * getLightColor();

	return (ambient + diffuse + specular) * attenuation;
}
