#include "Buffer.h"

Buffer::Buffer(int w, int h)
{
	this->w = w;
	this->h = h;

	setSize(w * h);
	depth = new float[w * h];

	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			float3 startColor = float3{ 0, 0, 0 };
			setPixelColor(x, y, startColor);
			setDepth(x, y, 1000000);
		}
	}
}

void Buffer::setSize(int size)
{
	color = new unsigned int[size];
}

void Buffer::setColorBuffer(unsigned int* color)
{
	this->color = color;
}

void Buffer::clearColor()
{
	color = new unsigned int[w * h];
}

void Buffer::clearDepth()
{
	depth = 0;
}

void Buffer::setPixelColor(int& x, int& y, float3& rgb)
{
	color[x + y * w] = //(int)(rgb.a * 255) << 24 |
					   (int)(rgb.r * 255) << 16 |
					   (int)(rgb.g * 255) << 8 |
					   (int)(rgb.b * 255);
	//color[x + y * w] = 16777216 * a + 65536 * r + 256 * g + b;
}

void Buffer::setDepth(int x, int y, float newDepth)
{
	depth[x + y * w] = newDepth;
}

int Buffer::getWidth()
{
	return w;
}

int Buffer::getHeight()
{
	return h;
}

int Buffer::getLength()
{
	return w * h;
}

float Buffer::getDepth(int x, int y)
{
	return depth[x + y * w];
}

float3 Buffer::getPixelColor(int& x, int& y)
{
	float r = ((color[x + y * w] >> 16) & 0xFF) / 255.0f;
	float g = ((color[x + y * w] >> 8) & 0xFF) / 255.0f;
	float b = (color[x + y * w] & 0xFF) / 255.0f;

	return float3{ r, g, b };
}