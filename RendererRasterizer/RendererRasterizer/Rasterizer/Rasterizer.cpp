#include "Rasterizer.h"

Rasterizer::Rasterizer(Buffer * buffer)
{
	this->buffer = buffer;
	fragment = Fragment();
}

void Rasterizer::drawTriangle(float3& v11, float3& v22, float3& v33, float3& c1, float3& c2, float3& c3, float3& n11, float3& n22, float3& n33, float3& tn1, float3& tn2, float3& tn3, Buffer* texture)
{
	// Vertex processor
	float3 v1 = vp.toProj(v11, 1);
	float3 v2 = vp.toProj(v22, 1);
	float3 v3 = vp.toProj(v33, 1);

	float3 n1 = vp.toProj(n11, 0);
	float3 n2 = vp.toProj(n22, 0);
	float3 n3 = vp.toProj(n33, 0);

	// From 0-1 to image resolution
	float x1 = xToCanonicalView(v1.x);
	float y1 = yToCanonicalView(v1.y);

	float x2 = xToCanonicalView(v2.x);
	float y2 = yToCanonicalView(v2.y);

	float x3 = xToCanonicalView(v3.x);
	float y3 = yToCanonicalView(v3.y);

	// Optimization
	float minX = min(x1, x2, x3);
	float maxX = max(x1, x2, x3);
	float minY = min(y1, y2, y3);
	float maxY = max(y1, y2, y3);

	// Culling
	minX = max(minX, 0.0f);
	maxX = min(maxX, float(buffer->getWidth() - 1));
	minY = max(minY, 0.0f);
	maxY = min(maxY, float(buffer->getHeight() - 1));

	float dx12 = x1 - x2;
	float dx23 = x2 - x3;
	float dx31 = x3 - x1;
	float dy12 = y1 - y2;
	float dy23 = y2 - y3;
	float dy31 = y3 - y1;

	float dx32 = x3 - x2;
	float dx13 = x1 - x3;
	float dy13 = y1 - y3;

	float l1Denom = dy23 * dx13 + dx32 * dy13;
	float l2Denom = dy31 * dx23 + dx13 * dy23;

	bool tl1 = dy12 < 0 || (dy12 == 0 && dx12 > 0);
	bool tl2 = dy23 < 0 || (dy23 == 0 && dx23 > 0);
	bool tl3 = dy31 < 0 || (dy31 == 0 && dx31 > 0);

	// Per vertex lighting
	//float3 c1Fragment = calculateColorPerVertex(v1, c1, n1);
	//float3 c2Fragment = calculateColorPerVertex(v2, c2, n2);
	//float3 c3Fragment = calculateColorPerVertex(v3, c3, n3);

	for (int y = minY; y <= maxY; y++)
	{
		for (int x = minX; x <= maxX; x++)
		{
			// Left-handed system cause of 3ds max using it
			if ((((dx12 * (y - y1) - dy12 * (x - x1) <= 0) && tl1) || (dx12 * (y - y1) - dy12 * (x - x1) < 0)) &&
				(((dx23 * (y - y2) - dy23 * (x - x2) <= 0) && tl2) || (dx23 * (y - y2) - dy23 * (x - x2) < 0)) &&
				(((dx31 * (y - y3) - dy31 * (x - x3) <= 0) && tl3) || (dx31 * (y - y3) - dy31 * (x - x3) < 0)))
			{
				// Barycentric coordinates
				float l1 = (dy23 * (x - x3) + dx32 * (y - y3)) / l1Denom;
				float l2 = (dy31 * (x - x3) + dx13 * (y - y3)) / l2Denom;
				float l3 = 1 - l1 - l2;

				float depth = l1 * v1.z + l2 * v2.z + l3 * v3.z;

				if (depth < buffer->getDepth(x, y))
				{
					// Per vertex lighting
					//float3 color = interpolateColor(c1Fragment, c2Fragment, c3Fragment, l1, l2, l3);
					// Per pixel lighting
					float3 color = calculateColorPerPixel(v1, v2, v3, c1, c2, c3, n1, n2, n3, l1, l2, l3, tn1, tn2, tn3, texture);
			
					buffer->setPixelColor(x, y, color);
					buffer->setDepth(x, y, depth);
				}
			}
		}	
	}
}

void Rasterizer::draw(Model* model, VertexProcessor& vp, Buffer* texture)
{
	setVp(vp);
	for (int i = 0; i < model->getIndexesAmount(); i++)
	{
		drawTriangle(model->vertices[model->vIndexes[i].a], model->vertices[model->vIndexes[i].b], model->vertices[model->vIndexes[i].c],
					 model->materials[model->mIndexes[i]].kd, model->materials[model->mIndexes[i]].kd, model->materials[model->mIndexes[i]].kd,
					 model->normals[model->nIndexes[i].a], model->normals[model->nIndexes[i].b], model->normals[model->nIndexes[i].c],
					 model->texturesVertices[model->tIndexes[i].a], model->texturesVertices[model->tIndexes[i].b], model->texturesVertices[model->tIndexes[i].c],
					 texture);
	}
}

void Rasterizer::draw(Triangle* triangle)
{
	for (int i = 0; i < 3; i++)
	{
		//draw(triangle->v[0], triangle->v[1], triangle->v[2],
			 //triangle->c[0], triangle->c[1], triangle->c[2]);
	}
}

float Rasterizer::xToCanonicalView(float& x)
{
	return (x + 1) * buffer->getWidth() * 0.5f;
}

float Rasterizer::yToCanonicalView(float& y)
{
	return (y + 1) * buffer->getHeight() * 0.5f;
}

float3 Rasterizer::calculateColorPerVertex(float3& v, float3& c, float3& n)
{
	float3 cFragment = float3{ c.r, c.g, c.b } * fragment.calculate(v, n, vp);

	return cFragment;
}

float3 Rasterizer::interpolateColor(float3& c1Fragment, float3& c2Fragment, float3& c3Fragment, float& l1, float& l2, float& l3)
{
	// Color interpolation
	float3 interpolatedColor = c1Fragment * l1 + c2Fragment * l2 + c3Fragment * l3;
	maxToOne(interpolatedColor);

	return interpolatedColor;
}

float3 Rasterizer::calculateColorPerPixel(float3& v1, float3& v2, float3& v3, float3& c1, float3& c2, float3& c3, float3& n1, float3& n2, float3& n3, float& l1, float& l2, float& l3, float3& tn1, float3& tn2, float3& tn3, Buffer* texture)
{
	// Normals interpolation
	float nx = l1 * n1.x + l2 * n2.x + l3 * n3.x;
	float ny = l1 * n1.y + l2 * n2.y + l3 * n3.y;
	float nz = l1 * n1.z + l2 * n2.z + l3 * n3.z;

	// Vertices interpolation
	float vx = l1 * v1.x + l2 * v2.x + l3 * v3.x;
	float vy = l1 * v1.y + l2 * v2.y + l3 * v3.y;
	float vz = l1 * v1.z + l2 * v2.z + l3 * v3.z;

	float3 interpolatedVert = float3{ vx, vy, vz };
	float3 interpolatedN = float3{ nx, ny, nz };

	float3 colorPerPixel;
	if (texture == NULL)
	{	
		// Color interpolation
		float cr = l1 * c1.r + l2 * c2.r + l3 * c3.r;
		float cg = l1 * c1.g + l2 * c2.g + l3 * c3.g;
		float cb = l1 * c1.b + l2 * c2.b + l3 * c3.b;

		colorPerPixel = float3{ cr, cg, cb } * fragment.calculate(interpolatedVert, interpolatedN, vp);
	}
	else
	{	
		// UV interpolation
		float interpolatedU = l1 * tn1.x + l2 * tn2.x + l3 * tn3.x;
		float interpolatedV = l1 * tn1.y + l2 * tn2.y + l3 * tn3.y;

		int column = (int)((texture->getHeight() - 1) * interpolatedV);
		int row = (int)((texture->getWidth() - 1) * interpolatedU);

		colorPerPixel = texture->getPixelColor(row, column) * fragment.calculate(interpolatedVert, interpolatedN, vp);
	}
	maxToOne(colorPerPixel);

	return colorPerPixel;
}

void Rasterizer::maxToOne(float3 &color)
{
	float maxValue = max(color.r, max(color.g, color.b));

	if (maxValue > 1.0f)
	{
		color = color / maxValue;
	}
}

void Rasterizer::addLight(Light* light)
{
	fragment.addLight(light);
}

void Rasterizer::setVp(VertexProcessor& vp)
{
	this->vp = vp;
}