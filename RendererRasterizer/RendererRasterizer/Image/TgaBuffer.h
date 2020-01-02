#pragma once
#include <iostream>
#include "Buffer.h"
#include "ITgaSavable.h"

class TgaBuffer : public Buffer, public ITgaSavable
{
public:
	TgaBuffer(int w, int h);
	void save(const char* fileName);
};

