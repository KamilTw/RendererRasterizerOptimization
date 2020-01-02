#pragma once
class ITgaSavable
{
public:
	virtual void save(const char* fileName) = 0;
};