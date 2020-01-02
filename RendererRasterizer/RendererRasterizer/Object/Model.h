#pragma once
#include <vector>
#include "../Math/Math.h"
#include "..//Object/Triangle.h"

using namespace std;

class Model
{
public:
	vector<float3> vertices;
	vector<int3> vIndexes;

	vector<Material> materials;
	vector<int> mIndexes;

	vector<float3> normals;
	vector<int3> nIndexes;

	vector<float3> texturesVertices;
	vector<int3> tIndexes;

public:
	int getVerticesAmount();
	int getIndexesAmount();
};