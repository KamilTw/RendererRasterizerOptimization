#pragma warning(disable:4996)
#include "TextureLoader.h"

Buffer TextureLoader::loadImage(const char* fileName)
{
	FILE* file = fopen(fileName, "rb+");
	unsigned short header[9];

	fread(header, 2, 9, file);
	int w = header[6];
	int h = header[7];

	unsigned int* color = new unsigned int[w * h];
	Buffer buffer = Buffer(w, h);

	fread(color, 4, w * h, file);
	buffer.setColorBuffer(color);

	fclose(file);

	printf("Texture %s loaded \n \n", fileName);

	return buffer;
}