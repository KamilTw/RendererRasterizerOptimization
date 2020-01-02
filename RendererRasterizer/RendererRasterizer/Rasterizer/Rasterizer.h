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

class Rasterizer
{
private:
	Buffer *buffer;
	Fragment fragment;
	VertexProcessor vp;

public:
	Rasterizer(Buffer *buffer);

	void drawTriangle(float3& v1, float3& v2, float3& v3, float3& c1, float3& c2, float3& c3, float3& n1, float3& n2, float3& n3, float3& tn1, float3& tn2, float3& tn3, Buffer* texture);
	void draw(Model* model, VertexProcessor& vp, Buffer* texture);
	void draw(Triangle* triangle);

	float xToCanonicalView(float& x);
	float yToCanonicalView(float& y);

	float3 calculateColorPerVertex(float3& v, float3& c, float3& n);
	float3 interpolateColor(float3& c1Fragment, float3& c2Fragment, float3& c3Fragment, float& l1, float& l2, float& l3);
	float3 calculateColorPerPixel(float3& v1, float3& v2, float3& v3, float3& c1, float3& c2, float3& c3, float3& n1, float3& n2, float3& n3, float& l1, float& l2, float& l3, float3& tn1, float3& tn2, float3& tn3, Buffer* texture);

	void maxToOne(float3& color);
	void addLight(Light* light);
	void setVp(VertexProcessor& vp);
};