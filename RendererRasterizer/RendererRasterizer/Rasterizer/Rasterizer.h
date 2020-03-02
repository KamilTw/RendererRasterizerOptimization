#pragma once
#include <vector>
#include <iostream>
#include "../Image/Buffer.h"
#include "../Object/Triangle.h"
#include "..//Object/Model.h"
#include "..//Shaders/VertexProcessor.h"
#include "..//Shaders/Fragment.h"
#include "..//Lights/Light.h"

using namespace std;

struct interpolationInfo
{
	float l1;
	float l2;
	float l3;
	int index;
};

struct pixelInfo
{
	int x;
	int y;
};

class Rasterizer
{
private:
	Buffer *buffer;
	Fragment fragment;
	VertexProcessor vp;
	vector<interpolationInfo> interpolationInfos;
	vector<pixelInfo> pixelInfos;
	int pixelInfosCounter = 0;
	bool isTextureIncluded;
	float imageWidth;
	float imageHeight;

public:
	Rasterizer(Buffer *buffer);
	void draw(Model* model, VertexProcessor& vp, Buffer* texture);
	vector<float3> executeVP(vector<float3>& vertices, int w);
	void findPixelInfo(Model* model, vector<float3>& verticesAfterVP);
	vector<float3> calculateFragPositions(Model* model, vector<float3>& verticesAfterVP);
	vector<float3> calculateFragNormals(Model* model, vector<float3>& normalsAfterVP);
	vector<float3> calculateLightNormals(vector<float3>& fragNormals);
	vector<float3> calculateSurfacePositions(vector<float3>& fragNormals);
	vector<float3> interpolateColor(Model* model, Buffer* texture);
	vector<float3> calculateColorPerPixel(vector<float3>& v, vector<float3>& c, vector<float3>& n, vector<float3>& surfacePos);
	void maxToOne(vector<float3>& color);
	void setBufferColor(vector<float3>& color);
	float3 interpolate(float3& v1, float3& v2, float3& v3, float& l1, float& l2, float& l3);
	float interpolateUV(float& v1, float& v2, float& v3, float& l1, float& l2, float& l3);

	float xToCanonicalView(float& x);
	float yToCanonicalView(float& y);

	//float3 calculateColorPerVertex(float3& v, float3& c, float3& n);

	void addLight(Light* light);
	void setVp(VertexProcessor& vp);
};