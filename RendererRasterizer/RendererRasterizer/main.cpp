#include <iostream>
#include <ctime>
#include "Image/TgaBuffer.h"
#include "Object/Triangle.h"
#include "Rasterizer/Rasterizer.h"
#include "Object/ObjectLoader.h"
#include "Shaders/VertexProcessor.h"
#include "Lights/DirectionalLight.h"
#include "Lights/Light.h"
#include "Lights/PointLight.h"
#include "Lights/SpotLight.h"
#include "Textures/TextureLoader.h"

#include <ratio>
#include <chrono>

using namespace std::chrono;

int main()
{
	std::clock_t start;
	double durationn;
	start = std::clock();

	TgaBuffer *colorBuffer = new TgaBuffer(512, 512);
	Rasterizer rasterizer = Rasterizer(colorBuffer);

	VertexProcessor vp = VertexProcessor();
	vp.setPerspective(45, 1.0f, 0.1f, 100.0f);
	vp.setLookAt(float3{ 0, 2, 3 }, float3{ 0, 0, 0 }, float3{ 0, 1, 0 });
	rasterizer.setVp(vp);


	// Lights
	Light* dl = new DirectionalLight();
	dl->setPosition(float3{ 0.5f, 0.5f, 0.0f });
	dl->setLightColor(float3{ 0.7f, 0.7f, 0.7f });
	dl->setAmbient(float3{ 0.2f, 0.2f, 0.2f });
	dl->setDiffuse(float3{ 0.6f, 0.6f, 0.6f });
	dl->setSpecular(float3{ 0.5f, 0.5f, 0.5f });
	dl->setShininess(10);
	rasterizer.addLight(dl);

	Light* pointLight1 = new PointLight();
	pointLight1->setPosition(float3{ -5.0f, 5.0f, 5.0f });
	pointLight1->setLightColor(float3{ 0.3f, 0.3f, 0.3f });
	pointLight1->setAmbient(float3{ 0.2f, 0.2f, 0.2f });
	pointLight1->setDiffuse(float3{ 0.6f, 0.6f, 0.6f });
	pointLight1->setSpecular(float3{ 0.9f, 0.9f, 0.9f });
	pointLight1->setShininess(10);
	pointLight1->setAttenuation(1, 0.014f, 0.007f);
	rasterizer.addLight(pointLight1);

	Light* spotLight1 = new Spotlight();
	spotLight1->setPosition(float3{ -0.0f, 1.7f, 4.3f });
	spotLight1->setDirection(float3{ 0.0f, -0.0f, -0.8f });
	spotLight1->setLightColor(float3{ 2.0f, 2.0f, 2.0f });
	spotLight1->setAmbient(float3{ 0.2f, 0.2f, 0.2f });
	spotLight1->setDiffuse(float3{ 0.6f, 0.6f, 0.6f });
	spotLight1->setSpecular(float3{ 0.9f, 0.9f, 0.9f });
	spotLight1->setShininess(10);
	spotLight1->setAttenuation(1, 0.007f, 0.0002f);
	spotLight1->setCutoff(cos(0.3f));
	spotLight1->setOuterCutoff(cos(0.6f));
	rasterizer.addLight(spotLight1);

	// Object loader
	double time_span = 0;

	ObjectLoader loader = ObjectLoader();
	Model box = loader.loadObject("box");
	Model sphere = loader.loadObject("sphere16");
	Model cone = loader.loadObject("cone");

	// Texture loader
	TextureLoader tl = TextureLoader();
	Buffer pokeballTexture = tl.loadImage("TextureImages/Pokeball.tga");
	Buffer woodTexture = tl.loadImage("TextureImages/Wood.tga");
	Buffer stoneTexture = tl.loadImage("TextureImages/Stone.tga");

	// Drawing
	vp.setIdentity();
	vp.multByScale(float3{ 0.7, 0.7, 0.7 });
	vp.multByTranslation(float3{ -2.5f, 0.0f, -5.0f });
	rasterizer.draw(&sphere, vp, NULL);

	vp.setIdentity();
	vp.multByScale(float3{ 0.7, 0.7, 0.7 });
	vp.multByRotation(-180, float3{ 0, 1, 0 });
	vp.multByTranslation(float3{ -2.5f, 0.0f, -8.0f });
	rasterizer.draw(&sphere, vp, &pokeballTexture);

	vp.setIdentity();
	vp.multByScale(float3{ 1.5, 1.0, 1.5 });
	vp.multByTranslation(float3{ 2.5f, -0.5f, -5.0f });
	rasterizer.draw(&cone, vp, NULL);

	vp.setIdentity();
	vp.multByScale(float3{ 1.5, 1.0, 1.5 });
	vp.multByTranslation(float3{ 2.5f, -0.5f, -8.0f });
	rasterizer.draw(&cone, vp, &stoneTexture);

	vp.setIdentity();
	vp.multByScale(float3{ 1.0, 1.0, 1.0 });
	vp.multByRotation(45, float3{ 0, 1, 0 });
	vp.multByTranslation(float3{ 0.0f, 0.0f, -5.0f });
	rasterizer.draw(&box, vp, NULL);

	vp.setIdentity();
	vp.multByScale(float3{ 1.0, 1.0, 1.0 });
	vp.multByRotation(-45, float3{ 0, 1, 0 });
	vp.multByRotation(180, float3{ 0, 0, 1 });
	vp.multByTranslation(float3{ 0.0f, 0.0f, -8.0f });
	rasterizer.draw(&box, vp, &woodTexture);

	vp.setIdentity();
	vp.multByScale(float3{ 10.0, 15.0, 1.0 });
	vp.multByRotation(-90, float3{ 1, 0, 0 });
	vp.multByTranslation(float3{ 0.0f, -1.0f, -10.0f });
	rasterizer.draw(&box, vp, NULL);

	//cout << rasterizer.time_span << endl;
	
	// 0.0005    -7
	// 




	
	cout << time_span << endl;


	colorBuffer->save("Image.tga");

	durationn = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	printf("\n \nRendered in %f seconds (%f fps) \n", durationn, 1 / durationn);
}