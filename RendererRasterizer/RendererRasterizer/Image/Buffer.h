#pragma once
#include "../Math/Math.h"

class Buffer
{
protected:
	unsigned int *color;
	float *depth;
	int w, h, minX, maxX, minY, maxY, length;

public:
	Buffer(int w, int h);
	void setSize(int size);
	void setColorBuffer(unsigned int* color);
	void clearColor();
	void clearDepth();
	void setPixelColor(int& x, int& y, float3& rgb);
	void setDepth(int x, int y, float depth);

	int getWidth();
	int getHeight();
	int getLength();
	float getDepth(int x, int y);
	float3 getPixelColor(int& x, int& y);
};