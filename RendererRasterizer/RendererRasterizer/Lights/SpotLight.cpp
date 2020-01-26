#include "SpotLight.h"

vector<float3> Spotlight::calculate(vector<float3>& Vv, vector<float3>& N, vector<float3>& surfacePos, int& counter)
{
	vector<float3> lightColor;
	lightColor.reserve(counter);

	float3 ambient = getAmbient() * getLightColor();
	float epsilon = getCutOff() - getOuterCutoff();

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
		float3 L = -getPosition() - V;
		L.normalize();
		V.normalize();

		// R - reflected light direction vector from N
		float3 R = reflect(-L, N[i]);

		float diffCoefficient = max(dotProduct(L, N[i]), 0.0f);
		float specCoefficient = powf(max(dotProduct(R, V), 0.0f), getShininess());

		float distance = (getPosition() - surfacePos[i]).getLength();

		float attenuation = 1.0f / (getConstantAtten() + getLinearAtten() * distance + getQuadraticAtten() * distance * distance);

		// Spotlight intensity
		// theta - angle between L and spotlight direction, needs to be larger than cutoff value to be inside spotlight cone
		// cutoff cos(0.28) = 0.96, outerCutoff cos(0.38) = 0.92
		// if dot product of two vectors is equal to one it means that vectors are pointing in the same direction
		// object inside cone gets 1 intensity, outside gets 0, around the cone 0-1
		float theta = dotProduct(L, getDirection());

		float intensity = saturate((theta - getOuterCutoff()) / epsilon);

		float3 diffuse = getDiffuse() * diffCoefficient * getLightColor();
		float3 specular = getSpecular() * specCoefficient * getLightColor();

		lightColor[i] = (ambient + diffuse + specular) * attenuation * intensity;
	}

	return lightColor;
}