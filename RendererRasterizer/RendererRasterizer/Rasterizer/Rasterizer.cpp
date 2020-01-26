#include "Rasterizer.h"

#include <ratio>
#include <chrono>

using namespace std::chrono;

Rasterizer::Rasterizer(Buffer * buffer)
{
	this->buffer = buffer;
	fragment = Fragment();
	imageWidth = buffer->getWidth();
	imageHeight = buffer->getHeight();

	interpolationInfos.reserve(imageHeight * imageWidth);
	pixelInfos.reserve(imageHeight * imageWidth);
}

void Rasterizer::draw(Model* model, VertexProcessor& vp, Buffer* texture)
{
	setVp(vp);
	counter = 0;
	if (texture != NULL)
	{
		isTextureIncluded = true;
	}
	else
	{
		isTextureIncluded = false;
	}

	vector<float3> verticesAfterVP = executeVP(model->vertices, 1);
	vector<float3> normalsAfterVP = executeVP(model->normals, 0);


	findPixelInfo(model, verticesAfterVP);

	vector<float3> fragPositions = calculateFragPositions(model, verticesAfterVP);
	vector<float3> fragNormals = calculateFragNormals(model, normalsAfterVP);

	vector<float3> lightNormals = calculateLightNormals(fragNormals);
	vector<float3> surfacePositions = calculateSurfacePositions(fragNormals);

	vector<float3> interpolatedColor = interpolateColor(model, texture);

	vector<float3> color = calculateColorPerPixel(fragPositions, interpolatedColor, lightNormals, surfacePositions);

	setBufferColor(color);
}

vector<float3> Rasterizer::executeVP(vector<float3>& v, int w)
{
	vector<float3> vAfterVP;
	vAfterVP.reserve(v.size());

	vAfterVP = vp.toProj(v, w);

	return vAfterVP;
}

void Rasterizer::findPixelInfo(Model* model, vector<float3>& verticesAfterVP)
{
	//high_resolution_clock::time_point t1 = high_resolution_clock::now();

	for (int i = 0; i < model->getIndexesAmount(); ++i)
	{
		// From 0-1 to image resolution
		float x1 = xToCanonicalView(verticesAfterVP[model->vIndexes[i].a].x);
		float y1 = yToCanonicalView(verticesAfterVP[model->vIndexes[i].a].y);

		float x2 = xToCanonicalView(verticesAfterVP[model->vIndexes[i].b].x);
		float y2 = yToCanonicalView(verticesAfterVP[model->vIndexes[i].b].y);

		float x3 = xToCanonicalView(verticesAfterVP[model->vIndexes[i].c].x);
		float y3 = yToCanonicalView(verticesAfterVP[model->vIndexes[i].c].y);

		// Optimization
		float minX = min(x1, x2, x3);
		float maxX = max(x1, x2, x3);
		float minY = min(y1, y2, y3);
		float maxY = max(y1, y2, y3);

		// Culling
		minX = max(minX, 0.0f);
		maxX = min(maxX, imageWidth);
		minY = max(minY, 0.0f);
		maxY = min(maxY, imageHeight);

		float dx12 = x1 - x2;
		float dx23 = x2 - x3;
		float dx31 = x3 - x1;
		float dy12 = y1 - y2;
		float dy23 = y2 - y3;
		float dy31 = y3 - y1;

		float dx32 = x3 - x2;
		float dx13 = x1 - x3;
		float dy13 = y1 - y3;

		float l1Denom = dy23 * dx13 + dx32 * dy13;
		float l2Denom = dy31 * dx23 + dx13 * dy23;

		bool tl1 = dy12 < 0 || (dy12 == 0 && dx12 > 0);
		bool tl2 = dy23 < 0 || (dy23 == 0 && dx23 > 0);
		bool tl3 = dy31 < 0 || (dy31 == 0 && dx31 > 0);

		// Per vertex lighting
		//float3 c1Fragment = calculateColorPerVertex(v1, c1, n1);
		//float3 c2Fragment = calculateColorPerVertex(v2, c2, n2);
		//float3 c3Fragment = calculateColorPerVertex(v3, c3, n3);

		for (int y = minY; y <= maxY; ++y)
		{
			for (int x = minX; x <= maxX; ++x)
			{
				// Left-handed system cause of 3ds max using it
				if ((((dx12 * (y - y1) - dy12 * (x - x1) <= 0) && tl1) || (dx12 * (y - y1) - dy12 * (x - x1) < 0)) &&
					(((dx23 * (y - y2) - dy23 * (x - x2) <= 0) && tl2) || (dx23 * (y - y2) - dy23 * (x - x2) < 0)) &&
					(((dx31 * (y - y3) - dy31 * (x - x3) <= 0) && tl3) || (dx31 * (y - y3) - dy31 * (x - x3) < 0)))
				{
					// Barycentric coordinates
					float l1 = (dy23 * (x - x3) + dx32 * (y - y3)) / l1Denom;
					float l2 = (dy31 * (x - x3) + dx13 * (y - y3)) / l2Denom;
					float l3 = 1 - l1 - l2;

					float depth = l1 * verticesAfterVP[model->vIndexes[i].a].z + l2 * verticesAfterVP[model->vIndexes[i].b].z + l3 * verticesAfterVP[model->vIndexes[i].c].z;

					if (depth < buffer->getDepth(x, y))
					{
						// Per vertex lighting
						//float3 color = interpolateColor(c1Fragment, c2Fragment, c3Fragment, l1, l2, l3);	

						buffer->setDepth(x, y, depth);

						interpolationInfos[counter] = interpolationInfo{ l1, l2, l3, i };
						pixelInfos[counter] = pixelInfo{ x, y };
						counter++;
					}
				}
			}
		}
	}

	//high_resolution_clock::time_point t2 = high_resolution_clock::now();
	//time_span += duration_cast<duration<double>>(t2 - t1).count();
}

vector<float3> Rasterizer::calculateFragPositions(Model* model, vector<float3>& verticesAfterVP)
{
	vector<float3> fragPositions;
	fragPositions.reserve(counter);
	int i;
	for (int j = 0; j < counter; ++j)
	{
		i = interpolationInfos[j].index;
		float3 fragPosition = (interpolate(verticesAfterVP[model->vIndexes[i].a], verticesAfterVP[model->vIndexes[i].b], verticesAfterVP[model->vIndexes[i].c], interpolationInfos[j].l1, interpolationInfos[j].l2, interpolationInfos[j].l3));

		fragPositions[j] = vp.toView(-fragPosition, 1);
	}

	return fragPositions;
}

vector<float3> Rasterizer::calculateFragNormals(Model* model, vector<float3>& normalsAfterVP)
{
	vector<float3> fragNormals;
	fragNormals.reserve(counter);
	int i;
	for (int j = 0; j < counter; ++j)
	{
		i = interpolationInfos[j].index;
		fragNormals[j] = (interpolate(normalsAfterVP[model->nIndexes[i].a], normalsAfterVP[model->nIndexes[i].b], normalsAfterVP[model->nIndexes[i].c], interpolationInfos[j].l1, interpolationInfos[j].l2, interpolationInfos[j].l3));
	}

	return fragNormals;
}

vector<float3> Rasterizer::calculateLightNormals(vector<float3>& fragNormals)
{
	vector<float3> lightNormals;
	lightNormals.resize(counter);
	int i;
	for (int j = 0; j < counter; ++j)
	{
		float3 N = vp.toView(fragNormals[j], 0);
		N.normalize();
		lightNormals[j] = N;
	}

	return lightNormals;
}

vector<float3> Rasterizer::calculateSurfacePositions(vector<float3>& fragNormals)
{
	vector<float3> surfacePositions;
	surfacePositions.reserve(counter);
	int i;
	for (int j = 0; j < counter; ++j)
	{
		float3 surfacePos = vp.toWorld(-fragNormals[j], 1);
		surfacePositions[j] = surfacePos;
	}

	return surfacePositions;
}

vector<float3> Rasterizer::interpolateColor(Model* model, Buffer* texture)
{
	vector<float3> interpolatedColor;
	interpolatedColor.reserve(counter);
	int i;
	if (isTextureIncluded)
	{
		vector<float> interpolatedU;
		interpolatedU.reserve(counter);
		vector<float> interpolatedV;
		vector<int> rows;
		rows.reserve(counter);
		vector<int> columns;
		columns.reserve(counter);
		interpolatedV.reserve(counter);

		for (int j = 0; j < counter; ++j)
		{
			i = interpolationInfos[j].index;
			interpolatedU[j] = (interpolateUV(model->texturesVertices[model->tIndexes[i].a].x, model->texturesVertices[model->tIndexes[i].b].x, model->texturesVertices[model->tIndexes[i].c].x, interpolationInfos[j].l1, interpolationInfos[j].l2, interpolationInfos[j].l3));
			interpolatedV[j] = (interpolateUV(model->texturesVertices[model->tIndexes[i].a].y, model->texturesVertices[model->tIndexes[i].b].y, model->texturesVertices[model->tIndexes[i].c].y, interpolationInfos[j].l1, interpolationInfos[j].l2, interpolationInfos[j].l3));
		}

		float textureHeight = (texture->getHeight() - 1);
		float textreWidth = (texture->getWidth() - 1);
		for (int j = 0; j < counter; ++j)
		{
			columns[j] = (int)(textureHeight * interpolatedV[j]);
			rows[j] = (int)(textreWidth * interpolatedU[j]);
		}
		for (int j = 0; j < counter; ++j)
		{
			interpolatedColor[j] = texture->getPixelColor(rows[j], columns[j]);
		}
	}
	else
	{
		for (int j = 0; j < counter; ++j)
		{
			i = interpolationInfos[j].index;
			interpolatedColor[j] = (interpolate(model->materials[model->mIndexes[i]].kd, model->materials[model->mIndexes[i]].kd, model->materials[model->mIndexes[i]].kd, interpolationInfos[j].l1, interpolationInfos[j].l2, interpolationInfos[j].l3));
		}
	}

	return interpolatedColor;
}

vector<float3> Rasterizer::calculateColorPerPixel(vector<float3>& v, vector<float3>& c, vector<float3>& n, vector<float3>& surfacePos)
{
	vector<float3> allLightsColor = fragment.calculate(v, n, surfacePos, counter);

	for (int i = 0; i < counter; ++i)
	{
		allLightsColor[i] = c[i] * allLightsColor[i];
	}

	maxToOne(allLightsColor);
	return allLightsColor;
}

void Rasterizer::maxToOne(vector<float3>& color)
{
	for (int i = 0; i < color.size(); ++i)
	{
		float maxValue = max(color[i].r, max(color[i].g, color[i].b));

		if (maxValue > 1.0f)
		{
			color[i] = color[i] / maxValue;
		}
	}
}

void Rasterizer::setBufferColor(vector<float3>& color)
{
	for (int j = 0; j < counter; ++j)
	{
		buffer->setPixelColor(pixelInfos[j].x, pixelInfos[j].y, color[j]);
	}
}

float3 Rasterizer::interpolate(float3& v1, float3& v2, float3& v3, float& l1, float& l2, float& l3)
{
	return float3{ l1 * v1.x + l2 * v2.x + l3 * v3.x, l1 * v1.y + l2 * v2.y + l3 * v3.y, l1 * v1.z + l2 * v2.z + l3 * v3.z };
}

float Rasterizer::interpolateUV(float& v1, float& v2, float& v3, float& l1, float& l2, float& l3)
{
	return l1 * v1 + l2 * v2 + l3 * v3;
}

float Rasterizer::xToCanonicalView(float& x)
{
	return (x + 1) * imageWidth * 0.5f;
}

float Rasterizer::yToCanonicalView(float& y)
{
	return (y + 1) * imageHeight * 0.5f;
}

float3 Rasterizer::calculateColorPerVertex(float3& v, float3& c, float3& n)
{
	//float3 cFragment = float3{ c.r, c.g, c.b } * fragment.calculate(v, n, vp);
	float3 cFragment;

	return cFragment;
}

void Rasterizer::addLight(Light* light)
{
	fragment.addLight(light);
}

void Rasterizer::setVp(VertexProcessor& vp)
{
	this->vp = vp;
}