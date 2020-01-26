#include "PointLight.h"

vector<float3> PointLight::calculate(vector<float3>& Vv, vector<float3>& N, vector<float3>& surfacePos, int& counter)
{
	vector<float3> lightColor;
	lightColor.reserve(counter);

	float3 lightPos = getPosition();
	lightPos.z = -lightPos.z;
	float3 ambient = getAmbient() * getLightColor();

	for (int i = 0; i < counter; ++i)
	{
		// Normal
		//float3 N = vertexProcessor.toView(fragNormal, 0);
		//N.normalize();

		// View direction (from object to observer)
		//float3 V = vertexProcessor.toView(-fragPosition, 1);
		float3 V = Vv[i];

		// Needed to distance in attenuation
		//float3 surfacePos = vertexProcessor.toWorld(-fragNormal, 1);

		// L - light direction (from fragment to light source)
		float3 L = lightPos - V;
		L.normalize();
		V.normalize();

		// R - reflected light direction vector from N
		float3 R = reflect(-L, N[i]);

		float diffCoefficient = max(dotProduct(L, N[i]), 0.0f);
		float specCoefficient = powf(max(dotProduct(R, V), 0.0f), getShininess());

		float distance = (getPosition() - surfacePos[i]).getLength();
		float attenuation = 1.0f / (getConstantAtten() + getLinearAtten() * distance + getQuadraticAtten() * distance * distance);
	
		float3 diffuse = getDiffuse() * diffCoefficient * getLightColor();
		float3 specular = getSpecular() * specCoefficient * getLightColor();

		lightColor[i] = (ambient + diffuse + specular) * attenuation;
	}

	return lightColor;
}