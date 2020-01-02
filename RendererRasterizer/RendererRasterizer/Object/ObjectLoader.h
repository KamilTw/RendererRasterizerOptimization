#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include "Model.h"

class ObjectLoader
{
private:
	vector<float3> vertices;
	vector<int3> vIndexes;

	vector<Material> materials;
	vector<int> mIndexes;
	vector<string> materialNames;

	vector<float3> normals;
	vector<int3> nIndexes;

	vector<float3> texturesVertices;
	vector<int3> tIndexes;

public:
	Model loadObject(std::string objectFileName);
	void loadMaterial(std::string objectFileName);
};