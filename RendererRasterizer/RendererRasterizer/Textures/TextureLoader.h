#pragma once
#include <iostream>
#include "../Image/Buffer.h"

class TextureLoader
{
public:
	Buffer loadImage(const char* fileName);
};