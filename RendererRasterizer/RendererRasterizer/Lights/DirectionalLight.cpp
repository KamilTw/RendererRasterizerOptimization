#include "DirectionalLight.h"
#include "..//Shaders/Fragment.h"

vector<float3> DirectionalLight::calculate(vector<float3>& Vv, vector<float3>& N, vector<float3>& surfacePos, int& counter)
{
	vector<float3> lightColor;
	lightColor.reserve(counter);

	float3 direction = getPosition();
	direction.normalize();
	float3 ambient = getAmbient() * getLightColor();

	for (int i = 0; i < counter; ++i)
	{
		// Normal
		//float3 N = vertexProcessor.toView(fragNormal, 0);
		//N.normalize();
		float3 V = Vv[i];

		// View direction (from object to observer)
		//float3 V = vertexProcessor.toView(-fragPosition, 1);
		V.normalize();

		// R - reflected light direction vector from N
		float3 R = reflect(-direction, N[i]);
		R.normalize();

		float diffCoefficient = max(dotProduct(direction, N[i]), 0.0f);
		float specCoefficient = powf(max(dotProduct(R, V), 0.0f), getShininess());

		float3 diffuse = getDiffuse() * diffCoefficient * getLightColor();
		float3 specular = getSpecular() * specCoefficient * getLightColor();

		lightColor[i] = ambient + diffuse + specular;
	}

	return lightColor;
}