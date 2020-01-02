#pragma warning(disable:4996)
#include "TgaBuffer.h"

TgaBuffer::TgaBuffer(int w, int h) :  Buffer(w, h)
{
}

void TgaBuffer::save(const char *fileName)
{
	FILE* file = fopen(fileName, "wb+");
	unsigned short header[9] = { 0x0000, 0x0002, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0100, 0x0100,
	0x0820 };

	header[6] = w;
	header[7] = h;

	fwrite(header, 2, 9, file);
	fwrite(color, 4, w * h, file);
	fclose(file);

	printf("File %s saved \n", fileName);
}